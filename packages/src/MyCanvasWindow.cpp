#include <stdlib.h>
#include <string.h>

#include <QAction>
#include <QFont>
#include <QLabel>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QProgressBar>
#include <QSpacerItem>
#include <QMainWindow>

#define DLL_IMPORT
#include "MyCanvasWindow.h"
#include "MyMenusToolbarsDef.h"

#if defined(WIN32) || defined(WIN64)
#  include <windows.h>
#  define strdup _strdup
#endif

#define GUI_EXPORT __declspec(dllimport)

QLabel  * pgBarText;
QWidget * pBar;
QProgressBar * pgBar;
QMainWindow * pgBarWindow;
QgsStatusBar * status_bar;

//char * txt = (char *) malloc(101);
//sprintf(txt , "cmd-%d, idx-%d, cnt-%d\n", command_id, index, count);
//QMessageBox::warning( 0, tr( "Message" ), txt);
//free(txt);

//> MyCanvasWindow class builds everything around the canvas, i.e. the plugin specific toolbar with menubar, iconbar
MyCanvasWindow::MyCanvasWindow(QgisInterface* iface, MyCanvas * mCanvas, QToolBar * tbar, QToolBar * tbar2, QToolBar * menuToolBar, QToolBar * iconToolBar)
{
    mQGisIface = iface;
    menuToolBar->clear();
    iconToolBar->clear();
    //tbar2->clear();

    this->mMyCanvas     = (AbstractCanvas *) mCanvas;
    this->_toolbar       = tbar;
    this->_toolbar2      = tbar2;
    this->_menuToolBar   = menuToolBar;
    this->_iconToolBar   = iconToolBar;
    this->def           = NULL;
    this->commands      = NULL;
    this->_menubar       = NULL;
    this->pluginActions = NULL;


    pgBarText = new QLabel();
    pgBarText->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    pgBarText->hide();

    pgBar = new QProgressBar();
    pgBar->setMaximum(0);
    pgBar->setValue(0);
    pgBar->hide();

    status_bar = mQGisIface->statusBarIface();
    status_bar->addPermanentWidget(pgBar, 0, QgsStatusBar::Anchor::AnchorRight);
}

MyCanvasWindow::~MyCanvasWindow()
{
}

void MyCanvasWindow::setMenusToolbarsDef(AbstractMenusToolbarsDef* p)
{  
    int index;
    int count;

    def = p;
    if ( commands == NULL ) return;

    // Initialise Menu and Toolbar

    if (_menuToolBar != NULL) _menuToolBar->clear();

    if (pluginActions != NULL) delete pluginActions;
    if (_menubar != NULL) delete _menubar;

    pluginActions = new PluginActions();
    _menubar = new QMenuBar();

    index = 0;
    updateCaptionsWithMnemonics(index);
    index = 0;
    count = 0;
    read_menu_item( index, count, NULL);
    _menuToolBar->addWidget(_menubar);

    QWidget* spacer = new QWidget();
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    _toolbar2->addWidget(spacer);
}
void MyCanvasWindow::setCommands(AbstractCommands* p)
{  
    int index = 0;
    int count = 0;

    commands = p;
    if ( def == NULL ) return;
    // Initialise Menu and Toolbar
    return; // assumed: first setcommands than setMenusToolbars in ....GisPlugin.f90 code
}

PluginActions * MyCanvasWindow::getPluginActions()
{
    return pluginActions;
}
int MyCanvasWindow::getMaxCanvasCount()
{
    return 1; //TODO implementation
}
int MyCanvasWindow::addCanvas()
{
    return 0; //TODO implementation
}
void MyCanvasWindow::closeCanvas(int index)
{
}
void MyCanvasWindow::close()
{
}
AbstractCanvas* MyCanvasWindow::getActiveCanvas()
{
    return (AbstractCanvas *) mMyCanvas;
}
int MyCanvasWindow::getActiveCanvasIndex()
{
    return 0; //TODO implementation
}
AbstractCanvas* MyCanvasWindow::getCanvas(int index)
{
    return (AbstractCanvas *) mMyCanvas; 
}
int MyCanvasWindow::getCanvasCount()
{
    return 1; //TODO implementation
}
void MyCanvasWindow::processEvents()
{
}
void* MyCanvasWindow::getQWidget()
{
    return this; //TODO implementation
}
void MyCanvasWindow::setStatusBarText(const char* s)
{
    mQGisIface->mainWindow()->setToolTip(s);
}
void MyCanvasWindow::pgbarSetTitle(const char* s)
{
    //int width;
    //QFontMetrics fm = this->fontMetrics(); //TODO QGIS 4.8.2 MfeAppForm == QWidget
    //width = fm.width(QString(s)); // Does not match exactly.

    pgBarText->setMaximumWidth(100+20);
    pgBarText->setText( tr(s) );
}

void MyCanvasWindow::pgbarSetMaximum(int index)
{
    pgBar->setMaximum( index );
    pgBar->setValue( 0 );
    pgBar->show();
    //pgBarText->show();
}

void MyCanvasWindow::pgbarSetValue(int index)
{
    pgBar->setValue( index );
    if (pgBar->maximum()==index)
    {
        pgBar->hide();
        pgBarText->hide();
    }
}
void MyCanvasWindow::setIconDir(const char* dirPath)
{
    iconDir = strdup(dirPath);
}
void MyCanvasWindow::setBinDir(const char* dirPath)
{
    binDir = strdup(dirPath);
}
void MyCanvasWindow::setIconFileName(const char* fileName)
{
//    setIcon(getIconSet(fileName).pixmap()); //TODO implementation
}
void MyCanvasWindow::setTitle(const char* title)
{
//    setCaption(title);  //TODO implementation
}

