///---------------------------------------------------------------------------------------------------------------------
///-- Author        ReactiioN
///-- Copyright     2016-2022, ReactiioN
///-- License       MIT
///---------------------------------------------------------------------------------------------------------------------
#pragma once

#include <mkrn/core/requirements.hpp>

namespace mkrn::math {
constexpr inline auto dist_epsilon{ 0.03125f };
;
template<std::size_t NumRows, std::size_t NumCols>
class matrix_t
{
    using type_array = std::array<float, NumRows * NumCols>;

    NODISCARD
    static
    constexpr
    auto
    clamp_index(
        const std::size_t index
    ) noexcept -> std::size_t
    {
        return std::clamp(
            index, 
            std::numeric_limits<std::size_t>::min(), 
            size() - 1
        );
    }

    NODISCARD
    static
    constexpr
    auto
    is_vector() noexcept -> bool
    {
        return (NumCols == 1 || NumRows == 1)
            && (NumCols == 1 ? NumRows : NumCols) <= 4;
    }

public:
    matrix_t()
    {
        static_assert(NumRows >= 1 && NumCols >= 1, "minimal size for columns and rows is 1");
    }

    matrix_t(
        const matrix_t& rhs
    ) = default;

    matrix_t(
        matrix_t&& rhs
    ) noexcept
    {
        *this = std::move(rhs);
    }

    explicit
    matrix_t(
        const type_array& values
    )
        : data_(values)
    {}

    template<typename ...Args>
    matrix_t(
        Args&& ...args
    ) noexcept
        : data_{ static_cast<float>(std::forward<Args>(args))... }
    {}

    ~matrix_t() = default;

    auto
    operator = (
        const matrix_t& rhs
    ) ->matrix_t& = default;

    auto
    operator = (
        matrix_t&& rhs
    ) noexcept -> matrix_t&
    {
        data_ = rhs.data_;

        rhs.clear();

        return *this;
    }

    NODISCARD
    static
    constexpr
    auto size() noexcept -> std::size_t
    {
        return NumRows * NumCols;
    }

    NODISCARD
    static
    constexpr
    auto
    rows() noexcept -> std::size_t
    {
        return NumRows;
    }

    NODISCARD
    static
    constexpr
    auto
    cols() noexcept -> std::size_t
    {
        return NumCols;
    }

    auto 
    operator += (
        const matrix_t& rhs
    ) noexcept -> matrix_t&
    {
        for (std::size_t i{}; i < size(); ++i) {
            data_[i] += rhs.data_[i];
        }

        return *this;
    }

    auto
    operator += (
        const float rhs
    ) noexcept -> matrix_t&
    {
        for (std::size_t i{}; i < size(); ++i) {
            data_[i] += rhs;
        }

        return *this;
    }

    auto 
    operator -= (
        const matrix_t& rhs
    ) noexcept -> matrix_t&
    {
        for (std::size_t i{}; i < size(); ++i) {
            data_[i] -= rhs.data_[i];
        }

        return *this;
    }

    auto
    operator -= (
        const float rhs
    ) noexcept -> matrix_t&
    {
        for (std::size_t i{}; i < size(); ++i) {
            data_[i] -= rhs;
        }

        return *this;
    }

    auto
    operator *= (
        const float rhs
    ) noexcept -> matrix_t&
    {
        for (std::size_t i{}; i < size(); ++i) {
            data_[i] *= rhs;
        }

        return *this;
    }

    auto
    operator /= (
        const float rhs
    ) noexcept -> matrix_t&
    {
        for (std::size_t i{}; i < size(); ++i) {
            data_[i] /= rhs;
        }

        return *this;
    }

    NODISCARD
    auto
    operator + (
        const matrix_t& rhs
    ) const noexcept -> matrix_t
    {
        auto lhs{ *this };

        lhs += rhs;

        return lhs;
    }

    NODISCARD
    auto
    operator + (
        const float rhs
    ) const noexcept -> matrix_t
    {
        auto lhs{ *this };

        lhs += rhs;

        return lhs;
    }

