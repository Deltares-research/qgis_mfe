#include <iostream>
#include <stdlib.h>

#if defined(WIN32) || defined(WIN64)
#  define strdup _strdup
#endif

#include "PluginAction.h"
#include "PluginActions.h"
#include "qgis_mfe.h"


PluginAction::PluginAction ( const char* menuText, AbstractCommands* commands, int command_id,
        QObject * aParent, void* theActions, char * iconDir)
    :
    QAction(aParent)
{
    QIcon   icon;
    char * iconFile = NULL;
    char * iconFileName = (char *) malloc(250*sizeof(char *));

    this->commands = commands;
    this->command_id = command_id;
    this->actions = theActions;

    sprintf(iconFileName, "%s", commands->getIconFileName(command_id));

    //    iconFile = (char *) malloc(strlen(iconDir)+1+strlen(iconFilename)+1);
    iconFile = (char *) malloc(250*sizeof(char *));
    iconFile[0] = '\0';
    strcat(iconFile, iconDir);
    strcat(iconFile, "/");
    strcat(iconFile, iconFileName);
    icon    = QIcon   ( iconFile ); 
    setIcon(icon);
    setText(menuText);
    setStatusTip();
    setVisible(true);
    setCheckable(true);
    update();
    connect(this, SIGNAL( triggered() ), this, SLOT( exec() ) );
}

void PluginAction::update() {
    this->setEnabled(commands->isEnabled( command_id ) );
    this->setChecked(commands->isOn(command_id));
    this->setVisible(commands->isVisible(command_id));
}

void PluginAction::exec( )
{
    // The command to the Delft3D plugin with command_id
    commands->execute(command_id );

    PluginActions* a = (PluginActions*) actions;
    a->updataAll(); 
}
void PluginAction::setStatusTip( )
{
    QString tip = commands->getDescription(command_id) ;
    QAction::setStatusTip( tip );
}

