// lib/matrix.cpp
#include <stdexcept>
#include <algorithm>
#include <cstring>
#include <cmath>

#include "include/matrix.hpp"
#include "include/constants.hpp"

using namespace KaloMLib;

std::pair<long, long> matrix::getShape() { return std::make_pair(ln, col); }

void matrix::resize(std::pair<long, long> shape)
{
    if (this->block != nullptr)
        delete this->block;
    this->block = (double *)malloc(1LL * sizeof(double) * shape.first * shape.second);
    memset(this->block, 0, sizeof(double) * 1LL * shape.first * shape.second);
    ln = shape.first, col = shape.second;
}

matrix::matrix() { this->block = nullptr, resize(std::make_pair(1, 1)); }

matrix::matrix(std::pair<long, long> shape) { this->block = nullptr, resize(shape); }

matrix::matrix(const matrix &target)
{
    this->block = nullptr, resize(std::make_pair(target.ln, target.col));
    memcpy(this->block, target.block, sizeof(double) * target.ln * target.col);
}

double *matrix::operator[](const long &rhs) { return block + 1LL * rhs * col; }

matrix matrix::operator+(const matrix &rhs)
{
    if (rhs.ln == this->ln && rhs.col == this->col)
    {
        matrix ret(this->getShape()), _rhs = rhs;
        for (long i = 0; i < ln; i++)
            for (long j = 0; j < col; j++)
                ret[i][j] = (*this)[i][j] + _rhs[i][j];
        return ret;
    }
    else
        throw std::invalid_argument("The shapes between two matrices are not identical.");
}

matrix matrix::operator*(const matrix &rhs)
{
    if (this->col == rhs.ln)
    {
        matrix ret(std::make_pair(this->ln, rhs.col)), _rhs = rhs;
        for (long i = 0; i < this->ln; i++)
            for (long k = 0; k < this->col; k++)
            {
                double mid = (*this)[i][k];
                for (long j = 0; j < rhs.col; j++)
                    ret[i][j] += mid * _rhs[k][j];
            }
        return ret;
    }
    else
        throw std::invalid_argument("The shapes between two matrices are invalid to operate multiplication.");
}

matrix matrix::transpose()
{
    matrix ret(std::make_pair(this->col, this->ln));
    for (long i = 0; i < this->ln; i++)
        for (long j = 0; j < this->col; j++)
            ret[j][i] = (*this)[i][j];
    return ret;
}

matrix matrix::identity(long rhs)
{
    matrix ret(std::make_pair(rhs, rhs));
    for (long i = 0; i < rhs; i++)
        ret[i][i] = 1;
    return ret;
}

matrix matrix::echelonForm(const matrix &rhs)
{
    matrix ret = rhs;
    for (long i = 0; i < ret.ln; i++)
    {
        double pivot = ret[i][i];
        if (fabs(pivot) < KALOMLIB_eps)
            throw std::invalid_argument("There is a zero pivot during elimination.");
        for (long j = i + 1; j < ret.ln; j++)
        {
            double rate = ret[j][i] / pivot;
            for (long k = i; k < ret.col; k++)
                ret[j][k] -= rate * ret[i][k];
        }
    }
    return ret;
}

matrix matrix::reducedEchelonForm(const matrix &rhs)
{
    matrix ret = echelonForm(rhs);
    for (long i = ret.ln - 1; i >= 0; i--)
    {
        double pivot = ret[i][i];
        for (long j = i - 1; j >= 0; j--)
        {
            double rate = ret[j][i] / pivot;
            for (long k = i; k < ret.col; k++)
                ret[j][k] -= rate * ret[i][k];
        }
    }
    return ret;
}

long matrix::rank(const matrix &rhs)
{
    matrix ret = eliminate(rhs);
    long res = 0, limit = std::min(rhs.ln, rhs.col);
    for (; res < limit; res++)
        if (fabs(ret[res][res]) < KALOMLIB_eps)
            break;
    return res;
}

matrix matrix::inverse(const matrix &rhs)
{
    if (rhs.ln == rhs.col)
    {
        matrix mma(std::make_pair(rhs.ln, rhs.col << 1)), _rhs = rhs;
        for (long i = 0; i < _rhs.ln; i++)
        {
            mma[i][i + _rhs.col] = 1;
            for (long j = 0; j < _rhs.col; j++)
                mma[i][j] = _rhs[i][j];
        }
        mma = reducedEchelonForm(mma);
        matrix ret(std::make_pair(rhs.ln, rhs.col));
        for (long i = 0; i < _rhs.ln; i++)
            for (long j = 0; j < _rhs.col; j++)
                ret[i][j] = mma[i][j + _rhs.col] / mma[i][i];
        return ret;
    }
    else
        throw std::invalid_argument("The matrix you wanna inverse is not a square matrix.");
}

matrix matrix::eliminate(const matrix &rhs)
{
    matrix ret = rhs;
    long pivot_pos = 0;
    for (long i = 0; i < ret.ln; i++, pivot_pos++)
    {
        double pivot = ret[i][pivot_pos];
        while (fabs(pivot) < KALOMLIB_eps && pivot_pos < ret.col)
        {
            for (long j = i + 1; j < ret.ln; j++)
                if (fabs(ret[j][pivot_pos]) >= KALOMLIB_eps)
                {
                    for (long k = pivot_pos; k < ret.col; k++)
                        std::swap(ret[i][k], ret[j][k]);
                    break;
                }
            pivot = ret[i][pivot_pos];
            if (fabs(pivot) < KALOMLIB_eps)
                pivot_pos++;
        }
        if (pivot_pos >= ret.col)
            break;
        for (long j = i + 1; j < ret.ln; j++)
        {
            double rate = ret[j][i] / pivot;
            for (long k = i; k < ret.col; k++)
                ret[j][k] -= rate * ret[i][k];
        }
    }
    for (long i = ret.ln - 1; i >= 0; i--)
    {
        long pivot_pos = i;
        while (fabs(ret[i][pivot_pos]) < KALOMLIB_eps && pivot_pos < ret.col)
            pivot_pos++;
        if (pivot_pos >= ret.col)
            continue;
        double pivot = ret[i][pivot_pos];
        for (long j = i - 1; j >= 0; j--)
        {
            double rate = ret[j][i] / pivot;
            for (long k = i; k < ret.col; k++)
                ret[j][k] -= rate * ret[i][k];
        }
        for (long j = pivot_pos; j < ret.col; j++)
            ret[i][j] /= pivot;
    }
    return ret;
}

matrix matrix::project(const matrix &rhs)
{
    matrix param = rhs;
    long rnk = rank(rhs);
    if (rnk == param.col)
    {
        matrix param_t = param.transpose();
        return (param * matrix::inverse(param_t * param)) * param_t;
    }
    else
        throw std::invalid_argument("The matrix doesn't describe a subspace.");
}