#ifndef MANIPULATE_BITMAP_UTF8STRING_H_
#define MANIPULATE_BITMAP_UTF8STRING_H_

#include "BaseDefine.h"
#include <string>

typedef std::string Utf8String;

std::wstring Utf8StringToWString(const Utf8String& source);

Utf8String WStringToUtf8String(const std::wstring& source);

#endif // MANIPULATE_BITMAP_UTF8STRING_H_