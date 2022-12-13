#ifndef _INC_MYMENUSTOOLBARSDEF
#define _INC_MYMENUSTOOLBARSDEF

#include "AbstractMenusToolbarsDef.h"

class MyMenusToolbarsDef : public AbstractMenusToolbarsDef
{
public:
    MyMenusToolbarsDef();
    ~MyMenusToolbarsDef();

    // a value of -1 means there are no more items
    int getLevel(int index);

    MenusToolbarsType getType(int index);

    const char* getCaption(int index);

    int getCommandId(int index) ;

};

#endif  /* _INC_MYMENUSTOOLBARSDEF */
