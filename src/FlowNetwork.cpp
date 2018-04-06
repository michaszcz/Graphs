#include "FlowNetwork.h"

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
        adjMatrix->addEdge(idxOfVertexFromLayer(0), i, Edge{random(1, 10)});

    // Connect vertices from layer to random from next layer
    unsigned int idxOfVertex = 1; // Skip source
    for (unsigned int i = 1; i <= N; ++i) {
        for (unsigned int j = 0; j < verticesOnLayer[i]; ++j) {
            unsigned int vertexToConnect = random<unsigned int>(1, verticesOnLayer[i + 1]) - 1;
            adjMatrix->addEdge(idxOfVertex,
                               idxOfVertexFromLayer(i + 1, vertexToConnect),
                               Edge{random(1, 10)});
            ++idxOfVertex;
        }
    }

    // If vertex has no incoming edges add random one from previous layer
    idxOfVertex = totalVertices - 2;
    for (unsigned int i = N; i >= 1; --i) {
        for (unsigned int j = 0; j < verticesOnLayer[i]; ++j) {
            if (!adjMatrix->hasIncomingEdges(idxOfVertex)) {
                int vertexToConnect = random<unsigned int>(1, verticesOnLayer[i - 1]) - 1;

                adjMatrix->addEdge(idxOfVertexFromLayer(i - 1,
                                                        vertexToConnect),
                                   idxOfVertex,
                                   Edge{random(1, 10)});
            }
            --idxOfVertex;
        }
    }

    // Add 2N random connections
    unsigned int connected = 0;
//        unsigned int notconn = 0; // TODO DEBUG
    while (connected < 2 * N) {
        unsigned int randomLayer = random<unsigned int>(1, N - 1);
        unsigned int randomVertexStart = random<unsigned int>(1, verticesOnLayer[randomLayer]) - 1;
        unsigned int randomVertexEnd =
                random<unsigned int>(1, verticesOnLayer[randomLayer + 1]) - 1;
        unsigned int rvsIdx = idxOfVertexFromLayer(randomLayer,
                                                   randomVertexStart);
        unsigned int rveIdx = idxOfVertexFromLayer(randomLayer + 1,
                                                   randomVertexEnd);

        if (random(0, 1)) {
            unsigned int tmp = rveIdx;
            rveIdx = rvsIdx;
            rvsIdx = tmp;
        }

        // TODO Choose version
        if (!adjMatrix->getEdge(rvsIdx, rveIdx)) {
            adjMatrix->addEdge(rvsIdx, rveIdx, Edge{random(1, 10)});
            ++connected;
        }
//            if (!adjMatrix->getEdge(rvsIdx, rveIdx) && !adjMatrix->getEdge(
//                    rveIdx, rvsIdx)) {
//                adjMatrix->addEdge(rvsIdx, rveIdx, Edge{random(1, 10)});
//                ++connected;
//            } else {
//                ++notconn;
//                if (notconn > 300) connected = 21;
//                std::cout << "not found: " << notconn << ", " << connected << std::endl;
//            }
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

unsigned int
FlowNetwork::idxOfVertexFromLayer(unsigned int layer, unsigned int vertex) const {
    if (layer == N + 1) // sink
        return totalVertices - 1;
    unsigned int idx = 0;
    for (unsigned int i = 0; i < layer; ++i)
        idx += verticesOnLayer[i];
    return idx + vertex;
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
                drawArrow(dc, vertices[start], vertices[end],
                          std::to_string(edge->val));
                if (!visited[end]) {
                    q.push(end);
                    visited[end] = true;
                }
            }
        }

//        if (q.empty())
//            for (unsigned int i = 0; i < totalVertices; ++i)
//                if (!visited[i]) {
//                    std::cout << "tak" << std::endl;
//                    q.push(i);
//                    visited[i] = true;
//                    break;
//                }
    }
}

void
FlowNetwork::drawVertex(wxDC &dc, const wxPoint &p, const wxString &label,
                        int radius) const {
    dc.DrawCircle(p, radius);
    dc.DrawLabel(label, wxRect(p.x - radius, p.y - radius, 2 * radius,
                               2 * radius), wxALIGN_CENTRE);
}
