#pragma once

#include <wx/wxprec.h>
#include "MainPanel.h"

class RightPanel : public wxPanel {
public:
    RightPanel(wxFrame *parent, MainPanel *main);

private:
    void init();

    MainPanel *mainPanel;
    wxTextCtrl *totalLayers;
    unsigned int layersVal;

    void onGenerateBtn(wxCommandEvent &);

    enum {
        GENERATE_BTN = wxID_HIGHEST + 1,
        TOTAL_LAYERS
    };

DECLARE_EVENT_TABLE()
};
