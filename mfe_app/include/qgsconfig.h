// QGSCONFIG.H

#ifndef QGSCONFIG_H
#define QGSCONFIG_H

// Version must be specified according to
// <int>.<int>.<int>-<any text>.
// or else upgrading old project file will not work
// reliably.
#define VERSION "2.2.0-Mooiman"

//used in vim src/core/qgis.cpp
//The way below should work but it resolves to a number like 0110 which the compiler treats as octal I think
//because debuggin it out shows the decimal number 72 which results in incorrect version status.
//As a short term fix I (Tim) am defining the version in top level cmake. It would be good to
//reinstate this more generic approach below at some point though
//#define VERSION_INT 180
#define VERSION_INT 20200
//used in main.cpp and anywhere else where the release name is needed
#define RELEASE_NAME "Mooiman"

#define QGIS_PLUGIN_SUBDIR "c:\OSGeo4W\apps\qgis\plugins"
#define QGIS_DATA_SUBDIR "c:\OSGeo4W\share"
#define QGIS_LIBEXEC_SUBDIR "c:\OSGeo4W\lib"
#define QGIS_LIB_SUBDIR "lib"
#define CMAKE_INSTALL_PREFIX "c:\OSGeo4W"
#define CMAKE_SOURCE_DIR "d:\git-checkouts\build"

#define HAVE_POSTGRESQL

#define HAVE_SPATIALITE

#define HAVE_MSSQL

/* #undef HAVE_PYTHON */

#endif