    NODISCARD
    auto
    operator - (
        const matrix_t& rhs
    ) const noexcept -> matrix_t
    {
        auto lhs{ *this };

        lhs -= rhs;

        return lhs;
    }

    NODISCARD
    auto
    operator - (
        const float rhs
    ) const noexcept -> matrix_t
    {
        auto lhs{ *this };

        lhs -= rhs;

        return lhs;
    }

    NODISCARD
    auto
    operator * (
        const matrix_t& rhs
    ) const noexcept -> matrix_t
    {
        auto lhs{ *this };

        lhs *= rhs;

        return lhs;
    }

    NODISCARD
    auto
    operator * (
        const float rhs
    ) const noexcept -> matrix_t
    {
        auto lhs{ *this };

        lhs *= rhs;

        return lhs;
    }

    NODISCARD
    auto
    operator / (
        const matrix_t& rhs
    ) const noexcept -> matrix_t
    {
        auto lhs{ *this };

        lhs /= rhs;

        return lhs;
    }

    NODISCARD
    auto
    operator / (
        const float rhs
    ) const noexcept -> matrix_t
    {
        auto lhs{ *this };

        lhs /= rhs;

        return lhs;
    }

    NODISCARD
    auto
    operator == (
        const matrix_t& rhs
    ) const noexcept -> bool
    {
        for (std::size_t i{}; i < size(); ++i) {
            if (at(i) != rhs(i)) {
                return false;
            }
        }

        return true;
    }

    NODISCARD
    auto
    operator == (
        const float rhs
    ) const noexcept -> bool
    {
        for (std::size_t i{}; i < size(); ++i) {
            if (at(i) != rhs) {
                return false;
            }
        }

        return true;
    }

    NODISCARD
    auto
    operator != (
        const matrix_t& rhs
    ) const noexcept -> bool
    {
        return !((*this) == rhs);
    }

    NODISCARD
    auto
    operator != (
        const float rhs
    ) const noexcept -> bool
    {
        return !((*this) == rhs);
    }

    auto
    operator () (
        const std::size_t index
    ) noexcept -> float&
    {
        return at(index);
    }

    auto
    operator () (
        const std::size_t row_index,
        const std::size_t col_index
    ) noexcept -> float&
    {
        return at(row_index, col_index);
    }

    NODISCARD
    auto
    operator () (
        const std::size_t index
    ) const noexcept -> const float&
    {
        return at(index);
    }

    NODISCARD
    auto
    operator () (
        const std::size_t row_index,
        const std::size_t col_index
    ) const noexcept -> const float&
    {
        return at(row_index, col_index);
    }

    auto
    at(
        const std::size_t index
    ) noexcept -> float&
    {
        return data_.at(clamp_index(index));
    }

    auto
    at(
        const std::size_t row_index,
        const std::size_t col_index
    ) noexcept -> float&
    {
        return at(row_index * NumCols + col_index);
    }

    NODISCARD
    auto
    at(
        const std::size_t index
    ) const noexcept -> const float&
    {
        return data_.at(clamp_index(index));
    }

    NODISCARD
    auto
    at(
        const std::size_t row_index,
        const std::size_t col_index
    ) const noexcept -> const float&
    {
        return at(row_index * NumCols + col_index);
    }

    NODISCARD
    auto
    is_zero() const noexcept -> bool
    {
        return all_of(0.f);
    }

    NODISCARD
    auto
    all_of(
        const float value
    ) const noexcept -> bool
    {
        return std::all_of(
            data_.begin(),
            data_.end(),
            [&value](const float x)
            {
                return x == value;
            } 
        );
    }

    NODISCARD
    auto
    normsqr() const noexcept -> float
    {
        static_assert(is_vector(), "norm() can only be used on vectors");

        auto value = 0.f;

        for (const auto& x : data_) {
            value += (x * x);
        }

        return value;
    }

