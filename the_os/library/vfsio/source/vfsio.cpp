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
	return buffer_v.subspan(buffer_v.size());
}

auto IStream::write(error& error_v, std::span<std::byte const> buffer_v) -> std::span<std::byte const>
{
	error_v = error::not_implemented;
	return buffer_v.subspan(0);
}

/////////////////
// vfsio::IFile
/////////////////

auto IFile::type(error& error_v) const -> node_type
{
	return node_type::file;
}

auto IFile::seek(error& error_v, std::int64_t offset_v, relative_to origin_v) -> std::uint64_t
{
	error_v = error::not_implemented;
	return 0;
}

auto IFile::tell(error& error_v) -> std::uint64_t
{	
	return seek(error_v, 0, relative_to::current);
}

auto IFile::size(error& error_v) -> std::uint64_t
{
	error_v = error::not_implemented;
	return 0;
}

auto IFile::resize(error& error_v, std::uint64_t new_size_v) -> std::uint64_t
{
	error_v = error::not_implemented;
	return 0;
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