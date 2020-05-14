#pragma once


#ifndef _DEBUG
#include "VMProtectSDK.h"

#define VM_BEGIN(str) VMProtectBegin(str);
#define VM_END VMProtectEnd();

#else

#define VM_BEGIN(str);
#define VM_END (void*)0;

#endif