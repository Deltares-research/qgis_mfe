#include <stdlib.h>
#include <string.h>

#include "MyGisConnection.h"

#if defined(WIN32) || defined(WIN64)
#  include <windows.h>
#  define strdup _strdup
#endif

MyGisConnection::MyGisConnection()
{
//    this->naam = _strdup("JanM");
}
MyGisConnection::~MyGisConnection()
{
}

bool MyGisConnection::isExceptionOccured()
{
    return false;
}
const char* MyGisConnection::getLastExceptionMessage()
{
    return (const char *) NULL;
}

// Get the selected shapes/features from the selected layer in GIS
AbstractResultSet* MyGisConnection::getSelectedShapesFromSelectedLayer()
{
    return (AbstractResultSet*) NULL;
}

// Get all shapes/features from the selected layer in the GIS
AbstractResultSet* MyGisConnection::getShapesFromSelectedLayer()
{
    return (AbstractResultSet*) NULL;
}

// Get all the shapes/feature from a in the GIS
AbstractResultSet* MyGisConnection::getShapesFromLayer(const char* layerName)
{
    return (AbstractResultSet*) NULL;
}

// Execute an SQL statement on the shape attributes of the selected layer in a GIS
// The function returns shapes and attributes in the resultSet.
AbstractResultSet* MyGisConnection::executeSqlShape(const char* whereClause)
{
    return (AbstractResultSet*) NULL;
}

// Get the name of the selected layer in the GIS
const char* MyGisConnection::getNameSelectedLayer()
{
    return (const char *) NULL;
}

// Calculate if a shape (point or line) has intersection(s) with one or more shapes of a given GIS layer
void MyGisConnection::hasIntersection(const char* searchLayerName, int nrPoints, double* xp/*[nrPoints]*/,
                                 double*  yp/*[nrPoints]*/, double searchTol,
                                 int bufSize, int* intersectedFID/*[bufSize]*/)
{
    return;
}

// Check if the selected layer in the GIS has a database connection.
bool MyGisConnection::hasDatabaseConnection()
{
    return false;
}

// Execute an SQL statement on the database connected to the GIS
AbstractResultSet* MyGisConnection::executeSqlDatabase(const char* tables, const char* subFields, const char* whereClause)
{
    return (AbstractResultSet*) NULL;
}

// Get the polyline length by a GIS function
double MyGisConnection::getPolylineLength(int nrPoints, double* xs/*[nrPoints]*/, double* ys/*[nrPoints]*/)
{
    return 0.0;
}

// Get the distance along the polyline of a given point by a GIS function
double MyGisConnection::getDistanceAlong(double xp, double yp, int nrPoints, double* xs/*[nrPoints]*/, double* ys/*[nrPoints]*/)
{
    return 0.0;
}

// Get the middle (=midpoint) of a polyline by a GIS function
double MyGisConnection::getPolylineMidpointX(int nrPoints, double* xs/*[nrPoints]*/, double* ys/*[nrPoints]*/)
{
    return 0.0;
}
double MyGisConnection::getPolylineMidpointY(int nrPoints, double* xs/*[nrPoints]*/, double* ys/*[nrPoints]*/)
{
    return 0.0;
}

// Get the name of the underlying data set (e.g. shape file) of the selected layer in the GIS
const char* MyGisConnection::getDataSetNameSelectedLayer()
{
    return (const char *) NULL;
}

// Add a shape file to the View and table of contents in ArcMap
void MyGisConnection::CreateFeatureLayerFromShapeFile(const char* nameShapeFile, const char* layerName)
{
    return;
}

// Select features in a layer by means of a where clause.
// The funtion returns the number of selected shapes/features.
int MyGisConnection::selectFeaturesInLayer(const char* layerName, const char* whereClause)
{
    return 0;
}

// Intersect the features in two layers. The overlay layer must be of type polygon.
// The resultset has the attributes of both layers
AbstractResultSet* MyGisConnection::intersectTwoLayers(const char* nameInputLayer, const char* nameOverlayLayer, double tolerance, const char* pathName, const char* dataSetName)
{
    return (AbstractResultSet*) NULL;
}

// Get a layer name based on the source name (=object class name) of this layer
const char* MyGisConnection::getLayerName(const char* objectClassName)
{
    return (const char *) NULL;
}

// Select the features in a layer by a given where clause for the attributes column(s).
// The resultset has the feature attributes and coordinates of the selection.
AbstractResultSet* MyGisConnection::getSelectedShapesByQuery(const char* layerName, const char* columnName, const char* whereClause)
{
    return (AbstractResultSet*) NULL;
}

