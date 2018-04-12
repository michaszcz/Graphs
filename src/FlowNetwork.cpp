#include <sstream>
#include <iostream>
#include "FlowNetwork.h"
#include <algorithm>

FlowNetwork::FlowNetwork(unsigned int N) : N(N) {
    verticesOnLayer.resize(N + 2);
    verticesOnLayer[0] = verticesOnLayer[N + 1] = 1; // source and sink
    // Random vertices
    for (unsigned int i = 1; i <= N; ++i) {
        verticesOnLayer[i] = random<unsigned int>(2, N);
        totalVertices += verticesOnLayer[i];
    }
    adjMatrix = std::make_unique<AdjacencyMatrix>(totalVertices);

    // Connect source and sink
    for (unsigned int i = idxOfVertexFromLayer(1);
         i < idxOfVertexFromLayer(1) + verticesOnLayer[1]; ++i)
        adjMatrix->addEdge(idxOfVertexFromLayer(0), i, Edge{0, random(1, 10)});

    // Connect vertices from layer to random from next layer
    unsigned int idxOfVertex = 1; // Skip source
    for (unsigned int i = 1; i <= N; ++i) {
        for (unsigned int j = 0; j < verticesOnLayer[i]; ++j) {
            unsigned int vertexToConnect =
                    random<unsigned int>(1, verticesOnLayer[i + 1]) - 1;
            adjMatrix->addEdge(idxOfVertex,
                               idxOfVertexFromLayer(i + 1, vertexToConnect),
                               Edge{0, random(1, 10)});
            ++idxOfVertex;
        }
    }

    // If vertex has no incoming edges add random one from previous layer
    idxOfVertex = totalVertices - 2;
    for (unsigned int i = N; i >= 1; --i) {
        for (unsigned int j = 0; j < verticesOnLayer[i]; ++j) {
            if (!adjMatrix->hasIncomingEdges(idxOfVertex)) {
                int vertexToConnect =
                        random<unsigned int>(1, verticesOnLayer[i - 1]) - 1;

                adjMatrix->addEdge(idxOfVertexFromLayer(i - 1,
                                                        vertexToConnect),
                                   idxOfVertex,
                                   Edge{0, random(1, 10)});
            }
            --idxOfVertex;
        }
    }

    // Add 2N random connections
    unsigned int connected = 0;
    unsigned int connectionFail = 0;
    while (connected < 2 * N) {
        auto randomLayerBegin = random<unsigned int>(0, N + 1);
        auto randomLayerEnd = random<unsigned int>(0, N + 1);
        unsigned int randomVertexStart =
                random<unsigned int>(1, verticesOnLayer[randomLayerBegin]) - 1;
        unsigned int randomVertexEnd =
                random<unsigned int>(1, verticesOnLayer[randomLayerEnd]) - 1;
        unsigned int rvsIdx = idxOfVertexFromLayer(randomLayerBegin,
                                                   randomVertexStart);
        unsigned int rveIdx = idxOfVertexFromLayer(randomLayerEnd,
                                                   randomVertexEnd);

        if (rvsIdx == totalVertices - 1 || rveIdx == 0 || rvsIdx == rveIdx)
            continue;

        if (!adjMatrix->getEdge(rvsIdx, rveIdx) && !adjMatrix->getEdge(
                rveIdx, rvsIdx)) {
            adjMatrix->addEdge(rvsIdx, rveIdx, Edge{0, random(1, 10)});
            ++connected;
        } else {
            ++connectionFail;
            if (connectionFail > 50 * N) break;
        }
    }
}

void FlowNetwork::draw(wxDC &dc) const {
    int height = dc.GetSize().GetHeight();
    int width = dc.GetSize().GetWidth();
    wxSize margin(40, 20);
    int widthPerLayer =
            (width - 2 * margin.x) / (verticesOnLayer.size() - 1);

    dc.SetBrush(*wxTRANSPARENT_BRUSH);
    wxPoint vertices[totalVertices];
    for (unsigned int i = 0; i < verticesOnLayer.size(); ++i) {
        for (unsigned int j = 0; j < verticesOnLayer[i]; ++j) {
            int heightPerVertex =
                    (height - 2 * margin.y) / (verticesOnLayer[i] + 1);
            vertices[idxOfVertexFromLayer(i, j)] = wxPoint(
                    margin.x + i * widthPerLayer,
                    margin.y +
                    (j + 1) * heightPerVertex);
        }
    }

    for (unsigned int i = 0; i < totalVertices; ++i) {
        drawVertex(dc, vertices[i], std::to_string(i));
    }

    dc.SetBrush(*wxBLACK_BRUSH);
    // BFS - drawing graph edges
    bool visited[totalVertices]{true};
    std::queue<unsigned int> q;
    q.push(0); // push source
    while (!q.empty()) {
        unsigned int start = q.front();
        q.pop();
        for (unsigned int end = 0; end < adjMatrix->size(); ++end) {
            Edge *edge = adjMatrix->getEdge(start, end);
            if (edge) {
                std::stringstream s;
                s << edge->val << "/" << edge->max;
                drawArrow(dc, vertices[start], vertices[end], s.str());
                if (!visited[end]) {
                    q.push(end);
                    visited[end] = true;
                }
            }
        }
    }
}

