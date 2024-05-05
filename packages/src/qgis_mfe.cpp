#include "qgis_mfe.h"
#include <QAction>

#define GUI_EXPORT __declspec(dllimport)

#include "qgisinterface.h"
//#include "qgsapplication.h"
#include <qgsmessagelog.h>

#if defined(WIN64)
#  include <windows.h>
#  define strdup _strdup 
#endif
    
#define LINE_LENGTH 125
#define MAX_M_ACTION 1

#define PROGRAM "QGIS_MFE"
#define QGIS_MFE_VERSION "0.96.00"
#define COMPANY "Deltares"
#define ARCH "Win64"

/* static */ const QString qgis_mfe::s_ident = QObject::tr("@(#)" COMPANY ", " PROGRAM ", " QGIS_MFE_VERSION ", " ARCH", " __DATE__", " __TIME__);
/* static */ const QString qgis_mfe::s_name = QObject::tr("" COMPANY ", " PROGRAM " Development");
/* static */ const QString qgis_mfe::s_description = QObject::tr("Map Front End plugin for QGIS ("  __DATE__", " __TIME__")");
/* static */ const QString qgis_mfe::s_category = QObject::tr("Plugins");
/* static */ const QString qgis_mfe::s_plugin_version = QObject::tr(QGIS_MFE_VERSION);
/* static */ const QgisPlugin::PluginType qgis_mfe::s_plugin_type = QgisPlugin::UI;
/* static */ const QString* s_plugin_icon;


