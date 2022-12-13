// PluginActions.cpp: implementation of the PluginActions class.
//
//////////////////////////////////////////////////////////////////////
#include <QMessageBox>
#include "PluginActions.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

PluginActions::PluginActions()
{
    m_size = 0;
    m_capacity = 10;
    m_actions = new PluginAction*[10];
}

PluginActions::~PluginActions()
{
    delete m_actions;
}

void PluginActions::updataAll() 
{
    for (int i = 0; i < m_size; i++) {
        m_actions[i]->update();
    }
}
void PluginActions::add(PluginAction* a) 
{
    ensureCapacity(m_size + 1);
    m_actions[m_size] = a;
    m_size++;
}
    
PluginAction* PluginActions::get(int index) {
    return m_actions[index];
}

int PluginActions::size() 
{
    return m_size;
}


void PluginActions::ensureCapacity(int newSize) {
    if (newSize < m_capacity) return;
    m_capacity = (int) (1.1 * newSize); 
    
    PluginAction** newPtrs = new PluginAction*[m_capacity];
 
    
    memcpy(newPtrs, m_actions, m_size * sizeof(PluginAction*));
    delete m_actions;
    m_actions = newPtrs;
}