void FlowNetwork::drawArrow(wxDC &dc, wxPoint start, wxPoint end,
                            const wxString &label, int radius) const {
    const double arrowheadLength = 10;
    wxRealPoint vec = end - start;
    wxRealPoint vecUnit(vec.x / sqrt(vec.x * vec.x + vec.y * vec.y),
                        vec.y / sqrt(vec.x * vec.x + vec.y * vec.y));
    start += wxPoint(vecUnit.x * radius, vecUnit.y * radius);
    end -= wxPoint(vecUnit.x * radius, vecUnit.y * radius);
    vec = end - start;

    wxRealPoint normalVec(-vecUnit.y * arrowheadLength / 3,
                          vecUnit.x * arrowheadLength / 3);
    wxRealPoint A = end - wxPoint(vecUnit.x * arrowheadLength,
                                  vecUnit.y * arrowheadLength);

    wxPoint arrowhead[] = {A - normalVec, end, A + normalVec};
    dc.DrawLine(start, end);
    dc.DrawPolygon(3, arrowhead);

    dc.DrawLabel(label, wxRect(start, end - wxPoint(vec / 2)),
                 wxALIGN_CENTRE);
}

void
FlowNetwork::drawVertex(wxDC &dc, const wxPoint &p, const wxString &label,
                        int radius) const {
    dc.DrawCircle(p, radius);
    dc.DrawLabel(label, wxRect(p.x - radius, p.y - radius, 2 * radius,
                               2 * radius), wxALIGN_CENTRE);
}

unsigned int
FlowNetwork::idxOfVertexFromLayer(unsigned int layer,
                                  unsigned int vertex) const {
    if (layer == N + 1) // sink
        return totalVertices - 1;
    unsigned int idx = 0;
    for (unsigned int i = 0; i < layer; ++i)
        idx += verticesOnLayer[i];
    return idx + vertex;
}

unsigned int FlowNetwork::fordFulkerson() {
    AdjacencyMatrix residualGraph(totalVertices);

    for (unsigned int i = 0; i < totalVertices; ++i)
        for (unsigned int j = 0; j < totalVertices; ++j) {
            Edge *edge = adjMatrix->getEdge(i, j);
            if (edge)
                residualGraph.addEdge(i, j, Edge{edge->max, edge->max});
        }

    unsigned int maxFlow = 0;
    unsigned int path[totalVertices];

    // Augment the flow if there is path from source to sink
    while (bfs(residualGraph, path)) {
        int pathFlow = INT_MAX;
        // find maximum pathFlow or minimum residual capacity
        for (unsigned int i = totalVertices - 1; i != 0; i = path[i]) {
            int flow = residualGraph.getEdge(path[i], i)->val;
            pathFlow = std::min(flow, pathFlow);
        }

        // Update main matrix
        for (unsigned int i = totalVertices - 1; i != 0; i = path[i])
            adjMatrix->getEdge(path[i], i)->val += pathFlow;

        // Update residual Graph
        for (unsigned int i = totalVertices - 1; i != 0; i = path[i]) {
            int flow = residualGraph.getEdge(path[i], i)->val - pathFlow;
            if (flow == 0)
                residualGraph.removeEdge(path[i], i);
            else if (flow > 0)
                residualGraph.addEdge(path[i], i, Edge{flow});
            else {
                residualGraph.addEdge(i, path[i], Edge{-flow});
            }
        }
        maxFlow += pathFlow;
    }

    return maxFlow;
}

bool FlowNetwork::bfs(AdjacencyMatrix &graph, unsigned int path[]) {
    bool visited[totalVertices]{true};
    std::queue<unsigned int> q;
    q.push(0); // push source
    while (!q.empty()) {
        unsigned int start = q.front();
        q.pop();
        for (unsigned int end = 0; end < graph.size(); ++end) {
            Edge *edge = graph.getEdge(start, end);
            if (edge && !visited[end]) {
                q.push(end);
                path[end] = start;
                visited[end] = true;
            }
        }
    }

    return visited[totalVertices - 1];
}

void FlowNetwork::saveToFile(const char *filename) const {
    adjMatrix->saveToFile(filename);
}
