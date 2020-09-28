#include "BaseDefine.h"
#include <cassert>
#include "Windows.h"
#include "tchar.h"
#include "gdiplus.h"
#include "MainWindow.h"
#include "MainWindowView.h"

using std::shared_ptr;
using std::make_shared;
using Gdiplus::GdiplusStartup;
using Gdiplus::GdiplusShutdown;
using Gdiplus::GdiplusStartupInput;
using Gdiplus::GdiplusStartupOutput;
using Gdiplus::Status;

int APIENTRY _tWinMain(HINSTANCE instance, HINSTANCE previous_instance, LPTSTR command_line, int show) {
  int flag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
  flag |= _CRTDBG_LEAK_CHECK_DF;
  flag |= _CRTDBG_ALLOC_MEM_DF;
  _CrtSetDbgFlag(flag);
  _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE | _CRTDBG_MODE_DEBUG);
  _CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDERR);
  _CrtSetBreakAlloc(-1);
  CoInitialize(nullptr);
  ULONG_PTR gdi_plus_token;
  Status gdiplus_initialize_status;
  GdiplusStartupInput gdip_startup_input;
  gdiplus_initialize_status = GdiplusStartup(&gdi_plus_token, &gdip_startup_input, nullptr);
  assert(gdiplus_initialize_status == Status::Ok);
  shared_ptr<MainWindow> the_main_window(make_shared<MainWindow>("Hello World", instance));
  shared_ptr<MainWindowView> the_main_window_view(make_shared<MainWindowView>(the_main_window));
  ::SetWindowPos(the_main_window->WindowHandle(), 0, 200, 200, 100, 100, SWP_NOZORDER);
  the_main_window_view->InvalidRect(FALSE);
  MSG message;
  while (GetMessage(&message, nullptr, 0, 0)) {
    TranslateMessage(&message);
    DispatchMessage(&message);
  }
  the_main_window_view.reset();
  the_main_window.reset();
  GdiplusShutdown(gdi_plus_token);
  CoUninitialize();
  return 0;
}