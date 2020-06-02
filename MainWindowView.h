#ifndef MANIPULATE_BITMAP_MAIN_WINDOW_VIEW_H_
#define MANIPULATE_BITMAP_MAIN_WINDOW_VIEW_H_

#include <memory>
#include <utility>
#include "Windows.h"
#include "MainWindow.h"

class MainWindowView {
public:
  MainWindowView(std::shared_ptr<MainWindow> the_main_window);

  MainWindowView(const MainWindowView& another) = delete;

  MainWindowView& operator=(const MainWindow& another) = delete;

  void InvalidRect(const RECT& rect, BOOL erase);

  void InvalidRect(BOOL erase);

  std::pair<bool, LRESULT> OnPaint(UINT msg, WPARAM w_param, LPARAM l_param);

private:
  std::shared_ptr<MainWindow> main_window_;
  signals::connection conn_of_paint_event_;
};

#endif // MANIPULATE_BITMAP_MAIN_WINDOW_VIEW_H_