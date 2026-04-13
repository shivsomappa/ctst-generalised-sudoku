#pragma once
#include <vector>
#include <stdexcept>
#include <algorithm>
#include <iostream>

// ============================================================
//  Grid3D  —  contiguous 3D int array backed by a flat vector
//  Memory layout: row-major (C order)
//  Index formula: [i][j][k] → i*(Y*Z) + j*Z + k
// ============================================================

class Grid3D {
public:

    // --------------------------------------------------------
    // Constructors
    // --------------------------------------------------------

    Grid3D() : X(0), Y(0), Z(0) {}

    Grid3D(int x, int y, int z, int init = 0)
        : X(x), Y(y), Z(z), data(x * y * z, init) {}

    // --------------------------------------------------------
    // Dynamic resize  (old data lost)
    // --------------------------------------------------------

    void resize(int x, int y, int z, int init = 0) {
        X = x; Y = y; Z = z;
        data.assign(x * y * z, init);
    }

    // --------------------------------------------------------
    // Dynamic resize  (preserves data where indices still valid)
    // --------------------------------------------------------

    void resizeKeep(int newX, int newY, int newZ, int init = 0) {
        std::vector<int> newData(newX * newY * newZ, init);

        int minX = std::min(X, newX);
        int minY = std::min(Y, newY);
        int minZ = std::min(Z, newZ);

        for (int i = 0; i < minX; i++)
            for (int j = 0; j < minY; j++)
                for (int k = 0; k < minZ; k++)
                    newData[i * (newY * newZ) + j * newZ + k] = data[index(i, j, k)];

        X = newX; Y = newY; Z = newZ;
        data = std::move(newData);
    }

    // --------------------------------------------------------
    // Add / remove a slice along X axis
    // --------------------------------------------------------

    void pushSliceX(int init = 0) {
        data.insert(data.end(), Y * Z, init);
        ++X;
    }

    void popSliceX() {
        if (X == 0) return;
        data.erase(data.end() - Y * Z, data.end());
        --X;
    }

    // --------------------------------------------------------
    // Element access  (bounds-checked)
    // --------------------------------------------------------

    int& at(int i, int j, int k) {
        check(i, j, k);
        return data[index(i, j, k)];
    }

    const int& at(int i, int j, int k) const {
        check(i, j, k);
        return data[index(i, j, k)];
    }

    // --------------------------------------------------------
    // Element access  (unchecked, fast)
    // --------------------------------------------------------

    int& operator()(int i, int j, int k)             { return data[index(i, j, k)]; }
    const int& operator()(int i, int j, int k) const { return data[index(i, j, k)]; }

    // --------------------------------------------------------
    // Dimensions
    // --------------------------------------------------------

    int xSize() const { return X; }
    int ySize() const { return Y; }
    int zSize() const { return Z; }
    int total()  const { return X * Y * Z; }
    bool empty() const { return data.empty(); }

    // --------------------------------------------------------
    // Reserve / shrink
    // --------------------------------------------------------

    void reserve(int x, int y, int z) { data.reserve(x * y * z); }
    void shrinkToFit()                { data.shrink_to_fit(); }

    // --------------------------------------------------------
    // Fill / clear
    // --------------------------------------------------------

    void fill(int val) { std::fill(data.begin(), data.end(), val); }
    void clear()       { X = 0; Y = 0; Z = 0; data.clear(); }

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
            std::cout << "[ slice " << i << " ]\n";
            for (int j = 0; j < Y; j++) {
                for (int k = 0; k < Z; k++)
                    std::cout << (*this)(i, j, k) << " ";
                std::cout << "\n";
            }
        }
    }

private:
    int X, Y, Z;
    std::vector<int> data;   // single contiguous block

    int index(int i, int j, int k) const {
        return i * (Y * Z) + j * Z + k;
    }

    void check(int i, int j, int k) const {
        if (i < 0 || i >= X || j < 0 || j >= Y || k < 0 || k >= Z)
            throw std::out_of_range("Grid3D: index out of range");
    }
};
