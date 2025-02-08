#pragma once
#include <stdexcept>
#include <utility>
#include <cstddef>
#include <functional>

/**
 * @brief Structure representing a hook for an intrusive list.
 */
struct IntrusiveListHook
{
    IntrusiveListHook* prev{ nullptr }; ///< Pointer to the previous element.
    IntrusiveListHook* next{ nullptr }; ///< Pointer to the next element.

    /**
     * @brief Checks if the element is linked in a list.
     * @return true if the element is linked, false otherwise.
     */
    bool is_linked() const
    {
        return (prev != nullptr || next != nullptr);
    }
};

/**
 * @brief Structure representing a node containing data and a hook for an intrusive list.
 */
struct Node
{
    IntrusiveListHook hook; ///< Hook for the intrusive list.
    int data; ///< Data of the node.

    /**
     * @brief Constructor to initialize the node with data.
     * @param d Data to initialize.
     */
    Node(int d) : data(d) {}

    /**
     * @brief Equality operator to compare two nodes.
     * @param other Other node to compare.
     * @return true if the data of the nodes are equal, false otherwise.
     */
    bool operator==(const Node& other) const
    {
        return data == other.data;
    }
};

/**
 * @brief Template class for an iterator of an intrusive list.
 * @tparam T Type of the elements in the list.
 * @tparam HookPtr Pointer to the hook of the intrusive list in the elements.
 */
template<typename T, IntrusiveListHook T::* HookPtr>
class IntrusiveListIterator
{
public:
    using value_type = T; ///< Type of the elements in the list.
    using reference = value_type&; ///< Reference to an element.
    using pointer = value_type*; ///< Pointer to an element.

    /**
     * @brief Default constructor initializing the iterator to nullptr.
     */
    IntrusiveListIterator() : m_node(nullptr)
    {}

    /**
     * @brief Constructor initializing the iterator with a list hook.
     * @param node Pointer to the list hook.
     */
    explicit IntrusiveListIterator(IntrusiveListHook* node) : m_node(node)
    {}

    /**
     * @brief Dereference operator.
     * @return Reference to the element pointed by the iterator.
     */
    reference operator*() const
    {
        return *get_value(m_node);
    }

    /**
     * @brief Pointer dereference operator.
     * @return Pointer to the element pointed by the iterator.
     */
    pointer operator->() const
    {
        return get_value(m_node);
    }

    /**
     * @brief Prefix increment operator.
     * @return Reference to the incremented iterator.
     */
    IntrusiveListIterator& operator++()
    {
        if (m_node)
            m_node = m_node->next;
        return *this;
    }

    /**
     * @brief Postfix increment operator.
     * @return Iterator before the increment.
     */
    IntrusiveListIterator operator++(int)
    {
        IntrusiveListIterator tmp(*this);
        ++(*this);
        return tmp;
    }

    /**
     * @brief Prefix decrement operator.
     * @return Reference to the decremented iterator.
     */
    IntrusiveListIterator& operator--()
    {
        if (m_node)
            m_node = m_node->prev;
        return *this;
    }

    /**
     * @brief Postfix decrement operator.
     * @return Iterator before the decrement.
     */
    IntrusiveListIterator operator--(int)
    {
        IntrusiveListIterator tmp(*this);
        --(*this);
        return tmp;
    }

    /**
     * @brief Equality operator.
     * @param other Other iterator to compare.
     * @return true if the iterators are equal, false otherwise.
     */
    bool operator==(const IntrusiveListIterator& other) const
    {
        return m_node == other.m_node;
    }

    /**
     * @brief Inequality operator.
     * @param other Other iterator to compare.
     * @return true if the iterators are different, false otherwise.
     */
    bool operator!=(const IntrusiveListIterator& other) const
    {
        return !(*this == other);
    }

    /**
     * @brief Gets the list hook pointed by the iterator.
     * @return Pointer to the list hook.
     */
    IntrusiveListHook* getNode() const
    {
        return m_node;
    }

    /**
     * @brief Gets the value of the element from the list hook.
     * @param hook Pointer to the list hook.
     * @return Pointer to the element.
     */
    static pointer get_value(IntrusiveListHook* hook)
    {
        if (!hook) return nullptr;

        std::size_t offset =
            reinterpret_cast<std::size_t>(
                &reinterpret_cast<const volatile char&>((((T*)0)->*HookPtr))  // Take this part on Internet
                );

        char* rawPtr = reinterpret_cast<char*>(hook) - offset;
        return reinterpret_cast<pointer>(rawPtr);
    }

private:
    IntrusiveListHook* m_node; ///< Pointer to the current list hook.

};



