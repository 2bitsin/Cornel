#include <vfsio/vfsio.hpp>

using vfsio::INode;
using vfsio::IStream;
using vfsio::IFile;
using vfsio::IDirectoryEntry;
using vfsio::IDirectory;
using vfsio::IVolume;
using vfsio::error;


/////////////////
// vfsio::INode
/////////////////

auto INode::type(error& error_v) const -> node_type
{
	error_v = error::not_implemented;
	return node_type::undefined;
}

///////////////////
// vfsio::IStream
///////////////////

auto IStream::type(error& error_v) const -> node_type
{
	return node_type::stream;
}

auto IStream::read(error& error_v, std::span<std::byte> buffer_v) -> std::span<std::byte>
{
	error_v = error::not_implemented;
	return buffer_v.subspan(0, 0);
}

auto IStream::write(error& error_v, std::span<std::byte const> buffer_v) -> std::span<std::byte const>
{
	error_v = error::not_implemented;
	return buffer_v;
}

auto vfsio::IStream::flush(error& error_v) -> bool
{
	error_v = error::not_implemented;	
	return true;
}

/////////////////
// vfsio::IFile
/////////////////

auto IFile::type(error& error_v) const -> node_type
{
	return node_type::file;
}

auto IFile::seek(error& error_v, std::intmax_t offset_v, relative_to origin_v) -> std::uintmax_t
{
	error_v = error::not_implemented;
	return 0;
}

auto IFile::tell(error& error_v) -> std::uintmax_t
{	
	return seek(error_v, 0, relative_to::current);
}

auto IFile::size(error& error_v) -> std::uintmax_t
{
	error_v = error::not_implemented;
	return 0;
}

auto IFile::resize(error& error_v, std::uintmax_t new_size_v) -> std::uintmax_t
{
	error_v = error::not_implemented;
	return 0;
}

auto IFile::read(error& error_v, std::span<std::byte> buffer_v, std::uintmax_t offset_v) -> std::span<std::byte>
{
	error_v = error::not_implemented;
	return buffer_v.subspan(0, 0);
}

auto IFile::write(error& error_v, std::span<std::byte const> buffer_v, std::uintmax_t offset_v) -> std::span<std::byte const>
{
	error_v = error::not_implemented;
	return buffer_v;
}

///////////////////////////
// vfsio::IDirectoryEntry
///////////////////////////

auto IDirectoryEntry::type(error& error_v) const -> node_type
{
	return node_type::directory_entry;
}

auto IDirectoryEntry::name(error& error_v) const -> std::string_view
{
	error_v = error::not_implemented;
	return {};
}

auto IDirectoryEntry::next(error& error_v) const -> IDirectoryEntry const*
{
	error_v = error::not_implemented;
	return nullptr;
}

auto IDirectoryEntry::open(error& error_v) const -> INode*
{
	error_v = error::not_implemented;
	return nullptr;
}

//////////////////////
// vfsio::IDirectory
//////////////////////

auto IDirectory::type(error& error_v) const -> node_type
{
	return node_type::directory;
}

auto IDirectory::exists(error& error_v, std::string_view path_v) const -> bool
{
	error_v = error::not_implemented;
	return false;
}

auto IDirectory::open(error& error_v, std::string_view name_v, std::uint32_t mode_v) const -> INode*
{
	error_v = error::not_implemented;
	return nullptr;
}

auto IDirectory::create(error& error_v, std::string_view name_v, std::uint32_t mode_v) -> INode*
{
	error_v = error::not_implemented;
	return nullptr;
}

auto IDirectory::remove(error& error_v, std::string_view name_v, bool recursive_v) -> bool
{
	error_v = error::not_implemented;
	return false;
}

auto IDirectory::rename(error& error_v, std::string_view old_name_v, std::string_view new_name_v) -> bool
{
	error_v = error::not_implemented;
	return false;
}

auto IDirectory::front(error& error_v) const -> IDirectoryEntry const*
{
	error_v = error::not_implemented;
	return nullptr;
}

auto IDirectory::back(error& error_v) const -> IDirectoryEntry const*
{
	error_v = error::not_implemented;
	return nullptr;
}

///////////////////
// vfsio::IVolume
///////////////////

