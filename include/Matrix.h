#pragma once

#include <cstddef>
#include <vector>

struct Size {
    size_t rows;
    size_t cols;
};

template<typename T>
class Matrix final {
public:
    Matrix(size_t rows = 0, size_t cols = 0) : size{rows, cols} {
        elements.resize(rows * cols);
    }

    // No boundary checks!!!
    T &at(size_t row, size_t col) {
        return elements[row * cols() + col];
    }

    size_t rows() const {
        return size.rows;
    }

    size_t cols() const {
        return size.cols;
    }

private:
    std::vector<T> elements;
    Size size{0, 0};
};
