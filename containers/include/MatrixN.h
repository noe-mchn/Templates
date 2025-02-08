#pragma once

#include <stdexcept>
#include <initializer_list>
#include <ostream>
#include "ArrayN.h"



/**
 * @class MatrixND
 * @brief A template class for representing a 2D matrix of fixed size.
 *
 * @tparam T The type of the elements in the matrix.
 * @tparam Rows The number of rows in the matrix.
 * @tparam Cols The number of columns in the matrix.
 */
template<typename T, std::size_t Rows, std::size_t Cols>
class MatrixND {
public:
    using value_type = T;
    using size_type = std::size_t;

    /**
     * @brief Default constructor that initializes all elements to the default value of T.
     */
    MatrixND() {
        for (size_type i = 0; i < Rows * Cols; ++i) {
            m_data[i] = T{};
        }
    }

    /**
     * @brief Constructor that initializes the matrix with a nested initializer list.
     *
     * @param init A nested initializer list containing the initial values for the matrix.
     * @throws std::runtime_error if the initializer list has more rows or columns than the matrix.
     */
    MatrixND(std::initializer_list<std::initializer_list<T>> init) {
        if (init.size() > Rows)
            throw std::runtime_error("Trop de lignes pour la matrice");

        size_type rowIndex = 0;
        for (auto rowList : init) {
            if (rowList.size() > Cols)
                throw std::runtime_error("Trop de colonnes dans une des lignes");

            size_type colIndex = 0;
            for (auto elem : rowList) {
                m_data[rowIndex * Cols + colIndex] = elem;
                ++colIndex;
            }

            for (; colIndex < Cols; ++colIndex) {
                m_data[rowIndex * Cols + colIndex] = T{};
            }
            ++rowIndex;
        }

        for (; rowIndex < Rows; ++rowIndex) {
            for (size_type colIndex = 0; colIndex < Cols; ++colIndex) {
                m_data[rowIndex * Cols + colIndex] = T{};
            }
        }
    }

    /**
     * @brief Accesses the element at the specified row and column.
     *
     * @param row The row index.
     * @param col The column index.
     * @return A reference to the element at the specified position.
     * @throws std::out_of_range if the row or column index is out of bounds.
     */
    T& operator()(size_type row, size_type col) {
        if (row >= Rows || col >= Cols)
            throw std::out_of_range("Index hors limites dans MatrixND::operator()");
        return m_data[row * Cols + col];
    }

    /**
     * @brief Accesses the element at the specified row and column (const version).
     *
     * @param row The row index.
     * @param col The column index.
     * @return A const reference to the element at the specified position.
     * @throws std::out_of_range if the row or column index is out of bounds.
     */
    const T& operator()(size_type row, size_type col) const {
        if (row >= Rows || col >= Cols)
            throw std::out_of_range("Index hors limites dans MatrixND::operator() const");
        return m_data[row * Cols + col];
    }

    /**
     * @brief Returns the number of rows in the matrix.
     *
     * @return The number of rows.
     */
    constexpr size_type rowCount() const {
        return Rows;
    }

    /**
     * @brief Returns the number of columns in the matrix.
     *
     * @return The number of columns.
     */
    constexpr size_type colCount() const {
        return Cols;
    }

    /**
     * @brief Multiplies two matrices and returns the result.
     *
     * @tparam OtherCols The number of columns in the second matrix.
     * @param lhs The left-hand side matrix.
     * @param rhs The right-hand side matrix.
     * @return The result of the matrix multiplication.
     */
    template<std::size_t OtherCols>
    static MatrixND<T, Rows, OtherCols> multiply(const MatrixND<T, Rows, Cols>& lhs,
        const MatrixND<T, Cols, OtherCols>& rhs) {
        MatrixND<T, Rows, OtherCols> result;
        for (size_type i = 0; i < Rows; ++i) {
            for (size_type j = 0; j < OtherCols; ++j) {
                T sum = T{};
                for (size_type k = 0; k < Cols; ++k) {
                    sum += lhs(i, k) * rhs(k, j);
                }
                result(i, j) = sum;
            }
        }
        return result;
    }

    /**
     * @brief Returns a pointer to the underlying data array (const version).
     *
     * @return A const pointer to the data array.
     */
    const T* data() const {
        return m_data.data();
    }

    /**
     * @brief Returns a pointer to the underlying data array.
     *
     * @return A pointer to the data array.
     */
    T* data() {
        return m_data.data();
    }

private:
    ArrayN<T, Rows* Cols> m_data; ///< The underlying data array.
};



/**
 * @brief Overloads the stream insertion operator to print the matrix.
 *
 * @tparam T The type of the elements in the matrix.
 * @tparam Rows The number of rows in the matrix.
 * @tparam Cols The number of columns in the matrix.
 * @param os The output stream.
 * @param mat The matrix to be printed.
 * @return The output stream.
 */
template<typename T, std::size_t Rows, std::size_t Cols>
std::ostream& operator<<(std::ostream& os, const MatrixND<T, Rows, Cols>& mat)
{
    os << "[\n";
    for (std::size_t i = 0; i < Rows; ++i)
    {
        os << "  [";
        for (std::size_t j = 0; j < Cols; ++j)
        {
            os << mat(i, j);
            if (j + 1 < Cols)
                os << ", ";
        }
        os << "]";
        if (i + 1 < Rows)
            os << ",\n";
        else
            os << "\n";
    }
    os << "]";
    return os;
}
