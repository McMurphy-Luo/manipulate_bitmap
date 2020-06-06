#include "MainWindowView.h"
#include <memory>
#include <sstream>
#include "Windowsx.h"
#include "gdiplus.h"

using std::pair;
using std::shared_ptr;
using std::make_pair;
using std::mem_fn;
using std::basic_stringstream;
using std::stringstream;
using std::bind;
using std::placeholders::_1;
using std::placeholders::_2;
using std::placeholders::_3;
using std::endl;
using Gdiplus::REAL;
using Gdiplus::Graphics;
using Gdiplus::GraphicsPath;
using Gdiplus::FillMode;
using Gdiplus::Status;
using Gdiplus::SolidBrush;
using Gdiplus::SmoothingMode;
using Gdiplus::Pen;
using Gdiplus::Point;
using Gdiplus::Rect;
using Gdiplus::Color;
using Gdiplus::Bitmap;

namespace
{
  Utf8String GetDeviceCapabilities(HDC dc) {
    stringstream output_stream;
    int result = GetDeviceCaps(dc, DRIVERVERSION);
    output_stream << u8"DRIVERVERSION is " << result << endl;
    result = GetDeviceCaps(dc, COLORRES);
    output_stream << u8"COLORRES is " << result << endl;
    result = GetDeviceCaps(dc, TECHNOLOGY);
    output_stream << u8"TECHNOLOGY is " << result << endl;
    result = GetDeviceCaps(dc, HORZSIZE);
    output_stream << u8"HORZSIZE is " << result << endl;
    result = GetDeviceCaps(dc, VERTSIZE);
    output_stream << u8"VERTSIZE is " << result << endl;
    result = GetDeviceCaps(dc, LOGPIXELSX);
    output_stream << u8"LOGPIXELSX is " << result << endl;
    result = GetDeviceCaps(dc, LOGPIXELSY);
    output_stream << u8"LOGPIXELSY is " << result << endl;
    result = GetDeviceCaps(dc, BITSPIXEL);
    output_stream << u8"BITSPIXEL is " << result << endl;
    result = GetDeviceCaps(dc, PLANES);
    output_stream << u8"PLANES is " << result << endl;
    result = GetDeviceCaps(dc, NUMBRUSHES);
    output_stream << u8"NUMBRUSHES is " << result << endl;
    result = GetDeviceCaps(dc, NUMPENS);
    output_stream << u8"NUMPENS is " << result << endl;
    result = GetDeviceCaps(dc, NUMFONTS);
    output_stream << u8"NUMFONTS is " << result << endl;
    result = GetDeviceCaps(dc, NUMCOLORS);
    output_stream << u8"NUMCOLORS is " << result << endl;
    result = GetDeviceCaps(dc, ASPECTX);
    output_stream << u8"ASPECTX is " << result << endl;
    result = GetDeviceCaps(dc, ASPECTY);
    output_stream << u8"ASPECTY is " << result << endl;
    result = GetDeviceCaps(dc, ASPECTXY);
    output_stream << u8"ASPECTXY is " << result << endl;
    result = GetDeviceCaps(dc, SCALINGFACTORX);
    output_stream << u8"SCALINGFACTORX is " << result << endl;
    result = GetDeviceCaps(dc, SCALINGFACTORY);
    output_stream << u8"SCALINGFACTORY is " << result << endl;
    result = GetDeviceCaps(dc, BLTALIGNMENT);
    output_stream << u8"BLTALIGNMENT is " << result << endl;
    result = GetDeviceCaps(dc, SHADEBLENDCAPS);
    output_stream << u8"SHADEBLENDCAPS is " << result << endl;
    result = GetDeviceCaps(dc, RASTERCAPS);
    output_stream << u8"RASTERCAPS is " << result << endl;
    result = GetDeviceCaps(dc, CURVECAPS);
    output_stream << u8"CURVECAPS is " << result << endl;
    result = GetDeviceCaps(dc, LINECAPS);
    output_stream << u8"LINECAPS is " << result << endl;
    result = GetDeviceCaps(dc, POLYGONALCAPS);
    output_stream << u8"POLYGONALCAPS is " << result << endl;
    result = GetDeviceCaps(dc, TEXTCAPS);
    output_stream << u8"TEXTCAPS is " << result << endl;
    result = GetDeviceCaps(dc, COLORMGMTCAPS);
    output_stream << u8"COLORMGMTCAPS is " << result << endl;
    return output_stream.str();
  }

