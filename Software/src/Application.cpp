/* Adding includes */
#include <wx/wx.h>
#include "Application.h"
#include "MainFrame.h"

/* Choosing which class represents the application */
wxIMPLEMENT_APP(Application)

/* Creation of the window itself */
bool Application::OnInit(){
  MainFrame* mainFrame = new MainFrame("C++ GUI");
  mainFrame-> Show();
  return true;
};
