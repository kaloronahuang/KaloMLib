// main.cpp
#include <bits/stdc++.h>

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
    else if (typ == "elm")
    {
        string operand;
        cin >> operand;
        if (!pool.count(operand))
            throw invalid_argument("Invalid operand name.");
        try
        {
            matrix ret = matrix::eliminate(pool[operand]);
            return ret;
        }
        catch (const std::exception &e)
        {
            cout << e.what() << '\n';
            return matrix::identity(1);
        }
    }
    else if (typ == "rnk")
    {
        string operand;
        cin >> operand;
        if (!pool.count(operand))
            throw invalid_argument("Invalid operand name.");
        try
        {
            long ret = matrix::rank(pool[operand]);
            matrix rret(make_pair(1, 1));
            rret[0][0] = ret;
            return rret;
        }
        catch (const std::exception &e)
        {
            cout << e.what() << '\n';
            return matrix::identity(1);
        }
    }
    else if (typ == "tps")
    {
        string operand;
        cin >> operand;
        if (!pool.count(operand))
            throw invalid_argument("Invalid operand name.");
        try
        {
            matrix ret = pool[operand].transpose();
            return ret;
        }
        catch (const std::exception &e)
        {
            cout << e.what() << '\n';
            return matrix::identity(1);
        }
    }
    else if (typ == "prj")
    {
        string operand;
        cin >> operand;
        if (!pool.count(operand))
            throw invalid_argument("Invalid operand name.");
        try
        {
            matrix ret = matrix::project(pool[operand]);
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
            else
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
        else if (typ == "lreg")
        {
            string modelName;
            cin >> modelName;
            cout << "Please input the filename of the training dataset: " << endl;
            string filename;
            cin >> filename;

            ifstream file;
            file.open(filename.c_str(), ios::in);
            string lnstr;
            vector<vector<string>> block;
            while (getline(file, lnstr))
            {
                stringstream ss(lnstr);
                string str;
                vector<string> ln;
                while (getline(ss, str, ','))
                    ln.push_back(str);
                block.push_back(ln);
            }
            file.close();
            cout << "Item count: " << block.size() - 1 << endl;
            vector<algorithm::linearRegression::dataset> training_set;
            long dimension = block[0].size() - 2;
            block.erase(block.begin());
            for (vector<string> &ln : block)
            {
                matrix piece(make_pair(dimension, 1));
                algorithm::linearRegression::dataset curt;
                for (long i = 0; i < dimension; i++)
                {
                    stringstream ss(ln[i]);
                    ss >> piece[i][0];
                }
                stringstream ss_y(ln[dimension]);
                curt.first = piece;
                ss_y >> curt.second;
                training_set.push_back(curt);
            }

            cout << "Please input the desired limit of iterations: " << endl;
            long limit;
            double alpha;
            cin >> limit;
            cout << "Please input the desired alpha: " << endl;
            cin >> alpha;
            pool[modelName] = algorithm::linearRegression::linearRegression(training_set, alpha, limit);
        }
        else if (typ == "regeval")
        {
            string modelName, featurename;
            cin >> modelName >> featurename;
            if (!pool.count(modelName))
                cout << "Undefined reference to matrix \"" << modelName << "\"" << endl;
            else if (!pool.count(featurename))
                cout << "Undefined reference to matrix \"" << featurename << "\"" << endl;
            else
                cout << algorithm::linearRegression::linearRegressionModelEvaluate(pool[modelName], pool[featurename]) << endl;
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