// RUN MLSupportCppHParser.exe
// corresponding multi language support files are generated


#ifndef _INC_ABSTRACTCOMMANDS
#define _INC_ABSTRACTCOMMANDS





class AbstractCommands {
    public:

    enum DefaultCommandIds {
        CMD_UNDO = -1,
        CMD_REDO = -2,
        CMD_CLOSE_WINDOW = -3
    };

    virtual ~AbstractCommands() {};

    virtual const char* getDescription(int commandId) = 0;

    virtual const char* getIconFileName(int commandId) = 0;

    virtual const char* getShortCutKey(int commandId) = 0;

    virtual bool isVisible(int commandId) = 0;

    virtual bool isEnabled(int commandId) = 0;

    virtual bool isOn(int commandId) = 0;

    virtual void execute(int commandId) = 0;

    virtual void execExternCmd(const char* commandString, const char* parameterString) = 0;

    /**
    /* when this version changes the CanvasWindow has to update
    /* the enabled, visible and on state of all buttons and menu items
    /* the CanvasWindow has to check the version every 500ms
    */
    virtual int getStateVersion() = 0;

};

#endif  /* _INC_ABSTRACTCOMMANDS */
