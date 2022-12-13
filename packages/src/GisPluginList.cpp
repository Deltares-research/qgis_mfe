#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#if defined(WIN32) || defined(WIN64)
#  include <windows.h>
#else
# include <ltdl.h>
# include <dlfcn.h>
#endif


#define DLL_EXPORT
#include "GisPluginList.h"
#include "F90Linkage.h"


GisPluginList::GisPluginList() {
    size = 0;
    capacity = 10;
    ptrs = new void*[10];

    okDllLoaded = false;
    okFunctionLoaded = false;
}


GisPluginList::~GisPluginList(){
    delete[] ptrs;
}


bool GisPluginList::isDllLoaded()
{
    // Function used by the frontend (MfeApp and ArcGis),
    // to compose and show a message.
    // The MfeApp program should stop.
    // The ArcGis frontend shoud not load the plugin.

    return okDllLoaded;
}


bool GisPluginList::isFunctionLoaded()
{
    // Function used by the frontend (MfeApp and ArcGis),
    // to compose and show a message.
    // The MfeApp program should stop.
    // The ArcGis frontend shoud not load the plugin.

    return okFunctionLoaded;
}


void GisPluginList::loadDll( const char* dirName, const char* dllName )
{
    // By using this function the frontend can load a specific DLL like (glut32.dll).
    // This DLL MUST be loaded before other DLL's (like our own plugin dll's).
    // So when our plugin DLL depends on a specific DLL (eg glut32.dll), this one will be loaded first
    // and not a DLL in the directory of the GIS program (like ArcMap) or a DLL in
    // the path environment variable.
    // The dll MUST  stay loaded
    // Most times this specific DLL is in the directory of all plugins.
    //
    // You must give the dirName and the dllName.
    // ===. dirName without the last \ character!!!!!!!
    // The DLL must be loaded including its full path.
    //
    char* fullName = new char[1024];

    okDllLoaded = true;

#if defined(WIN32) || defined(WIN64)
    strcpy( fullName, dirName);
    strcat( fullName, "\\" );   //only for MS-Windows because of \ in the path name
    strcat( fullName, dllName);

    void* libAddress = LoadLibrary((LPCSTR)fullName);
#else
    strcpy( fullName, dirName);
    strcat( fullName, "/" );
    strcat( fullName, dllName);
    lt_dlhandle libAddress = lt_dlopen(fullName) ;
#endif

    if ( libAddress == NULL )
    {
        okDllLoaded = false;
    }

    // No free-ing of the loaded library

    delete[] fullName;
}




void GisPluginList::add(const char* dllName, const char* apiSymbolName)
{
    // Make both booleans true.
    // When testing on false in the frontend, you can start tot test on the DLL first
    // and then on the function.
    // It causes only one error message.
    //
    typedef void * ( * MyProc)(void);
    typedef int entrypoint (const char *argument);
    entrypoint *run = NULL;

    //MyProc proc;

    okDllLoaded = true;
    okFunctionLoaded = true;

#if defined(WIN32) || defined(WIN64)
    {
        void * libraryAddress = LoadLibrary((LPCSTR)dllName);
        if ( libraryAddress == NULL )
        {
            okDllLoaded = false;
            return;
        }
        void* symbolAddress = (void*) GetProcAddress( (HINSTANCE) libraryAddress, apiSymbolName);
        if ( symbolAddress == NULL )
        {
            okFunctionLoaded = false;
            return;
        }

        // declaration of function fpGetAPi
        void* (F90_FUNCTION_POINTER fpGetApi)();

        // execution of function from the shared library and type casted
        // When executing the symbolAddress, it creates a plugin object as return value.
        // The symbolAddress points to a FORTRAN function.
        fpGetApi = (void* (F90_FUNCTION_POINTER)()) symbolAddress;

        // The address of this plugin is stored into the list.
        // So each plugin which is loaded by the front end becomes an
        // (unique) object in the frontend.
        // Then functions of the plugin object can be invoked.

        void* apiAddress = fpGetApi();

        ensureCapacity(size + 1);
        ptrs[size] = apiAddress;
        size++;
    }
#else
    {
        const char * msgtxt;
        int errors = lt_dlinit ();

        lt_dlhandle libraryAddress = lt_dlopen(&dllName[0]) ;

        if ( libraryAddress == NULL )
        {
            okDllLoaded = false;
            msgtxt = lt_dlerror ();
            fprintf(stderr, "%s\n", msgtxt);
            return;
        }

        run = (entrypoint *) lt_dlsym (libraryAddress, apiSymbolName);
        if ( run == NULL )
        {
            okFunctionLoaded = false;
            msgtxt = lt_dlerror ();
            fprintf(stderr, "%s\n", msgtxt);
            return;
        }

        void* ( * fpGetApi)();
        fpGetApi = (void* (F90_FUNCTION_POINTER)()) run;
        void* apiAddress = fpGetApi();
        ensureCapacity(size + 1);
        ptrs[size] = apiAddress;
        size++;
    }
#endif
}

