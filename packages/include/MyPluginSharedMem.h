// RUN MLSupportCppHParser.exe
// corresponding multi language support files are generated


#ifndef _INCLUDE_ABSTRACTPLUGINSHAREDMEM
#define _INCLUDE_ABSTRACTPLUGINSHAREDMEM

#include "DllLinkage.h"
#include "F90Linkage.h"
#include "AbstractFtnPointer.h"

class MyPluginSharedMem {
    private:

    public:

    MyPluginSharedMem(){};
    ~MyPluginSharedMem(){};

    void putMemPointer(AbstractFtnPointer* pntr){};
    AbstractFtnPointer* getMemPointer(){};
};

#endif // _INCLUDE_ABSTRACTPLUGINSHAREDMEM
