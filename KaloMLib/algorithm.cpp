// algorithm.cpp
#include <cmath>

#include "include/KaloMLib.hpp"

using namespace std;

namespace KaloMLib
{
    namespace algorithm
    {

        namespace linearRegression
        {

            matrix linearRegression(vector<dataset> &training_set, double alpha, long iter_limit)
            {
                if (training_set.size() == 0)
                    return matrix();
                long pcs = training_set.size(), dimension = training_set[0].first.ln + 1;
                matrix trainingX = matrix(make_pair(pcs, dimension)), trainingY = matrix(make_pair(pcs, 1));
                for (long i = 0; i < pcs; i++)
                {
                    trainingX[i][0] = 1;
                    for (long j = 1; j < dimension; j++)
                        trainingX[i][j] = training_set[i].first[j - 1][0];
                    trainingY[i][0] = training_set[i].second;
                }
                matrix theta(make_pair(dimension, 1));
                // normal equation;
                matrix lefthandside = trainingX.transpose() * trainingX;
                if (matrix::rank(lefthandside) == dimension)
                {
                    theta = matrix::inverse(lefthandside) * trainingX.transpose() * trainingY;
                    return theta;
                }
                // stocastic gradient descent;
                long iter = 0, data_id = 0;
                while (iter < iter_limit)
                {
                    iter++;
                    double error = linearRegressionModelEvaluate(theta, training_set[data_id].first) - training_set[data_id].second;
                    matrix gradient(make_pair(dimension, 1));
                    for (long j = 0; j < dimension; j++)
                        gradient[j][0] = alpha * error * trainingX[data_id][j];
                    theta = theta - gradient;
                    data_id++;
                    data_id %= pcs;
                }
                return theta;
            }

            double linearRegressionModelEvaluate(matrix &model, matrix &feature)
            {
                matrix input(make_pair(feature.ln + 1, 1));
                input[0][0] = 1;
                for (long i = 0; i < feature.ln; i++)
                    input[i + 1][0] = feature[i][0];
                return (model.transpose() * input)[0][0];
            }

            double linearRegressionModelError(vector<dataset> &training_set, matrix &theta)
            {
                long pcs = training_set.size(), dimension = training_set[0].first.ln + 1;
                matrix trainingX = matrix(make_pair(pcs, dimension)), trainingY = matrix(make_pair(pcs, 1));
                for (long i = 0; i < pcs; i++)
                {
                    trainingX[i][0] = 1;
                    for (long j = 1; j < dimension; j++)
                        trainingX[i][j] = training_set[i].first[j - 1][0];
                    trainingY[i][0] = training_set[i].second;
                }
                matrix error = (trainingX * theta - trainingY);
                return 0.5 * (error.transpose() * error)[0][0];
            }

        }

        namespace logisticRegression
        {

            inline double sigmoid(double x) { return 1.0 / (1 + exp(-x)); }

            inline double dsigmoid(double x) { return sigmoid(x) * (1 - sigmoid(x)); }

            double logisticRegressionModelEvaluate(matrix &model, matrix &feature)
            {
                matrix input(make_pair(feature.ln + 1, 1));
                input[0][0] = 1;
                for (long i = 0; i < feature.ln; i++)
                    input[i + 1][0] = feature[i][0];
                return sigmoid((model.transpose() * input)[0][0]);
            }

            matrix logisticRegression(std::vector<dataset> &training_set, double alpha, long iter_limit)
            {
                if (training_set.size() == 0)
                    return matrix();
                long pcs = training_set.size(), dimension = training_set[0].first.ln + 1;
                matrix trainingX = matrix(make_pair(pcs, dimension)), trainingY = matrix(make_pair(pcs, 1));
                for (long i = 0; i < pcs; i++)
                {
                    trainingX[i][0] = 1;
                    for (long j = 1; j < dimension; j++)
                        trainingX[i][j] = training_set[i].first[j - 1][0];
                    trainingY[i][0] = training_set[i].second;
                }
                matrix theta(make_pair(dimension, 1));
                // stocastic gradient descent;
                long iter = 0, data_id = 0;
                while (iter < iter_limit)
                {
                    iter++;
                    double error = logisticRegressionModelEvaluate(theta, training_set[data_id].first) - training_set[data_id].second;
                    matrix gradient(make_pair(dimension, 1));
                    for (long j = 0; j < dimension; j++)
                        gradient[j][0] = alpha * error * trainingX[data_id][j];
                    theta = theta + gradient;
                    data_id++;
                    data_id %= pcs;
                }
                return theta;
            }

        }

    }
}