/**
 * @brief Template class for an intrusive list.
 * @tparam T Type of the elements in the list.
 * @tparam HookPtr Pointer to the hook of the intrusive list in the elements.
 */
template<typename T, IntrusiveListHook T::* HookPtr>
class IntrusiveList
{
public:
    using value_type = T; ///< Type of the elements in the list.
    using size_type = std::size_t; ///< Type for the size of the list.
    using iterator = IntrusiveListIterator<T, HookPtr>; ///< Type for the list iterators.
    using const_iterator = IntrusiveListIterator<const T, HookPtr>; ///< Type for the constant list iterators.

    /**
     * @brief Default constructor initializing an empty list.
     */
    IntrusiveList() : m_head(nullptr), m_tail(nullptr), m_size(0) {}

    IntrusiveList(const IntrusiveList&) = delete; ///< Delete copy constructor.
    IntrusiveList& operator=(const IntrusiveList&) = delete; ///< Delete copy assignment operator.

    /**
     * @brief Destructor of the intrusive list.
     */
    ~IntrusiveList()
    {
        clear();
    }

    /**
     * @brief Gets an iterator to the beginning of the list.
     * @return Iterator to the beginning of the list.
     */
    iterator begin()
    {
        return iterator(m_head);
    }

    /**
     * @brief Gets an iterator to the end of the list.
     * @return Iterator to the end of the list.
     */
    iterator end()
    {
        return iterator(nullptr);
    }

    /**
     * @brief Gets a constant iterator to the beginning of the list.
     * @return Constant iterator to the beginning of the list.
     */
    const_iterator begin() const
    {
        return const_iterator(m_head);
    }

    /**
     * @brief Gets a constant iterator to the end of the list.
     * @return Constant iterator to the end of the list.
     */
    const_iterator end() const
    {
        return const_iterator(nullptr);
    }

    /**
     * @brief Gets a reference to the first element of the list.
     * @return Reference to the first element of the list.
     * @throws std::runtime_error if the list is empty.
     */
    T& front()
    {
        if (empty())
            throw std::runtime_error("front(): List is empty");
        return *begin();
    }

    /**
     * @brief Gets a constant reference to the first element of the list.
     * @return Constant reference to the first element of the list.
     * @throws std::runtime_error if the list is empty.
     */
    const T& front() const
    {
        if (empty())
            throw std::runtime_error("front(): List is empty");
        return *begin();
    }

    /**
     * @brief Gets a reference to the last element of the list.
     * @return Reference to the last element of the list.
     * @throws std::runtime_error if the list is empty.
     */
    T& back()
    {
        if (empty())
            throw std::runtime_error("back(): List is empty");
        return *iterator::get_value(m_tail);
    }

    /**
     * @brief Gets a constant reference to the last element of the list.
     * @return Constant reference to the last element of the list.
     * @throws std::runtime_error if the list is empty.
     */
    const T& back() const
    {
        if (empty())
            throw std::runtime_error("back(): List is empty");
        return *iterator::get_value(m_tail);
    }

    /**
     * @brief Checks if the list is empty.
     * @return true if the list is empty, false otherwise.
     */
    bool empty() const
    {
        return (m_size == 0);
    }

    /**
     * @brief Gets the size of the list.
     * @return Size of the list.
     */
    size_type size() const
    {
        return m_size;
    }

    /**
     * @brief Clears the list.
     */
    void clear()
    {
        while (!empty())
            pop_back();
    }

    /**
     * @brief Adds an element to the front of the list.
     * @param value Reference to the element to add.
     * @throws std::runtime_error if the element is already in a list.
     */
    void push_front(T& value)
    {
        IntrusiveListHook& hook = value.*HookPtr;
        if (hook.is_linked())
            throw std::runtime_error("push_front: Element already in a list.");

        hook.prev = nullptr;
        hook.next = m_head;
        if (m_head)
            m_head->prev = &hook;
        m_head = &hook;

        if (!m_tail)
            m_tail = &hook;

        ++m_size;
    }

    /**
     * @brief Adds an element to the back of the list.
     * @param value Reference to the element to add.
     * @throws std::runtime_error if the element is already in a list.
     */
    void push_back(T& value)
    {
        IntrusiveListHook& hook = value.*HookPtr;
        if (hook.is_linked())
            throw std::runtime_error("push_back: Element already in a list.");

        hook.next = nullptr;
        hook.prev = m_tail;
        if (m_tail)
            m_tail->next = &hook;
        m_tail = &hook;

        if (!m_head)
            m_head = &hook;

        ++m_size;
    }