// Make an intersection of two lines and return the X,Y of the intersection point.
// Xp and Yp are output parameters and for that reason arrays of dimension 1
void MyGisConnection::intersectTwoLines(int nrPoints1, double* X1/*[nrPoints1]*/, double* Y1/*[nrPoints1]*/,
                               int nrPoints2, double* X2/*[nrPoints2]*/, double* Y2/*[nrPoints2]*/,
                               int nrPoints,  double* Xp/*[nrPoints]*/ , double* Yp/*[nrPoints]*/ )
{
    return;
}

// Do several GIS operations and return the values: X,Y of a point, a length, an area and success.
// Because of the language bridge, the output parameters are arrays of lenght 1.
// This is a specific function for the Intwis-Sobek-Stekker
void MyGisConnection::getDatafromTwoLayers(const char* polygonLayerName, const char* polylineLayerName,
                                  int polygonID, int polylineID,
                                  int nrPoints, double* area/*[nrPoints]*/, double* length/*[nrPoints]*/,
                                  double* Xp/*[nrPoints]*/, double* Yp/*[nrPoints]*/, int* success/*[nrPoints]*/ )
{
}

// Clear the selection of the layer, given by its name
// If an empty string is given clear selections of all layers in the map
void MyGisConnection::clearSelection(const char* layerName)
{
    return;
}

// For Delft3D plugins.
// Function to get the coordinate system of the GIS, but this coordinate
// system was set and known by the plugin.
// Use this function to check if the coordinate system is not changed
// by the user with GIS commands/tools
const char* MyGisConnection::getGisCoordinateSystem()
{
    return (const char *) NULL;
}

// For Delft3D plugins.
// Try to get the coordinate system of the GIS itselfs.  It is the
// coordinate system of the map (=data frame) and this one is set in the GIS
// when loading layers, and/or by user actions in the GIS via commands/tools.
// The plugin then set the coordinate system, to use it in the plugin code.
// Invoke this function when loading or changing the plugin, so the user has
// no need to do it by a command in the plugin.
// A command of the plugin may overrule the settings.
const char* MyGisConnection::getAndSetCoordinateSystem()
{
    return (const char *) "";
}

// For Delft3D plugins.
// Set in ArcMap a spherical (=WGS1984) geographical coordinate system
const char* MyGisConnection::checkAndSetSphericalCoordinates(const char* coordinateSystem)
{
    return (const char *) NULL;
}

// For Delft3D plugins.
// Set a cartesian coordinate system in ArcMap.
// Mainly it is a check if the coordinate system is not geographic and
// has meters as unit.
// When the actual coordinate system is spherical (WGS1984) the original coordinate
// system is restored.
const char* MyGisConnection::checkAndSetCartesianCoordinates()
{
    return (const char *) NULL;
}

// A check if layers are present in the View (Map) of ArcMap
bool MyGisConnection::areLayersPresent()
{
    return false;
}

// For Delft3D plugins.
// Set the projection to stereographic in ArcMap.
// The long and lat coordinates of the middle of the screen window become
// parameters central meridian and lattitude of origin for this projection.
const char* MyGisConnection::setProjection(const char* projection, double x0, double y0)
{
    return (const char *) NULL;
}

// For Delft3D plugins.
// The spatial reference is restored by a geographical coordinate system.
// This geographical coordinate system is WGS1984 and was stored when calling
// the routine checkAndSetSphericalCoordinates
const char* MyGisConnection::restoreSpatialReference()
{
    return (const char *) NULL;
}

// For Delft3D plugins.
// The parameters (long, lat) for the centre of the current projected coordinate
// system are changed.
// This is because of panning or zooming on het screen (=View)
void MyGisConnection::changeProjectionCentre(int nrPoints, double* xs/*[nrPoints]*/, double* ys/*[nrPoints]*/)
{
    return;
}

// For Delft3D plugins, when changing plugins.
// Get the x,y values of the maximum extent, which is the extent after
// loading a *.mxd file.
// Values in metres or long,lat in degrees, depending on the current
// coordinate system
void MyGisConnection::getMaximumExtent(int nrPoints, double* xs/*[nrPoints]*/, double* ys/*[nrPoints]*/)
{
    return;
}

// For Delft3D plugins, when changing plugins.
// Get the x,y values as long, lat in degrees of the centre of the
// stereo projection
void MyGisConnection::getProjectionCentre(int nrPoints, double* point/*[nrPoints]*/)
{
    return;
}

// For Delft3D plugins, when changing plugins.
// Get the x,y values as long,lat in degrees of the extent, which is
// used when setting the projection centre.
void MyGisConnection::getProjectionExtent(int nrPoints, double* xs/*[nrPoints]*/, double* ys/*[nrPoints]*/)
{
    return;
}

