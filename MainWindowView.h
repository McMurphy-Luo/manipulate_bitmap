#ifndef MANIPULATE_BITMAP_MAIN_WINDOW_VIEW_H_
#define MANIPULATE_BITMAP_MAIN_WINDOW_VIEW_H_

#include <memory>
#include <utility>
#include "Windows.h"
#include "gdiplus.h"
#include "MainWindow.h"

struct RoundRectanglePathParam {
  LONG width;
  LONG height;
  Gdiplus::REAL radius;
};

class MainWindowView {
public:
  MainWindowView(std::shared_ptr<MainWindow> the_main_window);

  MainWindowView(const MainWindowView& another) = delete;

  MainWindowView& operator=(const MainWindow& another) = delete;

  void InvalidRect(const RECT& rect, BOOL erase);

  void InvalidRect(BOOL erase);

  std::pair<bool, LRESULT> OnPaint(UINT msg, WPARAM w_param, LPARAM l_param);

  std::pair<bool, LRESULT> OnNcHitTest(UINT msg, WPARAM w_param, LPARAM l_param);

private:
  void InitializeBoundPath(LONG width, LONG height, Gdiplus::REAL radius);

private:
  std::shared_ptr<MainWindow> main_window_;
  std::pair<bool, RoundRectanglePathParam> round_rectangle_param_;
  Gdiplus::GraphicsPath window_bound_path_;
  signals::connection conn_of_paint_event_;
  signals::connection conn_of_nc_hit_test_event_;
};

#endif // MANIPULATE_BITMAP_MAIN_WINDOW_VIEW_H_