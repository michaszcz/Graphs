#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#	include "wx/wx.h"
#endif

#include "MyApp.h"
#include "MyFrame.h"

bool MyApp::OnInit() {
    wxFrame *frame = new MyFrame(wxSize(800, 600));
    frame->Maximize(true);
	frame->Show(true);
    SetTopWindow(frame);
	return true;
}

IMPLEMENT_APP(MyApp)
