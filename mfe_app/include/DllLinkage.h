#undef DLL_LINK
#if defined(WIN32) || defined(WIN64)
#  ifdef DLL_EXPORT
#    define DLL_LINK __declspec( dllexport )
#  elif defined DLL_IMPORT
#    define DLL_LINK __declspec( dllimport )
#  else
#    define DLL_LINK
#  endif
#else
#  define DLL_LINK
#endif

//#undef DLL_EXPORT
//#undef DLL_IMPORT

