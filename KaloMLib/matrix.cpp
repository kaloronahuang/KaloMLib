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
            for (int k = i; k < ret.col; k++)
                ret[j][k] -= rate * ret[i][k];
        }
    }
    return ret;
}

long matrix::rank(const matrix &rhs)
{
    return -1;
}

matrix matrix::inverse(const matrix &rhs)
{
    if (rhs.ln == rhs.col)
    {
        matrix mma(std::make_pair(rhs.ln, rhs.col << 1)), _rhs = rhs;
        for (int i = 0; i < _rhs.ln; i++)
        {
            mma[i][i + _rhs.col] = 1;
            for (int j = 0; j < _rhs.col; j++)
                mma[i][j] = _rhs[i][j];
        }
        mma = reducedEchelonForm(mma);
        matrix ret(std::make_pair(rhs.ln, rhs.col));
        for (int i = 0; i < _rhs.ln; i++)
            for (int j = 0; j < _rhs.col; j++)
                ret[i][j] = mma[i][j + _rhs.col];
        return ret;
    }
    else
        throw std::invalid_argument("The matrix you wanna inverse is not a square matrix.");
}