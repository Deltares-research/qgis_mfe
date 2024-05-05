#include <stdlib.h>

#if defined (WIN32) || defined(WIN64)
#  define DLL_EXPORT
#endif
#include "QApp.h"

static QApplication* app = NULL;

static int appHWND = 0;

void QApp::init(int HWND)
{
   if (app != NULL) return;
   int argc = 0;
   char* argv[] = {"", 0};
   app = new QApplication(argc, argv);

   appHWND = HWND;

}

void QApp::close()
{
   if (app == NULL) return;
   delete app;
   app = NULL;
}


int QApp::getHWND()
{
        return appHWND;
}
