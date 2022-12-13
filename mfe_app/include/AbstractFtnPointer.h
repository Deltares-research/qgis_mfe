// RUN MLSupportCppHParser.exe
// corresponding multi language support files are generated 


#ifndef _INCLUDE_ABSTRACTFTNMEMPOINTERS
#define _INCLUDE_ABSTRACTFTNMEMPOINTERS


class AbstractFtnPointer {
    private:


    public:

    virtual ~AbstractFtnPointer() {};

    virtual void copyMyHandle(int size, void* f90MyHandle) = 0;

};

#endif // _INCLUDE_ABSTRACTFTNMEMPOINTERS
