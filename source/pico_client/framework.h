#ifndef _FRAMEWORK_H_
#define _FRAMEWORK_H_

#ifndef __cplusplus
#error Error! :: Please use the "C++"
#endif

#define UNICODE

#define _UNICODE

#include "targetver.h"
// Исключите редко используемые компоненты из заголовков Windows
#define WIN32_LEAN_AND_MEAN
// Файлы заголовков Windows
#include <windows.h>
#include <windowsx.h>

#include <winbase.h>

#include <shellapi.h>
#pragma comment(lib, "winmm.lib")
// Файлы заголовков среды выполнения C
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <inttypes.h>

#endif // _FRAMEWORK_H_
