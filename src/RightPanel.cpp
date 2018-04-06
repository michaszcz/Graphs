#include "RightPanel.h"
#include <wx/valnum.h>

BEGIN_EVENT_TABLE(RightPanel, wxPanel)
                EVT_BUTTON(GENERATE_BTN, RightPanel::onGenerateBtn)
END_EVENT_TABLE()

RightPanel::RightPanel(wxFrame *parent, MainPanel *main) : wxPanel(parent) {
    this->SetBackgroundColour(wxColour(255, 220, 0));
    this->mainPanel = main;

    init();
}

void RightPanel::init() {
    wxBoxSizer *vbox = new wxBoxSizer(wxVERTICAL);
    vbox->Prepend(0,40);

    auto text = new wxStaticText(this, wxID_ANY, _T("Wybierz liczbę warstw:"));
    vbox->Add(text, 0, wxALL | wxCENTER, 10);

    totalLayers = new wxTextCtrl(this, TOTAL_LAYERS, _T("3"));
    auto validator = wxIntegerValidator<unsigned int>(&layersVal);
    validator.SetRange(0, 20);
    totalLayers->SetValidator(validator);
    vbox->Add(totalLayers, 0, wxALL | wxCENTER, 10);

    auto generateBtn = new wxButton(this, GENERATE_BTN,
                                    _T("Wygeneruj sieć przepływową"));
    vbox->Add(generateBtn, 0, wxALL | wxCENTER, 10);

    this->SetSizer(vbox);
}

void RightPanel::onGenerateBtn(wxCommandEvent &) {
    if (Validate() && TransferDataFromWindow())
        mainPanel->createNetwork(layersVal);
}