char * msgtxt = (char *) malloc(101 *sizeof(char *));
char * actionIcon  = (char *) malloc(250 *sizeof(char *));
QString pluginsHomeBin;
QString pluginsIconDir;
QString csv_file;
int nr_dlls;
char ** prg_names;
bool delft3d_enabled;
QIcon * icon_picture;
//
//-----------------------------------------------------------------------------
//
qgis_mfe::qgis_mfe(QgisInterface* iface):
     QgisPlugin(s_name, s_description, s_category, s_plugin_version, s_plugin_type),
     mQGisIface(iface)
{
#include "vsi.xpm"
    mQGisIface = iface;
    delft3d_enabled = false;
    icon_picture = new QIcon(QPixmap(vsi));
}
//
//-----------------------------------------------------------------------------
//
qgis_mfe::~qgis_mfe()
{
    int i;
    i = 5;
    free(m_plugins_home);
}
//
//-----------------------------------------------------------------------------
//
void qgis_mfe::initGui()
{
    std::cout << "HelloWorldPlugin::initGui" << std::endl;

    QgsCoordinateReferenceSystem _crs = QgsProject::instance()->crs();
    QString crs_auth = _crs.authid();
    if (crs_auth == "") crs_auth = "<empty>";
    if (_crs == QgsCoordinateReferenceSystem("EPSG:4326") ||  // wgs84 == espg:4326
        _crs == QgsCoordinateReferenceSystem("EPSG:28991") ||   // Amersfoort old
        _crs == QgsCoordinateReferenceSystem("EPSG:28992"))  // Amersfoort new
    {
        QString msg = QString(tr("The QGIS project CRS is: ")) + crs_auth + "\n" + QString(tr("Please make the Delft3D plugin CRS equal to the QGIS project CRS."));
        QgsMessageLog::logMessage(msg, "QGIS MFE", Qgis::Warning, true);
    }
    else
    {
        QString msg = QString(tr("The QGIS project CRS is: ")) + crs_auth + "\n" + QString(tr("Please adjust the project CRS to the CRS's supported by the Delft3D plugins (i.e. 28992)."));
        QgsMessageLog::logMessage(msg, "QGIS MFE", Qgis::Warning, true);
    }

    int i;
    i = 5;
    //int myHWND;
    long error;
    QString current_dir;

    connect(this->mQGisIface, SIGNAL(projectRead()), this, SLOT(project_read()));
    //
    // QApp needed for QT dialog boxes (e.g. License messages)
    //
    //QWidget * mw = mQGisIface->mainWindow();
    //WId parentHWND = mw->winId();
    //QApp::init((int) parentHWND);
    //QApp::init( NULL );


    //int argc = 0;
    //char ** argv = NULL;
    //QApplication app(argc, argv);


    //
    // Get and set paths before loading the plugins
    //
    QString pluginsHome = QProcessEnvironment::systemEnvironment().value("WL_PLUGINS_HOME", ""); // Gezeik met \\ en /
    if (pluginsHome.size() == 0) 
    {
        QString msgtxt = QString("Delft3D 4 is not yet installed.\nEnvironment variable WL_PLUGINS_HOME is missing.");
        QMessageBox::information(0, tr("QGIS_MFE plugin"), msgtxt);
        pluginsHome = QString("<not set>");
    }
    for (int j = 0; j<pluginsHome.count(); j++)
    {
        if (pluginsHome[j] == '\\') // escape backslash needed
        {
            pluginsHome[j] = '/';
        }
    }
    m_plugins_home = strdup(pluginsHome.toStdString().c_str());
    pluginsHomeBin = pluginsHome + QString("/bin");
    csv_file = pluginsHomeBin + QString("/gis_plugins.csv");

    current_dir = QDir::currentPath();  // get current working directory  (_getcwd(current_dir, _MAX_PATH);)
    QDir::setCurrent(pluginsHomeBin);  // (void)_chdir(pluginsHomeBin);
    //
    // Load the plugins list
    //
    csv_file_loaded = false;
    error = loadPluginList(); // read plugin.csv file, return prognames[] if plugin is loaded
    if (error == 0)
    { 
        csv_file_loaded = true; 
    }
    else
    {
        menu_bar = nullptr;
        menuBar = nullptr;
        iconBar = nullptr;
        tbar2 = nullptr;
        tbar = nullptr;
        cb = nullptr;
        mainAction = nullptr;
        mMyCanvas = nullptr;
        return;
    }

    QDir::setCurrent(current_dir);  // (void)_chdir(current_dir);

    //
    // Set Deltares icon
    //
    QIcon actionIcon = *icon_picture;

    QFile pluginsIcon = pluginsHome + "/icons/deltares.ico1";
    if (pluginsIcon.exists())
    {
        actionIcon = QIcon(pluginsHome + "/icons/deltares.ico");
    }

    mainAction = new QAction(actionIcon, tr("&Activate Delft3D cursor"));
    mainAction->setToolTip("Activate Delft3D cursor on canvas");

    tbar = mQGisIface->addToolBar("Delft3D main toolbar");
    tbar->addAction(mainAction);
    //
    // Set list of plugins into combobox
    //
    cb = new QComboBox();
    for (i = 1; i <= nr_dlls; i++)
    {
        if (prg_names[i] != NULL) {
            cb->addItem(prg_names[i]);
        }
    }
    cb->setToolTip("Select a Delft3D plugin");

    tbar->addWidget(cb);
    if (error != 0) { return; }

    connect(mainAction, SIGNAL(triggered()), this, SLOT(runAction()));
    connect(cb, SIGNAL(activated(int)), this, SLOT(activatePlugin(int)));
    connect(cb, SIGNAL(activated(int)), this, SLOT(add_memory_layer(int)));

//------------------------------------------------------------------------------
    tbar2 = mQGisIface->addToolBar(prg_names[0]);
    tbar2->setToolTip("Toolbar of the selected Delft3D plugin, containing menus and icons");
    tbar2->setWhatsThis("Jan Mooiman");
    tbar2->setVisible(false);
    tbar2->hide();

    QIcon icon_open = QIcon(pluginsHome + QString("/icons/file_open.png"));
    menuAction = new QAction(icon_open, tr("&Open"));
    menuAction->setStatusTip(tr("Dummy menu item"));
    menuAction->setEnabled(true);
    connect(menuAction, SIGNAL(triggered()), this, SLOT(openFile()));

    QIcon icon_dummy = QIcon(pluginsHome + QString("/icons/file_open.png"));
    iconAction = new QAction(icon_dummy, tr("&Open"));
    iconAction->setStatusTip(tr("Dummy icon item"));
    iconAction->setEnabled(true);
    connect(iconAction, SIGNAL(triggered()), this, SLOT(openFile()));

    menu_bar = new QMenuBar();
    fileMenu = menu_bar->addMenu(tr("&File"));
    fileMenu->addAction(menuAction);

    QHBoxLayout * hl_menu = new QHBoxLayout();
    hl_menu->addWidget(menu_bar);
    hl_menu->addStretch(2);
    QWidget * w_menu = new QWidget();
    w_menu->setLayout(hl_menu);

    menuBar = new QToolBar();
    tbar2->addWidget(menuBar);

    tbar2->addSeparator();

    iconBar = new QToolBar;
    tbar2->addWidget(iconBar);

    QWidget* spacer = new QWidget();
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    tbar2->addWidget(spacer);

    //
    // Initialise GisPlugin, each plugin needs a canvas
    //
    CanvasWindow = (AbstractCanvasWindow **)malloc(cb->count() * sizeof(AbstractCanvasWindow *));
    for (i = 0; i < cb->count(); i++)
    {
        CanvasWindow[i] = (AbstractCanvasWindow *)NULL;
    }
    //------------------------------------------------------------------------------
    //
    // create our map tool
    //
    mMyCanvas = new MyCanvas(mQGisIface->mapCanvas());
    activatePlugin(0);  // first plugin in combobox is selected (by default)
    delft3d_enabled = true;
    add_memory_layer(0);

    //
    // Key events
    //
    connect(mMyCanvas, SIGNAL(keyPressEvent(QKeyEvent *)), mMyCanvas, SLOT(MyKeyPressEvent(QKeyEvent *)));
    connect(mMyCanvas, SIGNAL(keyReleased(QKeyEvent *)), mMyCanvas, SLOT(MyKeyReleasedEvent(QKeyEvent *)));
}
//
//-----------------------------------------------------------------------------
//
void qgis_mfe::runAction()
{
    mQGisIface->mapCanvas()->setMapTool(mMyCanvas);
    tbar2->setVisible(true);
}
//
//-----------------------------------------------------------------------------
//
void qgis_mfe::add_memory_layer(int idx)
{
    if (!delft3d_enabled) { return; }  // does not make scratch layer if Delft3D plugins are not loaded

    return;  // do not add a vectorlayer for the moment (2019-04-29)

    QgsLayerTree * treeRoot = QgsProject::instance()->layerTreeRoot();  // root is invisible
    QgsLayerTreeGroup * treeGroup = treeRoot->insertGroup(0, tr("Mesh (unstructured)"));
    QgsVectorLayer * vl_1 = new QgsVectorLayer("Point?crs=epsg:28992", "Points (Mooiman) vl_1", "memory");  // _crs.authid() == EPSG:28992
    QgsVectorLayer * vl_2 = new QgsVectorLayer("LineString?crs=epsg:28992", "Splines (Mooiman) vl_2", "memory");  // _crs.authid() == EPSG:28992
    QgsVectorLayer * vl_3 = new QgsVectorLayer("LineString?crs=epsg:28992", "Polylines (Mooiman) vl_3", "memory");  // _crs.authid() == EPSG:28992
    QgsVectorLayer * vl_4 = new QgsVectorLayer("Polygon?crs=epsg:28992", "Polygons (Mooiman) vl_4", "memory");  // _crs.authid() == EPSG:28992
    QgsVectorDataProvider * dp_vl_1 = vl_1->dataProvider();
    QgsVectorDataProvider * dp_vl_2 = vl_2->dataProvider();
    QgsVectorDataProvider * dp_vl_3 = vl_3->dataProvider();
    QgsVectorDataProvider * dp_vl_4 = vl_4->dataProvider();
    treeGroup->insertLayer(0, vl_1);
    treeGroup->insertLayer(1, vl_2);
    treeGroup->insertLayer(2, vl_3);
    treeGroup->insertLayer(3, vl_4);


    QList< QgsMapLayer * > layers = mQGisIface->mapCanvas()->layers();
    int cnt = layers.count();

    for (int i = 0; i < layers.count(); i++)
    {
        QString jan = layers[i]->name();
        if (jan == QString("Points(Mooiman) vl_1"))
        {
            return;
        }
    }
    
    // add attribute
    QList <QgsField> lMyAttribField;
    // create fields
    lMyAttribField << QgsField("TrackId", QVariant::Int)
         << QgsField("Label", QVariant::String)
         << QgsField("Type", QVariant::String)
         << QgsField("Color", QVariant::String)
         << QgsField("PosX", QVariant::Double)
         << QgsField("PosY", QVariant::Double)
         << QgsField("TimeStamp", QVariant::String);

    dp_vl_1->addAttributes(lMyAttribField);
    dp_vl_2->addAttributes(lMyAttribField);
    dp_vl_2->addAttributes(lMyAttribField);
    dp_vl_3->addAttributes(lMyAttribField);
}

