// main.cpp
#include <cstdio>
#include <algorithm>
#include <string>
#include <map>
#include <cstring>
#include <iostream>

#include "cli.hpp"

using namespace std;
using namespace KaloMLib;

map<string, matrix> pool;

matrix processArithmetic(string typ)
{
    if (typ == "add")
    {
        string operand1, operand2;
        cin >> operand1 >> operand2;
        if ((!pool.count(operand1)) || (!pool.count(operand2)))
            throw invalid_argument("Invalid operand name.");
        matrix ret = pool[operand1] + pool[operand2];
        return ret;
    }
    else if (typ == "mul")
    {
        string operand1, operand2;
        cin >> operand1 >> operand2;
        if ((!pool.count(operand1)) || (!pool.count(operand2)))
            throw invalid_argument("Invalid operand name.");
        matrix ret = pool[operand1] * pool[operand2];
        return ret;
    }
    else if (typ == "ref")
    {
        string operand;
        cin >> operand;
        if (!pool.count(operand))
            throw invalid_argument("Invalid operand name.");
        try
        {
            matrix ret = matrix::reducedEchelonForm(pool[operand]);
            return ret;
        }
        catch (const std::exception &e)
        {
            cout << e.what() << '\n';
            return matrix::identity(1);
        }
    }
    else if (typ == "ef")
    {
        string operand;
        cin >> operand;
        if (!pool.count(operand))
            throw invalid_argument("Invalid operand name.");
        try
        {
            matrix ret = matrix::echelonForm(pool[operand]);
            return ret;
        }
        catch (const std::exception &e)
        {
            cout << e.what() << '\n';
            return matrix::identity(1);
        }
    }
    else if (typ == "inv")
    {
        string operand;
        cin >> operand;
        if (!pool.count(operand))
            throw invalid_argument("Invalid operand name.");
        try
        {
            matrix ret = matrix::inverse(pool[operand]);
            return ret;
        }
        catch (const std::exception &e)
        {
            cout << e.what() << '\n';
            return matrix::identity(1);
        }
    }
    else
        throw invalid_argument("Invalid operator.");
}

int main()
{
    while (true)
    {
        cout << "kalomlib_cli>";
        string typ;
        cin >> typ;
        if (typ == "exit")
            break;
        else if (typ == "set")
        {
            string name;
            cin >> name;
            pool[name] = inputMatrix();
        }
        else if (typ == "get")
        {
            string name;
            cin >> name;
            if (!pool.count(name))
                cout << "Undefined reference to matrix \"" << name << "\"" << endl;
            printMatrix(pool[name]);
        }
        else if (typ == "cal")
        {
            string retname, ctyp;
            cin >> retname >> ctyp;
            try
            {
                pool[retname] = processArithmetic(ctyp);
            }
            catch (const std::exception &e)
            {
                std::cerr << e.what() << '\n';
            }
        }
        else
        {
            try
            {
                matrix ret = processArithmetic(typ);
                printMatrix(ret);
            }
            catch (const std::exception &e)
            {
                std::cerr << e.what() << '\n';
            }
        }
    }
    return 0;
}