#pragma once

#include <random>
#include <memory>
#include <wx/dc.h>
#include <queue>
#include <list>
#include "Representation.h"

static thread_local std::mt19937 gen{std::random_device{}()};

template<typename T>
inline T random(T min, T max) {
    return std::uniform_int_distribution<T>{min, max}(gen);
}

class FlowNetwork {
public:
    explicit FlowNetwork(unsigned int N);

    unsigned int
    idxOfVertexFromLayer(unsigned int layer, unsigned int vertex = 0) const;

    void draw(wxDC &dc) const;

    unsigned int fordFulkerson();

    void saveToFile(const char *filename) const;

private:
    void drawVertex(wxDC &dc, const wxPoint &p, const wxString &label,
                    int radius = 15) const;

    void drawArrow(wxDC &dc, wxPoint start, wxPoint end,
                   const wxString &label, int radius = 17) const;

    bool bfs(AdjacencyMatrix &matrix, unsigned int path[]);

    std::vector<unsigned int> verticesOnLayer;
    std::unique_ptr<AdjacencyMatrix> adjMatrix = nullptr;
    unsigned int totalVertices = 2; // + source + sink
    const unsigned int N; // number of layers
};