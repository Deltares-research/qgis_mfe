#ifndef qgis_mfe_H
#define qgis_mfe_H

#include "QObject"
#include <iostream>

#include <QAction>
#include <QComboBox>
#include <QDesktopWidget>
#include <QDir>
#include <QIcon>
#include <QLayout> 
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QProcessEnvironment>
#include <QSizePolicy> 
#include <QString> 
#include <QSpacerItem> 
#include <QToolBar>
#include <QtMath>
#include <QFile>

#include <qgsapplication.h>
#include <qgslayertree.h>
#include <qgisplugin.h>
#include <qgisinterface.h>
#include <qgscoordinatereferencesystem.h>
#include <qgsmessagelog.h>

#include <direct.h> // for getcwd
#include <stdlib.h> // for MAX_PATH

#include "GisPluginList.h"
#include "MyCanvasWindow.h"
#include "MyGisconnection.h"
#include "MyPluginSharedMem.h"
#include "QApp.h"

class QAction;
class qgis_mfe
    : public QObject, public QgisPlugin

{
        Q_OBJECT

    public:
        static const QString s_ident, s_name, s_description, s_category, s_version, s_icon, s_plugin_version;
        static const QgisPlugin::PluginType s_plugin_type;
        
        qgis_mfe(QgisInterface* iface);
        ~qgis_mfe();
        void initGui();
        void unload();
        QIcon get_icon_file(QDir, QString);

        //  qgis_mfe functions
        long loadPluginList();
        void initPlugin(int);
        char* get_dll_name(char *, GisPluginList *, int);
        char* stripSpaces(char *);

    private slots:
        void activatePlugin(int);
        void runAction();
        void add_memory_layer(int);

    private:
    //! Pointer to the QGIS interface object
        QgisInterface * mQGisIface;
        QgsMapCanvas  * mCanvas; // Pointer to the QGIS canvas

        QAction  * mainAction;
        QAction  * menuAction;
        QAction  * iconAction;

        QMenu * fileMenu;

        QToolBar * tbar;  // complete toolbar for plugin (ie menu and icons)
        QToolBar * tbar2;  
        QToolBar * iconBar;
        QToolBar * menuBar;
        QComboBox* cb;

        QMenuBar * menu_bar;

        //  qgis_mfe local variables
        GisPluginList * pluginList;
        AbstractCanvasWindow ** CanvasWindow;
        AbstractGisPlugin    *  activePlugin;
        AbstractCanvasWindow *  activeCanvasWindow;
        MyCanvas * mMyCanvas;

        char * msgtxt;
        char * actionIcon;
        char * m_plugins_home;
        QString pluginsHomeBin;
        QString csv_file;
        int nr_dlls;
        char ** prg_names;
        bool csv_file_loaded;
};
#endif