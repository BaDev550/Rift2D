#pragma once

#include <Core/Types.h>

template<typename T>
class List final {
public:
	List() = default;
	virtual ~List() = default;
	List(const List&) = delete;
	List& operator=(const List&) = delete;
	List(List&&) noexcept = default;
	List& operator=(List&&) noexcept = default;
private:
	using Iterator = T*;
	using ConstIterator = const T*;
	using ReverseIterator = std::reverse_iterator<Iterator>;
	using ConstReverseIterator = std::reverse_iterator<ConstIterator>;

	T* _data = nullptr;
	u32 _size = 0;
	u32 _capacity = 0;
};