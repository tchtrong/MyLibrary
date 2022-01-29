module;
#include <iterator>
#include <initializer_list>
#include <utility>

export module dlist;



export namespace ml {
	template <class T>
	class DList {
	public:
		using value_type = T;
		using size_type = std::size_t;
		using difference_type = std::ptrdiff_t;
		using reference = value_type&;
		using const_reference = const value_type&;
		using pointer = value_type*;
		using const_pointer = const value_type*;

	private:
		//==================================================
		//=================     Node    ====================
		//==================================================
		struct node {
			node* m_prev{};
			node* m_next{};
		};

		struct node_data : public node {
			template<class ...Args>
			node_data(Args&&...args) :m_val{ std::forward<Args>(args)... } {}
			value_type m_val;
		};

	public:
		//==================================================
		//================     Iterator    =================
		//==================================================
		class iterator {
		public:
			iterator() = default;

			using value_type = DList<T>::value_type;
			using difference_type = DList<T>::difference_type;
			using reference = DList<T>::reference;
			using pointer = DList<T>::pointer;

			bool operator==(const iterator& rhs) const {
				return m_parrent == rhs.m_parrent
					&& m_ptrNode == rhs.m_ptrNode;
			}

			reference operator*() const {
				return reinterpret_cast<node_data*>(m_ptrNode)->m_val;
			}
			pointer operator->() const {
				return &(reinterpret_cast<node_data*>(m_ptrNode)->m_val);
			}

			iterator& operator++() {
				m_ptrNode = m_ptrNode->m_next;
				return *this;
			}
			iterator operator++(int) {
				iterator p = *this;
				++(*this);
				return p;
			}

			iterator& operator--() {
				m_ptrNode = m_ptrNode->m_prev;
				return *this;
			}
			iterator operator--(int) {
				iterator p = *this;
				--(*this);
				return p;
			}

		private:
			friend DList<T>;
			iterator(DList<T>* parrent, node* value) : m_parrent{ parrent }, m_ptrNode{ value }{}
			DList<T>* m_parrent{};
			node* m_ptrNode{};
		};
		class const_iterator {
		public:
			const_iterator() = default;
			const_iterator(const iterator& iter) : m_parrent{ iter.m_parrent }, m_ptrNode{ iter.m_ptrNode }{}

			using value_type = DList<T>::value_type;
			using difference_type = DList<T>::difference_type;
			using reference = DList<T>::const_reference;
			using pointer = DList<T>::const_pointer;

			bool operator==(const const_iterator& rhs) const {
				return m_parrent == rhs.m_parrent
					&& m_ptrNode == rhs.m_ptrNode;
			}

			reference operator*() const {
				return reinterpret_cast<node_data*>(m_ptrNode)->m_val;
			}
			pointer operator->() const {
				return &(reinterpret_cast<node_data*>(m_ptrNode)->m_val);
			}

			const_iterator& operator++() {
				m_ptrNode = m_ptrNode->m_next;
				return *this;
			}
			const_iterator operator++(int) {
				const_iterator p = *this;
				++(*this);
				return p;
			}

			const_iterator& operator--() {
				m_ptrNode = m_ptrNode->m_prev;
				return *this;
			}
			const_iterator operator--(int) {
				const_iterator p = *this;
				--(*this);
				return p;
			}

		private:
			friend DList<T>;
			const_iterator(const DList<T>* parrent, node* value) : m_parrent{ parrent }, m_ptrNode{ value }{}
			operator iterator() {
				return { const_cast<DList<T>*>(this->m_parrent), this->m_ptrNode };
			}
			const DList<T>* m_parrent{};
			node* m_ptrNode{};
		};
		using reverse_iterator = std::reverse_iterator<iterator>;
		using const_reverse_iterator = std::reverse_iterator<const_iterator>;

	public:
		//==================================================
		//=====             Member functions           =====
		//==================================================

		//=====     Constructor     =====

		DList() :m_rend{ new node{} }, m_end{ new node{} } {}

		DList(size_type count, const T& value) :DList() {
			while (count) {
				this->push_back(value);
				--count;
			}
		}

		explicit DList(size_type count) :DList() {
			while (count) {
				this->push_back(T{});
				--count;
			}
		}

