// RUN MLSupportCppHParser.exe
// corresponding multi language support files are generated

#ifndef _INC_GISPLUGINLIST
#define _INC_GISPLUGINLIST

#include "DllLinkage.h"
#include "AbstractGisPlugin.h"

class GisPluginList {

private:
    int size;
    int capacity;
    void** ptrs;
    bool okDllLoaded;
    bool okFunctionLoaded;

public:
    DLL_LINK GisPluginList();
    DLL_LINK ~GisPluginList();
    DLL_LINK bool isDllLoaded();
    DLL_LINK bool isFunctionLoaded();
    DLL_LINK void loadDll(const char* dirName, const char* dllName);
    DLL_LINK void add(const char* filePath, const char* apiSymbolName);
    DLL_LINK int getSize();
    DLL_LINK AbstractGisPlugin* getGisPlugin(int index);
    DLL_LINK void loadFtnDll(const char* dllName);

private:
    void ensureCapacity(int newSize);


};

#undef DLL_LINK

#endif  /* _INC_GISPLUGINLIST */