  void OutputDeviceCapabilities(HDC dc) {
    Utf8String device_capabilities = GetDeviceCapabilities(dc);
    stringstream output_stream;
    output_stream << "Begin Output Device Capabilities" << endl;
    output_stream << device_capabilities;
    output_stream << "End Output Device Capabilities" << endl;
    OutputDebugStringW(Utf8StringToWString(output_stream.str()).c_str());
  }
}

MainWindowView::MainWindowView(shared_ptr<MainWindow> the_main_window)
  : main_window_(the_main_window)
  , round_rectangle_param_(false, RoundRectanglePathParam())
  , window_bound_path_(FillMode::FillModeAlternate)
  , conn_of_paint_event_(
    the_main_window->Connect(
      WM_PAINT,
      bind(mem_fn(&MainWindowView::OnPaint), this, _1, _2, _3)
    )
  )
  , conn_of_nc_hit_test_event_(
    the_main_window->Connect(
      WM_NCHITTEST,
      bind(mem_fn(&MainWindowView::OnNcHitTest), this, _1, _2, _3)
    )
  )
{

}

void MainWindowView::InvalidRect(const RECT& rect, BOOL erase) {
  if (!main_window_) {
    assert(false);
    return;
  }
  return main_window_->InvalidRect(rect, erase);
}

void MainWindowView::InvalidRect(BOOL erase) {
  if (!main_window_) {
    assert(false);
    return;
  }
  return main_window_->InvalidRect(erase);
}

