#include "MainWindow.h"
#include <cassert>
#include <tchar.h>
#include <iostream>

using std::wstring;
using std::function;
using std::mem_fn;
using std::placeholders::_1;
using std::pair;
using std::make_pair;
using std::unordered_map;
using signals::signal;
using signals::connection;

namespace
{
  LRESULT CALLBACK WindowProc(HWND window_handle, UINT msg, WPARAM w_param, LPARAM l_param)
  {
    switch (msg) {
      case WM_CREATE:
      {
        CREATESTRUCT* p_create_struct = reinterpret_cast<CREATESTRUCT*>(l_param);
        MainWindow* p_created_wnd = reinterpret_cast<MainWindow*>(p_create_struct->lpCreateParams);
        SetWindowLongPtr(window_handle, GWLP_USERDATA, (LONG_PTR)p_created_wnd);
        return 0;
      }
      case WM_DESTROY:
      {
        PostQuitMessage(0);
        return 0;
      }
    }
    MainWindow* theWindow = reinterpret_cast<MainWindow*>(GetWindowLongPtrW(window_handle, GWLP_USERDATA));
    if (theWindow) {
      pair<bool, LRESULT> message_handle_result = theWindow->Trigger(msg, w_param, l_param);
      if (message_handle_result.first) {
        return message_handle_result.second;
      }
    }
    return DefWindowProc(window_handle, msg, w_param, l_param);
  }

  WNDCLASSEX MainWindowClass(HINSTANCE instance)
  {
    WNDCLASSEX the_window_class;
    the_window_class.cbSize = sizeof(the_window_class);
    the_window_class.style = CS_DBLCLKS;
    the_window_class.lpfnWndProc = WindowProc;
    the_window_class.cbClsExtra = 0;
    the_window_class.cbWndExtra = 0;
    the_window_class.hInstance = instance;
    the_window_class.hIcon = nullptr;
    the_window_class.hCursor = LoadCursorW(nullptr, MAKEINTRESOURCEW(32512));
    the_window_class.hbrBackground = nullptr;
    the_window_class.lpszMenuName = nullptr;
    the_window_class.hIconSm = nullptr;
    return the_window_class;
  }

  ATOM RegisterMainWindow(HINSTANCE module_handle) {
    static bool is_class_registered = false;
    static ATOM register_result;
    if (!is_class_registered) {
      WNDCLASSEX main_window_class = MainWindowClass(module_handle);
      wstring class_name = Utf8StringToWString(kMainWindowClass);
      main_window_class.lpszClassName = class_name.c_str();
      register_result = RegisterClassEx(&main_window_class);
      assert(register_result);
      is_class_registered = true;
    }
    return register_result;
  }

  class WindowMessageSlotTriggerPolicy {
  public:
    WindowMessageSlotTriggerPolicy(UINT message, WPARAM w_param, LPARAM l_param)
      : message_(message)
      , w_param_(w_param)
      , l_param_()
      , handled_(false)
      , result_(0) {

    }

    bool Trigger(function<pair<bool, LRESULT>(UINT, WPARAM, LPARAM)> slot) {
      pair<bool, LRESULT> result = slot(message_, w_param_, l_param_);
      handled_ = result.first;
      result_ = result.second;
      return !handled_;
    }

    bool Handled() const { return handled_; }

    LRESULT Result() const { return result_; }

  private:
    UINT message_;
    WPARAM w_param_;
    LPARAM l_param_;
    bool handled_;
    LRESULT result_;
  };
}

MainWindow::MainWindow(const Utf8String& window_name, HINSTANCE module_handle)
  : window_handle_(NULL)
  , last_message_()
  , signals_()
{
  RegisterMainWindow(module_handle);
  wstring class_name = Utf8StringToWString(kMainWindowClass);
  window_handle_ = CreateWindowEx(
    WS_EX_OVERLAPPEDWINDOW,
    class_name.c_str(),
    Utf8StringToWString(window_name).c_str(),
    WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
    CW_USEDEFAULT,
    CW_USEDEFAULT,
    CW_USEDEFAULT,
    CW_USEDEFAULT,
    nullptr,
    nullptr,
    module_handle,
    this
  );
  int error_code = GetLastError();
  TCHAR buffer[BUFSIZ];
  _itot_s(error_code, buffer, ARRAYSIZE(buffer), 10);
  OutputDebugString(buffer);
  assert(IsWindow(window_handle_));
}

MainWindow::~MainWindow() {
  if (::IsWindow(window_handle_)) {
    DestroyWindow(window_handle_);
  }
}

void MainWindow::Show(int show_flags)
{
  assert(IsWindow(window_handle_));
  ShowWindow(window_handle_, show_flags);
}

HWND MainWindow::WindowHandle() const
{
  return window_handle_;
}

MSG MainWindow::LastMessage() const
{
  return last_message_;
}

pair<bool, LRESULT> MainWindow::Trigger(UINT msg, WPARAM w_param, LPARAM l_param)
{
  unordered_map<UINT, signal<pair<bool, LRESULT>, UINT, WPARAM, LPARAM>>::iterator it = signals_.find(msg);
  if (it != signals_.end()) {
    WindowMessageSlotTriggerPolicy special_trigger(msg, w_param, l_param);
    it->second(bind(mem_fn(&WindowMessageSlotTriggerPolicy::Trigger), &special_trigger, _1));
    return make_pair<bool, LRESULT>(special_trigger.Handled(), special_trigger.Result());
  }
  return make_pair<bool, LRESULT>(false, 0);
}

RECT MainWindow::ClientRectangle() const
{
  RECT result;
  BOOL succeeded = GetClientRect(window_handle_, &result);
  assert(succeeded);
  return result;
}

connection MainWindow::Connect(UINT msg, function<pair<bool, LRESULT>(UINT, WPARAM, LPARAM)> handle)
{
  unordered_map<UINT, signal<pair<bool, LRESULT>, UINT, WPARAM, LPARAM>>::iterator it = signals_.find(msg);
  if (it == signals_.end()) {
    signal<pair<bool, LRESULT>, UINT, WPARAM, LPARAM> new_signal;
    it = signals_.emplace(msg, move(new_signal)).first;
  }
  return it->second.connect(handle);
}