    NODISCARD
    auto
    norm() const noexcept -> float
    {
        return std::sqrt(normsqr());
    }

    NODISCARD
    auto
    row(
        const std::size_t index
    ) const noexcept -> matrix_t<1, NumCols>
    {
        std::array<float, NumCols> values{};

        for (std::size_t i{}; i < NumCols; ++i) {
            values[i] = at(index, i);
        }

        return matrix_t<1, NumCols>(values);
    }

    NODISCARD
    auto
    col(
        const std::size_t index
    ) const noexcept -> matrix_t<NumRows, 1>
    {
        std::array<float, NumRows> values{};

        for (std::size_t i{}; i < NumRows; ++i) {
            values[i] = at(i, index);
        }

        return matrix_t<NumRows, 1>(values);
    }

    auto
    row(
        const std::size_t            index,
        const matrix_t<1, NumCols>& row_values
    ) noexcept -> matrix_t&
    {
        for (std::size_t i{}; i < NumCols; ++i) {
            at(index, i) = row_values;
        }

        return *this;
    }

    auto
    col(
        const std::size_t            index,
        const matrix_t<NumRows, 1>& col_values
    ) noexcept -> matrix_t&
    {
        for (std::size_t i{}; i < NumRows; ++i) {
            at(i, index) = col_values;
        }

        return *this;
    }

    auto
    fill(
        const float value
    ) noexcept -> matrix_t&
    {
        data_.fill(value);

        return *this;
    }

    auto
    clear() noexcept -> matrix_t&
    {
        return fill(0.f);
    }

    auto
    set(
        const type_array& values
    ) -> matrix_t&
    {
        data_ = values;

        return *this;
    }

    auto
    normalize() noexcept -> matrix_t&
    {
        static_assert(is_vector(), "normalize() can only be used on vectors");

        (*this) /= norm();

        return *this;
    }

    NODISCARD
    auto
    normalized() const noexcept -> matrix_t
    {
        auto lhs{ *this };

        lhs.normalize();

        return lhs;
    }

    template<std::size_t rhs_num_rows, std::size_t rhs_num_cols>
    NODISCARD
    auto
    dot(
        const matrix_t<rhs_num_rows, rhs_num_cols>& rhs
    ) const noexcept -> float
    {
        static_assert(is_vector() && matrix_t<rhs_num_rows, rhs_num_cols>::is_vector());

        auto       value  = 0.f;
        const auto length = std::min(size(), rhs_num_rows * rhs_num_cols);

        for (std::size_t i{}; i < length; ++i) {
            value += at(i) * rhs(i);
        }

        return value;
    }

    NODISCARD
    auto
    cross(
        const matrix_t& rhs
    ) const noexcept -> matrix_t
    {
        static_assert(size() == 3, "cross() can only be used for 3 dimensional vectors");

        return matrix_t{
            at(1) * rhs(2) - at(2) * rhs(1),
            at(2) * rhs(0) - at(0) * rhs(2),
            at(0) * rhs(1) - at(1) * rhs(0),
        };
    }

    NODISCARD
    auto
    ncross(
        const matrix_t& rhs
    ) const noexcept -> matrix_t
    {
        auto ret{ cross(rhs) };

        ret.normalize();

        return ret;
    }

    NODISCARD
    auto
    identity() noexcept -> matrix_t&
    {
        static_assert(NumRows == 4 && NumCols == 4);

        fill(0.f);

        for (std::size_t i{}; i < 4; ++i) {
            at(i, i) = 1.f;
        }

        return *this;
    }

private:
    type_array data_{};
};

using angle2    = matrix_t<1, 2>;
using angle3    = matrix_t<1, 3>;
using angle4    = matrix_t<1, 4>;
using matrix3x4 = matrix_t<3, 4>;
using matrix4x4 = matrix_t<4, 4>;
using vector2   = matrix_t<2, 1>;
using vector3   = matrix_t<3, 1>;
using vector4   = matrix_t<4, 1>;
using vmatrix   = matrix4x4;
}
