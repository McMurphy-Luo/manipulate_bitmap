#ifndef MANIPULATE_BITMAP_MAIN_WINDOW_H_
#define MANIPULATE_BITMAP_MAIN_WINDOW_H_

#include "BaseDefine.h"
#include <unordered_map>
#include <Windows.h>
#include "Utf8String.h"
#include "signals.h"

const char* const kMainWindowClass = u8"MainWindow.McMurphy.Luo";

class MainWindow
{
public:
  MainWindow(const Utf8String& window_name, HINSTANCE module_handle);

  ~MainWindow();

  MainWindow(const MainWindow&) = delete;

  MainWindow& operator=(const MainWindow&) = delete;

  void Show(int show_flags);

  void InvalidRect(const RECT& rect, BOOL erase);

  void InvalidRect(BOOL erase);

  RECT ClientRectangle() const;

  RECT WindowRectangle() const;

  HWND WindowHandle() const;

  MSG LastMessage() const;

  bool PostMessage(UINT msg, WPARAM wParam, LPARAM lParam);

  std::pair<bool, LRESULT> Trigger(UINT msg, WPARAM w_param, LPARAM l_param);

  signals::connection Connect(UINT msg, std::function<std::pair<bool, LRESULT>(UINT, WPARAM, LPARAM)> handle);

private:
  HWND window_handle_;
  MSG last_message_;
  std::unordered_map<UINT, signals::signal<std::pair<bool, LRESULT>, UINT, WPARAM, LPARAM>> signals_;
};

#endif // MANIPULATE_BITMAP_MAIN_WINDOW_H_