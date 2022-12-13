// RUN MLSupportCppHParser.exe
// corresponding multi language support files are generated

#ifndef _INC_MYGISCONNECTION
#define _INC_MYGISCONNECTION

#include "AbstractResultSet.h"

class MyGisConnection
{
    public:

    MyGisConnection();
    ~MyGisConnection();

    bool isExceptionOccured();
    const char* getLastExceptionMessage();

    // Get the selected shapes/features from the selected layer in GIS
    AbstractResultSet* getSelectedShapesFromSelectedLayer();

    // Get all shapes/features from the selected layer in the GIS
    AbstractResultSet* getShapesFromSelectedLayer();

    // Get all the shapes/feature from a in the GIS
    AbstractResultSet* getShapesFromLayer(const char* layerName);

    // Execute an SQL statement on the shape attributes of the selected layer in a GIS
    // The function returns shapes and attributes in the resultSet.
    AbstractResultSet* executeSqlShape(const char* whereClause);

    // Get the name of the selected layer in the GIS
    const char* getNameSelectedLayer();

    // Calculate if a shape (point or line) has intersection(s) with one or more shapes of a given GIS layer
    void hasIntersection(const char* searchLayerName, int nrPoints, double* xp/*[nrPoints]*/,
                                 double*  yp/*[nrPoints]*/, double searchTol,
                                 int bufSize, int* intersectedFID/*[bufSize]*/);

    // Check if the selected layer in the GIS has a database connection.
    bool hasDatabaseConnection();

    // Execute an SQL statement on the database connected to the GIS
    AbstractResultSet* executeSqlDatabase(const char* tables, const char* subFields, const char* whereClause);

    // Get the polyline length by a GIS function
    double getPolylineLength(int nrPoints, double* xs/*[nrPoints]*/, double* ys/*[nrPoints]*/);

    // Get the distance along the polyline of a given point by a GIS function
    double getDistanceAlong(double xp, double yp, int nrPoints, double* xs/*[nrPoints]*/, double* ys/*[nrPoints]*/);

    // Get the middle (=midpoint) of a polyline by a GIS function
    double getPolylineMidpointX(int nrPoints, double* xs/*[nrPoints]*/, double* ys/*[nrPoints]*/);
    double getPolylineMidpointY(int nrPoints, double* xs/*[nrPoints]*/, double* ys/*[nrPoints]*/);

    // Get the name of the underlying data set (e.g. shape file) of the selected layer in the GIS
    const char* getDataSetNameSelectedLayer();

    // Add a shape file to the View and table of contents in ArcMap
    void CreateFeatureLayerFromShapeFile(const char* nameShapeFile, const char* layerName);

    // Select features in a layer by means of a where clause.
    // The funtion returns the number of selected shapes/features.
    int selectFeaturesInLayer(const char* layerName, const char* whereClause);

    // Intersect the features in two layers. The overlay layer must be of type polygon.
    // The resultset has the attributes of both layers
    AbstractResultSet* intersectTwoLayers(const char* nameInputLayer, const char* nameOverlayLayer, double tolerance, const char* pathName, const char* dataSetName);

    // Get a layer name based on the source name (=object class name) of this layer
    const char* getLayerName(const char* objectClassName);

    // Select the features in a layer by a given where clause for the attributes column(s).
    // The resultset has the feature attributes and coordinates of the selection.
    AbstractResultSet* getSelectedShapesByQuery(const char* layerName, const char* columnName, const char* whereClause) ;

    // Make an intersection of two lines and return the X,Y of the intersection point.
    // Xp and Yp are output parameters and for that reason arrays of dimension 1
    void intersectTwoLines(int nrPoints1, double* X1/*[nrPoints1]*/, double* Y1/*[nrPoints1]*/,
                                   int nrPoints2, double* X2/*[nrPoints2]*/, double* Y2/*[nrPoints2]*/,
                                   int nrPoints,  double* Xp/*[nrPoints]*/ , double* Yp/*[nrPoints]*/ );

