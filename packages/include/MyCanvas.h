// RUN MLSupportCppHParser.exe
// corresponding multi language support files are generated

#ifndef _INC_MYCANVAS
#define _INC_MYCANVAS

#include <assert.h>

#include "AbstractCanvas.h"
#include "coordinates.h"

#define GUI_EXPORT __declspec(dllimport)
#include "qgisinterface.h"
#include "qgsmaptool.h"
#include "qgsmaptoolemitpoint.h"
#include "qgspointxy.h"
#include "qgsmapcanvas.h"
#include "qgsmapcanvasitem.h"
#include "qgsmapmouseevent.h"

#include <QtCore/QVector>
#include <QtCore/QObject>
#include <QtCore/QPointer>

#include <QtGui/QColor>
#include <QtGui/QKeyEvent>
#include <QtGui/QMouseEvent>
#include <QtGui/QPainter>
#include <QtGui/QWheelEvent>

#include <QtWidgets/QMessageBox>

#include "qgsmaptool.h"
#include "qgsrubberband.h"

#define NR_CACHES 7
#define IMAGE_WIDTH  3840 // 1420 // 
#define IMAGE_HEIGHT 2140 // 1080 // 

class MyCanvas : public QgsMapTool, public QgsMapCanvasItem, public AbstractCanvas
{
    Q_OBJECT

public:  // function containing the emit-statement
    void canvasDoubleClickEvent(QgsMapMouseEvent *);
    void canvasMoveEvent(QgsMapMouseEvent *);
    void canvasPressEvent(QgsMapMouseEvent *);
    void canvasReleaseEvent(QgsMapMouseEvent *) override;
    void wheelEvent(QWheelEvent *);

signals:  // functions send by the emit-statement
    void MouseDoubleClickEvent(QMouseEvent*);
    void MouseMoveEvent(QMouseEvent*);
    void MousePressEvent(QMouseEvent*);
    void MouseReleaseEvent(QgsMapMouseEvent *);
    void WheelEvent(QWheelEvent*);

private slots:
    void renderPlugin(QPainter *);
    void renderCompletePlugin(QPainter *);
/********** Events a widget must handle or forward to plugin **********/
    void MyMouseDoubleClickEvent(QMouseEvent*);
    void MyMouseMoveEvent(QMouseEvent*);
    void MyMousePressEvent(QMouseEvent*);
    void MyMouseReleaseEvent(QgsMapMouseEvent*);
    void MyWheelEvent(QWheelEvent* e);

    void MyKeyPressEvent(QKeyEvent*);
    void MyKeyReleasedEvent(QKeyEvent*);

    //bool canvasToolTipEvent(QHelpEvent* we);


public:
    MyCanvas(QgsMapCanvas*);
    ~MyCanvas();

    void empty_caches();

    void setListener(AbstractCanvasListener * );

    void startDrawing(int);
    void finishDrawing();

    // Set the background colour (in Qt) with predefined colour names
    // or with a colour value based on rgb values.
    void setCanvasBackgroundColor(const char* colourName);
    void setCanvasBackgroundColorRgb(int colourIn);

    // Create a new draw cache in the frontend and it gets the given index.
    bool newCache(int index);

    // Get the cache index of default available draw cache of the map frontend.
    int getBackgroundCache();

    // Copy a background cache into the drawcache. Both caches are in the
    // frontend and their indexes are used.
    // The drawcache becomes the currently used cache to draw text and geometry.
    void copyCache(int backgroundCacheIndex, int drawCacheIndex);

    // Copy a bitmap (first parameter) into another bitmap (second parameter).
    // It can be a cache bitmap or the bitmap of the window. Both are in the frontend.
    // The bitmap is indicated by the index of the cache. If the first parameter
    // is -1, then the bitmap of the frontend window is used.
    void copyBitmap(int sourceIndex, int destinationIndex);

    // A set routines to draw marker symbols and to set some properties of
    // a marker symbol.
    // A marker symbol can be a circle, cross etc, see enumeration MarkerStyle
    // at the top of this file.
    // A marker symbol is drawn on a x,y position, where x,y are in world coordinates.
    // The current settings of the marker symbol are used when drawing the symbol.
    //
    void drawMarkerSymbol(double x, double y);
    void setMarkerSymbolStyle(MarkerStyle style);
    void setMarkerSymbolColor(int rgb);
    void setMarkerSymbolSize(int size);
    
    void drawDot(double x, double y);
    void drawMultiDot(int nrPoints, double* xs, double* ys, int* rgb);

    // Draw a single point with currently set colour by routine fillColor()
    void drawPoint(double x, double y);

