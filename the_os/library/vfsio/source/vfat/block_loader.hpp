namespace vfsio::vfat
{
	template <std::size_t Block_size>
	struct block_loader
	{
		block_loader(Iblock& block_v, std::pmr::memory_resource* alloc_v = nullptr)
		: m_alloc(alloc_v ? alloc_v : std::pmr::get_default_resource())
		,	m_block(block_v)		
		{}

		auto load(std::uintmax_t block_start_v, std::size_t block_count_v) const 
			-> std::optional<memory::buffer<std::byte>>
		{
			auto const count_v = block_count_v * Block_size;
			auto const start_v = block_start_v * Block_size;
			
			auto buffer_v = memory::buffer<std::byte>(*m_alloc, count_v);
			
			auto const read_bytes_v = m_block.load(buffer_v, start_v);
			
			if (read_bytes_v != buffer_v.size())
				return std::nullopt;
			
			return buffer_v;
		}
				
	private:
		std::pmr::memory_resource* m_alloc;
		Iblock& m_block;	

	};
}