		template<std::input_iterator InputIt>
		DList(InputIt first, InputIt last) :DList() {
			while (first != last) {
				this->push_back(*first);
				++first;
			}
		}

		DList(const DList& other) :DList() {
			for (auto& x : other) {
				this->push_back(x);
			}
		}

		DList(DList&& other) noexcept :DList() {
			for (auto&& x : other) {
				this->push_back(std::move(x));
			}
		}

		DList(std::initializer_list<T> init) :DList() {
			for (auto& x : init) {
				this->push_back(x);
			}
		}

		////=====     Assignment operator     =====

		DList& operator=(const DList& other) {
			if (this != &other) {
				this->clear();
				for (auto& x : other) {
					this->push_back(x);
				}
			}
			return *this;
		}

		DList& operator=(DList&& other) noexcept {
			if (this != &other) {
				this->clear();
				for (auto&& x : other) {
					this->push_back(std::move(x));
				}
			}
			return *this;
		}

		DList& operator=(std::initializer_list<T> ilist) {
			for (auto& x : ilist) {
				this->push_back(x);
			}
		}

		////=====     Assignment functions     =====

		void assign(size_type count, const T& value) {
			this->clear();
			while (count) {
				this->push_back(value);
				--count;
			}
		}

		template<std::input_iterator InputIt>
		void assign(InputIt first, InputIt last) {
			this->clear();
			while (first != last) {
				this->push_back(*first);
				++first;
			}
		}

		void assign(std::initializer_list<T> ilist) {
			this->clear();
			for (auto& x : ilist) {
				this->push_back(x);
			}
		}

		//=====     Destructor     =====

		~DList() {
			this->clear();
		}

		//=====     Element access     =====

		reference front() {
			return reinterpret_cast<node_data*>(m_rend->m_next)->m_val;
		}
		const_reference front() const {
			return reinterpret_cast<node_data*>(m_rend->m_next)->m_val;
		}

		reference back() {
			return reinterpret_cast<node_data*>(m_end->m_prev)->m_val;
		}
		const_reference back() const {
			return reinterpret_cast<node_data*>(m_end->m_prev)->m_val;
		}

		//=====     Iterators      =====

		iterator begin() noexcept {
			return { this, m_rend->m_next };
		}
		const_iterator begin() const noexcept {
			return { this, m_rend->m_next };
		}
		const_iterator cbegin() const noexcept {
			return begin();
		}

		iterator end() {
			return { this, m_end };
		}
		const_iterator end() const noexcept {
			return { this, m_end };
		}
		const_iterator cend() const noexcept {
			return end();
		}

		reverse_iterator rbegin() noexcept {
			return std::reverse_iterator(iterator{ this, m_end->m_prev });
		}
		const_reverse_iterator rbegin() const noexcept {
			return std::reverse_iterator(const_iterator{ this, m_end->m_prev });
		}
		const_reverse_iterator crbegin() const noexcept {
			return rbegin();
		}

		reverse_iterator rend() noexcept {
			return std::reverse_iterator(iterator{ this, m_rend });
		}
		const_reverse_iterator rend() const noexcept {
			return std::reverse_iterator(const_iterator{ this, m_rend });
		}
		const_reverse_iterator crend() const noexcept {
			return rend();
		}

		//=====     Capacity      =====

		bool empty() const {
			return m_rend->m_next == nullptr;
		}

		int size() const {
			return m_size;
		}

		//=====     Modifiers       =====

		void clear() noexcept {
			while (m_rend->m_next) {
				this->pop_back();
			}
		}

		iterator insert(const_iterator pos, const T& value) {
			return emplace(pos, value);
		}

		iterator insert(const_iterator pos, T&& value) {
			return emplace(pos, std::move(value));
		}

		iterator insert(const_iterator pos, size_type count, const T& value) {
			if (!count) {
				return pos;
			}
			else {
				auto p = emplace(pos, value);
				--count;
				while (count > 0) {
					emplace(pos, value);
					--count;
				}
				return p;
			}
		}

		template<std::input_iterator InputIt>
		iterator insert(const_iterator pos, InputIt first, InputIt last) {
			if (first == last) {
				return pos;
			}
			else {
				auto p = emplace(pos, *first);
				++first;
				while (first != last) {
					emplace(pos, *first);
				}
				return p;
			}
		}