    // Draw an array of points according the given array of colours
    void drawMultiPoint(int nrPoints,
        double* xs/*[nrPoints]*/ , double* ys/*[nrPoints]*/ , int* rgb/*{nrPoints]*/);

    // Draw a polygon. The polygon gets a color according the routine setFillColor
    void drawPolygon(int nrPoints,
        double* xs/*[nrPoints]*/ , double* ys/*[nrPoints]*/);

    // Draw a polyline. This line width and line colour has to be set before this call
    void drawPolyline(int nrPoints,
        double* xs/*[nrPoints]*/ , double* ys/*[nrPoints]*/);

    void drawRectangle(double x, double y, int width, int height);

    // Draw the text. Textposition, font size and color has to bet set first by routines setFont.....
    // and setTextAlignment()
    void drawText(double x, double y, int width, int height, const char* text);

    void setPointSize(int size);
    void setLineWidth(int size);
    void setLineColor(int rgb);
    void setLineStyle(LineStyle style);
    void setFillColor(int rgb);
    void setFontName(const char* name);
    void setFontColor(int rgb);
    void setFontPointSize(int size);
    void setFontItalic(bool value);
    void setFontBold(bool value);
    void setFontUnderline(bool value);
    void setTextAlignment(Alignment value);
    bool isFontAvailable(const char* name);
    int getTextWidth(const char* name);
    int getTextHeight(const char* name, int maxWidth);

    double getPixelWidth(double x, double y);
    double getPixelHeight(double x, double y);

    double getMinX();
    double getMaxX();
    double getMinY();
    double getMaxY();
    double getMinVisibleX();
    double getMaxVisibleX();
    double getMinVisibleY();
    double getMaxVisibleY();
    int getWidth();
    int getHeight();
    int getPixelXFromXY(double x, double y);
    int getPixelYFromXY(double x, double y);
    double getXFromPixel(int pixelX, int pixelY);
    double getYFromPixel(int pixelX, int pixelY);
    int getPixelColor(double X, double Y);

    void setFullExtend(double minX, double maxX, double minY, double maxY);
    void zoomToExtend(double minX, double maxX, double minY, double maxY);

protected:
    void drawCircleMarker(double x, double y);
    void drawSquareMarker(double x, double y);
    void drawCrossMarker(double x, double y);
    void drawXMarker(double x, double y);
    void drawDiamondMarker(double x, double y);

    // From world (document) coordinates to Qt
    int qx( double wx ) { return (int)( (     ((wx) - min_X)/window_width ) * (double)frame_width ); }
    int qy( double wy ) { return (int)( (1.0 -((wy) - min_Y)/window_height) * (double)frame_height); }
    // From Qt coordinates to world (document) coordinates
    double wx ( int qx ) { return ( min_X + ((qx)-1)*dx ); }
    double wy ( int qy ) { return ( min_Y + (frame_height - ((qy)+1))*dy ); }

private:
    // functions
    void myBitBlt( int backgroundCacheIndex, int drawCacheIndex);
    void bitBlt(QPixmap* , int, int, const QPixmap* );
    void checkBuffer0();
    void copyBufferToScreen();
    void paint( QPainter * );

    // variables
    QgisInterface* mQGisIface;
    QPointer<QgsMapCanvas> mMapCanvas;
    QPointer<QgsMapCanvas> mMapCanvasItem;
    QPixmap * mMapCanvasMap;
    AbstractCanvas * mCanvas;

    AbstractCanvasListener* listener;
    bool drawing;
    QPainter* qgis_painter; // The painter object during a Plugin drawing sequence.
    QPainter* mCache_painter; // The painter object during a cache drawing sequence.

    MarkerStyle markerSymbolStyle;
    QColor markerSymbolColor;
    int markerSymbolSize;

    // The coordinate system transformation. Y is flipped.
    double scale; //
    double dx;    // distance of one pixel in world coordinates
    double dy;    // distance of one pixel in world coordinates
    int frame_width; // in pixels
    int frame_height; // in pixels
    double window_width;    // in world coordinates
    double window_height;   // in world coordinates

    double min_X, min_Y, max_X, max_Y; // extent
    double pixelWidth, pixelHeight;

    double radius; // For SetPointSize/DrawPoint. Measured in (fractional) pixels.
    QImage* buffer0;   // Draw cache for double buffering
    QImage* cacheArray[NR_CACHES];   // caches for the plugin
    QColor bgColour;
    QColor fillColor;
    QColor textColour;
    int drawCache;
    bool printing;


    void InitDrawEachCaches(void);
    void DrawEachCaches(void);

};

#endif  /* _INC_ABSTRACTCANVAS */
