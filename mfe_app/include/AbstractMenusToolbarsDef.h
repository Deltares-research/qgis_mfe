// RUN MLSupportCppHParser.exe
// corresponding multi language support files are generated



#ifndef _INC_ABSTRACTMENUSTOOLBARSDEF
#define _INC_ABSTRACTMENUSTOOLBARSDEF



class AbstractMenusToolbarsDef {
    public:

    enum MenusToolbarsType {
        DROPDOWN_MENU = 0,
        POPUP_MENU = 1,
        TOOLBAR = 2,
        COMMAND = 3,
        SEPARATOR = 4,
    };



    virtual ~AbstractMenusToolbarsDef() {};

    // a value of -1 means there are no more items
    virtual int getLevel(int index) = 0;

    virtual MenusToolbarsType getType(int index) = 0;

    virtual const char* getCaption(int index) = 0;

    virtual int getCommandId(int index) = 0;

};

#endif  /* _INC_ABSTRACTMENUSTOOLBARSDEF */