    /**
     * @brief Inserts an element at a given position in the list.
     * @param pos Iterator indicating the position of insertion.
     * @param value Reference to the element to insert.
     * @throws std::runtime_error if the element is already in a list.
     */
    void insert(iterator pos, T& value)
    {
        IntrusiveListHook& hook = value.*HookPtr;
        if (hook.is_linked())
            throw std::runtime_error("insert: Element already in a list.");

        if (pos == begin())
        {
            push_front(value);
            return;
        }

        if (pos == end())
        {
            push_back(value);
            return;
        }

        IntrusiveListHook* posHook = pos.getNode();
        hook.next = posHook;
        hook.prev = posHook->prev;

        if (posHook->prev)
            posHook->prev->next = &hook;
        else
            m_head = &hook;

        posHook->prev = &hook;
        ++m_size;
    }

    /**
     * @brief Removes the first element of the list.
     */
    void pop_front()
    {
        if (empty())
            return;

        IntrusiveListHook* oldHead = m_head;
        m_head = m_head->next;
        if (m_head)
            m_head->prev = nullptr;
        else
            m_tail = nullptr;

        oldHead->prev = nullptr;
        oldHead->next = nullptr;
        --m_size;
    }

    /**
     * @brief Removes the last element of the list.
     */
    void pop_back()
    {
        if (empty() || m_tail == nullptr)
            return;

        IntrusiveListHook* oldTail = m_tail;
        m_tail = m_tail->prev;

        if (m_tail != nullptr)
            m_tail->next = nullptr;

        oldTail->prev = nullptr;
        oldTail->next = nullptr;
        --m_size;
    }

    /**
     * @brief Removes an element at a given position in the list.
     * @param pos Iterator indicating the position of the element to remove.
     * @return Iterator to the element following the removed element.
     * @throws std::runtime_error if the iterator is invalid.
     */
    iterator erase(iterator pos)
    {
        IntrusiveListHook* hook = pos.getNode();
        if (!hook)
            throw std::runtime_error("erase: Invalid iterator");

        IntrusiveListHook* nxt = hook->next;
        if (hook->prev)
            hook->prev->next = hook->next;
        else
            m_head = hook->next;

        if (hook->next)
            hook->next->prev = hook->prev;
        else
            m_tail = hook->prev;

        hook->next = nullptr;
        hook->prev = nullptr;
        --m_size;

        return iterator(nxt);
    }



    /**
     * @brief Removes an element from the list.
     * @param value Reference to the element to be removed.
     */
    static void remove(T& value)
    {
        IntrusiveListHook& hook = value.*HookPtr;
        if (!hook.is_linked())
            return;

        if (hook.prev)
            hook.prev->next = hook.next;
        if (hook.next)
            hook.next->prev = hook.prev;

        hook.prev = nullptr;
        hook.next = nullptr;
    }

    /**
     * @brief Swaps the contents of this list with another list.
     * @param other Reference to the other list.
     */
    void swap(IntrusiveList& other)
    {
        std::swap(m_head, other.m_head);
        std::swap(m_tail, other.m_tail);
        std::swap(m_size, other.m_size);
    }

    /**
     * @brief Merges another list into this list.
     * @param other Reference to the other list.
     */
    void merge(IntrusiveList& other)
    {
        if (other.empty())
            return;

        if (m_tail)
        {
            m_tail->next = other.m_head;
            other.m_head->prev = m_tail;
            m_tail = other.m_tail;
        }
        else
        {
            m_head = other.m_head;
            m_tail = other.m_tail;
        }
        m_size += other.m_size;
        other.m_head = other.m_tail = nullptr;
        other.m_size = 0;
    }

    /**
     * @brief Splices another list into this list at a given position.
     * @param pos Iterator indicating the position to splice at.
     * @param other Reference to the other list.
     */
    void splice(iterator pos, IntrusiveList& other)
    {
        if (other.empty())
            return;

        IntrusiveListHook* posHook = pos.getNode();
        if (!posHook)
        {
            if (m_tail)
            {
                m_tail->next = other.m_head;
                other.m_head->prev = m_tail;
            }
            else
            {
                m_head = other.m_head;
            }
            m_tail = other.m_tail;
        }
        else
        {
            IntrusiveListHook* prev = posHook->prev;
            if (prev)
            {
                prev->next = other.m_head;
                other.m_head->prev = prev;
            }
            else
            {
                m_head = other.m_head;
            }
            posHook->prev = other.m_tail;
            other.m_tail->next = posHook;
        }

        m_size += other.m_size;
        other.m_head = other.m_tail = nullptr;
        other.m_size = 0;
    }