		iterator insert(const_iterator pos, std::initializer_list<T> ilist) {
			if (!ilist.size()) {
				return pos;
			}
			else {
				auto begin_ = ilist.begin();
				auto end_ = ilist.end();
				auto p = emplace(pos, *begin_);
				++begin_;
				while (begin_ != end_) {
					emplace(pos, *begin_);
					++begin_;
				}
				return p;
			}
		}

		template< class... Args >
		iterator emplace(const_iterator pos, Args&&... args) {
			if (pos.m_parrent == this) {
				node* new_node = new node_data{ std::forward<Args>(args)... };
				if (!m_rend->m_next) {
					m_rend->m_next = new_node;
					new_node->m_prev = m_rend;

					m_end->m_prev = new_node;
					new_node->m_next = m_end;
				}
				else {
					new_node->m_prev = pos.m_ptrNode->m_prev;
					new_node->m_next = pos.m_ptrNode;

					new_node->m_prev->m_next = new_node;
					new_node->m_next->m_prev = new_node;
				}
				++m_size;
				return { this, new_node };
			}
			else {
				//TODO: Throw exception
			}
		}

		iterator erase(const_iterator pos) {
			if (pos.m_parrent == this && pos.m_ptrNode != this->m_end && pos.m_ptrNode != this->m_rend) {
				if (m_rend->m_next == m_end->m_prev) {
					delete m_rend->m_next;
					m_rend->m_next = nullptr;
					m_end->m_prev = nullptr;
				}
			}
			else {

			}
		}
		iterator erase(const_iterator first, const_iterator last);

		template< class... Args >
		reference emplace_back(Args&&... args) {
			return *emplace(end(), std::forward<Args>(args)...);
		}

		template< class... Args >
		reference emplace_front(Args&&... args) {
			return *emplace(begin(), std::forward<Args>(args)...);
		}

		void push_front(T&& value) {
			emplace(begin(), std::move(value));
		}

		void push_front(const T& value) {
			emplace(begin(), value);
		}

		void pop_front() {
			if (m_rend->m_next) {
				if (m_rend->m_next == m_end->m_prev) {
					delete reinterpret_cast<node_data*>(m_rend->m_next);
					m_rend->m_next = nullptr;
					m_end->m_prev = nullptr;
				}
				else {
					m_rend->m_next = m_rend->m_next->m_next;

					m_rend->m_next->m_prev->m_next = nullptr;
					m_rend->m_next->m_prev->m_prev = nullptr;

					delete reinterpret_cast<node_data*>(m_rend->m_next->m_prev);

					m_rend->m_next->m_prev = m_rend;
				}
				--m_size;
			}
		}

		void push_back(T&& value) {
			emplace(end(), std::move(value));
		}

		void push_back(const T& value) {
			emplace(end(), value);
		}

		void pop_back() {
			if (m_end->m_prev) {
				if (m_rend->m_next == m_end->m_prev) {
					delete reinterpret_cast<node_data*>(m_end->m_prev);
					m_rend->m_next = nullptr;
					m_end->m_prev = nullptr;
				}
				else {
					m_end->m_prev = m_end->m_prev->m_prev;

					m_end->m_prev->m_next->m_next = nullptr;
					m_end->m_prev->m_next->m_prev = nullptr;

					delete reinterpret_cast<node_data*>(m_end->m_prev->m_next);

					m_end->m_prev->m_next = m_end;
				}
				--m_size;
			}
		}

		//=====     Operations       =====

		template<class Compare>
		void sort(Compare comp) {
			sort_impl(m_rend->m_next, m_size, comp);
		}

		void sort() {
			sort(std::less<>{});
		}

		void reverse() {
			if (m_size <= 1) {
				return;
			}
			//TODO
		}

	private:

		template<class Compare>
		static node_data* sort_impl(node_data* first, const size_type size_, Compare comp) {

			switch (size_) {
			case 0:
				return first;
			case 1:
				return first->m_next;
			default:
				break;
			}

			auto mid = sort_impl(first, size_ / 2, comp);
			const auto last = sort_impl(mid, size_ - size_ / 2, comp);
			first = merge_same(first, mid, last, comp);
			return last;
		}

	private:
		node* m_rend{};
		node* m_end{};
		int m_size{};
	};
}