int GisPluginList::getSize() {
    return size;
}


AbstractGisPlugin* GisPluginList::getGisPlugin(int index) {
    return (AbstractGisPlugin*) ptrs[index];
}


void GisPluginList::ensureCapacity(int newSize) {
    if (newSize < capacity) return;
    capacity = (int) (1.1 * newSize) + 10;

    void** newPtrs = new void*[capacity];

    memcpy(newPtrs, ptrs,  size * sizeof(void*));

    delete[] ptrs;
    ptrs = newPtrs;
}


void GisPluginList::loadFtnDll(const char* dllPath)
{
    // NOTE:
    // Because ArcGis 9.2 needs the Fortran DLL (DFORRT.DLL) the library dfor.lib
    // will be used instead.
    // So no need for using this function, but you can use it as a kind of documentation.
    //
    // The are problems when using a release version of the pluingins and
    // using the AcrMap program of ArcGIS.
    // The reason is that the /Bin directory of ArcMap contains an old version
    // of the DLL  DFORRT.DLL and when loading our plugins these DLL will be used.
    // However ArcMap itsefs does not need this DLL.
    // So there is the possibility to load this DLL before loading the plugin.
    // This function will do so.
    //
    // The plugin will be loaded in the Add function of this class.
    // Just before calling Add call this function in the ArcMap frontend.
    //
    // There is no need to load DFORRTD.DLL (debug version) because this will be
    // done in the right way by the operating system.
    //
    // When loading MfeApp (Qt version) frontend there is also no need to use
    // this function, because the right version of DFORRT.DLL is present in the
    // Bin directory of MfeApp and the plugins DLL's.

#ifdef NDEBUG
    // Only in the Release version of the plugins (and Release version of the other
    // DLL's) this action is necessary.
    // However the ArcMap frontend does nothing know about Debug and Relase versions
    //
    // So when building the Release version of the DLL's of this Frontend_Plugin project
    // and of the Plugin DLL's, put these together in one Bin directory.
    // Than it will work correctly.
    //
    // The caller is responsable to give the right directory name, which contains
    // the DLL.
    // It is not possible to popup a message window of Qt, because the QApp object
    // is not present in ArcMap (the ArcMap window and its HWND is not present)
    //
    char fullName[1024];

    // The full path name of the DLL's of the plugin is given. DFORRT.DLL must
    // be in this path. (In the future, may be other DLL's to load too)
    //
    strcpy (fullName, dllPath);
    strcat( fullName, "\\DFORRT.DLL");     //Only for Microsoft PC directories

    void* ftnLibraryAddress = LoadLibrary(fullName);

    // Not assert., but an error message, see  GisPluginList::add
    assert(ftnLibraryAddress != NULL);
#endif
}
