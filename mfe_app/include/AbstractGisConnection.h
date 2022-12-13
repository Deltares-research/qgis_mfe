// RUN MLSupportCppHParser.exe
// corresponding multi language support files are generated

#ifndef _INC_ABSTRACTGISCONNECTION
#define _INC_ABSTRACTGISCONNECTION

#include "AbstractResultSet.h"

class AbstractGisConnection {
    public:

    virtual ~AbstractGisConnection() {};

    virtual bool isExceptionOccured() = 0;
    virtual const char* getLastExceptionMessage() = 0;

    // Get the selected shapes/features from the selected layer in GIS
    virtual AbstractResultSet* getSelectedShapesFromSelectedLayer() = 0;

    // Get the selected shapes/features from a layer in GIS
    virtual AbstractResultSet* getSelectedShapesFromLayer(const char* layerName) = 0;

    // Get all shapes/features from the selected layer in the GIS
    virtual AbstractResultSet* getShapesFromSelectedLayer() = 0;

    // Get all the shapes/feature from a layer in the GIS
    virtual AbstractResultSet* getShapesFromLayer(const char* layerName) = 0;

    // Execute an SQL statement on the shape attributes of the selected layer in a GIS
    // The function returns shapes and attributes in the resultSet.
    virtual AbstractResultSet* executeSqlShape(const char* layerName, const char* whereClause, const char* fieldNames) = 0;

    // Get the number of shapes/features in a layer.
    // There may be selected in the layer, then selected = true.
    virtual int getNumberOfShapesFromLayer(const char* layerName, bool selected) = 0;

    // Get the name of the selected layer in the GIS
    virtual const char* getNameSelectedLayer() = 0;

    // Calculate if a shape (point or line) has intersection(s) with one or more shapes of a given GIS layer
    virtual void hasIntersection(const char* searchLayerName, int nrPoints, double* xp/*[nrPoints]*/,
                                 double*  yp/*[nrPoints]*/, double searchTol,
                                 int bufSize, int* intersectedFID/*[bufSize]*/) = 0;

    // Check if the selected layer in the GIS has a database connection.
    virtual bool hasDatabaseConnection(const char* layerName) = 0;

    // Execute an SQL statement on the database connected to the GIS
    virtual AbstractResultSet* executeSqlDatabase(const char* tables, const char* subFields, const char* whereClause) = 0;

    // Get the polyline length by a GIS function
    virtual double getPolylineLength(int nrPoints, double* xs/*[nrPoints]*/, double* ys/*[nrPoints]*/) = 0;

    // Get the distance along the polyline of a given point by a GIS function
    virtual double getDistanceAlong(double xp, double yp, int nrPoints, double* xs/*[nrPoints]*/, double* ys/*[nrPoints]*/) = 0;

    // Get the middle (=midpoint) of a polyline by a GIS function
    virtual double getPolylineMidpointX(int nrPoints, double* xs/*[nrPoints]*/, double* ys/*[nrPoints]*/) = 0;
    virtual double getPolylineMidpointY(int nrPoints, double* xs/*[nrPoints]*/, double* ys/*[nrPoints]*/) = 0;

    // Get the name of the underlying data set (e.g. shape file) of the selected layer in the GIS
    virtual const char* getDataSetNameSelectedLayer() = 0;

    // Add a shape file to the View and table of contents in ArcMap
    virtual void CreateFeatureLayerFromShapeFile(const char* nameShapeFile, const char* layerName) = 0;

    // Select features in a layer by means of a where clause.
    // The funtion returns the number of selected shapes/features.
    virtual int selectFeaturesInLayer(const char* layerName, const char* whereClause) = 0;

    // Intersect the features in two layers. The overlay layer must be of type polygon.
    // The resultset has the attributes of both layers
    virtual AbstractResultSet* intersectTwoLayers(const char* nameInputLayer, const char* nameOverlayLayer, double tolerance, const char* pathName, const char* dataSetName) = 0;

    // Get a layer name based on the source name (=object class name) of this layer
    virtual const char* getLayerName(const char* objectClassName) = 0;

    // Select the features in a layer by a given where clause for the attributes column(s).
    // The resultset has the feature attributes and coordinates of the selection.
    virtual AbstractResultSet* getSelectedShapesByQuery(const char* layerName, const char* columnName, const char* whereClause)  = 0;

