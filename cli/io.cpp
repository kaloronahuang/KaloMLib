// io.cpp
#include "cli.hpp"
#include <cstdio>
#include <algorithm>

using namespace std;
using namespace KaloMLib;

void printMatrix(matrix &rhs)
{
    printf("[\n");
    for (int i = 0; i < rhs.ln; i++)
    {
        printf("[");
        for (int j = 0; j < rhs.col; j++)
            printf("%.4lf, ", rhs[i][j]);
        printf("],\n");
    }
    printf("]\n");
}

matrix inputMatrix()
{
    printf("Please input the shape of the matrix : [n lns] [m cols]\n");
    int n, m;
    scanf("%d%d", &n, &m);
    printf("Please input the values of the matrix\n");
    matrix ret(make_pair(n, m));
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            scanf("%lf", &ret[i][j]);
    return ret;
}