    /**
     * @brief Splices an element from another list into this list at a given position.
     * @param pos Iterator indicating the position to splice at.
     * @param other Reference to the other list.
     * @param it Iterator indicating the element to splice.
     * @throws std::runtime_error if the iterator is invalid.
     */
    void splice(iterator pos, IntrusiveList& other, iterator it)
    {
        IntrusiveListHook* hook = it.getNode();
        if (!hook)
            throw std::runtime_error("splice: Invalid iterator");

        if (hook->prev)
            hook->prev->next = hook->next;
        else
            other.m_head = hook->next;

        if (hook->next)
            hook->next->prev = hook->prev;
        else
            other.m_tail = hook->prev;
        other.m_size--;

        IntrusiveListHook* posHook = pos.getNode();
        if (!posHook)
        {
            hook->prev = m_tail;
            hook->next = nullptr;
            if (m_tail)
                m_tail->next = hook;
            m_tail = hook;
            if (!m_head)
                m_head = hook;
        }
        else
        {
            hook->next = posHook;
            hook->prev = posHook->prev;
            if (posHook->prev)
                posHook->prev->next = hook;
            else
                m_head = hook;
            posHook->prev = hook;
        }
        m_size++;
    }

    /**
     * @brief Splices a range of elements from another list into this list at a given position.
     * @param pos Iterator indicating the position to splice at.
     * @param other Reference to the other list.
     * @param first Iterator indicating the first element of the range to splice.
     * @param last Iterator indicating the end of the range to splice.
     */
    void splice(iterator pos, IntrusiveList& other, iterator first, iterator last)
    {
        if (first == last)
            return;

        IntrusiveListHook* firstHook = first.getNode();
        IntrusiveListHook* posHook = pos.getNode();

        IntrusiveListHook* blockTail = nullptr;
        {
            auto it = first;
            while (it != last)
            {
                blockTail = it.getNode();
                ++it;
            }
        }

        if (firstHook->prev)
            firstHook->prev->next = last.getNode();
        else
            other.m_head = last.getNode();

        if (last.getNode())
            last.getNode()->prev = firstHook->prev;
        else
            other.m_tail = firstHook->prev;

        size_type count = 0;
        {
            auto it = first;
            while (it != last)
            {
                ++count;
                ++it;
            }
        }
        other.m_size -= count;

        if (!posHook)
        {
            if (m_tail)
            {
                m_tail->next = firstHook;
                firstHook->prev = m_tail;
            }
            else
            {
                m_head = firstHook;
                firstHook->prev = nullptr;
            }
            m_tail = blockTail;
            blockTail->next = nullptr;
        }
        else
        {
            IntrusiveListHook* prev = posHook->prev;
            if (prev)
            {
                prev->next = firstHook;
                firstHook->prev = prev;
            }
            else
            {
                m_head = firstHook;
                firstHook->prev = nullptr;
            }
            posHook->prev = blockTail;
            blockTail->next = posHook;
        }
        m_size += count;
    }


    /**
     * @brief Removes elements from the list based on a predicate.
     * @tparam P Type of the predicate function.
     * @param p Predicate function that returns true for elements to be removed.
     */
    template <typename P>
    void remove_if(P p)
    {
        for (auto it = begin(); it != end(); )
        {
            T& value = *it;
            auto next = ++it;
            if (p(value))
            {
                erase(--it);
            }
            it = next;
        }
    }

    /**
     * @brief Removes consecutive duplicate elements from the list.
     */
    void unique()
    {
        if (empty())
            return;

        auto it = begin();
        auto nextIt = it;
        ++nextIt;
        while (nextIt != end())
        {
            if (*it == *nextIt)
            {
                nextIt = erase(nextIt);
            }
            else
            {
                ++it;
                ++nextIt;
            }
        }
    }

    /**
     * @brief Sorts the elements in the list using the default comparison operator.
     */
    void sort()
    {
        sort(std::less<T>());
    }


private:
    IntrusiveListHook* m_head; ///< Pointer to the first element in the list.
    IntrusiveListHook* m_tail; ///< Pointer to the last element in the list.
    size_type m_size; ///< Number of elements in the list.
};
