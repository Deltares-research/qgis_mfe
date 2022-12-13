// RUN MLSupportCppHParser.exe
// corresponding multi language support files are generated


#ifndef _INC_MYCANVASWINDOW
#define _INC_MYCANVASWINDOW

#include <QObject>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QToolBar>

#include "qgisinterface.h"
#include "qgsapplication.h"
#include "qgsmapcanvas.h"
#include "qgsstatusbar.h"
#include "qgsvectorlayer.h"
#include "qgsproject.h"
#include "qgisinterface.h"
//#include "GisPluginList.h"
#include "qgisplugin.h"

//#include "AbstractCanvas.h"
#include "AbstractCanvasWindow.h"
#include "AbstractMenusToolbarsDef.h"
#include "AbstractCommands.h"
#include "MyCanvas.h"
#include "PluginActions.h"
#include "PluginAction.h"


#include "F90Linkage.h"
#include "DllLinkage.h"

class QPopupMenu;
class FileDialog;

class MyCanvasWindow : public QObject, public AbstractCanvasWindow
{
    Q_OBJECT

public:
    MyCanvasWindow(QgisInterface*, MyCanvas *, QToolBar *, QToolBar *, QToolBar *, QToolBar *);
    ~MyCanvasWindow();

    void setMenusToolbarsDef(AbstractMenusToolbarsDef* p);
    void setCommands(AbstractCommands* p);

    int getMaxCanvasCount();
    int addCanvas();
    void closeCanvas(int index);
    void close();
    AbstractCanvas* getActiveCanvas();
    int getActiveCanvasIndex();
    AbstractCanvas* getCanvas(int index);
    int getCanvasCount();
    PluginActions * getPluginActions();
    void processEvents();
    void* getQWidget();
    void setStatusBarText(const char* s);
    void pgbarSetTitle(const char* s);
    void pgbarSetMaximum(int index);
    void pgbarSetValue(int index);
    void setIconDir(const char* dirPath);
    void setBinDir(const char* dirPath);
    void setIconFileName(const char* fileName);
    void setTitle(const char* title);

    // Get height and widht of the screen (desktop)
    int getDesktopHeight();
    int getDesktopWidth();

    // Functions to manipulate the canvas window
    void resizeCanvasWindow(int h, int w);
    void moveCanvasWindow(int h, int w);

public slots:
    void read_menu_item( int &, int &, QMenu *);

private:
    AbstractMenusToolbarsDef * def;
    AbstractCommands * commands;
    char * iconDir;
    char * binDir;
    char * captionsWithMnemonics[1000];
    QToolBar * _toolbar;
    QToolBar * _toolbar2;
    QToolBar * _menuToolBar;
    QToolBar * _iconToolBar;
    QMenuBar * _menubar;
    QAction** mAction;
    PluginActions* pluginActions;

    QgisInterface* mQGisIface;
    AbstractCanvas * mMyCanvas;

    void updateCaptionsWithMnemonics(int &);
    char findFreeMnemonic(bool * , const char *);
    char * insertMnemonicAmpersand(const char *, char);
    void read_toolbar( int & );
};

#endif /* _INC_MYCANVASWINDOW */
