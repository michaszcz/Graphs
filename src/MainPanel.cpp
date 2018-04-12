#include "MainPanel.h"

BEGIN_EVENT_TABLE(MainPanel, wxPanel)
                EVT_PAINT(MainPanel::onPaint)
END_EVENT_TABLE()

MainPanel::MainPanel(wxFrame *parent) : wxPanel(parent) {
    backgroundColour.Set("#fff");
    this->SetBackgroundColour(backgroundColour);
}

void MainPanel::draw(wxDC &dc) const {
    dc.Clear();
    if (network)
        network->draw(dc);
}

void MainPanel::onPaint(wxPaintEvent &) {
    wxPaintDC dc(this);
    draw(dc);
}

unsigned int MainPanel::createNetwork(unsigned int totalLayers) {
    this->network = std::make_unique<FlowNetwork>(totalLayers);
    this->network->saveToFile("Graph.txt");
    return this->network->fordFulkerson();
}