//
//-----------------------------------------------------------------------------
//
void qgis_mfe::initPlugin(int idx)
{
    MyCanvasWindow * W;
    MyGisConnection * G;
    MyGisConnection * S;

    W = new MyCanvasWindow(mQGisIface, mMyCanvas, tbar, tbar2, menuBar, iconBar);
    G = new MyGisConnection();
    S = NULL;

    activePlugin      = (AbstractGisPlugin *) pluginList->getGisPlugin(idx);
    CanvasWindow[idx] = (AbstractCanvasWindow *) W;
    activePlugin->init((AbstractCanvasWindow *) W, (AbstractGisConnection *) G, (AbstractPluginSharedMem *) S);

    PluginActions * pluginActions = W->getPluginActions();
    connect(mMyCanvas, SIGNAL(MouseReleaseEvent(QgsMapMouseEvent *)), pluginActions, SLOT(updataAll()));
}

//-----------------------------------------------------------------------------
//
long qgis_mfe::loadPluginList(void)
{
    // read plugin.csv file
    // output: prg_names[] when dll is valid
    //
    // char * csv_file = strdup("c:/Program Files (x86)/Quantum GIS Wroclaw/plugins/delft3d/GISPlugins.csv");
    int idx;
    char * prg_name;
    char * record = (char *)malloc(LINE_LENGTH + 1);
    char * pchr;
    bool eof_file;
    long error = 1;

    prg_names = NULL;
    FILE * fp = fopen(csv_file.toLatin1(), "r");
    if (fp != NULL)
    {
        idx = 0;  // first line of plugin.csv file, this line contains the overall name
        pluginList = new GisPluginList();
        record = fgets(record, LINE_LENGTH, fp);
        if (record == NULL) { return error; }

        // Look for character \n and replace it by \0
        pchr = strchr(record, '\n');
        if (pchr != NULL) { *pchr = '\0'; }

        prg_name = get_dll_name(record, pluginList, idx);
        if (prg_name != NULL)
        {
            prg_names = (char **)malloc((idx + 1) * sizeof(char *));
            prg_names[idx] = strdup(prg_name);
        }

        eof_file = false;

        while (!eof_file)
        {
            record = fgets(record, LINE_LENGTH, fp);
            if (record == NULL)
            {
                eof_file = true;
            }
            else
            {
                // Look for character \n and replace it by \0
                pchr = strchr(record, '\n');
                if (pchr != NULL) { *pchr = '\0'; }
                idx++; /// next line of plugin.csv file, this line contains dll and entry function
                prg_name = get_dll_name(record, pluginList, idx);
                if (prg_name != NULL)
                {
                    prg_names = (char **)realloc(prg_names, (idx + 1) * sizeof(char *));
                    prg_names[idx] = strdup(prg_name);
                }
                else
                {
                    idx--;
                }
            }
        }
        nr_dlls = idx;
        fclose(fp);
        error = 0;
        if (nr_dlls == 0)
        {
            error = 2;
        }
    }
    if (error != 0)
    {
        if (error == 1)
        {
            QMessageBox::warning(0, tr("QGIS_MFE plugin"),
                QString("Check if file \'gis_plugins.csv\' exists on directory:\n") + pluginsHomeBin);
        }
        if (error == 2)
        {
            QMessageBox::warning(0, tr("QGIS_MFE plugin"),
                QString("Non of the specified plugins in file \'gis_plugins.csv\' is read. The file is on directory: \n") + pluginsHomeBin);
        }
        prg_names = (char **)malloc((2 + 1) * sizeof(char *));
        prg_names[0] = strdup("Combobox Plugins");
        prg_names[1] = strdup("Plugin 1");
        prg_names[2] = strdup("Plugin 2");
        nr_dlls = 2;
        error = 1;
    }
    free(record); record = NULL;
    return error;
}
//
//-----------------------------------------------------------------------------
//
char* qgis_mfe::get_dll_name(char * record, GisPluginList * pluginList, int idx)
{
    char * text = NULL;
    char * dllName = NULL;
    char ** record_tok;
    int nr;

    /*
    * Tokenize record string
    */
    if (record != NULL)
    {
        nr = 1;
        record_tok = (char **)malloc(sizeof(char *));
        record_tok[nr - 1] = strdup(strtok(record, ","));

        while (record_tok[nr - 1] != NULL)
        {
            nr++;
            record_tok = (char **)realloc(record_tok, (nr) * sizeof(char *));
            record_tok[nr - 1] = strdup(strtok(NULL, ","));
        }
        nr--;
    }

    dllName = (char *)malloc(strlen(m_plugins_home) + 5 + 1);
    if (record_tok[1] == NULL) {
        text = strdup(record_tok[0]);
    }
    else {
        record_tok[0] = stripSpaces(record_tok[0]);
        record_tok[1] = stripSpaces(record_tok[1]);
        dllName[0] = '\0';
        dllName = strcat(dllName, m_plugins_home);
        dllName = strcat(dllName, "/bin/");

        SetDllDirectory((LPCSTR)dllName);
        pluginList->add((const char *)record_tok[0], (const char *)record_tok[1]);
        text = NULL;
        if (!pluginList->isDllLoaded())
        {
            QString msg = QString("Could not load DLL \'") + record_tok[0] + "\'" +
                "\nPlease check the plugins directory and the path environment variable.";
            QgsMessageLog::logMessage(msg, "QGIS mfe", Qgis::Info, true);
        }
        else if (!pluginList->isFunctionLoaded())
        {
            QMessageBox::warning(0, "Frontend loads a function from DLL",
                QString("Could not find function name,\n") +
                "Function name = >" + record_tok[1] + "<" +
                "\nPlease check the function name." +
                "\nThe name is case sensitive");
        }
        else
        {
            AbstractGisPlugin * aP = (AbstractGisPlugin *)pluginList->getGisPlugin(idx - 1);  // get pointer to active plugin
            text = strdup(aP->getName());  // get active plugin name
        }
    }
    if (text == NULL) return NULL;
    return stripSpaces(text);
}
//
//-----------------------------------------------------------------------------
//
char* qgis_mfe::stripSpaces(char* string)
{
    //Remove first trailing whitespaces and than leading
    char* s;

    for (s = string + strlen(string) - 1;
        (*s == ' ' || *s == '\t' || *s == '\r' || *s == '\n');
        s--);
    *(s + 1) = '\0';

    for (s = string; *s == ' ' || *s == '\t'; s++);

    strcpy(string, s);
    return string;
}
//
//-----------------------------------------------------------------------------
//
void qgis_mfe::unload()
{
    if (this->menuBar != nullptr) { delete this->menuBar; } // the menu toolbar on the Delft3D toolbar tbar2 
    if (this->iconBar != nullptr) { delete this->iconBar; }// the icon toolbar on the Delft3D toolbar tbar2 
    if (this->tbar2 != nullptr) { delete this->tbar2; }  // delete toolbar with menu and icons

    if (this->cb != nullptr) { delete this->cb; }  // delete combobox (selection of plugins) on the Delft3D toolbar tbar
    if (this->mainAction != nullptr) { delete this->mainAction; } // delete main Icon on the Delft3D toolbar tbar
    if (this->tbar != nullptr) { delete this->tbar; } // delete main icon and combobox with plugins
    //disconnect(mainAction, SIGNAL(triggered()), this, SLOT(runAction()));
    //disconnect(cb, SIGNAL(activated(int)), this, SLOT(activatePlugin(int)));
    //disconnect(menuAction, SIGNAL(triggered()), this, SLOT(openFile()));
    //disconnect(iconAction, SIGNAL(triggered()), this, SLOT(openFile()));
    //disconnect(mMyCanvas, SIGNAL(keyPressEvent(QKeyEvent *)), mMyCanvas, SLOT(MyKeyPressEvent(QKeyEvent *)));
    //disconnect(mMyCanvas, SIGNAL(keyReleased(QKeyEvent *)), mMyCanvas, SLOT(MyKeyReleasedEvent(QKeyEvent *)));

    // Clean the buffers
    if (this->mMyCanvas != nullptr) { delete this->mMyCanvas; } // delete main icon and combobox with plugins
    delft3d_enabled = false;
    // remove scratch layer
    QList< QgsMapLayer * > layers = mQGisIface->mapCanvas()->layers();
    int cnt = layers.count();

    for (int i = 0; i < layers.count(); i++)
    {
        //QMessageBox::warning(0, tr("Message"), QString(tr("Layer name: ")) + layers[i]->name() );
        QString jan = layers[i]->name();
        if (jan == QString("Points (Mooiman) vl_1") )
        {
            QgsProject::instance()->removeMapLayer(layers[i]);
        }
    }
}
//
//=============================================================================
//
void qgis_mfe::activatePlugin(int idx)
{
    mQGisIface->mapCanvas()->setMapTool( mMyCanvas );
    if (menuBar != NULL) {
        menuBar->setEnabled( true );
    }
    runAction();

    //// Generate menu and toolbar every time the combobox is activated
    initPlugin(idx); // TODO it uses getCanvas

    activePlugin = pluginList->getGisPlugin(idx);
    activeCanvasWindow = CanvasWindow[idx];
    mMyCanvas->empty_caches();
}
///////////////////////////////////////////////////////////////////////////////
/**
 * Required extern functions needed  for every plugin
 * These functions can be called prior to creating an instance
 * of the plugin class
 */

