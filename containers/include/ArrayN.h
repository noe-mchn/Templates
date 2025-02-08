#pragma once
#include <iostream>
#include <exception>
#include <algorithm>
#include <initializer_list>

#pragma once
#include <iostream>
#include <exception>
#include <algorithm>
#include <initializer_list>

/**
 * @brief A fixed-size array class template.
 *
 * @tparam Type The type of elements stored in the array.
 * @tparam N The size of the array.
 */
template<typename Type, size_t N>
class ArrayN
{
    template<typename type, size_t size>
    friend std::ostream& operator<<(std::ostream& os, const ArrayN<type, size>& tab);

public:
    using value_type = Type;
    using size_type = size_t;
    using iterator = value_type*;
    using const_iterator = const value_type*;

    static constexpr size_type size = N;

    /**
     * @brief Default constructor. Initializes the array with default values.
     */
    ArrayN()
    {
        std::fill(m_data, m_data + size, value_type{});
    }

    /**
     * @brief Constructor with initializer list.
     *
     * @param list The initializer list to initialize the array.
     * @throws std::runtime_error if the size of the initializer list does not match the array size.
     */
    ArrayN(const std::initializer_list<value_type>& list)
    {
        if (list.size() != size)
            throw std::runtime_error("Invalid size in initializer_list");
        std::copy(list.begin(), list.end(), m_data);
    }

    /**
     * @brief Copy constructor.
     *
     * @param other The array to copy from.
     */
    ArrayN(const ArrayN& other)
    {
        std::copy(other.begin(), other.end(), m_data);
    }

    /**
     * @brief Destructor.
     */
    ~ArrayN() = default;

    /**
     * @brief Copy assignment operator.
     *
     * @param other The array to copy from.
     * @return ArrayN& A reference to the assigned array.
     */
    ArrayN& operator=(const ArrayN& other)
    {
        if (this != &other)
            std::copy(other.begin(), other.end(), m_data);
        return *this;
    }

    /**
     * @brief Access element by index.
     *
     * @param idx The index of the element.
     * @return value_type& A reference to the element.
     * @throws std::runtime_error if the index is out of range.
     */
    value_type& operator[](size_type idx)
    {
        if (idx >= size)
            throw std::runtime_error("Out of range (operator[])");
        return m_data[idx];
    }

    /**
     * @brief Access element by index (const version).
     *
     * @param idx The index of the element.
     * @return const value_type& A const reference to the element.
     * @throws std::runtime_error if the index is out of range.
     */
    const value_type& operator[](size_type idx) const
    {
        if (idx >= size)
            throw std::runtime_error("Out of range (operator[] const)");
        return m_data[idx];
    }

    /**
     * @brief Access element by index with bounds checking.
     *
     * @param idx The index of the element.
     * @return value_type& A reference to the element.
     * @throws std::runtime_error if the index is out of range.
     */
    value_type& at(size_type idx)
    {
        if (idx >= size)
            throw std::runtime_error("Out of range (at)");
        return m_data[idx];
    }

    /**
     * @brief Access element by index with bounds checking (const version).
     *
     * @param idx The index of the element.
     * @return const value_type& A const reference to the element.
     * @throws std::runtime_error if the index is out of range.
     */
    const value_type& at(size_type idx) const
    {
        if (idx >= size)
            throw std::runtime_error("Out of range (at const)");
        return m_data[idx];
    }

    /**
     * @brief Check if the array is empty.
     *
     * @return true if the array is empty, false otherwise.
     */
    bool empty() const
    {
        return (size == 0);
    }

    /**
     * @brief Get the size of the array.
     *
     * @return constexpr size_type The size of the array.
     */
    static constexpr size_type Size()
    {
        return size;
    }

    /**
     * @brief Get the first element of the array.
     *
     * @return value_type& A reference to the first element.
     */
    value_type& front()
    {
        return m_data[0];
    }

    /**
     * @brief Get the first element of the array (const version).
     *
     * @return const value_type& A const reference to the first element.
     */
    const value_type& front() const
    {
        return m_data[0];
    }

    /**
     * @brief Get the last element of the array.
     *
     * @return value_type& A reference to the last element.
     */
    value_type& back()
    {
        return m_data[size - 1];
    }

