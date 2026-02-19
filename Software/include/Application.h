/* Preventing recursion */
#ifndef _Application_H_
#define _Application_H_

/* Adding includes */
#include <wx/wx.h>

class Application : public wxApp{
public:
  bool OnInit();
};
