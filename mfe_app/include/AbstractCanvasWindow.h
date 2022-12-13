// RUN MLSupportCppHParser.exe
// corresponding multi language support files are generated


#ifndef _INC_ABSTRACTCANVASWINDOW
#define _INC_ABSTRACTCANVASWINDOW

#include "AbstractCanvas.h"
#include "AbstractMenusToolbarsDef.h"
#include "AbstractCommands.h"

class AbstractCanvasWindow {
    public:

    virtual ~AbstractCanvasWindow() {};

    virtual void setMenusToolbarsDef(AbstractMenusToolbarsDef* p) = 0;
    virtual void setCommands(AbstractCommands* p) = 0;

    virtual int getMaxCanvasCount() = 0;
    virtual int addCanvas() = 0;
    virtual void closeCanvas(int index) = 0;
    virtual void close() = 0;
    virtual AbstractCanvas* getActiveCanvas() = 0;
    virtual int getActiveCanvasIndex() = 0;
    virtual AbstractCanvas* getCanvas(int index) = 0;
    virtual int getCanvasCount() = 0;
    virtual void processEvents() = 0;
    virtual void* getQWidget() = 0;
    virtual void setStatusBarText(const char* s) = 0;
    virtual void pgbarSetTitle(const char* s) = 0;
    virtual void pgbarSetMaximum(int index) = 0;
    virtual void pgbarSetValue(int index) = 0;
    virtual void setIconDir(const char* dirPath) = 0;
    virtual void setBinDir(const char* dirPath) = 0;
    virtual void setIconFileName(const char* fileName) = 0;
    virtual void setTitle(const char* title) = 0;

    // Get height and widht of the screen (desktop)
    virtual int getDesktopHeight() = 0;
    virtual int getDesktopWidth() = 0;

    // Functions to manipulate the canvas window
    virtual void resizeCanvasWindow(int h, int w) = 0;
    virtual void moveCanvasWindow(int h, int w) = 0;

};

#endif /* _INC_ABSTRACTCANVASWINDOW */
