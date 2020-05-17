#ifndef MANIPULATE_BITMAP_BASE_DEFINES_H_
#define MANIPULATE_BITMAP_BASE_DEFINES_H_

#ifdef _DEBUG
#define MANIPULATE_BITMAP_DEBUG
#endif

#ifdef MANIPULATE_BITMAP_DEBUG

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <cassert>

#endif


#endif // MANIPULATE_BITMAP_BASE_DEFINES_H_