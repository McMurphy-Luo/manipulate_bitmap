#include "MainWindowView.h"
#include <memory>
#include <sstream>
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
using Gdiplus::Graphics;
using Gdiplus::Status;
using Gdiplus::Pen;
using Gdiplus::Rect;
using Gdiplus::Color;

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
  , conn_of_paint_event_(the_main_window->Connect(
    WM_PAINT,
    bind(mem_fn(&MainWindowView::OnPaint), this, _1, _2, _3)
  ))
  , main_window_device_context_(NULL) {
  

}

void MainWindowView::ReDraw() {
  if (!main_window_) {
    return;
  }
  RECT main_window_rect = main_window_->ClientRectangle();
  HDC window_dc = GetDC(main_window_->WindowHandle());
  HDC paint_dc = CreateCompatibleDC(window_dc);
  OutputDeviceCapabilities(window_dc);
  OutputDeviceCapabilities(paint_dc);
  basic_stringstream<TCHAR> debug_stream;

  BITMAPINFO bitmap_info;
  bitmap_info.bmiHeader.biSize = sizeof(bitmap_info);
  bitmap_info.bmiHeader.biWidth = main_window_rect.right - main_window_rect.left;
  bitmap_info.bmiHeader.biHeight = main_window_rect.top - main_window_rect.bottom;
  bitmap_info.bmiHeader.biPlanes = 1;
  bitmap_info.bmiHeader.biBitCount = 32;
  bitmap_info.bmiHeader.biCompression = BI_RGB;
  bitmap_info.bmiHeader.biSizeImage = 0;
  bitmap_info.bmiHeader.biXPelsPerMeter = 0;
  bitmap_info.bmiHeader.biYPelsPerMeter = 0;
  bitmap_info.bmiHeader.biClrUsed = 0;
  bitmap_info.bmiHeader.biClrImportant = 0;

  DWORD* pointer_to_pixels = NULL;
  HBITMAP bitmap = CreateDIBSection(paint_dc, &bitmap_info, DIB_RGB_COLORS, (void**)&pointer_to_pixels, NULL, 0);
  HGDIOBJ old_bitmap = SelectObject(paint_dc, bitmap);



  int result;

  int x = 0;
  int y = 0;

  while (x < 40) {
    while (y < 40) {
      *(pointer_to_pixels + x * bitmap_info.bmiHeader.biWidth + y) = 0x0F120000;
      ++y;
    }
    ++x;
    y = 0;
  }

  
  /*
  HPEN pen_of_solid_color = CreatePen(PS_SOLID, 20, RGB(23, 23, 23));
  HBRUSH solid_color_brush = CreateSolidBrush(RGB(93, 99, 6));
  HGDIOBJ old_pen = SelectObject(paint_dc, pen_of_solid_color);
  POINT zero_p;
  zero_p.x = 0;
  zero_p.y = 0;
  MoveToEx(paint_dc, 0, 0, &zero_p);
  LineTo(paint_dc, main_window_rect.right - main_window_rect.left, 0);
  int result = FillRect(paint_dc, &main_window_rect, solid_color_brush);
  if (result == 0) {
    debug_stream << TEXT("failed to update FillRect GetLastError() is ") << GetLastError() << endl;
    assert(false);
  }
  SelectObject(paint_dc, old_pen);
  DeleteObject(pen_of_solid_color);
  DeleteObject(solid_color_brush);
  */

  
  
  POINT location;
  location.x = main_window_rect.left;
  location.y = main_window_rect.top;
  SIZE client_rect_size;
  client_rect_size.cx = main_window_rect.right - main_window_rect.left;
  client_rect_size.cy = main_window_rect.bottom - main_window_rect.top;
  BLENDFUNCTION blend_function;

  blend_function.BlendOp = AC_SRC_OVER;
  blend_function.BlendFlags = 0;
  blend_function.SourceConstantAlpha = 255;
  blend_function.AlphaFormat = AC_SRC_ALPHA;
  POINT zero;
  zero.x = 0;
  zero.y = 0;
  result = UpdateLayeredWindow(
    main_window_->WindowHandle(),
    window_dc,
    &location,
    &client_rect_size,
    paint_dc,
    &zero, //&location,
    0,
    &blend_function, //&blend_function,
    ULW_ALPHA
  );
  if (result == 0) {
    debug_stream << TEXT("failed to update main window GetLastError() is ") << GetLastError() << endl;
    assert(false);
  }
  OutputDebugString(debug_stream.str().c_str());
  
  SelectObject(paint_dc, old_bitmap);
  DeleteObject(bitmap);
  DeleteDC(paint_dc);
  ReleaseDC(main_window_->WindowHandle(), window_dc);
}

pair<bool, LRESULT> MainWindowView::OnPaint(UINT msg, WPARAM w_param, LPARAM l_param) {
  PAINTSTRUCT paint_structure;
  HDC paint_dc = BeginPaint(main_window_->WindowHandle(), &paint_structure);

  

  basic_stringstream<TCHAR> debug_stream;
  debug_stream << TEXT("paint_structure.fErase ") << paint_structure.fErase
    << TEXT(" paint_structure.rcPaint ") << paint_structure.rcPaint.left
    << TEXT(" ") << paint_structure.rcPaint.top
    << TEXT(" ") << paint_structure.rcPaint.right
    << TEXT(" ") << paint_structure.rcPaint.bottom
    << endl;

  /*
  {
    Graphics graphis(paint_dc);
    Status gdiplus_function_status;

    Color bg_color(255, 255, 255, 255);
    gdiplus_function_status = graphis.Clear(bg_color);
    if (gdiplus_function_status != Status::Ok) {
      debug_stream << TEXT("failed to clear the graphics status is ") << gdiplus_function_status << endl;
      assert(false);
    }

    Pen solid_color_pen(Color(0x23, 0x23, 0x23), 20);
    RECT main_window_client_rect = main_window_->ClientRectangle();
    Rect client_rect(
      main_window_client_rect.left,
      main_window_client_rect.top,
      main_window_client_rect.right - main_window_client_rect.left - 1,
      main_window_client_rect.bottom - main_window_client_rect.top - 1
    );
    gdiplus_function_status = graphis.DrawRectangle(&solid_color_pen, client_rect);
    if (gdiplus_function_status != Status::Ok) {
      debug_stream << TEXT("failed to draw rectangle status is ") << gdiplus_function_status << endl;
      assert(false);
    }
  }
  */

  OutputDebugStringW(debug_stream.str().c_str());
  EndPaint(main_window_->WindowHandle(), &paint_structure);
  return make_pair(true, 0);
}