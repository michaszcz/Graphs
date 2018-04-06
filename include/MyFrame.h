#pragma once

#include "MainPanel.h"
#include "RightPanel.h"

class MyFrame : public wxFrame {
public:

    MyFrame(const wxSize &size, const wxString &title = _T("Sieci przepływowe")) :
            wxFrame(nullptr, -1, title) {
        this->SetStatusBar(nullptr);
        this->SetSize(size);
        this->Center();

        initFrame();
    }

private:
    void initFrame() {
        auto *hbox = new wxBoxSizer(wxHORIZONTAL);
        mainPanel = new MainPanel(this);
        rightPanel = new RightPanel(this, mainPanel);
        hbox->Add(mainPanel, 7, wxEXPAND | wxALL, 3);
        hbox->Add(rightPanel, 3, wxEXPAND | wxALL, 3);

        this->SetSizer(hbox);
        this->SetBackgroundColour(_T("#b0b0b2"));
    }

    void OnExit(wxCommandEvent &) {
        Close(true);
    }

    MainPanel *mainPanel;
    RightPanel *rightPanel;
};
