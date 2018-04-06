#pragma once

#include <wx/wxprec.h>
#include "FlowNetwork.h"

class MainPanel : public wxPanel {
public:
    MainPanel(wxFrame *parent);

    void createNetwork(unsigned int);

    void draw(wxDC &dc) const;
private:
    void onPaint(wxPaintEvent &);

DECLARE_EVENT_TABLE()

    wxColour backgroundColour;
    std::unique_ptr<FlowNetwork> network;
};