pair<bool, LRESULT> MainWindowView::OnPaint(UINT msg, WPARAM w_param, LPARAM l_param) {
  if (!main_window_) {
    return make_pair(false, 0);;
  }
  PAINTSTRUCT paint_structure;
  HDC window_dc = BeginPaint(main_window_->WindowHandle(), &paint_structure);
  basic_stringstream<TCHAR> debug_stream;
  debug_stream << TEXT("paint_structure.fErase ") << paint_structure.fErase
    << TEXT(" paint_structure.rcPaint ") << paint_structure.rcPaint.left
    << TEXT(" ") << paint_structure.rcPaint.top
    << TEXT(" ") << paint_structure.rcPaint.right
    << TEXT(" ") << paint_structure.rcPaint.bottom
    << endl;
  HDC paint_dc = CreateCompatibleDC(window_dc);
  OutputDeviceCapabilities(window_dc);
  OutputDeviceCapabilities(paint_dc);
  RECT window_rect = main_window_->WindowRectangle();
  LONG window_width = window_rect.right - window_rect.left;
  LONG window_height = window_rect.bottom - window_rect.top;
  LONG drawable_area_width = window_width - 1;
  LONG drawable_area_height = window_height - 1;
  BITMAPINFO bitmap_info;
  bitmap_info.bmiHeader.biSize = sizeof(bitmap_info);
  bitmap_info.bmiHeader.biWidth = window_width;
  bitmap_info.bmiHeader.biHeight = -window_height;
  bitmap_info.bmiHeader.biPlanes = 1;
  bitmap_info.bmiHeader.biBitCount = 32;
  bitmap_info.bmiHeader.biCompression = BI_RGB;
  bitmap_info.bmiHeader.biSizeImage = 0;
  bitmap_info.bmiHeader.biXPelsPerMeter = 0;
  bitmap_info.bmiHeader.biYPelsPerMeter = 0;
  bitmap_info.bmiHeader.biClrUsed = 0;
  bitmap_info.bmiHeader.biClrImportant = 0;
  BYTE* pointer_to_pixels = NULL;
  HBITMAP bitmap = CreateDIBSection(paint_dc, &bitmap_info, DIB_RGB_COLORS, (void**)&pointer_to_pixels, NULL, 0);
  {
    Bitmap bitmap(
      &bitmap_info,
      pointer_to_pixels
    );
    Graphics graphics(&bitmap);
    graphics.SetSmoothingMode(SmoothingMode::SmoothingModeAntiAlias);
    Status gdiplus_function_status = graphics.GetLastStatus();
    if (gdiplus_function_status != Status::Ok) {
      debug_stream << TEXT("failed to create the graphics object status is ") << gdiplus_function_status << endl;
      assert(false);
    }
    InitializeBoundPath(drawable_area_width, drawable_area_height, 20);
    Pen solid_color_pen(Color(0x77, 0x23, 0x23, 0x23), 1);
    gdiplus_function_status = solid_color_pen.GetLastStatus();
    if (gdiplus_function_status != Status::Ok) {
      debug_stream << TEXT("failed to create the solid_color_pen object status is ") << gdiplus_function_status << endl;
      assert(false);
    }
    gdiplus_function_status = graphics.DrawPath(&solid_color_pen, &window_bound_path_);
    if (gdiplus_function_status != Status::Ok) {
      debug_stream << TEXT("failed to DrawPath status is ") << gdiplus_function_status << endl;
      assert(false);
    }
    Color bg_color(Color(0xFF, 0xFF, 0xFF));
    SolidBrush brush(bg_color);
    gdiplus_function_status = brush.GetLastStatus();
    if (gdiplus_function_status != Status::Ok) {
      debug_stream << TEXT("failed to create the brush object status is ") << gdiplus_function_status << endl;
      assert(false);
    }
    gdiplus_function_status = graphics.FillPath(&brush, &window_bound_path_);
    if (gdiplus_function_status != Status::Ok) {
      debug_stream << TEXT("failed to FillPath status is ") << gdiplus_function_status << endl;
      assert(false);
    }
  }
  HGDIOBJ old_bitmap = SelectObject(paint_dc, bitmap);
  BLENDFUNCTION blend_function;
  blend_function.BlendOp = AC_SRC_OVER;
  blend_function.BlendFlags = 0;
  blend_function.SourceConstantAlpha = 255;
  blend_function.AlphaFormat = AC_SRC_ALPHA;
  SIZE size_of_window;
  size_of_window.cx = window_width;
  size_of_window.cy = window_height;
  POINT position_of_window;
  position_of_window.x = window_rect.left;
  position_of_window.y = window_rect.top;
  POINT zero;
  zero.x = 0;
  zero.y = 0;
  UPDATELAYEREDWINDOWINFO layered_window_update_param;
  layered_window_update_param.cbSize = sizeof(layered_window_update_param);
  layered_window_update_param.hdcDst = window_dc;
  layered_window_update_param.pptDst = NULL;
  layered_window_update_param.psize = &size_of_window;
  layered_window_update_param.hdcSrc = paint_dc;
  layered_window_update_param.pptSrc = &zero;
  layered_window_update_param.crKey = RGB(0xF0, 0xF0, 0xF0);
  layered_window_update_param.pblend = &blend_function;
  layered_window_update_param.dwFlags = ULW_ALPHA | ULW_EX_NORESIZE;
  layered_window_update_param.prcDirty = NULL;
  int result = UpdateLayeredWindowIndirect(main_window_->WindowHandle(), &layered_window_update_param);
  if (result == 0) {
    debug_stream << TEXT("failed to update main window GetLastError() is ") << GetLastError() << endl;
    assert(false);
  }
  SelectObject(paint_dc, old_bitmap);
  DeleteObject(bitmap);
  DeleteDC(paint_dc);
  OutputDebugStringW(debug_stream.str().c_str());
  EndPaint(main_window_->WindowHandle(), &paint_structure);
  return make_pair(true, 0);
}

pair<bool, LRESULT> MainWindowView::OnNcHitTest(UINT msg, WPARAM w_param, LPARAM l_param) {
  if (!main_window_) {
    return make_pair(false, 0);
  }
  RECT window_rect = main_window_->WindowRectangle();
  POINT pt;
  pt.x = GET_X_LPARAM(l_param);
  pt.y = GET_Y_LPARAM(l_param);
  if (PtInRect(&window_rect, pt)) {
    return make_pair(true, HTCAPTION);
  }
  return make_pair(false, 0);
}

