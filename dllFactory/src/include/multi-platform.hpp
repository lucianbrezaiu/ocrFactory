#if defined(_MSC_VER)
    //  Microsoft 
#define EXPORT_C extern "C" __declspec(dllexport)
#define EXPORT_CPP __declspec(dllexport)
#define IMPORT __declspec(dllimport)
#elif defined(__GNUC__)
    //  GCC
#define EXPORT_C extern "C" __attribute__((visibility("default")))
#define EXPORT_CPP __attribute__((visibility("default")))
#define IMPORT
#define __stdcall
#else
    //  do nothing and hope for the best?
#define EXPORT_C
#define EXPORT_CPP
#define IMPORT
#define __stdcall
#pragma warning Unknown dynamic link import/export semantics.
#endif