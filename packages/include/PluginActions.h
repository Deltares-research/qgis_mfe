#ifndef PLUGINACTIONS_H
#define PLUGINACTIONS_H

#include "PluginAction.h"

class PluginActions : public QObject
{
    Q_OBJECT

private:
    PluginAction** m_actions;
    int m_size;
    int m_capacity;

public:
    PluginActions();
    virtual ~PluginActions();
    void add(PluginAction* a);
    PluginAction* get(int index);
    int size();
    void ensureCapacity(int newSize);

public slots:
    void updataAll();
};

#endif // PLUGINACTIONS_H
