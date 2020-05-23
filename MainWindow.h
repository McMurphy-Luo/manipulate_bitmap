#ifndef ZOOM_LOGO_MAIN_WINDOW_H_
#define ZOOM_LOGO_MAIN_WINDOW_H_

#include "BaseDefine.h"
#include <unordered_map>
#include <Windows.h>
#include "Utf8String.h"
#include "signals.h"

const char* const kMainWindowClass = u8"MainWindow.mcmurphy.luo";

class MainWindow
{
public:
  MainWindow(const Utf8String& window_name, HINSTANCE module_handle);

  MainWindow(const MainWindow&) = delete;

  MainWindow& operator=(const MainWindow&) = delete;

  void Show(int show_flags);

  HWND WindowHandler() const;

  RECT ClientRectangle() const;

  MSG LastMessage() const;

  std::pair<bool, LRESULT> Trigger(UINT msg, WPARAM w_param, LPARAM l_param);

  signals::connection Connect(UINT msg, std::function<std::pair<bool, LRESULT>(UINT, WPARAM, LPARAM)> handler);

private:
  HWND window_handler_;
  MSG last_message_;
  std::unordered_map<UINT, signals::signal<std::pair<bool, LRESULT>, UINT, WPARAM, LPARAM>> signals_;
};

#endif // MAIN_WINDOW_H_