#pragma once

#include <wx/wxprec.h>
#include "MainPanel.h"

class RightPanel : public wxPanel {
public:
    RightPanel(wxFrame *parent, MainPanel *main);

private:
    void init();

    void onGenerateBtn(wxCommandEvent &);
    void setMaxFlow(unsigned int val);

    MainPanel *mainPanel;
    wxTextCtrl *totalLayers;
    wxStaticText *maxFlowText;

    unsigned int layersVal;

    enum {
        GENERATE_BTN = wxID_HIGHEST + 1,
        TOTAL_LAYERS
    };

DECLARE_EVENT_TABLE()
};