// Get height and widht of the screen (desktop)
int MyCanvasWindow::getDesktopHeight()
{
    // Desktop object is used to return the screen height
    // QWidget *d = QApplication::desktop();
    // return d->height();
    return 100;
}
int MyCanvasWindow::getDesktopWidth()
{
    // Desktop object is used to return the screen height
    // QWidget *d = QApplication::desktop();
    // return d->width();
    return 300;
}

// Functions to manipulate the canvas window
void MyCanvasWindow::resizeCanvasWindow(int h, int w)
{
//    resize(w, h); //TODO janm
}
void MyCanvasWindow::moveCanvasWindow(int h, int w)
{
//    move(w, h); TOD janm
}

//=============================================================================
void MyCanvasWindow::read_menu_item( int & index, int & count, QMenu * parent )
{      
    QIcon   icon;
    QString caption;
    int command_id;

    int parent_level = def->getLevel( index );

    if ( parent_level < 0 ) {return;}
      parent_level = parent_level - 1;

      while (def->getLevel(index) > parent_level)
      {
        switch (def->getType(index))
        {
        case AbstractMenusToolbarsDef::TOOLBAR :
            read_toolbar( index ); // updates index
            break;

        case AbstractMenusToolbarsDef::DROPDOWN_MENU  :
        {
            // add new submenu            
            QMenu * newmenu;
            caption = QString (def->getCaption(index));
            if (def->getLevel(index)==0)
            {
                newmenu = _menubar->addMenu(caption);
            }
            else
            {
                newmenu = parent->addMenu(caption);
            }
            index++;
            read_menu_item( index, count, newmenu );
            break;
        }
        case  AbstractMenusToolbarsDef::SEPARATOR  :
            command_id = def->getCommandId(index);
            parent->addSeparator();
            ++index;
            break;
        case AbstractMenusToolbarsDef::COMMAND:
        {
            // Normal menu item.
            command_id = def->getCommandId(index);

            PluginAction* action = new PluginAction(captionsWithMnemonics [index],
                                                    commands, 
                                                    command_id,
                                                    parent, 
                                                    pluginActions,
                                                    iconDir);
        
            pluginActions->add(action);
            parent->addAction((QAction *) action);

            ++index;
            count++;
            break;
        } // end case
        } // end switch
      } //end while
}
void MyCanvasWindow::updateCaptionsWithMnemonics(int &index) {
    bool usedMnemonics[256];
    memset(usedMnemonics, 0, 256 * sizeof(bool));

    int level = def->getLevel(index);
    if ( level < 0 ) return;

    while (def->getLevel(index) >= level) {
        if (def->getLevel(index) > level) {
            updateCaptionsWithMnemonics(index);
            continue;
        }

        int type = def->getType(index);

        if (type != AbstractMenusToolbarsDef::DROPDOWN_MENU
            && type != AbstractMenusToolbarsDef::COMMAND) {
            index++;
            continue;
        }

        const char* caption = def->getCaption(index);
        const char* pos = strchr(caption, '&');
        char mnemonic;
        if (pos == NULL) {
            mnemonic = findFreeMnemonic(usedMnemonics, caption);
        } else {
            mnemonic  = pos[1];
        }
        usedMnemonics[(int) mnemonic] = true;
        bool test = usedMnemonics[(int) mnemonic];
        captionsWithMnemonics[index] = insertMnemonicAmpersand(caption, mnemonic);
        index++;
    }
}
void MyCanvasWindow::read_toolbar( int & index  )
{
    PluginAction* button = NULL;
    int command_id;
    int base_index = index;
    const int base_level = def->getLevel ( base_index );
    
    for (int i = base_index+1; def->getLevel(i) > base_level ; ++i)
    {
        
        // For the child[i] at index,
        if (def->getType(i) == AbstractMenusToolbarsDef::SEPARATOR)
        {
            _iconToolBar->addSeparator(); // Separator((QAction *) button);
        }
        else
        {
            // Child is command
            command_id = def->getCommandId( i );
            PluginAction* button = new PluginAction(def->getCaption(i),
                                                    commands,
                                                    command_id,
                                                    _iconToolBar, 
                                                    pluginActions,
                                                    iconDir);
            pluginActions->add(button);
            _iconToolBar->addAction((QAction *) button);
        }
    }

    //Update index with the number of toolbar buttons(children)
    int level = def->getLevel(index);
    if ( level < 0 ) return;
    index++;
    while (def->getLevel(index) > level)
    {
        index++;
    }
}
char MyCanvasWindow::findFreeMnemonic(bool* usedMnemonics, const char* caption) {
    for (int i = 0; caption[i] != '\0'; i++) {
        char candidate = caption[i];
        if (candidate == ' ') continue;
        if (!usedMnemonics[(int) candidate]) return candidate;
    }
    return '\0';
}
char * MyCanvasWindow::insertMnemonicAmpersand(const char* text, char mnemonic) {

    if (strchr(text, '&') != NULL) return strdup(text);
    if (mnemonic == '\0') return strdup(text);
    if (strchr(text, mnemonic) == NULL) return strdup(text);

    char* res = (char*) malloc(strlen(text) + 2);

    strcpy(res, text);

    const char* posold = strchr(text, mnemonic);
    char* posnew = strchr(res, mnemonic);

    posnew[0] = '&';

    strcpy(&posnew[1], posold);

    return res;
}
