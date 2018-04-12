#pragma once

#include <memory>
#include <fstream>
#include "Matrix.h"

struct Edge {
    int val = 0;
    int max = 0;
};

class AdjacencyMatrix final {
public:
    explicit AdjacencyMatrix(const unsigned int totalVertices = 0) {
        matrixPtr = new Matrix<Edge *>(totalVertices, totalVertices);
    }

    ~AdjacencyMatrix() {
        for (size_t i = 0; i < size(); ++i)
            for (size_t j = 0; j < size(); ++j)
                delete matrixPtr->at(i, j);
        delete matrixPtr;
    }

    void addEdge(const unsigned int start, const unsigned int end,
                 const Edge &edge) {
        delete matrixPtr->at(start, end);
        matrixPtr->at(start, end) = new Edge(edge);
    }

    Edge *getEdge(const unsigned int start, const unsigned int end) {
        return matrixPtr->at(start, end);
    }

    size_t size() const {
        return matrixPtr->rows();
    }

    bool hasOutgoingEdges(unsigned int vertex) {
        for (size_t col = 0; col < size(); ++col)
            if (matrixPtr->at(vertex, col))
                return true;
        return false;
    }

    bool hasIncomingEdges(unsigned int vertex) {
        for (size_t row = 0; row < size(); ++row)
            if (matrixPtr->at(row, vertex))
                return true;
        return false;
    }

    void removeEdge(unsigned int start, unsigned int end) {
        delete matrixPtr->at(start, end);
        matrixPtr->at(start, end) = nullptr;
    }

    void saveToFile(const char *filename) const {
        std::ofstream file(filename);

        for (size_t i = 0; i < size(); ++i) {
            for (size_t j = 0; j < size(); ++j) {
                Edge *edge = matrixPtr->at(i, j);
                if (edge)
                    file << edge->max;
                else
                    file << 0;
                file << " ";
            }
            file << std::endl;
        }

        file.close();
    }

private:
    Matrix<Edge *> *matrixPtr = nullptr;
};