    // Make an intersection of two lines and return the X,Y of the intersection point.
    // Xp and Yp are output parameters and for that reason arrays of dimension 1
    virtual void intersectTwoLines(int nrPoints1, double* X1/*[nrPoints1]*/, double* Y1/*[nrPoints1]*/,
                                   int nrPoints2, double* X2/*[nrPoints2]*/, double* Y2/*[nrPoints2]*/,
                                   int nrPoints,  double* Xp/*[nrPoints]*/ , double* Yp/*[nrPoints]*/ ) = 0;

    // Do several GIS operations and return the values: X,Y of a point, a length, an area and success.
    // Because of the language bridge, the output parameters are arrays of lenght 1.
    // ===> This is a specific function for the Intwis-Sobek-Stekker
    virtual void getDatafromTwoLayers(const char* polygonLayerName, const char* polylineLayerName,
                                      const char* polygonIdent, const char* polylineIdent,
                                      int nrPoints, double* area/*[nrPoints]*/, double* length/*[nrPoints]*/,
                                      double* Xp/*[nrPoints]*/, double* Yp/*[nrPoints]*/, int* success/*[nrPoints]*/ ) = 0;

    // Clear the selection of the layer, given by its name
    // If an empty string is given clear selections of all layers in the map
    virtual void clearSelection(const char* layerName) = 0;

    // For Delft3D plugins.
    // Function to get the coordinate system of the GIS, but this coordinate
    // system was set and known by the plugin.
    // Use this function to check if the coordinate system is not changed
    // by the user with GIS commands/tools
    virtual const char* getGisCoordinateSystem() = 0;

    // For Delft3D plugins.
    // Try to get the coordinate system of the GIS itselfs.  It is the
    // coordinate system of the map (=data frame) and this one is set in the GIS
    // when loading layers, and/or by user actions in the GIS via commands/tools.
    // The plugin then set the coordinate system, to use it in the plugin code.
    // Invoke this function when loading or changing the plugin, so the user has
    // no need to do it by a command in the plugin.
    // A command of the plugin may overrule the settings.
    virtual const char* getAndSetCoordinateSystem() = 0;

    // For Delft3D plugins.
    // Set in ArcMap a spherical (=WGS1984) geographical coordinate system
    virtual const char* checkAndSetSphericalCoordinates(const char* coordinateSystem) = 0;

    // For Delft3D plugins.
    // Set a cartesian coordinate system in ArcMap.
    // Mainly it is a check if the coordinate system is not geographic and
    // has meters as unit.
    // When the actual coordinate system is spherical (WGS1984) the original coordinate
    // system is restored.
    virtual const char* checkAndSetCartesianCoordinates() = 0;

    // A check if layers are present in the View (Map) of ArcMap
    virtual bool areLayersPresent() = 0;

    // For Delft3D plugins.
    // Set the projection to stereographic in ArcMap.
    // The long and lat coordinates of the middle of the screen window become
    // parameters central meridian and lattitude of origin for this projection.
    virtual const char* setProjection(const char* projection, double x0, double y0) = 0;

    // For Delft3D plugins.
    // The spatial reference is restored by a geographical coordinate system.
    // This geographical coordinate system is WGS1984 and was stored when calling
    // the routine checkAndSetSphericalCoordinates
    virtual const char* restoreSpatialReference() = 0;

    // For Delft3D plugins.
    // The parameters (long, lat) for the centre of the current projected coordinate
    // system are changed.
    // This is because of panning or zooming on het screen (=View)
    virtual void changeProjectionCentre(int nrPoints, double* xs/*[nrPoints]*/, double* ys/*[nrPoints]*/) = 0;

    // For Delft3D plugins, when changing plugins.
    // Get the x,y values of the maximum extent, which is the extent after
    // loading a *.mxd file.
    // Values in metres or long,lat in degrees, depending on the current
    // coordinate system
    virtual void getMaximumExtent(int nrPoints, double* xs/*[nrPoints]*/, double* ys/*[nrPoints]*/) = 0;

    // For Delft3D plugins, when changing plugins.
    // Get the x,y values as long, lat in degrees of the centre of the
    // stereo projection
    virtual void getProjectionCentre(int nrPoints, double* point/*[nrPoints]*/) = 0;

    // For Delft3D plugins, when changing plugins.
    // Get the x,y values as long,lat in degrees of the extent, which is
    // used when setting the projection centre.
    virtual void getProjectionExtent(int nrPoints, double* xs/*[nrPoints]*/, double* ys/*[nrPoints]*/) = 0;

};

#endif  /* _INC_ABSTRACTGISCONNECTION */