void MainWindowView::InitializeBoundPath(LONG width, LONG height, REAL radius) {
  if (
    round_rectangle_param_.first
    && 
    width == round_rectangle_param_.second.width
    &&
    height == round_rectangle_param_.second.height
    &&
    radius == round_rectangle_param_.second.radius
  ) {
    return;
  }
  Status status = window_bound_path_.Reset();
  basic_stringstream<TCHAR> debug_stream;
  if (status != Status::Ok) {
    debug_stream << TEXT("failed to Reset() the window_bound_path_ status is ") << status << endl;
    assert(false);
  }
  status = window_bound_path_.StartFigure();
  if (status != Status::Ok) {
    debug_stream << TEXT("failed to StartFigure() status is ") << status << endl;
    assert(false);
  }
  status = window_bound_path_.AddLine(
    static_cast<INT>(0),
    static_cast<INT>(height - radius),
    static_cast<INT>(0),
    static_cast<INT>(radius)
  );
  if (status != Status::Ok) {
    debug_stream << TEXT("failed to AddLine() status is ") << status << endl;
    assert(false);
  }
  status = window_bound_path_.AddArc(
    static_cast<INT>(0),
    static_cast<INT>(0),
    static_cast<INT>(radius * 2),
    static_cast<INT>(radius * 2),
    180,
    90
  );
  if (status != Status::Ok) {
    debug_stream << TEXT("failed to AddArc() status is ") << status << endl;
    assert(false);
  }
  status = window_bound_path_.AddLine(
    static_cast<INT>(radius),
    static_cast<INT>(0),
    static_cast<INT>(width - radius),
    static_cast<INT>(0)
  );
  if (status != Status::Ok) {
    debug_stream << TEXT("failed to AddLine() status is ") << status << endl;
    assert(false);
  }
  status = window_bound_path_.AddArc(
    static_cast<INT>(width - radius * 2),
    static_cast<INT>(0),
    static_cast<INT>(radius * 2),
    static_cast<INT>(radius * 2),
    270,
    90
  );
  if (status != Status::Ok) {
    debug_stream << TEXT("failed to AddArc() status is ") << status << endl;
    assert(false);
  }
  status = window_bound_path_.AddLine(
    static_cast<INT>(width),
    static_cast<INT>(radius),
    static_cast<INT>(width),
    static_cast<INT>(height - radius)
  );
  if (status != Status::Ok) {
    debug_stream << TEXT("failed to AddLine() status is ") << status << endl;
    assert(false);
  }
  status = window_bound_path_.AddArc(
    static_cast<INT>(width - 2 * radius),
    static_cast<INT>(height - 2 * radius),
    static_cast<INT>(radius * 2),
    static_cast<INT>(radius * 2),
    0,
    90
  );
  if (status != Status::Ok) {
    debug_stream << TEXT("failed to AddArc() status is ") << status << endl;
    assert(false);
  }
  status = window_bound_path_.AddLine(
    static_cast<INT>(width - radius),
    static_cast<INT>(height),
    static_cast<INT>(radius),
    static_cast<INT>(height)
  );
  if (status != Status::Ok) {
    debug_stream << TEXT("failed to AddLine() status is ") << status << endl;
    assert(false);
  }
  status = window_bound_path_.AddArc(
    static_cast<INT>(0),
    static_cast<INT>(height - 2 * radius),
    static_cast<INT>(radius * 2),
    static_cast<INT>(radius * 2),
    90,
    90
  );
  if (status != Status::Ok) {
    debug_stream << TEXT("failed to AddArc() status is ") << status << endl;
    assert(false);
  }
  status = window_bound_path_.CloseFigure();
  if (status != Status::Ok) {
    debug_stream << TEXT("failed to CloseFigure() status is ") << status << endl;
    assert(false);
  }
  round_rectangle_param_.first = true;
  round_rectangle_param_.second.width = width;
  round_rectangle_param_.second.height = height;
  round_rectangle_param_.second.radius = radius;
}