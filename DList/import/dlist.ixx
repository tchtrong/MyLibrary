module;
#include <iterator>
#include <initializer_list>
#include <utility>

export module dlist;

export namespace MyLib {
    template <class T>
    class DList {

        using value_type = T;
        using size_type = std::size_t;
        using difference_type = std::ptrdiff_t;
        using reference = value_type&;
        using const_reference = const value_type&;
        using pointer = value_type*;
        using const_pointer = const value_type*;

    private:
        //==================================================
        //================     Node    =====================
        //==================================================
        struct node {
            node() = default;
            explicit node(const T& value) :m_val{ value } {};
            explicit node(T&& value) :m_val{ std::move(value) } {};
            T m_val{};
            node* m_prev{};
            node* m_next{};
        };

    public:
        //==================================================
        //================     Iterator    =================
        //==================================================
        class iterator : public std::bidirectional_iterator_tag {
        public:
            iterator() = default;

            using value_type = DList<T>::value_type;
            using difference_type = DList<T>::difference_type;
            using reference = DList<T>::reference;
            using pointer = DList<T>::pointer;

            bool operator==(const iterator& rhs) const {
                return m_ptrNode == rhs.m_ptrNode;
            }

            reference operator*() const {
                return m_ptrNode->m_val;
            }
            pointer operator->() const {
                return &(m_ptrNode->m_val);
            }

            iterator& operator++() {
                if (m_parrent && !is_end) {
                    if (is_rend) {
                        is_rend = false;
                        m_ptrNode = m_parrent->m_tail;
                    }
                    else {
                        if (m_ptrNode == m_parrent->m_tail) {
                            is_end = true;
                            m_ptrNode = nullptr;
                        }
                        else {
                            m_ptrNode = m_ptrNode->m_prev;
                        }
                    }
                }
                return *this;
            }
            iterator operator++(int) {
                iterator p = *this;
                ++(*this);
                return p;
            }

            iterator& operator--() {
                if (m_parrent && !is_rend) {
                    if (is_end) {
                        is_end = false;
                        m_ptrNode = m_parrent->m_tail;
                    }
                    else {
                        if (m_ptrNode == m_parrent->m_head) {
                            is_rend = true;
                            m_ptrNode = nullptr;
                        }
                        else {
                            m_ptrNode = m_ptrNode->m_prev;
                        }
                    }
                }
                return *this;
            }
            iterator operator--(int) {
                iterator p = *this;
                --(*this);
                return p;
            }

        private:
            friend DList<T>;

            iterator(bool is_rend_, bool is_end_, DList<T>* parrent) 
                : is_rend{ is_rend_ }, is_end{ is_end_ }, m_parrent{ parrent }{}

            iterator(DList<T>* parrent, node* value) : m_parrent{ parrent }, m_ptrNode{ value }{}

            bool is_rend = false;
            bool is_end = false;

            DList<T>* m_parrent;
            node* m_ptrNode{};
        };
        using const_iterator = typename DList<const value_type>::iterator;
        using reverse_iterator = std::reverse_iterator<iterator>;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    public:
        //==================================================
        //=====             Member functions           =====
        //==================================================

        //=====     Constructor     =====

        DList() = default;

        DList(size_type count, const T& value) {
            while (count > 0) {
                this->push_back(value);
                --count;
            }
        }

        explicit DList(size_type count) {
            while (count > 0) {
                this->push_back(T());
                --count;
            }
        }

        template<std::input_iterator InputIt>
        DList(InputIt first, InputIt last) {
            while (first != last) {
                this->push_back(*first);
                ++first;
            }
        }

        DList(const DList& other) {
            for (const auto& x : other) {
                this->push_back(x);
            }
        }

        DList(DList&& other) noexcept {
            for (auto&& x : other) {
                this->push_back(std::move(x));
            }
        }

        DList(std::initializer_list<T> init) {
            for (auto x : init) {
                this->push_back(std::move(x));
            }
        }

        ////=====     Assignment operator     =====

        //DList& operator=(const DList& other);

        //DList& operator=(DList&& other) noexcept;

        //DList& operator=(std::initializer_list<T> ilist);

        ////=====     Assignment functions     =====

        //void assign(size_type count, const T& value);

        //template< class InputIt >
        //void assign(InputIt first, InputIt last);

        //void assign(std::initializer_list<T> ilist);

        //=====     Destructor     =====

        ~DList() {
            this->clear();
        }

        //=====     Element access     =====

        reference front() {
            return m_head->m_val;
        }
        const_reference front() const {
            return m_head->m_val;
        }

        reference back() {
            return m_tail->m_val;
        }
        const_reference back() const {
            return m_tail->m_val;
        }