    /**
     * @brief Get the last element of the array (const version).
     *
     * @return const value_type& A const reference to the last element.
     */
    const value_type& back() const
    {
        return m_data[size - 1];
    }

    /**
     * @brief Get a pointer to the underlying data.
     *
     * @return value_type* A pointer to the data.
     */
    value_type* data()
    {
        return m_data;
    }

    /**
     * @brief Get a pointer to the underlying data (const version).
     *
     * @return const value_type* A const pointer to the data.
     */
    const value_type* data() const
    {
        return m_data;
    }

    /**
     * @brief Get an iterator to the beginning of the array.
     *
     * @return iterator An iterator to the beginning.
     */
    iterator begin()
    {
        return m_data;
    }

    /**
     * @brief Get an iterator to the end of the array.
     *
     * @return iterator An iterator to the end.
     */
    iterator end()
    {
        return m_data + size;
    }

    /**
     * @brief Get a const iterator to the beginning of the array.
     *
     * @return const_iterator A const iterator to the beginning.
     */
    const_iterator begin() const
    {
        return m_data;
    }

    /**
     * @brief Get a const iterator to the end of the array.
     *
     * @return const_iterator A const iterator to the end.
     */
    const_iterator end() const
    {
        return m_data + size;
    }

    /**
     * @brief Get a const iterator to the beginning of the array.
     *
     * @return const_iterator A const iterator to the beginning.
     */
    const_iterator cbegin() const
    {
        return m_data;
    }

    /**
     * @brief Get a const iterator to the end of the array.
     *
     * @return const_iterator A const iterator to the end.
     */
    const_iterator cend() const
    {
        return m_data + size;
    }

    /**
     * @brief Fill the array with a specified value.
     *
     * @param value The value to fill the array with.
     */
    void fill(const value_type& value)
    {
        std::fill(m_data, m_data + size, value);
    }

    /**
     * @brief Swap the contents of this array with another array.
     *
     * @param other The other array to swap with.
     */
    void swap(ArrayN& other)
    {
        for (size_type i = 0; i < size; ++i)
            std::swap(m_data[i], other.m_data[i]);
    }

private:
    value_type m_data[size];
};

/**
 * @brief Output stream operator for ArrayN.
 *
 * @tparam type The type of elements stored in the array.
 * @tparam size The size of the array.
 * @param os The output stream.
 * @param tab The array to output.
 * @return std::ostream& The output stream.
 */
template<typename type, size_t size>
std::ostream& operator<<(std::ostream& os, const ArrayN<type, size>& tab)
{
    os << "(";
    for (size_t i = 0; i < tab.size - 1; ++i)
        os << tab.m_data[i] << ", ";
    os << tab.m_data[tab.size - 1] << ")";
    return os;
}

/**
 * @brief Transform a range of elements using a unary function.
 *
 * @tparam InputIt The type of the input iterator.
 * @tparam OutputIt The type of the output iterator.
 * @tparam Fn The type of the unary function.
 * @param first The beginning of the input range.
 * @param last The end of the input range.
 * @param d_first The beginning of the destination range.
 * @param fn The unary function to apply to each element.
 * @return OutputIt An iterator to the end of the destination range.
 */
template<typename InputIt, typename OutputIt, typename Fn>
OutputIt transform(InputIt first, InputIt last, OutputIt d_first, Fn fn)
{
    for (; first != last; ++first, ++d_first)
        *d_first = fn(*first);
    return d_first;
}

/**
 * @brief Concatenate two ArrayN objects.
 *
 * @tparam Type The type of elements stored in the arrays.
 * @tparam N1 The size of the first array.
 * @tparam N2 The size of the second array.
 * @param arr1 The first array.
 * @param arr2 The second array.
 * @return ArrayN<Type, N1 + N2> The concatenated array.
 */
template<typename Type, size_t N1, size_t N2>
ArrayN<Type, N1 + N2> concat(const ArrayN<Type, N1>& arr1, const ArrayN<Type, N2>& arr2)
{
    ArrayN<Type, N1 + N2> result;
    std::copy(arr1.begin(), arr1.end(), result.begin());
    std::copy(arr2.begin(), arr2.end(), result.begin() + N1);
    return result;
}
