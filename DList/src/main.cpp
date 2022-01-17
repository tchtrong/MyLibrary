import dlist;

#include <iterator>

int main() {
	using MyLib::DList;
	using iterator = MyLib::DList<int>::iterator;
	using const_iterator = MyLib::DList<int>::const_iterator;

	// Check if iterator is a bidirectional_iterator
	static_assert(std::bidirectional_iterator<iterator>);
	static_assert(std::bidirectional_iterator<const_iterator>);
}