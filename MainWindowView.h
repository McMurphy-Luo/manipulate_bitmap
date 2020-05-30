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

  void ReDraw();

  std::pair<bool, LRESULT> OnPaint(UINT msg, WPARAM w_param, LPARAM l_param);

private:
  std::shared_ptr<MainWindow> main_window_;
  signals::connection conn_of_paint_event_;
  HDC main_window_device_context_;
};

#endif // MANIPULATE_BITMAP_MAIN_WINDOW_VIEW_H_