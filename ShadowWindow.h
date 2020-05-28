#ifndef MANIPULATE_BITMAP_SHADOW_WINDOW_H_
#define MANIPULATE_BITMAP_SHADOW_WINDOW_H_

#include "Windows.h"
#include "tchar.h"
#include "signals.h"

const char* const kShadowWindowClass = u8"ShadowWindow.McMurphy.Luo";

class ShadowWindow {
public:
  ShadowWindow();

  ~ShadowWindow();

  HWND WindowHandle() const { return window_handle_;  }

  std::pair<bool, LRESULT> Trigger(UINT msg, WPARAM w_param, LPARAM l_param);

  signals::connection Connect(UINT msg, std::function<std::pair<bool, LRESULT>(UINT, WPARAM, LPARAM)> handle);

private:
  HWND window_handle_;
};

#endif //MANIPULATE_BITMAP_SHADOW_WINDOW_H_