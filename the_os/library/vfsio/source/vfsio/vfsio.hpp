#pragma once

#include <cstddef>
#include <cstdint>

#include <string_view>

#include <memory>

namespace vfsio
{
#if 0
	struct Node
	{
		virtual ~Node () = default;
	};
	
	struct DirectoryEntryNode:
		public Node
	{
		virtual auto name() const -> std::string_view = 0;
	};

	template <typename T>
	struct IteratorNode:
		public Node
	{
		virtual auto next () -> bool = 0;
		virtual auto get  () const -> std::unique_ptr<T> = 0;
		virtual auto end  () const -> bool = 0;
	};


	struct iterator_end
	{
				
	};
	
	template <typename T>
	struct iterator
	{	
		iterator(std::unique_ptr<IteratorNode<T>> node) 
		:	m_node (std::move(node))
		{}

		auto operator++ () -> iterator&
		{
			m_node->next();
			return *this;
		}

		auto operator* () const 
			-> decltype(m_node->get())
		{
			return m_node->get();
		}

	private:
		std::unique_ptr<IteratorNode<T>> m_node;
	};

	struct DirectoryNode:
		public Node
	{
		virtual auto begin () -> iterator<DirectoryEntryNode> = 0;
		virtual auto end   () -> iterator_end = 0;	
	};	
#endif
}