// algorithm.hpp
#pragma once
#include "matrix.hpp"

namespace KaloMLib
{

    namespace algorithm
    {

        namespace linearRegression
        {

            typedef std::pair<matrix, double> dataset;

            matrix linearRegression(std::vector<dataset> &training_set, double alpha, long iter_limit);
            double linearRegressionModelEvaluate(matrix &model, matrix &feature);
            double linearRegressionModelError(std::vector<dataset> &training_set, matrix &theta);

        }

        namespace logisticRegression
        {

            typedef std::pair<matrix, bool> dataset;

            double sigmoid(double x);
            double dsigmoid(double x);
            matrix logisticRegression(std::vector<dataset> &training_set, double alpha, long iter_limit);
            double logisticRegressionModelEvaluate(matrix &model, matrix &feature);

        }

    }

}