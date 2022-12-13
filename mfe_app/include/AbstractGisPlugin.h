// RUN MLSupportCppHParser.exe
// corresponding multi language support files are generated


#ifndef _INC_ABSTRACTGISPLUGIN
#define _INC_ABSTRACTGISPLUGIN

#include "AbstractCanvasWindow.h"
#include "AbstractGisConnection.h"
#include "AbstractPluginSharedMem.h"

class AbstractGisPlugin {
    private:


    public:

    virtual ~AbstractGisPlugin() {};

    virtual const char* getName() = 0;
    virtual const char* getDescription() = 0;
    virtual void setGuiName(const char* guiString) = 0;
    virtual void init(AbstractCanvasWindow* canvasWindow,
            AbstractGisConnection* gisConnection, AbstractPluginSharedMem* pluginSharedMem) = 0;
    virtual void exec() = 0;
    virtual bool isRunning() = 0;
};

#endif /* _INC_ABSTRACTGISPLUGIN */
