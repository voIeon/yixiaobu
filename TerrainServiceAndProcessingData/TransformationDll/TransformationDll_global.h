#pragma once

#ifdef TRANSFORMATION_EXPORTS
#define TRANSFORMATION_DECL __declspec(dllexport)
#else
#define TRANSFORMATION_DECL __declspec(dllimport)
#endif