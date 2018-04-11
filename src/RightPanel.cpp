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

    auto text2 = new wxStaticText(this, wxID_ANY, _T("Maksymalny przepływ wynosi: "));
    vbox->Add(text2, 0, wxALL | wxCENTER, 10);

    maxFlowText = new wxStaticText(this, wxID_ANY, _T(""));
    vbox->Add(maxFlowText, 0, wxALL | wxCENTER);

    this->SetSizer(vbox);
}

void RightPanel::onGenerateBtn(wxCommandEvent &) {
    if (Validate() && TransferDataFromWindow())
        setMaxFlow(mainPanel->createNetwork(layersVal));
}

void RightPanel::setMaxFlow(unsigned int val) {
    maxFlowText->SetLabelText(wxString(std::to_string(val)));
    GetParent()->Refresh();
}

