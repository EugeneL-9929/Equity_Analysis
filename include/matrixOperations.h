#ifndef MATRIXOPERATIONS_H
#define MATRIXOPERATIONS_H

#include <string>
#include <vector>
#include <iostream>

using namespace std;

namespace MO
{
    template <typename T>
    vector<T> sumColumn(const vector<vector<T>> &matrix)
    {
        vector<T> sumColumnMatrix{};
        if (!matrix.empty())
        {
            size_t numColumns = matrix[0].size();
            sumColumnMatrix.resize(numColumns, T{});
            for (const auto &row : matrix)
            {
                if (row.size() != numColumns)
                    cout << "inconsistent columns length." << endl;
                for (size_t j = 0; j < numColumns; ++j)
                    sumColumnMatrix[j] += row[j];
            }
        }
        return sumColumnMatrix;
    }

    template <typename T>
    vector<T> sumRow(const vector<vector<T>> &matrix)
    {
        vector<T> sumRowMatrix{};
        if (!matrix.empty())
        {
            size_t numRows = matrix.size();
            size_t numColumns = matrix[0].size();
            sumRowMatrix.resize(numRows, T{});
            size_t ii{};
            for (const auto &row : matrix)
            {
                if (row.size() != numColumns)
                    cout << "inconsistent columns length." << endl;
                for (size_t j = 0; j < numColumns; ++j)
                    sumRowMatrix[ii] += row[j];
                ii++;
            }
        }
        return sumRowMatrix;
    }
}

#endif