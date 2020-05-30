#include "ShadowWindow.h"
#include <utility>
#include "Utf8String.h"

using std::pair;
using std::function;
using std::wstring;
using std::make_pair;
using std::unique_ptr;
using signals::connection;

namespace
{
  LRESULT CALLBACK ShadowWindowProc(HWND window_handle, UINT msg, WPARAM l_param, LPARAM w_param) {
    switch (msg) {
    case WM_CREATE:
    {
      CREATESTRUCT* p_create_struct = reinterpret_cast<CREATESTRUCT*>(l_param);
      ShadowWindow* p_created_wnd = reinterpret_cast<ShadowWindow*>(p_create_struct->lpCreateParams);
      SetWindowLongPtrW(window_handle, GWLP_USERDATA, (LONG_PTR)p_created_wnd);
      return 0;
    }
    case WM_DESTROY:
    {
      PostQuitMessage(0);
      return 0;
    }
    }
    ShadowWindow* theWindow = reinterpret_cast<ShadowWindow*>(GetWindowLongPtrW(window_handle, GWLP_USERDATA));
    if (theWindow) {
      pair<bool, LRESULT> message_handle_result = theWindow->Trigger(msg, w_param, l_param);
      if (message_handle_result.first) {
        return message_handle_result.second;
      }
    }
    return DefWindowProcW(window_handle, msg, w_param, l_param);
  }

  unique_ptr<WCHAR[]> WStringToStringBuffer(const wstring& the_string) {
    size_t buffer_size = the_string.size() + 1;
    unique_ptr<WCHAR[]> buffer(new WCHAR[buffer_size]);
    memset(buffer.get(), 0, sizeof(WCHAR) * buffer_size);
    wcscpy_s(buffer.get(), buffer_size, the_string.c_str());
    return buffer;
  }

  WNDCLASSEXW ShadowWindowClass(HINSTANCE instance)
  {
    WNDCLASSEXW the_window_class;
    the_window_class.cbSize = sizeof(the_window_class);
    the_window_class.style = 0;
    the_window_class.lpfnWndProc = ShadowWindowProc;
    the_window_class.cbClsExtra = 0;
    the_window_class.cbWndExtra = 0;
    the_window_class.hInstance = instance;
    the_window_class.hIcon = nullptr;
    the_window_class.hCursor = LoadCursorW(nullptr, MAKEINTRESOURCEW(32512));
    the_window_class.hbrBackground = nullptr;
    the_window_class.lpszMenuName = nullptr;
    the_window_class.hIconSm = nullptr;
    return the_window_class;
  }

  ATOM RegisterShadowWindowClass(HINSTANCE instance) {
    static bool is_shadow_window_class_registered = false;
    static ATOM register_result;
    if (is_shadow_window_class_registered) {
      return register_result;
    }
    WNDCLASSEXW shadow_window_class = ShadowWindowClass(instance);
    unique_ptr<WCHAR[]> buffer = WStringToStringBuffer(Utf8StringToWString(kShadowWindowClass));
    shadow_window_class.lpszClassName = buffer.get();
    register_result = RegisterClassExW(&shadow_window_class);
    assert(register_result);
    is_shadow_window_class_registered = register_result != 0;
    return register_result;
  }
}

ShadowWindow::ShadowWindow(const Utf8String& window_name, HINSTANCE module_handle)
  : window_handle_(NULL) {
  RegisterShadowWindowClass(module_handle);
  unique_ptr<WCHAR[]> buffer_of_class_name = WStringToStringBuffer(Utf8StringToWString(kShadowWindowClass));
  unique_ptr<WCHAR[]> buffer_of_window_name = WStringToStringBuffer(Utf8StringToWString(window_name));
}

ShadowWindow::~ShadowWindow() {
  if (IsWindow(window_handle_)) {
    DestroyWindow(window_handle_);
  }
}

pair<bool, LRESULT> ShadowWindow::Trigger(UINT msg, WPARAM w_param, LPARAM l_param) {
  return make_pair(false, 0);
}

connection ShadowWindow::Connect(UINT msg, function<pair<bool, LRESULT>(UINT, WPARAM, LPARAM)> handle) {
  return connection();
}
