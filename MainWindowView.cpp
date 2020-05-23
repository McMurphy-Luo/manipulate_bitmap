#include "MainWindowView.h"

using std::pair;
using std::make_pair;

pair<bool, LRESULT> MainWindowView::OnPaint(UINT msg, WPARAM w_param, LPARAM l_param) {
  return make_pair(true, 0);
}