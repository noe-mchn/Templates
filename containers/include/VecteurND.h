#pragma once
#include "ArrayN.h"
#include <cmath>
#include <stdexcept>
#include <ostream>

/**
 * @brief This class VectorND uses the ArrayN<T, N> you provided earlier
 * as the internal container for the data.
 *
 * @tparam T Type of the elements.
 * @tparam N Number of elements in the vector.
 */
template<typename T, std::size_t N>
class VectorND
{
public:
    using value_type = T;
    using size_type = std::size_t;

    /**
     * @brief Default constructor that initializes all elements to the default value of T.
     */
    VectorND()
    {
        for (size_type i = 0; i < N; ++i)
            m_data[i] = T{};
    }

    /**
     * @brief Constructor that initializes the vector with a list of values.
     *
     * @param init Initializer list of values.
     * @throws std::runtime_error if the initializer list is larger than the vector size.
     */
    VectorND(std::initializer_list<T> init)
    {
        if (init.size() > N)
            throw std::runtime_error("Initializer list too large for VectorND");
        auto it = init.begin();
        for (size_type i = 0; i < N; ++i)
        {
            if (it != init.end())
            {
                m_data[i] = *it;
                ++it;
            }
            else
            {
                m_data[i] = T{};
            }
        }
    }

    /**
     * @brief Accesses the element at the given index.
     *
     * @param index Index of the element to access.
     * @return Reference to the element at the given index.
     */
    T& operator[](size_type index)
    {
        return m_data[index];
    }

    /**
     * @brief Accesses the element at the given index (const version).
     *
     * @param index Index of the element to access.
     * @return Const reference to the element at the given index.
     */
    const T& operator[](size_type index) const
    {
        return m_data[index];
    }

    /**
     * @brief Returns the size of the vector.
     *
     * @return Number of elements in the vector.
     */
    constexpr size_type size() const
    {
        return N;
    }

    /**
     * @brief Computes the dot product of two vectors.
     *
     * @param lhs Left-hand side vector.
     * @param rhs Right-hand side vector.
     * @return Dot product of the two vectors.
     */
    static T dot(const VectorND& lhs, const VectorND& rhs)
    {
        T result = T{};
        for (size_type i = 0; i < N; ++i)
        {
            result += lhs.m_data[i] * rhs.m_data[i];
        }
        return result;
    }

    /**
     * @brief Computes the cross product of two 3D vectors.
     *
     * @param lhs Left-hand side vector.
     * @param rhs Right-hand side vector.
     * @return Cross product of the two vectors.
     * @throws static_assert if the vector size is not 3.
     */
    static VectorND cross(const VectorND& lhs, const VectorND& rhs)
    {
        static_assert(N == 3, "Cross product is only defined for 3D vectors");
        VectorND result;
        result[0] = lhs.m_data[1] * rhs.m_data[2] - lhs.m_data[2] * rhs.m_data[1];
        result[1] = lhs.m_data[2] * rhs.m_data[0] - lhs.m_data[0] * rhs.m_data[2];
        result[2] = lhs.m_data[0] * rhs.m_data[1] - lhs.m_data[1] * rhs.m_data[0];
        return result;
    }

    /**
     * @brief Computes the Euclidean norm (length) of the vector.
     *
     * @return Euclidean norm of the vector.
     */
    T norm() const
    {
        T sumSq = T{};
        for (size_type i = 0; i < N; ++i)
        {
            sumSq += m_data[i] * m_data[i];
        }
        return std::sqrt(sumSq);
    }

    /**
     * @brief Normalizes the vector (makes it unit length).
     *
     * @throws std::runtime_error if the vector has zero length.
     */
    void normalize()
    {
        T length = norm();
        if (length == T{})
            throw std::runtime_error("Cannot normalize a zero-length vector");

        for (size_type i = 0; i < N; ++i)
            m_data[i] /= length;
    }

    /**
     * @brief Returns a normalized copy of the vector.
     *
     * @return Normalized copy of the vector.
     */
    VectorND normalized() const
    {
        VectorND copy(*this);
        copy.normalize();
        return copy;
    }

    /**
     * @brief Equality operator.
     *
     * @param other Vector to compare with.
     * @return true if the vectors are equal, false otherwise.
     */
    bool operator==(const VectorND& other) const
    {
        for (size_type i = 0; i < N; ++i)
        {
            if (m_data[i] != other.m_data[i])
                return false;
        }
        return true;
    }

    /**
     * @brief Inequality operator.
     *
     * @param other Vector to compare with.
     * @return true if the vectors are not equal, false otherwise.
     */
    bool operator!=(const VectorND& other) const
    {
        return !(*this == other);
    }

private:
    ArrayN<T, N> m_data; ///< Internal container for the vector data.
};

/**
 * @brief Stream insertion operator for VectorND.
 *
 * @tparam T Type of the elements.
 * @tparam N Number of elements in the vector.
 * @param os Output stream.
 * @param vec Vector to insert into the stream.
 * @return Reference to the output stream.
 */
template<typename T, std::size_t N>
std::ostream& operator<<(std::ostream& os, const VectorND<T, N>& vec)
{
    os << "(";
    for (std::size_t i = 0; i < N; ++i)
    {
        os << vec[i];
        if (i + 1 < N)
            os << ", ";
    }
    os << ")";
    return os;
}
