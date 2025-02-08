#pragma once
#include <iostream>
#include <stdexcept>
#include <initializer_list>
#include <algorithm>
#include <memory>
#include <limits>
#include <iterator>

/**
 * @brief A doubly linked list implementation.
 *
 * @tparam T The type of the elements in the list.
 */
template<typename T>
class ListN
{
private:
    /**
     * @brief A node in the doubly linked list.
     */
    struct Node
    {
        T data;       ///< The data stored in the node.
        Node* prev;   ///< Pointer to the previous node.
        Node* next;   ///< Pointer to the next node.

        /**
         * @brief Constructs a node with the given value.
         *
         * @param value The value to store in the node.
         */
        Node(const T& value) : data(value), prev(nullptr), next(nullptr) {}
    };

    Node* m_head;          ///< Pointer to the head of the list.
    Node* m_tail;          ///< Pointer to the tail of the list.
    std::size_t m_size;    ///< The number of elements in the list.

    /**
     * @brief Deletes all nodes in the list.
     */
    void delete_all()
    {
        while (!empty())
            pop_back();
    }

public:
    using value_type = T;  ///< The type of the elements in the list.

    /**
     * @brief An iterator for the list.
     */
    class iterator
    {
    public:
        using value_type = T;          ///< The type of the elements in the list.
        using reference = value_type&; ///< Reference to an element.
        using pointer = value_type*;   ///< Pointer to an element.

        /**
         * @brief Constructs an iterator pointing to nullptr.
         */
        iterator() : m_node(nullptr) {}

        /**
         * @brief Constructs an iterator pointing to the given node.
         *
         * @param node The node to point to.
         */
        explicit iterator(Node* node) : m_node(node) {}

        /**
         * @brief Dereferences the iterator.
         *
         * @return Reference to the element pointed to by the iterator.
         */
        reference operator*() const
        {
            return m_node->data;
        }

        /**
         * @brief Dereferences the iterator.
         *
         * @return Pointer to the element pointed to by the iterator.
         */
        pointer operator->() const
        {
            return &(m_node->data);
        }

        /**
         * @brief Pre-increment operator.
         *
         * @return Reference to the incremented iterator.
         */
        iterator& operator++()
        {
            m_node = m_node->next;
            return *this;
        }

        /**
         * @brief Post-increment operator.
         *
         * @return Copy of the iterator before incrementing.
         */
        iterator operator++(int)
        {
            iterator tmp(*this);
            m_node = m_node->next;
            return tmp;
        }

        /**
         * @brief Pre-decrement operator.
         *
         * @return Reference to the decremented iterator.
         */
        iterator& operator--()
        {
            if (!m_node)
                throw std::out_of_range("Cannot decrement iterator when m_node is null.");
            if (!m_node->prev)
                throw std::out_of_range("Cannot decrement iterator at the beginning of the list.");
            m_node = m_node->prev;
            return *this;
        }

        /**
         * @brief Post-decrement operator.
         *
         * @return Copy of the iterator before decrementing.
         */
        iterator operator--(int)
        {
            iterator tmp(*this);
            m_node = m_node->prev;
            return tmp;
        }

        /**
         * @brief Equality comparison operator.
         *
         * @param other The iterator to compare with.
         * @return True if the iterators point to the same node, false otherwise.
         */
        bool operator==(const iterator& other) const
        {
            return m_node == other.m_node;
        }

        /**
         * @brief Inequality comparison operator.
         *
         * @param other The iterator to compare with.
         * @return True if the iterators do not point to the same node, false otherwise.
         */
        bool operator!=(const iterator& other) const
        {
            return m_node != other.m_node;
        }

    private:
        Node* m_node; ///< Pointer to the current node.
        friend class const_iterator;
        friend class ListN;
    };

    /**
     * @brief A constant iterator for the list.
     */
    class const_iterator
    {
    public:
        using value_type = T;          ///< The type of the elements in the list.
        using reference = const value_type&; ///< Reference to an element.
        using pointer = const value_type*;   ///< Pointer to an element.

        /**
         * @brief Constructs a constant iterator pointing to nullptr.
         */
        const_iterator() : m_node(nullptr) {}

