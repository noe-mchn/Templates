#pragma once
#include <iostream>
#include <algorithm>
#include <stdexcept>
#include <initializer_list>
#include <memory>
#include <limits>
#include <utility>

/**
 * @class VectorN
 * @brief A dynamic array class template that provides a similar interface to std::vector.
 *
 * @tparam T The type of elements stored in the vector.
 */
template<typename T>
class VectorN
{
public:
    using value_type = T;                ///< The type of elements stored in the vector.
    using size_type = std::size_t;       ///< An unsigned integral type used for sizes.
    using reference = value_type&;       ///< A reference to an element.
    using const_reference = const value_type&; ///< A const reference to an element.
    using pointer = value_type*;         ///< A pointer to an element.
    using const_pointer = const value_type*; ///< A const pointer to an element.
    using iterator = value_type*;        ///< An iterator to an element.
    using const_iterator = const value_type*; ///< A const iterator to an element.

    /**
     * @brief Default constructor. Constructs an empty vector.
     */
    VectorN()
        : m_size(0), m_capacity(0), m_data(nullptr)
    {
    }

    /**
     * @brief Constructs a vector with n copies of val.
     * @param n The number of elements.
     * @param val The value to initialize elements with.
     */
    explicit VectorN(size_type n, const value_type& val = value_type())
        : m_size(n), m_capacity(n), m_data(nullptr)
    {
        m_data = (m_capacity != 0) ? new value_type[m_capacity] : nullptr;
        std::fill_n(m_data, m_size, val);
    }

    /**
     * @brief Constructs a vector with the contents of the initializer list.
     * @param init_list The initializer list to initialize elements with.
     */
    VectorN(const std::initializer_list<value_type>& init_list)
        : m_size(init_list.size()), m_capacity(init_list.size()), m_data(nullptr)
    {
        m_data = (m_capacity != 0) ? new value_type[m_capacity] : nullptr;
        std::copy(init_list.begin(), init_list.end(), m_data);
    }

    /**
     * @brief Copy constructor. Constructs a vector with a copy of the contents of other.
     * @param other Another vector to copy the contents from.
     */
    VectorN(const VectorN& other)
        : m_size(other.m_size), m_capacity(other.m_capacity), m_data(nullptr)
    {
        if (m_capacity > 0)
        {
            m_data = new value_type[m_capacity];
            std::copy(other.m_data, other.m_data + m_size, m_data);
        }
    }

    /**
     * @brief Copy assignment operator. Replaces the contents with a copy of the contents of other.
     * @param other Another vector to copy the contents from.
     * @return *this
     */
    VectorN& operator=(const VectorN& other)
    {
        if (this != &other)
        {
            delete[] m_data;
            m_size = other.m_size;
            m_capacity = other.m_capacity;
            m_data = (m_capacity > 0) ? new value_type[m_capacity] : nullptr;
            std::copy(other.m_data, other.m_data + m_size, m_data);
        }
        return *this;
    }

    /**
     * @brief Destructor. Destroys the vector.
     */
    ~VectorN()
    {
        delete[] m_data;
    }

    /**
     * @brief Assigns new contents to the vector, replacing its current contents.
     * @param count The number of elements to assign.
     * @param value The value to assign to the elements.
     */
    void assign(size_type count, const value_type& value)
    {
        delete[] m_data;
        m_size = count;
        m_capacity = count;
        m_data = (m_capacity != 0) ? new value_type[m_capacity] : nullptr;
        for (size_type i = 0; i < m_size; ++i)
        {
            m_data[i] = value;
        }
    }

    /**
     * @brief Assigns new contents to the vector, replacing its current contents, from a range.
     * @tparam InputIt Input iterator type.
     * @param first The first iterator of the range.
     * @param last The last iterator of the range.
     */
    template<typename InputIt>
    void assign_range(InputIt first, InputIt last)
    {
        size_type count = static_cast<size_type>(std::distance(first, last));
        delete[] m_data;
        m_size = count;
        m_capacity = count;
        m_data = (m_capacity != 0) ? new value_type[m_capacity] : nullptr;
        size_type i = 0;
        for (; first != last; ++first, ++i)
        {
            m_data[i] = *first;
        }
    }

