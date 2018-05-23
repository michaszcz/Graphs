#pragma once

#include <wx/wx.h>
#include "FlowNetwork.h"

class MainPanel : public wxPanel {
public:
    MainPanel(wxFrame *parent);

    unsigned int createNetwork(unsigned int);

    void draw(wxDC &dc) const;
private:
    void onPaint(wxPaintEvent &);

DECLARE_EVENT_TABLE()

    wxColour backgroundColour;
    std::unique_ptr<FlowNetwork> network;
};
