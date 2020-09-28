#ifndef MANIPULATE_BITMAP_SHADOW_WINDOW_H_
#define MANIPULATE_BITMAP_SHADOW_WINDOW_H_

#include <unordered_map>
#include "Windows.h"
#include "tchar.h"
#include "signals.h"
#include "Utf8String.h"

const char* const kShadowWindowClass = u8"ShadowWindow.McMurphy.Luo";

class MainWindow;

class ShadowWindow {
public:
  ShadowWindow(const Utf8String& window_name, HINSTANCE module_handle, std::shared_ptr<MainWindow> main_window);

  ~ShadowWindow();

  HWND WindowHandle() const { return window_handle_;  }

  std::pair<bool, LRESULT> Trigger(UINT msg, WPARAM w_param, LPARAM l_param);

  signals::connection Connect(UINT msg, std::function<std::pair<bool, LRESULT>(UINT, WPARAM, LPARAM)> handle);

private:
  HWND window_handle_;
  std::unordered_map<UINT, signals::signal<std::pair<bool, LRESULT>, UINT, WPARAM, LPARAM>> signals_;
  std::shared_ptr<MainWindow> main_window_;
};

#endif //MANIPULATE_BITMAP_SHADOW_WINDOW_H_