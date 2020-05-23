#ifndef MANIPULATE_BITMAP_MAIN_WINDOW_VIEW_H_
#define MANIPULATE_BITMAP_MAIN_WINDOW_VIEW_H_

#include "Windows.h"
#include <utility>

class MainWindowView {
public:
  std::pair<bool, LRESULT> OnPaint(UINT msg, WPARAM w_param, LPARAM l_param);
};

#endif // MANIPULATE_BITMAP_MAIN_WINDOW_VIEW_H_