        //=====     Iterators      =====

        iterator begin() noexcept {
            return { m_head, this };
        }
        const_iterator begin() const noexcept {
            return { m_head, this };
        }
        const_iterator cbegin() const noexcept {
            return begin();
        }

        iterator end() {
            return { false, true, this };
        }
        const_iterator end() const noexcept {
            return { false, true, this };
        }
        const_iterator cend() const noexcept {
            return end();
        }

        reverse_iterator rbegin() noexcept {
            return { m_tail, this };
        }
        const_reverse_iterator rbegin() const noexcept {
            return { m_tail, this };
        }
        const_reverse_iterator crbegin() const noexcept {
            return rbegin();
        }

        reverse_iterator rend() noexcept {
            return { true, false, this };
        }
        const_reverse_iterator rend() const noexcept {
            return { true, false, this };
        }
        const_reverse_iterator crend() const noexcept {
            return rend();
        }

        //=====     Capacity      =====

        bool empty() const {
            return m_head == nullptr;
        }

        int size() const {
            return m_size;
        }

        //=====     Modifiers       =====

        void clear() noexcept {
            if (m_head) {
                while (m_head != m_tail) {
                    node* p = m_head->m_next;

                    p->m_prev = nullptr;
                    m_head->m_next = nullptr;

                    delete m_head;
                    m_head = p;
                }
                delete m_head;
                m_head = nullptr;
                m_tail = nullptr;
            }
        }

        iterator insert(const_iterator pos, const T& value) {
            
        }

        iterator insert(const_iterator pos, T&& value);

        iterator insert(const_iterator pos, size_type count, const T& value);

        template<std::input_iterator InputIt>
        iterator insert(const_iterator pos, InputIt first, InputIt last);

        iterator insert(const_iterator pos, std::initializer_list<T> ilist);

        template< class... Args >
        iterator emplace(const_iterator pos, Args&&... args);

        void push_front(T&& value) {
            if (m_head == nullptr) {
                m_head = new node(std::move(value));
                m_tail = m_head;
            }
            else {
                m_head->m_prev = new node(std::move(value));
                m_head->m_prev->m_next = m_head;
                m_head = m_head->m_prev;
            }

            ++m_size;
        }

        void push_front(const T& value) {
            T tmp{ value };
            this->push_front(std::move(tmp));
        }

        void pop_front() {
            node* p = m_head;
            if (m_head->m_next) {
                m_head = m_head->m_next;
                m_head->m_prev = nullptr;
            }
            else {
                m_head = nullptr;
                m_tail = nullptr;
            }
            --m_size;

            delete p;
        }

        void push_back(T&& value) {
            if (m_head == nullptr) {
                m_head = new node(std::move(value));
                m_tail = m_head;
            }
            else {
                m_tail->m_next = new node(std::move(value));
                m_tail->m_next->m_prev = m_tail;
                m_tail = m_tail->m_next;
            }

            ++m_size;
        }

        void push_back(const T& value) {
            T tmp{ value };
            this->push_back(std::move(tmp));
        }

        void pop_back() {
            node* p = m_tail;
            if (m_tail->m_prev) {
                m_tail = m_tail->m_prev;
                m_tail->m_next = nullptr;
            }
            else {
                m_head = nullptr;
                m_tail = nullptr;
            }
            --m_size;

            delete p;
        }

        //=====     Operations       =====

        void sort(bool ascending = true) {
            _ascending();
            if (!ascending) {
                reverse();
            }
        }

        void reverse() {
            if (m_size <= 1) {
                return;
            }

            for (node* i = m_head; i; i = i->m_prev) {
                std::swap(i->m_prev, i->m_next);
            }
            std::swap(m_head, m_tail);
        }

        //=====     Misc       =====

        bool is_found(const T& val) const {
            for (node* i = m_head; i; i = i->m_next) {
                if (i->m_val == val) {
                    return true;
                }
            }
            return false;
        }

    private:
        void _ascending() {
            if (m_size <= 1) {
                return;
            }

            node* left = m_head;
            node* right = m_tail;
            node* k = left;

            while (left != right)
            {
                for (node* i = left; i != right; i = i->m_next) {
                    if (i->m_val > i->m_next->m_val)
                    {
                        std::swap(i->m_val, i->m_next->m_val);
                        k = i;
                    }
                }

                for (node* i = right = k; i != left; i = i->m_prev) {
                    if (i->m_val < i->m_prev->m_val)
                    {
                        std::swap(i->m_val, i->m_prev->m_val);
                        k = i;
                    }
                }

                left = k;
            }
        }

        //void _descending();

    private:
        node* m_head{};
        node* m_tail{};
        int m_size{};
    };
}