        /**
         * @brief Constructs a constant iterator pointing to the given node.
         *
         * @param node The node to point to.
         */
        explicit const_iterator(const Node* node) : m_node(node) {}

        /**
         * @brief Constructs a constant iterator from a non-constant iterator.
         *
         * @param it The non-constant iterator to copy from.
         */
        const_iterator(const iterator& it) : m_node(it.m_node) {}

        /**
         * @brief Dereferences the iterator.
         *
         * @return Reference to the element pointed to by the iterator.
         */
        reference operator*() const
        {
            return m_node->data;
        }

        /**
         * @brief Dereferences the iterator.
         *
         * @return Pointer to the element pointed to by the iterator.
         */
        pointer operator->() const
        {
            return &(m_node->data);
        }

        /**
         * @brief Pre-increment operator.
         *
         * @return Reference to the incremented iterator.
         */
        const_iterator& operator++()
        {
            m_node = m_node->next;
            return *this;
        }

        /**
         * @brief Post-increment operator.
         *
         * @return Copy of the iterator before incrementing.
         */
        const_iterator operator++(int)
        {
            const_iterator tmp(*this);
            m_node = m_node->next;
            return tmp;
        }

        /**
         * @brief Pre-decrement operator.
         *
         * @return Reference to the decremented iterator.
         */
        const_iterator& operator--()
        {
            m_node = m_node->prev;
            return *this;
        }

        /**
         * @brief Post-decrement operator.
         *
         * @return Copy of the iterator before decrementing.
         */
        const_iterator operator--(int)
        {
            const_iterator tmp(*this);
            m_node = m_node->prev;
            return tmp;
        }

        /**
         * @brief Equality comparison operator.
         *
         * @param other The iterator to compare with.
         * @return True if the iterators point to the same node, false otherwise.
         */
        bool operator==(const const_iterator& other) const
        {
            return m_node == other.m_node;
        }

        /**
         * @brief Inequality comparison operator.
         *
         * @param other The iterator to compare with.
         * @return True if the iterators do not point to the same node, false otherwise.
         */
        bool operator!=(const const_iterator& other) const
        {
            return m_node != other.m_node;
        }

    private:
        const Node* m_node; ///< Pointer to the current node.
    };

    /**
     * @brief Constructs an empty list.
     */
    ListN() : m_head(nullptr), m_tail(nullptr), m_size(0) {}

    /**
     * @brief Constructs a list with elements from an initializer list.
     *
     * @param init The initializer list to copy elements from.
     */
    ListN(const std::initializer_list<T>& init)
        : m_head(nullptr), m_tail(nullptr), m_size(0)
    {
        for (const auto& val : init)
            push_back(val);
    }

    /**
     * @brief Copy constructor.
     *
     * @param other The list to copy elements from.
     */
    ListN(const ListN& other) : m_head(nullptr), m_tail(nullptr), m_size(0)
    {
        for (auto it = other.begin(); it != other.end(); ++it)
            push_back(*it);
    }

    /**
     * @brief Copy assignment operator.
     *
     * @param other The list to copy elements from.
     * @return Reference to the assigned list.
     */
    ListN& operator=(const ListN& other)
    {
        if (this != &other)
        {
            clear();
            for (auto it = other.begin(); it != other.end(); ++it)
                push_back(*it);
        }
        return *this;
    }

    /**
     * @brief Destructor.
     */
    ~ListN()
    {
        clear();
    }

    /**
     * @brief Returns a reference to the first element in the list.
     *
     * @return Reference to the first element.
     * @throws std::out_of_range If the list is empty.
     */
    T& front()
    {
        if (empty())
            throw std::out_of_range("ListN is empty (front)");
        return m_head->data;
    }

    /**
     * @brief Returns a constant reference to the first element in the list.
     *
     * @return Constant reference to the first element.
     * @throws std::out_of_range If the list is empty.
     */
    const T& front() const
    {
        if (empty())
            throw std::out_of_range("ListN is empty (front const)");
        return m_head->data;
    }

    /**
     * @brief Returns a reference to the last element in the list.
     *
     * @return Reference to the last element.
     * @throws std::out_of_range If the list is empty.
     */
    T& back()
    {
        if (empty())
            throw std::out_of_range("ListN is empty (back)");
        return m_tail->data;
    }