    /**
     * @brief Appends elements from a range to the end of the vector.
     * @tparam InputIt Input iterator type.
     * @param first The first iterator of the range.
     * @param last The last iterator of the range.
     */
    template<typename InputIt>
    void append_range(InputIt first, InputIt last)
    {
        size_type count = static_cast<size_type>(std::distance(first, last));
        if (count == 0)
            return;
        if (m_size + count > m_capacity)
            reserve(std::max(m_capacity * 2, m_size + count));
        for (; first != last; ++first)
            m_data[m_size++] = *first;
    }

    /**
     * @brief Access specified element with bounds checking.
     * @param pos Position of the element to return.
     * @return Reference to the requested element.
     * @throws std::out_of_range if pos >= size().
     */
    reference at(size_type pos)
    {
        if (pos >= m_size)
            throw std::out_of_range("VectorN: out of range (at)");
        return m_data[pos];
    }

    /**
     * @brief Access specified element with bounds checking.
     * @param pos Position of the element to return.
     * @return Const reference to the requested element.
     * @throws std::out_of_range if pos >= size().
     */
    const_reference at(size_type pos) const
    {
        if (pos >= m_size)
            throw std::out_of_range("VectorN: out of range (at const)");
        return m_data[pos];
    }

    /**
     * @brief Access specified element.
     * @param pos Position of the element to return.
     * @return Reference to the requested element.
     */
    reference operator[](size_type pos)
    {
        return m_data[pos];
    }

    /**
     * @brief Access specified element.
     * @param pos Position of the element to return.
     * @return Const reference to the requested element.
     */
    const_reference operator[](size_type pos) const
    {
        return m_data[pos];
    }

    /**
     * @brief Access the first element.
     * @return Reference to the first element.
     */
    reference front()
    {
        return m_data[0];
    }

    /**
     * @brief Access the first element.
     * @return Const reference to the first element.
     */
    const_reference front() const
    {
        return m_data[0];
    }

    /**
     * @brief Access the last element.
     * @return Reference to the last element.
     */
    reference back()
    {
        return m_data[m_size - 1];
    }

    /**
     * @brief Access the last element.
     * @return Const reference to the last element.
     */
    const_reference back() const
    {
        return m_data[m_size - 1];
    }

    /**
     * @brief Returns an iterator to the beginning.
     * @return Iterator to the first element.
     */
    iterator begin()
    {
        return m_data;
    }

    /**
     * @brief Returns an iterator to the beginning.
     * @return Const iterator to the first element.
     */
    const_iterator begin() const
    {
        return m_data;
    }

    /**
     * @brief Returns a const iterator to the beginning.
     * @return Const iterator to the first element.
     */
    const_iterator cbegin() const
    {
        return m_data;
    }

    /**
     * @brief Returns an iterator to the end.
     * @return Iterator to the element following the last element.
     */
    iterator end()
    {
        return m_data + m_size;
    }

    /**
     * @brief Returns an iterator to the end.
     * @return Const iterator to the element following the last element.
     */
    const_iterator end() const
    {
        return m_data + m_size;
    }

    /**
     * @brief Returns a const iterator to the end.
     * @return Const iterator to the element following the last element.
     */
    const_iterator cend() const
    {
        return m_data + m_size;
    }

    /**
     * @brief Checks if the container has no elements.
     * @return true if the container is empty, false otherwise.
     */
    bool empty() const
    {
        return (m_size == 0);
    }

    /**
     * @brief Returns the number of elements in the container.
     * @return The number of elements in the container.
     */
    size_type size() const
    {
        return m_size;
    }

