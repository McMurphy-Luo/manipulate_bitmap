#include "MainWindowView.h"
#include <memory>
#include <sstream>
#include "gdiplus.h"

using std::pair;
using std::shared_ptr;
using std::make_pair;
using std::mem_fn;
using std::basic_stringstream;
using std::bind;
using std::placeholders::_1;
using std::placeholders::_2;
using std::placeholders::_3;

MainWindowView::MainWindowView(shared_ptr<MainWindow> the_main_window)
  : main_window_(the_main_window) 
  , conn_of_paint_event_(the_main_window->Connect(
    WM_PAINT,
    bind(mem_fn(&MainWindowView::OnPaint), this, _1, _2, _3)
  )) {
  

}

pair<bool, LRESULT> MainWindowView::OnPaint(UINT msg, WPARAM w_param, LPARAM l_param) {
  PAINTSTRUCT paint_structure;
  HDC paint_dc = BeginPaint(main_window_->WindowHandler(), &paint_structure);
  {
    basic_stringstream<TCHAR> debug_stream;
    debug_stream << TEXT("paint_structure.fErase ") << paint_structure.fErase
      << TEXT(" paint_structure.rcPaint ") << paint_structure.rcPaint.left
      << TEXT(" ") << paint_structure.rcPaint.top
      << TEXT(" ") << paint_structure.rcPaint.right
      << TEXT(" ") << paint_structure.rcPaint.bottom
      << std::endl;
    Gdiplus::Graphics graphis(paint_dc);
    if (paint_structure.fErase) {
      Gdiplus::Color bg_color(255, 255, 255, 255);
      graphis.Clear(bg_color);
    }
    OutputDebugString(debug_stream.str().c_str());
  }
  EndPaint(main_window_->WindowHandler(), &paint_structure);
  return make_pair(true, 0);
}