    /**
     * @brief Returns a constant reference to the last element in the list.
     *
     * @return Constant reference to the last element.
     * @throws std::out_of_range If the list is empty.
     */
    const T& back() const
    {
        if (empty())
            throw std::out_of_range("ListN is empty (back const)");
        return m_tail->data;
    }

    /**
     * @brief Returns an iterator to the beginning of the list.
     *
     * @return Iterator to the beginning of the list.
     */
    iterator begin()
    {
        return iterator(m_head);
    }

    /**
     * @brief Returns a constant iterator to the beginning of the list.
     *
     * @return Constant iterator to the beginning of the list.
     */
    const_iterator begin() const
    {
        return const_iterator(m_head);
    }

    /**
     * @brief Returns a constant iterator to the beginning of the list.
     *
     * @return Constant iterator to the beginning of the list.
     */
    const_iterator cbegin() const
    {
        return const_iterator(m_head);
    }

    /**
     * @brief Returns an iterator to the end of the list.
     *
     * @return Iterator to the end of the list.
     */
    iterator end()
    {
        return iterator(nullptr);
    }

    /**
     * @brief Returns a constant iterator to the end of the list.
     *
     * @return Constant iterator to the end of the list.
     */
    const_iterator end() const
    {
        return const_iterator(nullptr);
    }

    /**
     * @brief Returns a constant iterator to the end of the list.
     *
     * @return Constant iterator to the end of the list.
     */
    const_iterator cend() const
    {
        return const_iterator(nullptr);
    }

    /**
     * @brief Returns the number of elements in the list.
     *
     * @return The number of elements in the list.
     */
    std::size_t capacity() const
    {
        return m_size;
    }

    /**
     * @brief Returns the maximum number of elements the list can hold.
     *
     * @return The maximum number of elements the list can hold.
     */
    std::size_t max_size() const
    {
        return std::numeric_limits<std::size_t>::max();
    }

    /**
     * @brief Checks if the list is empty.
     *
     * @return True if the list is empty, false otherwise.
     */
    bool empty() const
    {
        return (m_size == 0);
    }

    /**
     * @brief Returns the number of elements in the list.
     *
     * @return The number of elements in the list.
     */
    std::size_t size() const
    {
        return m_size;
    }

    /**
     * @brief Clears the list, deleting all elements.
     */
    void clear()
    {
        delete_all();
        m_head = m_tail = nullptr;
        m_size = 0;
    }

    /**
     * @brief Adds an element to the front of the list.
     *
     * @param value The value to add.
     */
    void push_front(const T& value)
    {
        Node* node = new Node(value);
        node->next = m_head;
        node->prev = nullptr;

        if (m_head)
            m_head->prev = node;
        else
            m_tail = node;

        m_head = node;
        ++m_size;
    }

    /**
     * @brief Adds an element to the back of the list.
     *
     * @param value The value to add.
     */
    void push_back(const T& value)
    {
        Node* node = new Node(value);
        node->prev = m_tail;
        node->next = nullptr;

        if (m_tail)
            m_tail->next = node;
        else
            m_head = node;

        m_tail = node;
        ++m_size;
    }

    /**
     * @brief Removes the first element from the list.
     */
    void pop_front()
    {
        if (empty())
            return;

        Node* old_head = m_head;
        m_head = m_head->next;

        if (m_head)
            m_head->prev = nullptr;
        else
            m_tail = nullptr;

        delete old_head;
        --m_size;
    }

    /**
     * @brief Removes the last element from the list.
     */
    void pop_back()
    {
        if (empty())
            return;

        Node* old_tail = m_tail;
        m_tail = m_tail->prev;

        if (m_tail)
            m_tail->next = nullptr;
        else
            m_head = nullptr;

        delete old_tail;
        --m_size;
    }

    /**
     * @brief Inserts an element at the specified position.
     *
     * @param pos The position to insert the element at.
     * @param value The value to insert.
     * @return An iterator pointing to the inserted element.
     */
    iterator insert(iterator pos, const T& value)
    {
        if (pos == begin()) {
            push_front(value);
            return begin();
        }
        else if (pos == end()) {
            push_back(value);
            iterator it = end();
            --it;
            return it;
        }
        else {
            Node* current = pos.m_node;
            Node* newNode = new Node(value);
            newNode->prev = current->prev;
            newNode->next = current;
            current->prev->next = newNode;
            current->prev = newNode;
            ++m_size;
            return iterator(newNode);
        }
    }