    // Do several GIS operations and return the values: X,Y of a point, a length, an area and success.
    // Because of the language bridge, the output parameters are arrays of lenght 1.
    // This is a specific function for the Intwis-Sobek-Stekker
    void getDatafromTwoLayers(const char* polygonLayerName, const char* polylineLayerName,
                                      int polygonID, int polylineID,
                                      int nrPoints, double* area/*[nrPoints]*/, double* length/*[nrPoints]*/,
                                      double* Xp/*[nrPoints]*/, double* Yp/*[nrPoints]*/, int* success/*[nrPoints]*/ );

    // Clear the selection of the layer, given by its name
    // If an empty string is given clear selections of all layers in the map
    void clearSelection(const char* layerName);

    // For Delft3D plugins.
    // Function to get the coordinate system of the GIS, but this coordinate
    // system was set and known by the plugin.
    // Use this function to check if the coordinate system is not changed
    // by the user with GIS commands/tools
    const char* getGisCoordinateSystem();

    // For Delft3D plugins.
    // Try to get the coordinate system of the GIS itselfs.  It is the
    // coordinate system of the map (=data frame) and this one is set in the GIS
    // when loading layers, and/or by user actions in the GIS via commands/tools.
    // The plugin then set the coordinate system, to use it in the plugin code.
    // Invoke this function when loading or changing the plugin, so the user has
    // no need to do it by a command in the plugin.
    // A command of the plugin may overrule the settings.
    const char* getAndSetCoordinateSystem();

    // For Delft3D plugins.
    // Set in ArcMap a spherical (=WGS1984) geographical coordinate system
    const char* checkAndSetSphericalCoordinates(const char* coordinateSystem);

    // For Delft3D plugins.
    // Set a cartesian coordinate system in ArcMap.
    // Mainly it is a check if the coordinate system is not geographic and
    // has meters as unit.
    // When the actual coordinate system is spherical (WGS1984) the original coordinate
    // system is restored.
    const char* checkAndSetCartesianCoordinates();

    // A check if layers are present in the View (Map) of ArcMap
    bool areLayersPresent();

    // For Delft3D plugins.
    // Set the projection to stereographic in ArcMap.
    // The long and lat coordinates of the middle of the screen window become
    // parameters central meridian and lattitude of origin for this projection.
    const char* setProjection(const char* projection, double x0, double y0);

    // For Delft3D plugins.
    // The spatial reference is restored by a geographical coordinate system.
    // This geographical coordinate system is WGS1984 and was stored when calling
    // the routine checkAndSetSphericalCoordinates
    const char* restoreSpatialReference();

    // For Delft3D plugins.
    // The parameters (long, lat) for the centre of the current projected coordinate
    // system are changed.
    // This is because of panning or zooming on het screen (=View)
    void changeProjectionCentre(int nrPoints, double* xs/*[nrPoints]*/, double* ys/*[nrPoints]*/);

    // For Delft3D plugins, when changing plugins.
    // Get the x,y values of the maximum extent, which is the extent after
    // loading a *.mxd file.
    // Values in metres or long,lat in degrees, depending on the current
    // coordinate system
    void getMaximumExtent(int nrPoints, double* xs/*[nrPoints]*/, double* ys/*[nrPoints]*/);

    // For Delft3D plugins, when changing plugins.
    // Get the x,y values as long, lat in degrees of the centre of the
    // stereo projection
    void getProjectionCentre(int nrPoints, double* point/*[nrPoints]*/);

    // For Delft3D plugins, when changing plugins.
    // Get the x,y values as long,lat in degrees of the extent, which is
    // used when setting the projection centre.
    void getProjectionExtent(int nrPoints, double* xs/*[nrPoints]*/, double* ys/*[nrPoints]*/);

};

#endif  /* _INC_MYGISCONNECTION */
