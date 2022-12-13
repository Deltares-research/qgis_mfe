/*
 * Copyright 2003 Waterloopkundig Laboratorium Delft.
 * Author: Michiel Salters @ LogicaCMG
 *
 * This file defines PluginAction, a classs derived from QAction. The
 * QAction::activated signal is alway connected to public slot exec,
 * which invokes the MFE Execute Command call. The command Id in that
 * call is set by the first argument of the ctors. Other arguments are
 * passed unmodified to the QAction ctor.
 *
 * The tooltip for this action is also provided by MFE; it doesn't need
 * to be set manually.
 */
#ifndef PLUGINACTION_H_INCLUDED
#define PLUGINACTION_H_INCLUDED

#include <qaction.h>
#include "AbstractCanvasListener.h"
#include "AbstractCommands.h"


class PluginAction : public QAction
{
    Q_OBJECT
    int command_id;
    AbstractCommands* commands;
    QObject *parent;
    void* actions;

    void setToolTip( );

public:
    PluginAction ( const char* menuText,  AbstractCommands* commands, int command_id,
            QObject * parent, void* theActions);
    void update();

public slots:
    void exec( );

};

#endif   // PLUGINACTION_H_INCLUDED