// Return the type (either UI or MapLayer plugin)
QGISEXTERN int type()
{
    return qgis_mfe::s_plugin_type; // eerste, na selectie in plugin manager
}

// Class factory to return a new instance of the plugin class
QGISEXTERN QgisPlugin* classFactory(QgisInterface* iface)
{
    qgis_mfe* p = new qgis_mfe(iface);
    return (QgisPlugin*)p; // tweede na selectie in plugin manager
}

QGISEXTERN const QString* name()
{
    return &qgis_mfe::s_name; // derde vanuit QGIS
}

QGISEXTERN const QString* category()
{
    return &qgis_mfe::s_category; //
}

QGISEXTERN const QString* description()
{
    return &qgis_mfe::s_description; // tweede vanuit QGIS
}

QGISEXTERN const QString* version()
{
    std::cout << "::version" << std::endl;
    return &qgis_mfe::s_plugin_version;
}

//QGISEXTERN QString const& icon() // derde vanuit QGIS
QGISEXTERN const QString* icon()
{
    QString program_files = QProcessEnvironment::systemEnvironment().value("ProgramFiles", "");
    QString q_icon_file = program_files + QString("/deltares/qgis_umesh/icons/qgis_umesh.png");
    q_icon_file = QStringLiteral("C:/Program Files/deltares/qgis_umesh/icons/qgis_umesh.png");
    return &q_icon_file;

}
// Delete ourself
QGISEXTERN void unload(QgisPlugin* the_qgis_mfePointer)
{
    the_qgis_mfePointer->unload();
    delete the_qgis_mfePointer;
}