    /**
     * @brief Inserts a range of elements at the specified position.
     *
     * @tparam InputIt The type of the input iterator.
     * @param pos The position to insert the elements at.
     * @param first The beginning of the range to insert.
     * @param last The end of the range to insert.
     */
    template<typename InputIt>
    void insert_range(iterator pos, InputIt first, InputIt last)
    {
        for (; first != last; ++first)
        {
            pos = insert(pos, *first);
            ++pos;
        }
    }

    /**
     * @brief Constructs and inserts an element at the specified position.
     *
     * @param pos The position to insert the element at.
     * @param value The value to insert.
     * @return An iterator pointing to the inserted element.
     */
    iterator emplace(iterator pos, const T& value)
    {
        return insert(pos, value);
    }

    /**
     * @brief Erases the element at the specified position.
     *
     * @param pos The position of the element to erase.
     * @return An iterator pointing to the element following the erased element.
     * @throws std::out_of_range If the position is the end iterator.
     */
    iterator erase(iterator pos)
    {
        if (pos == end())
            throw std::out_of_range("ListN erase: cannot erase end iterator");

        Node* target = pos.m_node;
        iterator ret(target->next);

        if (target->prev)
            target->prev->next = target->next;
        else
            m_head = target->next;

        if (target->next)
            target->next->prev = target->prev;
        else
            m_tail = target->prev;

        delete target;
        --m_size;
        return ret;
    }

    /**
     * @brief Constructs and adds an element to the back of the list.
     *
     * @param value The value to add.
     */
    void emplace_back(const T& value)
    {
        push_back(value);
    }

    /**
     * @brief Constructs and adds an element to the front of the list.
     *
     * @param value The value to add.
     */
    void emplace_front(const T& value)
    {
        push_front(value);
    }

    /**
     * @brief Swaps the contents of this list with another list.
     *
     * @param other The list to swap contents with.
     */
    void swap(ListN& other)
    {
        std::swap(m_head, other.m_head);
        std::swap(m_tail, other.m_tail);
        std::swap(m_size, other.m_size);
    }

    /**
     * @brief Appends a range of elements to the end of the list.
     *
     * @tparam InputIt The type of the input iterator.
     * @param first The beginning of the range to append.
     * @param last The end of the range to append.
     */
    template<typename InputIt>
    void append_range(InputIt first, InputIt last)
    {
        for (; first != last; ++first)
            push_back(*first);
    }

    /**
     * @brief Resizes the list to the specified size.
     *
     * @param new_size The new size of the list.
     * @param value The value to initialize new elements with (if any).
     */
    void resize(std::size_t new_size, const T& value = T())
    {
        if (new_size < m_size) {
            while (m_size > new_size)
                pop_back();
        }
        else if (new_size > m_size) {
            while (m_size < new_size)
                push_back(value);
        }
    }

    /**
     * @brief Assigns the specified number of elements to the list.
     *
     * @param count The number of elements to assign.
     * @param value The value to assign to each element.
     */
    void assign(std::size_t count, const T& value)
    {
        clear();
        for (std::size_t i = 0; i < count; ++i)
            push_back(value);
    }

    /**
     * @brief Assigns a range of elements to the list.
     *
     * @tparam InputIt The type of the input iterator.
     * @param first The beginning of the range to assign.
     * @param last The end of the range to assign.
     */
    template<typename InputIt>
    void assign_range(InputIt first, InputIt last)
    {
        clear();
        for (auto it = first; it != last; ++it)
            push_back(*it);
    }
};

/**
 * @brief Outputs the contents of the list to the given output stream.
 *
 * @tparam T The type of the elements in the list.
 * @param os The output stream to write to.
 * @param lst The list to output.
 * @return The output stream.
 */
template<typename T>
std::ostream& operator<<(std::ostream& os, const ListN<T>& lst)
{
    os << "{";
    auto it = lst.begin();
    while (it != lst.end())
    {
        os << *it;
        ++it;
        if (it != lst.end())
            os << ", ";
    }
    os << "}";
    return os;
}