    /**
     * @brief Returns the number of elements that the container has currently allocated space for.
     * @return The number of elements that the container has currently allocated space for.
     */
    size_type capacity() const
    {
        return m_capacity;
    }

    /**
     * @brief Returns the maximum number of elements the container is able to hold due to system or library implementation limitations.
     * @return The maximum number of elements.
     */
    size_type max_size() const
    {
        return std::numeric_limits<size_type>::max() / sizeof(value_type);
    }

    /**
     * @brief Increases the capacity of the container to a value that's greater or equal to new_cap.
     * @param new_cap New capacity of the container.
     */
    void reserve(size_type new_cap)
    {
        if (new_cap <= m_capacity)
            return;

        pointer new_data = new value_type[new_cap];

        for (size_type i = 0; i < m_size; ++i)
            new_data[i] = std::move(m_data[i]);

        delete[] m_data;
        m_data = new_data;
        m_capacity = new_cap;
    }

    /**
     * @brief Resizes the container to contain new_size elements.
     * @param new_size New size of the container.
     * @param val The value to initialize the new elements with.
     */
    void resize(size_type new_size, const value_type& val = value_type())
    {
        if (new_size < m_size)
        {
            m_size = new_size;
        }
        else if (new_size > m_size)
        {
            if (new_size > m_capacity)
                reserve(new_size);

            for (size_type i = m_size; i < new_size; ++i)
                m_data[i] = val;
            m_size = new_size;
        }
    }

    /**
     * @brief Adds an element to the end.
     * @param val The value of the element to add.
     */
    void push_back(const value_type& val)
    {
        if (m_size >= m_capacity)
            reserve(m_capacity == 0 ? 1 : m_capacity * 2);
        m_data[m_size++] = val;
    }

    /**
     * @brief Adds an element to the beginning.
     * @param val The value of the element to add.
     */
    void push_front(const value_type& val)
    {
        if (m_size >= m_capacity)
            reserve(m_capacity == 0 ? 1 : m_capacity * 2);

        for (size_type i = m_size; i > 0; --i)
            m_data[i] = std::move(m_data[i - 1]);

        m_data[0] = val;
        ++m_size;
    }

    /**
     * @brief Removes the last element.
     */
    void pop_back()
    {
        if (m_size > 0)
            --m_size;
    }

    /**
     * @brief Removes the first element.
     */
    void pop_front()
    {
        if (m_size == 0)
            return;
        for (size_type i = 0; i < m_size - 1; ++i)
            m_data[i] = std::move(m_data[i + 1]);
        --m_size;
    }

    /**
     * @brief Clears the contents.
     */
    void clear()
    {
        m_size = 0;
    }


    /**
     * @brief Inserts an element at the specified position.
     * @param pos Iterator to the position where the element should be inserted.
     * @param val The value of the element to insert.
     * @return Iterator pointing to the inserted element.
     * @throws std::out_of_range if pos is out of range.
     */
    iterator insert(iterator pos, const value_type& val)
    {
        size_type index = static_cast<size_type>(pos - begin());
        if (index > m_size)
            throw std::out_of_range("VectorN: insert position out of range");

        if (m_size >= m_capacity)
            reserve(m_capacity == 0 ? 1 : m_capacity * 2);

        for (size_type i = m_size; i > index; --i)
            m_data[i] = std::move(m_data[i - 1]);

        m_data[index] = val;
        ++m_size;

        return (begin() + index);
    }

    /**
     * @brief Constructs an element in-place at the specified position.
     * @tparam Args Types of the arguments to forward to the constructor of the element.
     * @param pos Iterator to the position where the element should be constructed.
     * @param args Arguments to forward to the constructor of the element.
     * @return Iterator pointing to the emplaced element.
     * @throws std::out_of_range if pos is out of range.
     */
    template<typename... Args>
    iterator emplace(iterator pos, Args&&... args)
    {
        size_type index = static_cast<size_type>(pos - begin());
        if (index > m_size)
            throw std::out_of_range("VectorN: emplace position out of range");

        if (m_size >= m_capacity)
            reserve(m_capacity == 0 ? 1 : m_capacity * 2);

        for (size_type i = m_size; i > index; --i)
            m_data[i] = std::move(m_data[i - 1]);

        m_data[index] = T(std::forward<Args>(args)...);
        ++m_size;

        return (begin() + index);
    }

