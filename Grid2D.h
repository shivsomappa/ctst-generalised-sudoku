#pragma once
#include <vector>
#include <stdexcept>
#include <algorithm>
#include <iostream>

// ============================================================
//  Grid2D  —  contiguous 2D int array backed by a flat vector
//  Memory layout: row-major (C order)
//  Index formula: [i][j] → i*Y + j
// ============================================================

class Grid2D {
public:

    // --------------------------------------------------------
    // Constructors
    // --------------------------------------------------------

    Grid2D() : X(0), Y(0) {}

    Grid2D(int x, int y, int init = 0)
        : X(x), Y(y), data(x * y, init) {}

    // --------------------------------------------------------
    // Dynamic resize  (old data lost)
    // --------------------------------------------------------

    void resize(int x, int y, int init = 0) {
        X = x; Y = y;
        data.assign(x * y, init);
    }

    // --------------------------------------------------------
    // Dynamic resize  (preserves data where indices still valid)
    // --------------------------------------------------------

    void resizeKeep(int newX, int newY, int init = 0) {
        std::vector<int> newData(newX * newY, init);

        int minX = std::min(X, newX);
        int minY = std::min(Y, newY);

        for (int i = 0; i < minX; i++)
            for (int j = 0; j < minY; j++)
                newData[i * newY + j] = data[index(i, j)];

        X = newX; Y = newY;
        data = std::move(newData);
    }

    // --------------------------------------------------------
    // Add / remove a row along X axis
    // --------------------------------------------------------

    void pushRow(int init = 0) {
        data.insert(data.end(), Y, init);
        ++X;
    }

    void popRow() {
        if (X == 0) return;
        data.erase(data.end() - Y, data.end());
        --X;
    }

    // --------------------------------------------------------
    // Element access  (bounds-checked)
    // --------------------------------------------------------

    int& at(int i, int j) {
        check(i, j);
        return data[index(i, j)];
    }

    const int& at(int i, int j) const {
        check(i, j);
        return data[index(i, j)];
    }

    // --------------------------------------------------------
    // Element access  (unchecked, fast)
    // --------------------------------------------------------

    int& operator()(int i, int j)             { return data[index(i, j)]; }
    const int& operator()(int i, int j) const { return data[index(i, j)]; }

    // --------------------------------------------------------
    // Dimensions
    // --------------------------------------------------------

    int xSize() const { return X; }
    int ySize() const { return Y; }
    int total()  const { return X * Y; }
    bool empty() const { return data.empty(); }

    // --------------------------------------------------------
    // Reserve / shrink
    // --------------------------------------------------------

    void reserve(int x, int y)  { data.reserve(x * y); }
    void shrinkToFit()          { data.shrink_to_fit(); }

    // --------------------------------------------------------
    // Fill / clear
    // --------------------------------------------------------

    void fill(int val) { std::fill(data.begin(), data.end(), val); }
    void clear()       { X = 0; Y = 0; data.clear(); }

    // --------------------------------------------------------
    // Raw contiguous pointer
    // --------------------------------------------------------

    int*       rawData()       { return data.data(); }
    const int* rawData() const { return data.data(); }

    // --------------------------------------------------------
    // Debug print
    // --------------------------------------------------------

    void print() const {
        for (int i = 0; i < X; i++) {
            for (int j = 0; j < Y; j++)
                std::cout << (*this)(i, j) << " ";
            std::cout << "\n";
        }
    }

private:
    int X, Y;
    std::vector<int> data;   // single contiguous block

    int index(int i, int j) const {
        return i * Y + j;
    }

    void check(int i, int j) const {
        if (i < 0 || i >= X || j < 0 || j >= Y)
            throw std::out_of_range("Grid2D: index out of range");
    }
};
