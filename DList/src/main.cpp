import dlist;

#include <iterator>
#include <list>
#include <iostream>

int main() {
	using ml::DList;
	using iterator = ml::DList<int>::iterator;
	using const_iterator = ml::DList<int>::const_iterator;

	// Check if iterator is a bidirectional_iterator
	static_assert(std::bidirectional_iterator<iterator>);
	static_assert(std::bidirectional_iterator<const_iterator>);
}