auto IVolume::type(error& error_v) const -> node_type
{
	return node_type::volume;
}

auto IVolume::root(error& error_v) -> IDirectory*
{
	error_v = error::not_implemented;
	return nullptr;
}

//////////////////
// helper::IFile
//////////////////

using namespace vfsio;

auto helper::IFile::read(error& error_v, std::span<std::byte> buffer_v) -> std::span<std::byte>
{
	error_v = error::none;
	const auto offset_v = tell(error_v);
	if (error_v != error::none)
		return buffer_v.subspan(0, 0);
	const auto result_v = read(error_v, buffer_v, offset_v);
	seek(error_v, result_v.size(), relative_to::current);
	return result_v;
}

auto helper::IFile::write(error& error_v, std::span<std::byte const> buffer_v) -> std::span<std::byte const>
{
	error_v = error::none;
	const auto offset_v = tell(error_v);
	if (error_v != error::none)
		return buffer_v;
	const auto result_v = write(error_v, buffer_v, offset_v);
	seek(error_v, buffer_v.size() - result_v.size(), relative_to::current);
	return result_v;
}

auto helper::IFile::tell(error& error_v) -> std::uintmax_t
{
	error_v = error::none;
	return seek(error_v, 0, relative_to::current);
}

auto helper::IFile::seek(error& error_v, std::intmax_t offset_v, relative_to origin_v) -> std::uintmax_t
{
	error_v = error::none;
	if (origin_v == relative_to::current && !offset_v)
		return m_offset;

	const auto size_v = size(error_v);
	if (error_v != error::none)
		return m_offset;

	std::uintmax_t new_offset_v { 0 };

	switch(origin_v)
	{
	case relative_to::beginning:
		new_offset_v = (std::uintmax_t)std::max<std::intmax_t>(0, offset_v); 
		break;
	case relative_to::current:
		new_offset_v = (std::uintmax_t)std::max<std::intmax_t>(0, m_offset + offset_v);
		break;
	case relative_to::ending:
		new_offset_v = (std::uintmax_t)std::max<std::intmax_t>(0, size_v + offset_v);
		break;		
	}
	
	if (new_offset_v > size_v)
	{
		const auto new_size_v = resize(error_v, new_offset_v);
		if (error_v != error::none)
			return m_offset;
		m_offset = std::min(new_size_v, new_offset_v);
	}
	else {
		m_offset = new_offset_v;
	}
	return m_offset;
}

//////////////////////////
// error from_errno(int)
//////////////////////////

#include <cerrno>
auto vfsio::from_errno(int errno_v) -> error
{
	using vfsio::error;
	switch(errno_v)
	{
	case 0               : return error::none;
	case EPERM           : return error::access_denied ;
	case ENOENT          : return error::not_found ;
	case ESRCH           : return error::other ;
	case EINTR           : return error::other ;
	case EIO             : return error::io_error ;
	case ENXIO           : return error::other ;
	case E2BIG           : return error::other ;
	case ENOEXEC         : return error::other ;
	case EBADF           : return error::other ;
	case ECHILD          : return error::other ;
	case EAGAIN          : return error::other ;
	case ENOMEM          : return error::out_of_memory ;
	case EACCES          : return error::other ;
	case EFAULT          : return error::other ;
	case EBUSY           : return error::other ;
	case EEXIST          : return error::other ;
	case EXDEV           : return error::other ;
	case ENODEV          : return error::other ;
	case ENOTDIR         : return error::other ;
	case EISDIR          : return error::other ;
	case ENFILE          : return error::other ;
	case EMFILE          : return error::other ;
	case ENOTTY          : return error::other ;
	case EFBIG           : return error::other ;
	case ENOSPC          : return error::other ;
	case ESPIPE          : return error::other ;
	case EROFS           : return error::other ;
	case EMLINK          : return error::other ;
	case EPIPE           : return error::other ;
	case EDOM            : return error::other ;
	case EDEADLK         : return error::other ;
	case ENAMETOOLONG    : return error::other ;
	case ENOLCK          : return error::other ;
	case ENOSYS          : return error::other ;
	case ENOTEMPTY       : return error::other ;
	default              : return error::other ;
	}	
};