    /**
     * @brief Constructs an element in-place at the end.
     * @tparam Args Types of the arguments to forward to the constructor of the element.
     * @param args Arguments to forward to the constructor of the element.
     */
    template<typename... Args>
    void emplace_back(Args&&... args)
    {
        if (m_size >= m_capacity)
            reserve(m_capacity == 0 ? 1 : m_capacity * 2);

        m_data[m_size++] = T(std::forward<Args>(args)...);
    }

    /**
     * @brief Erases the element at the specified position.
     * @param pos Iterator to the position of the element to erase.
     * @return Iterator pointing to the element that followed the erased element.
     * @throws std::out_of_range if pos is out of range.
     */
    iterator erase(iterator pos)
    {
        size_type index = static_cast<size_type>(pos - begin());
        if (index >= m_size)
            throw std::out_of_range("VectorN: erase position out of range");

        for (size_type i = index; i < m_size - 1; ++i)
            m_data[i] = std::move(m_data[i + 1]);

        --m_size;
        return (begin() + index);
    }

    /**
     * @brief Inserts a range of elements at the specified position.
     * @tparam InputIt Input iterator type.
     * @param pos Iterator to the position where the elements should be inserted.
     * @param first Iterator to the first element in the range.
     * @param last Iterator to the element following the last element in the range.
     * @throws std::out_of_range if pos is out of range.
     */
    template<typename InputIt>
    void insert_range(iterator pos, InputIt first, InputIt last)
    {
        size_type index = static_cast<size_type>(pos - begin());
        if (index > m_size)
            throw std::out_of_range("VectorN: insert_range position out of range");

        size_type count = static_cast<size_type>(std::distance(first, last));
        if (count == 0)
            return;

        if (m_size + count > m_capacity)
            reserve(std::max(m_capacity * 2, m_size + count));

        for (size_type i = m_size; i > index; --i)
            m_data[i + count - 1] = std::move(m_data[i - 1]);

        size_type idx = index;
        for (; first != last; ++first, ++idx)
        {
            m_data[idx] = *first;
        }

        m_size += count;
    }

    /**
     * @brief Swaps the contents of the vector with another vector.
     * @param other The vector to swap contents with.
     */
    void swap(VectorN& other)
    {
        std::swap(m_data, other.m_data);
        std::swap(m_size, other.m_size);
        std::swap(m_capacity, other.m_capacity);
    }

    /**
     * @brief Returns a pointer to the underlying array serving as element storage.
     * @return Pointer to the underlying array.
     */
    pointer data()
    {
        return m_data;
    }

    /**
     * @brief Returns a const pointer to the underlying array serving as element storage.
     * @return Const pointer to the underlying array.
     */
    const_pointer data() const
    {
        return m_data;
    }

private:
    /**
     * @brief The number of elements in the vector.
     */
    size_type m_size;

    /**
     * @brief The capacity of the vector (number of elements it can hold without reallocating).
     */
    size_type m_capacity;

    /**
     * @brief Pointer to the array holding the elements.
     */
    pointer m_data;
};


/**
 * @brief Overload of the stream insertion operator for VectorN.
 * @tparam T The type of elements stored in the vector.
 * @param os The output stream.
 * @param vec The vector to output.
 * @return The output stream.
 */
template<typename T>
std::ostream& operator<<(std::ostream& os, const VectorN<T>& vec)
{
    os << "[";
    for (std::size_t i = 0; i < vec.size(); ++i)
    {
        os << vec[i];
        if (i < vec.size() - 1)
            os << ", ";
    }
    os << "]";
    return os;
}
