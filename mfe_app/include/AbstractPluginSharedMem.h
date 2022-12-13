// RUN MLSupportCppHParser.exe
// corresponding multi language support files are generated


#ifndef _INCLUDE_ABSTRACTPLUGINSHAREDMEM
#define _INCLUDE_ABSTRACTPLUGINSHAREDMEM

#include "DllLinkage.h"
#include "F90Linkage.h"
#include "AbstractFtnPointer.h"

class AbstractPluginSharedMem {
    private:

    public:

    virtual ~AbstractPluginSharedMem() {};

    virtual void putMemPointer(AbstractFtnPointer* pntr) = 0;
    virtual AbstractFtnPointer* getMemPointer() = 0;
};

#endif // _INCLUDE_ABSTRACTPLUGINSHAREDMEM
