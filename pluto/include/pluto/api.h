#pragma once
#ifdef PLUTO_DLL_EXPORT
#define PLUTO_API __declspec(dllexport)
#else
#define PLUTO_API __declspec(dllimport)
#endif
