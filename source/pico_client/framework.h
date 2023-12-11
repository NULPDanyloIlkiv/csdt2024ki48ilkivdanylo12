#ifndef _FRAMEWORK_H_
#define _FRAMEWORK_H_

#ifndef __cplusplus
#error Error! :: Please use the "C++"
#endif

#define UNICODE

#define _UNICODE

#include "targetver.h"
// ��������� ����� ������������ ���������� �� ���������� Windows
#define WIN32_LEAN_AND_MEAN
// ����� ���������� Windows
#include <windows.h>
#include <windowsx.h>

#include <winbase.h>

#include <shellapi.h>
#pragma comment(lib, "winmm.lib")
// ����� ���������� ����� ���������� C
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <inttypes.h>

#endif // _FRAMEWORK_H_
