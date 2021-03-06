// matrix.hpp
#pragma once
#include <algorithm>
#include <vector>

namespace KaloMLib
{

    class matrix
    {
    public:
        double *operator[](const long &rhs);
        std::pair<long, long> getShape();
        void resize(std::pair<long, long> shape);
        matrix();
        matrix(std::pair<long, long> shape);
        matrix(const matrix &target);
        matrix operator+(const matrix &rhs);
        matrix operator-(const matrix &rhs);
        matrix operator*(const double &rhs);
        matrix operator*(const matrix &rhs);
        matrix transpose();

        static matrix identity(long rhs);
        static matrix echelonForm(const matrix &rhs);
        static matrix reducedEchelonForm(const matrix &rhs);
        static long rank(const matrix &rhs);
        static matrix inverse(const matrix &rhs);
        static matrix eliminate(const matrix &rhs);
        static matrix project(const matrix &rhs);

        long ln, col;

    private:
        double *block;
    };

}