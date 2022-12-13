#include <stdlib.h>
#include <string.h>

#define DLL_EXPORT
#include "MyCanvas.h"

#define GUI_EXPORT __declspec(dllimport)
#include "qgsmapcanvas.h"
#include "qgsmapcanvasmap.h"
//#include "qgscursors.h"
#include "qgsmaptopixel.h"
#include "qgsrubberband.h"
#include "qgscoordinatereferencesystem.h"
#include "qgsmaptool.h"
#include "qgspoint.h"
#include "qgsapplication.h"

#include <QObject>
#include <QPointer>
#include <QVBoxLayout>
#include <QLabel>

#if defined(WIN32) || defined(WIN64)
#  include <windows.h>
#  define strdup _strdup
#endif

#define DRAW_CACHES false

QPaintDevice * pd;

QWidget * w;
QVBoxLayout * vb;
QLabel * label[NR_CACHES];

//
// caches
// 0: drawing cache
// 5, 4, 3, 2, 1: stapled caches
// 6 contains the background picture
//
//-----------------------------------------------------------------------------
//
MyCanvas::MyCanvas(QgsMapCanvas* mapCanvas) :
    QgsMapTool( mapCanvas ),
    QgsMapCanvasItem( mapCanvas ),
    printing(false)
    {
    QgsMapTool::setCursor(QgsApplication::getThemeCursor(QgsApplication::Cursor::CrossHair));
    
    mMapCanvas = mapCanvas;
    mMapCanvasItem = mapCanvas;
    drawing = true;

    qgis_painter = NULL;
    mCache_painter = new QPainter();

    buffer0 = new QImage(IMAGE_WIDTH, IMAGE_HEIGHT, QImage::Format_ARGB32_Premultiplied);
    buffer0->fill(Qt::transparent);

    for (int i=0; i<NR_CACHES; i++)
    {
        cacheArray[i] = NULL;
        newCache(i);
    }
    //
    // Render events
    // This calls the renderer everytime the canvas has drawn itself
    //
    connect(mMapCanvas, SIGNAL(renderComplete(QPainter *)), this, SLOT(renderCompletePlugin(QPainter *)));
    //
    // Key events
    //
    connect(mMapCanvas, SIGNAL(keyPressed(QKeyEvent *)), this, SLOT(MyKeyPressEvent(QKeyEvent *)));
    connect(mMapCanvas, SIGNAL(keyReleased(QKeyEvent *)), this, SLOT(MyKeyReleasedEvent(QKeyEvent *)));
    //
    // Mouse events
    //
    connect(this, SIGNAL(MouseDoubleClickEvent(QMouseEvent *)), this, SLOT(MyMouseDoubleClickEvent(QMouseEvent *)));
    connect(this, SIGNAL(MouseMoveEvent(QMouseEvent *)), this, SLOT(MyMouseMoveEvent(QMouseEvent *)));
    connect(this, SIGNAL(MousePressEvent(QMouseEvent *)), this, SLOT(MyMousePressEvent(QMouseEvent *)));
    connect(this, SIGNAL(MouseReleaseEvent(QgsMapMouseEvent *)), this, SLOT(MyMouseReleaseEvent(QgsMapMouseEvent *)));
    connect(this, SIGNAL(WheelEvent(QWheelEvent * we)), this, SLOT(MyWheelEvent(QWheelEvent * we)));

    if (DRAW_CACHES) {
        InitDrawEachCaches(); // debug utility
    }
}
//
//-----------------------------------------------------------------------------
//
MyCanvas::~MyCanvas()
{
   for ( int j=0; j<NR_CACHES; j++ )
   {
       if (cacheArray[j] != NULL)
       {
           delete cacheArray[j];
       }
   }
}


void MyCanvas::empty_caches()
{
    if (buffer0 != NULL)
    {
        buffer0->fill(Qt::transparent);
    }
    for (int j = 0; j<NR_CACHES; j++)
    {
        if (cacheArray[j] != NULL)
        {
            cacheArray[j]->fill(Qt::transparent);
        }
    }
    mMapCanvas->update();
}


//
//-----------------------------------------------------------------------------
//
void MyCanvas::paint( QPainter * p )
{
    //p->drawImage(0, 0, *cacheArray[NR_CACHES - 1]);
    for (int i = NR_CACHES - 1; i >= 0; i--) {
        p->drawImage(0, 0, *cacheArray[i]);
    }
}
//
//-----------------------------------------------------------------------------
//
void MyCanvas::startDrawing(int cache_i)
{
    bool clear_cache;

    if (printing) return;

    // Starts with a clean Qt background.
    // When the plugin applies caches, you must start with this startDrawing()
    // function. Then between startDrawing() and finishDrawing() functions you
    // may apply copyCache() and copyBitmap().

    clear_cache = true;
    if (cache_i >= 200) { // force repaint, do not clear current cache contents.
        clear_cache = false;
        cache_i = cache_i - 200;
    }
    if (cache_i >= 100)  // force repaint, and clear current cache contents.
    {
        cache_i = cache_i - 100;
    }
    cache_i = max(0, min(cache_i, NR_CACHES - 1));
    
    if (drawing)
    {
        if (mCache_painter->isActive()) {
            mCache_painter->end(); // end painting on cacheArray[i]
        }
    }
    if (clear_cache) {
        cacheArray[cache_i]->fill(Qt::transparent);
    }
    mCache_painter->begin(cacheArray[cache_i]);
    drawCache = cache_i;
    drawing = true;

    if (DRAW_CACHES) {
        DrawEachCaches(); // debug utility
    }
}
//
//-----------------------------------------------------------------------------
//
void MyCanvas::finishDrawing()
{
    // QMessageBox::warning(0, "Message", QString(tr("MyCanvas::finishDrawing.")));
    if (mCache_painter->isActive()) {
        mCache_painter->end();
    }
    mMapCanvas->update();
    drawing = false;

    if (DRAW_CACHES) {
        DrawEachCaches(); // debug utility
    }
}
//
//-----------------------------------------------------------------------------
//
void MyCanvas::copyBufferToScreen()
{
}
//
//-----------------------------------------------------------------------------
//
void MyCanvas::renderCompletePlugin(QPainter * Painter)
{
    //QMessageBox::warning(0, "Message", QString(tr("MyCanvas::renderCompletePlugin") ));
    renderPlugin( Painter );
}
void MyCanvas::renderPlugin( QPainter * Painter )
{
    // OK JanM QMessageBox::warning(0, "Message", QString(tr("MyCanvas::renderPlugin") ));
    // need width/height of paint device
    int myWidth = Painter->device()->width();  //pixels
    int myHeight = Painter->device()->height(); //pixels
    int width  = 500; //pixels
    int height = 250; //pixels

    this->qgis_painter = Painter;

    mMapCanvas->setMinimumWidth(width);
    mMapCanvas->setMinimumHeight(height);
    scale = mMapCanvas->scale();

    QRect frame = mMapCanvas->frameRect();
    frame_width = frame.width(); //total frame width in pixels
    frame_height = frame.height();  //total height width in pixels

    QgsRectangle extent = mMapCanvas->extent();
    window_width = extent.width();   //total frame width in world coordinates
    window_height = extent.height(); //total height width in world coordinates
    min_X = extent.xMinimum();
    min_Y = extent.yMinimum();

    dx = window_width/frame_width;
    dy = window_height/frame_height;

    this->qgis_painter->setViewport(qx(min_X), qy(min_Y), qx(window_width), qy(window_height));    

    if (listener != NULL)
    {
        listener->onAfterRedraw(false); // TODO: In onafterredraw zit ook een teken event
    }
}
//
//-----------------------------------------------------------------------------
//
void MyCanvas::canvasDoubleClickEvent(QgsMapMouseEvent * me )
{
    emit MouseDoubleClickEvent( me );
}
void MyCanvas::canvasMoveEvent(QgsMapMouseEvent * me )
{
    //QMessageBox::warning(0, "Message", QString(tr("MyCanvas::canvasMoveEvent: %1").arg(me->button())));
    emit MouseMoveEvent( me );
}
void MyCanvas::canvasPressEvent(QgsMapMouseEvent * me )
{
    emit MousePressEvent( me );
}
void MyCanvas::canvasReleaseEvent(QgsMapMouseEvent * me )
{
    //QMessageBox::warning(0, "Message", QString(tr("MyCanvas::canvasReleaseEvent: %1").arg(me->button())));
    emit MouseReleaseEvent( me );
}
void MyCanvas::wheelEvent( QWheelEvent * we )
{
    //QMessageBox::warning(0, "Message", QString(tr("MyCanvas::wheelEvent")));
    this->empty_caches();
    mMapCanvas->refresh();
    mMapCanvas->update();
    emit WheelEvent( we );
}
//
//-----------------------------------------------------------------------------
//
void MyCanvas::setListener(AbstractCanvasListener* p)
{
    listener = p;

    // Invoke onActivate to set the initial coordinate system for the plugin
    if (listener != NULL)
    {
        listener->onActivate();
	}
}
//
//-----------------------------------------------------------------------------
//
// Set the background colour (in Qt) with predefined colour names
// or with a colour value based on rgb values.
void MyCanvas::setCanvasBackgroundColor(const char* colourName)
{
    // bgColour is global for this class
//    if ( strcmp(colourName, "WHITE")  == 0 ) bgColour = white;
//    if ( strcmp(colourName, "GRAY")   == 0 ) bgColour = gray;
//    if ( strcmp(colourName, "LIGHTGRAY")   == 0 ) bgColour = lightGray;
//    if ( strcmp(colourName, "DARKGRAY")   == 0 ) bgColour = darkGray;
//    if ( strcmp(colourName, "BLACK")   == 0 ) bgColour = black;
}
//
//-----------------------------------------------------------------------------
//
void MyCanvas::setCanvasBackgroundColorRgb(int colourIn)
{
    bgColour = QColor( colourIn );
}
//
//-----------------------------------------------------------------------------
//
// Create a new draw cache in the frontend and it gets the given index.
bool MyCanvas::newCache(int cacheNr)
{
    // 5 caches are available to speed up drawing object by the plugin.
    // ===> Note that ArcGis frontend has 5 caches too.
    //
    // Caches are implemented as QPixmap. Their size is maximum (2048, 1536) and
    // no resize of the pixmap (=cache) during its use.
    //
    // There is an array of pixmaps (cacheArray) and its size is 5
    // The calling plugin provides a cacheIndex, which is an index in cacheArray
    // The cacheIndex will be used in function copyCache.

    // ===> TO DO
    //      Dit gaat alleen goed omdat the frontend MfeApp slecht 1 plugin heeft!!
    //      Moet op termijn anders, zie ook bij ArcGis frontend (in gebruik zijnde
    //      index nummers bewaren in een frontend module als Plugins.bas)

    if ( cacheNr < 0  ||  cacheNr > NR_CACHES-1 )
    {
        return false;
    }

    if ( cacheArray[cacheNr] == NULL )
    {
        cacheArray[cacheNr] = new QImage(IMAGE_WIDTH, IMAGE_HEIGHT, QImage::Format_ARGB32_Premultiplied);
    }

    return true;
}
//
//-----------------------------------------------------------------------------
//
// Get the cache index of default available draw cache of the map frontend.
int MyCanvas::getBackgroundCache()
{
    return NR_CACHES-1; // Cache in which background should be copyied
}
//
//-----------------------------------------------------------------------------
//
void MyCanvas::copyCache(int sourceCacheIndex, int destCacheIndex)
{
	// No need for this function, still avalaible because of interface with plugins
}
//
//-----------------------------------------------------------------------------
//
void MyCanvas::copyBitmap(int sourceCacheIndex, int destCacheIndex)
{
	// No need for this function, still avalaible because of interface with plugins
}
//
//-----------------------------------------------------------------------------
//
// A set routines to draw marker symbols and to set so me properties of
// a marker symbol.
// A marker symbol can be a circle, cross etc, see enumeration MarkerStyle
// at the top of this file.
// A marker symbol is drawn on a x,y position, where x,y are in world coordinates.
// The current settings of the marker symbol are used when drawing the symbol.
//
void MyCanvas::drawMarkerSymbol(double x, double y)
{
    // 5 marker styles are available, same as in ArcGis
    switch ( markerSymbolStyle )
    {
      case MS_Circle :
        drawCircleMarker(x, y);
        break;
      case MS_Square :
        drawSquareMarker(x, y);
        break;
      case MS_Cross :
        drawCrossMarker(x, y);
        break;
      case MS_X :
        drawXMarker(x, y);
        break;
      case MS_Diamond :
        drawDiamondMarker(x, y);
        break;
    }
}
//
//-----------------------------------------------------------------------------
//
void MyCanvas::drawCircleMarker(double x, double y)
{
    QPen current_pen;
    double markerSize;

    markerSize = (double)markerSymbolSize ;
    current_pen = mCache_painter->pen();
    mCache_painter->setPen( markerSymbolColor );

    QPoint center = QPoint(qx(x), qy(y));
    mCache_painter->drawEllipse(center, int(0.5*markerSize), int(0.5*markerSize));

    mCache_painter->setPen( current_pen );
}
//
//-----------------------------------------------------------------------------
//
void MyCanvas::drawSquareMarker(double x, double y)
{
    QPen current_pen;
    int x1;
    int y1;
    double markerSize;

    markerSize = (double)markerSymbolSize ;
    current_pen = mCache_painter->pen();
    mCache_painter->setPen( markerSymbolColor );

    x1 = qx( x ) - markerSymbolSize;
    y1 = qy( y ) - markerSymbolSize;
    mCache_painter->drawRect(x1, y1, int(2.0*markerSymbolSize), int(2.0*markerSymbolSize)) ;

    mCache_painter->setPen( current_pen );
}
//
//-----------------------------------------------------------------------------
//
void MyCanvas::drawCrossMarker(double x, double y)
{
    QPen current_pen;
    int x1;
    int y1;
    int x2;
    int y2;
    double markerSize;

    markerSize = (double)markerSymbolSize ;
    current_pen = mCache_painter->pen();
    mCache_painter->setPen( markerSymbolColor );

    x1 = qx( x-markerSize ) ;
    x2 = qx( x+markerSize ) ;
    y1 = qy( y ) ;
    y2 = y1 ;
    mCache_painter->drawLine(x1,y1,x2,y2) ;

    x1 = qx( x ) ;
    x2 = x1 ;
    y1 = qy( y-markerSize ) ;
    y2 = qy( y+markerSize ) ;
    mCache_painter->drawLine(x1, y1, x2, y2) ;

    mCache_painter->setPen( current_pen );
}
//
//-----------------------------------------------------------------------------
//
void MyCanvas::drawXMarker(double x, double y)
{
    // For the time being
    drawCrossMarker(x, y);
}
//
//-----------------------------------------------------------------------------
//
void MyCanvas::drawDiamondMarker(double x, double y)
{
    // For the time being
    drawCrossMarker(x, y);
}
//
//-----------------------------------------------------------------------------
//
void MyCanvas::setMarkerSymbolStyle(MarkerStyle style)
{
    markerSymbolStyle = style;
}
//
//-----------------------------------------------------------------------------
//
void MyCanvas::setMarkerSymbolColor(int rgb)
{
    markerSymbolColor = QColor(rgb);
}
//
//-----------------------------------------------------------------------------
//
void MyCanvas::setMarkerSymbolSize(int size)
{
    markerSymbolSize = size;
 }
//
//-----------------------------------------------------------------------------
//
void MyCanvas::drawDot(double x, double y)
{
    // Don't draw an edge
    QPen current_pen = mCache_painter->pen();
    mCache_painter->setPen( Qt::NoPen );
    QPoint center = QPoint(qx(x), qy(y));
    mCache_painter->drawEllipse( center, int(radius), int(radius) );
    mCache_painter->setPen( current_pen );
}

void MyCanvas::drawMultiDot(int nrPoints, double* xs , double* ys , int* rgb)
{
    int    i, j, k;
    int    index;
    QPen   current_pen;
    double xMin, xMax, yMin, yMax;
    int    iMin, iMax, jMin, jMax;
    int    sizeI, sizeJ;
    double sx, sy;

    if (radius==0) {return;}
    if (radius < 4.0) { // i.e. 1, 2 and 3
        drawMultiPoint(nrPoints, xs , ys , rgb);
        return;
    }

    // NOTE that in this function is a check if the coordinates are outside the screen/display
    //
    // These are world coordinates
    //
    xMin = getMinVisibleX();
    xMax = getMaxVisibleX();
    yMin = getMinVisibleY();
    yMax = getMaxVisibleY();

    iMin = qx( xMin );
    iMax = qx( xMax );
    // function qy() mirrors the min and max!!!
    jMax = qy( yMin );
    jMin = qy( yMax );

    sizeI = iMax - iMin + 1;
    sizeJ = jMax - jMin + 1;

    int *pixelArray = new int [sizeI*sizeJ];

    // Initial value 0
    //
    memset(pixelArray, 0, (sizeI*sizeJ) * sizeof(int));

    // Don't draw an edge
    current_pen = mCache_painter->pen();
    mCache_painter->setPen( Qt::NoPen );


    // Loop for all point
    //
    sx = radius/scale; // scale x
    sy = radius/scale; // scale y

    for ( k = 0; k != nrPoints; k++ )
    {
        // Convert from world to pixel coordinates.
        // Functions qx() and qy() take into account that (0,0) is upper left corner
        //
        i = qx( xs[k]-sx );
        j = qy( ys[k]+sy );      //Y-flip

        if ( i >= 0 && i < sizeI && j >= 0 && j < sizeJ)
        {
            index = i + j*sizeI;
            if ( pixelArray[index] == 0 )
            {
                // Pixel not drawn yet.
                //
                mCache_painter->setBrush( QColor( rgb[k] ) );
                mCache_painter->drawEllipse( QPoint(i, j), int(radius), int(radius));
                pixelArray[index] = 1;
            }
        }
    }

    mCache_painter->setPen( current_pen );

    delete[] pixelArray;
}
//
//-----------------------------------------------------------------------------
//
// Draw a single point with currently set colour by routine fillColor()
void MyCanvas::drawPoint(double x, double y)
{
    QPen current_pen = mCache_painter->pen();
    mCache_painter->setPen(Qt::NoPen);
    QPoint center = QPoint(qx(x), qy(y));
    mCache_painter->drawEllipse(center, int(0.5*radius), int(0.5*radius));
    mCache_painter->setPen(current_pen);
}
//
//-----------------------------------------------------------------------------
//
// Draw an array of points according the given array of colours
void MyCanvas::drawMultiPoint(int nrPoints, double* xs, double* ys, int* rgb)
{
    int    i, j, k;
    double xMin, xMax, yMin, yMax;
    int    iMin, iMax, jMin, jMax;
    int    sizeI, sizeJ;
    unsigned int * buffer_;
    unsigned int transparent;
    unsigned int colour;

    // NOTE that in this function is a check if the coordinates are outside the screen/display
    //
    // These are world coordinates
    //
    xMin = getMinVisibleX();
    xMax = getMaxVisibleX();
    yMin = getMinVisibleY();
    yMax = getMaxVisibleY();

    iMin = qx(xMin);
    iMax = qx(xMax);
    // function qy() mirrors the min and max!!!
    jMax = qy(yMin);
    jMin = qy(yMax);

    sizeI = iMax - iMin + 1;
    sizeJ = jMax - jMin + 1;

    buffer_ = new unsigned int[sizeI*sizeJ];
    memset(buffer_, 0, (sizeI*sizeJ) * sizeof(int));

    transparent = QColor(Qt::transparent).rgba();

    for (k = 0; k < sizeI*sizeJ; k++)
    {
        buffer_[k] = transparent;
    }

    // Loop for all points
    //
    if (radius < 2.0) { // radius == 1
        for (k = 0; k < nrPoints; k++)
        {
            i = qx(xs[k]);
            j = qy(ys[k]);
            if (i >= 0 + 1 && i < sizeI - 1 && j >= 0 + 1 && j < sizeJ - 1)
            {
                // Draw a 'plus' symbol on screen.
                colour = QColor(rgb[k]).rgba();
                buffer_[i - 1 + j   *sizeI] = colour;
                buffer_[i + j   *sizeI] = colour;
                buffer_[i + 1 + j   *sizeI] = colour;
                buffer_[i + (j - 1)*sizeI] = colour;
                buffer_[i + (j + 1)*sizeI] = colour;
            }
        }
    }
    else if (radius < 3.0) { // radius == 2
        for (k = 0; k < nrPoints; k++)
        {
            i = qx(xs[k]);
            j = qy(ys[k]);
            if (i >= 0 + 1 && i < sizeI - 1 && j >= 0 + 1 && j < sizeJ - 1)
            {
                // Draw a '2x2 square' symbol on screen.
                colour = QColor(rgb[k]).rgba();
                buffer_[i + j   *sizeI] = colour;
                buffer_[i + 1 + j   *sizeI] = colour;
                buffer_[i + (j + 1)*sizeI] = colour;
                buffer_[i + 1 + (j + 1)*sizeI] = colour;
            }
        }
    }
    else if (radius < 4.0) { // radius == 3
        for (k = 0; k < nrPoints; k++)
        {
            i = qx(xs[k]);
            j = qy(ys[k]);
            if (i >= 0 + 1 && i < sizeI - 1 && j >= 0 + 1 && j < sizeJ - 1)
            {
                // Draw a '3x3 square' symbol on screen.
                colour = QColor(rgb[k]).rgba();
                buffer_[i - 1 + (j - 1)*sizeI] = colour;
                buffer_[i + (j - 1)*sizeI] = colour;
                buffer_[i + 1 + (j - 1)*sizeI] = colour;
                buffer_[i - 1 + j   *sizeI] = colour;
                buffer_[i + j   *sizeI] = colour;
                buffer_[i + 1 + j   *sizeI] = colour;
                buffer_[i - 1 + (j + 1)*sizeI] = colour;
                buffer_[i + (j + 1)*sizeI] = colour;
                buffer_[i + 1 + (j + 1)*sizeI] = colour;
            }
        }
    }
    mCache_painter->setCompositionMode(QPainter::CompositionMode_SourceOver);
    QImage* qimage = new QImage((unsigned char*)buffer_, sizeI, sizeJ, QImage::Format_ARGB32_Premultiplied);
    mCache_painter->drawImage(0, 0, *qimage);
    delete[] buffer_;
    delete qimage;
}
//-----------------------------------------------------------------------------
//
// Draw a polygon. The polygon gets a color according the routine setFillColor
void MyCanvas::drawPolygon(int nrPoints,
	double* xs/*[nrPoints]*/, double* ys/*[nrPoints]*/)
{
	QPolygon polygon;
    for (int i=0; i<nrPoints; i++)
    {
        polygon << QPoint(qx(xs[i]), qy(ys[i]));
    }
    mCache_painter->drawPolygon(polygon);
}
//
//-----------------------------------------------------------------------------
//
// Draw a polyline. This line width and line colour has to be set before this call
void MyCanvas::drawPolyline(int nrPoints,
    double* xs/*[nrPoints]*/ , double* ys/*[nrPoints]*/)
{
    assert( nrPoints != 0 );
    QPolygon polyline;
    for (int i=0; i<nrPoints; i++)
    {
        polyline << QPoint(qx(xs[i]), qy(ys[i]));
    }
    mCache_painter->drawPolyline(polyline);
}
//
//-----------------------------------------------------------------------------
//
void MyCanvas::drawRectangle(double x, double y, int width, int height)
{
    mCache_painter->drawRect( qx(x), qy(y), width, height);
}
//
//-----------------------------------------------------------------------------
//
// Draw the text. Textposition, font size and color has to bet set first by routines setFont.....
// and setTextAlignment()
void MyCanvas::drawText(double xleft, double ybottom, int width, int height, const char* text)
{
    QPen currentPen(mCache_painter->pen() );
    mCache_painter->setPen( textColour );

    mCache_painter->drawText( QPoint( qx(xleft), qy(ybottom) ), QString(text) );

    mCache_painter->setPen( currentPen );
}
//
//-----------------------------------------------------------------------------
//
void MyCanvas::setPointSize(int size)
{
    radius = double(size) /2.; // 2 is diameter/radius
    radius += 1.; // Empirical / correct for edge type "Qt::NoPen"
    if (size == 0) radius = 0.;
}
//
//-----------------------------------------------------------------------------
//
void MyCanvas::setLineWidth(int width)
{
    QPen currentPen(mCache_painter->pen() );
    currentPen.setWidth( width );
    mCache_painter->setPen( currentPen );
}
//
//-----------------------------------------------------------------------------
//
void MyCanvas::setLineColor(int rgb)
{
    QPen currentPen(mCache_painter->pen() );
    currentPen.setColor( rgb );
    mCache_painter->setPen( currentPen );
}
//
//-----------------------------------------------------------------------------
//
void MyCanvas::setLineStyle(LineStyle style)
{
    QPen currentPen(mCache_painter->pen());
    currentPen.setStyle((enum Qt::PenStyle) style);
    mCache_painter->setPen(currentPen);
}
//
//-----------------------------------------------------------------------------
//
void MyCanvas::setFillColor(int rgb)
{
    if (rgb == 0) {
        mCache_painter->setBrush(Qt::NoBrush );
    } else {
		QBrush * qbr =  new QBrush( QColor( rgb ) );
        mCache_painter->setBrush(*qbr);
    }
}
//
//-----------------------------------------------------------------------------
//
void MyCanvas::setFontName(const char* name)
{
    assert(mCache_painter); // True only inside a paintEvent

    // The mCache_painter gets a new font.
    mCache_painter->setFont( QFont(name) );
}
//
//-----------------------------------------------------------------------------
//
void MyCanvas::setFontColor(int rgb)
{
    textColour.setRgb( QRgb( rgb ) );
}
//
//-----------------------------------------------------------------------------
//
void MyCanvas::setFontPointSize(int size)
{
    QFont currentFont(mCache_painter->font() );
    currentFont.setPointSize( size );
    mCache_painter->setFont( currentFont );
}
//
//-----------------------------------------------------------------------------
//
void MyCanvas::setFontItalic(bool value)
{
    QFont currentFont(mCache_painter->font() );
    currentFont.setItalic( value );
    mCache_painter->setFont( currentFont );
}
//
//-----------------------------------------------------------------------------
//
void MyCanvas::setFontBold(bool value)
{
    //=======om te testen met pixmap================
    //buffer0->load( QString("D:\\NL-Zuidwest.bmp") , "BMP" , 0 ) ;
    //buffer0->save( QString("D:\\QtPixmap.jpg") , "JPEG" ) ;

    //=====dit is de originele code
    //QFont newfont( mCache_painter->font() );
    //newfont.setBold( isBold );
    //mCache_painter->setFont( newfont );
}
//
//-----------------------------------------------------------------------------
//
void MyCanvas::setFontUnderline(bool value)
{
    QFont currentFont(mCache_painter->font() );
    currentFont.setUnderline( value );
    mCache_painter->setFont( currentFont );
}
//
//-----------------------------------------------------------------------------
//
void MyCanvas::setTextAlignment(Alignment value)
{
//TODO implementation
}
//
//-----------------------------------------------------------------------------
//
bool MyCanvas::isFontAvailable(const char* name)
{
    return (bool) true; //TODO implementation
}
//
//-----------------------------------------------------------------------------
//
int MyCanvas::getTextWidth(const char* name)
{
    int size = (mMapCanvas->fontMetrics()).horizontalAdvance(name);
    return size;
}
//
//-----------------------------------------------------------------------------
//
int MyCanvas::getTextHeight(const char* name, int maxWidth)
{
    int size = (mMapCanvas->fontMetrics()).height();
    return size;
}
//
//-----------------------------------------------------------------------------
//
double MyCanvas::getPixelWidth(double x, double y)
{
   return dx; // height of one pixel in world co-ordinates
}
//
//-----------------------------------------------------------------------------
//
double MyCanvas::getPixelHeight(double x, double y)
{
   return dy; // height of one pixel in world co-ordinates
}
//
//-----------------------------------------------------------------------------
//
double MyCanvas::getMinX()
{
   return min_X;
}
//
//-----------------------------------------------------------------------------
//
double MyCanvas::getMaxX()
{
   return max_X;
}
//
//-----------------------------------------------------------------------------
//
double MyCanvas::getMinY()
{
   return min_Y;
}
//
//-----------------------------------------------------------------------------
//
double MyCanvas::getMaxY()
{
   return max_Y;
}
//
//-----------------------------------------------------------------------------
//
double MyCanvas::getMinVisibleX()
{
    QgsRectangle extent = mMapCanvas->extent();
    return extent.xMinimum();
}
//
//-----------------------------------------------------------------------------
//
double MyCanvas::getMaxVisibleX()
{
    QgsRectangle extent = mMapCanvas->extent();
    return extent.xMaximum();
}
//
//-----------------------------------------------------------------------------
//
double MyCanvas::getMinVisibleY()
{
    QgsRectangle extent = mMapCanvas->extent();
    return extent.yMinimum();
}
//
//-----------------------------------------------------------------------------
//
double MyCanvas::getMaxVisibleY()
{
    QgsRectangle extent = mMapCanvas->extent();
    return extent.yMaximum();
}
//
//-----------------------------------------------------------------------------
//
int MyCanvas::getWidth()
{
    QRect frame = mMapCanvas->frameRect();
    return frame.width();
}
//
//-----------------------------------------------------------------------------
//
int MyCanvas::getHeight()
{
    QRect frame = mMapCanvas->frameRect();
    return frame.height();
}
//
//-----------------------------------------------------------------------------
//
int MyCanvas::getPixelXFromXY(double x, double y)
{
    return qx((double) x);
}
//
//-----------------------------------------------------------------------------
//
int MyCanvas::getPixelYFromXY(double x, double y)
{
   return qy((double) y);
}
//
//-----------------------------------------------------------------------------
//
double MyCanvas::getXFromPixel(int pixelX, int pixelY)
{
    return wx(pixelX);
}
//
//-----------------------------------------------------------------------------
//
double MyCanvas::getYFromPixel(int pixelX, int pixelY)
{
    return wy(pixelY);
}
//
//-----------------------------------------------------------------------------
//
int MyCanvas::getPixelColor(double x, double y)
{
    return cacheArray[drawCache]->pixel((int)wx(x), (int)wy(y));
}
//
//-----------------------------------------------------------------------------
//
void MyCanvas::setFullExtend(double minX, double maxX, double minY, double maxY)
{
    // Full exent is set by the MFE (ie QGIS, ArcGIS, ...)
}
//
//-----------------------------------------------------------------------------
//
void MyCanvas::zoomToExtend(double minX, double maxX, double minY, double maxY)
{
    // Zoom to exent is done by the MFE (ie QGIS, ArcGIS, ...)
}

//
//-----------------------------------------------------------------------------
// ********* Events a widget must handle or forward to plugin **********
//-----------------------------------------------------------------------------
//
//

//bool MyCanvas::canvasToolTipEvent(QHelpEvent* we)
//{
//    QMessageBox::warning(0, "Message", QString(tr("MyCanvas::canvasToolTipEvent(QHelpEvent* we)")));
//    return false;
//}


void MyCanvas::MyWheelEvent ( QWheelEvent * we )
{
    QMessageBox::warning( 0, "Message", QString(tr("MyCanvas::MyWheelEvent")));
    mMapCanvas->update();
}

//
//-----------------------------------------------------------------------------
//
void MyCanvas::MyMouseDoubleClickEvent( QMouseEvent * me)
{
    QMessageBox::warning( 0, "Message", QString(tr("MyCanvas::mouseDoubleClickEvent: %1").arg(me->button())));
}
//
//-----------------------------------------------------------------------------
//
void MyCanvas::MyMouseMoveEvent      ( QMouseEvent * me)
{
    if (listener != NULL)
    {
        //QMessageBox::warning(0, "Message", QString(tr("MyCanvas::MyMouseMoveEvent: %1").arg(me->button())));
        listener->onMouseMove(wx(me->x()), wy(me->y()), (AbstractCanvasListener::ButtonState) me->button() );
    }
}
//
//-----------------------------------------------------------------------------
//
void MyCanvas::MyMousePressEvent     ( QMouseEvent * me)
{
    if (listener != NULL)
    {
        listener->onMouseDown(wx(me->x()), wy(me->y()), (AbstractCanvasListener::ButtonState) me->button() );
    }
}
//
//-----------------------------------------------------------------------------
//
void MyCanvas::MyMouseReleaseEvent   (QgsMapMouseEvent * me)
{
    if (listener != NULL)
    {
        listener->onMouseUp(wx(me->x()), wy(me->y()), (AbstractCanvasListener::ButtonState) me->button() );
    }
}
//
//-----------------------------------------------------------------------------
//
void MyCanvas::MyKeyPressEvent( QKeyEvent* ke)
{
    if (ke->modifiers() & Qt::ShiftModifier)
    {
        int pressed_key = ke->key() & Qt::ShiftModifier;
        {
            QMessageBox::warning(0, "Message", QString(tr("MyCanvas::MyKeyPressEvent: %1").arg(ke->key())));
        }
    }

    if (listener != NULL)
    {
		listener->onKeyDown((AbstractCanvasListener::KeyCode) ke->key(), (AbstractCanvasListener::KeyboardModifier) int(ke->modifiers()));
        ke->accept();
    }
}
//
//-----------------------------------------------------------------------------
//
void MyCanvas::MyKeyReleasedEvent( QKeyEvent * ke)
{
    if (listener != NULL)
    {
        listener->onKeyUp((AbstractCanvasListener::KeyCode) ke->key(), (AbstractCanvasListener::KeyboardModifier) int(ke->modifiers()));
        ke->accept();
    }
}

//
//=============================================================================
//
void MyCanvas::checkBuffer0()
{
}
void MyCanvas::myBitBlt( int srcIndx, int destIndx)
{
}
void MyCanvas:: bitBlt( QPixmap* dst, int x, int y, const QPixmap* src )
{
}


void MyCanvas::InitDrawEachCaches(void) // debug utility
{
    /* Draw each cache separately */
    vb = new QVBoxLayout(mMapCanvas);
    for (int i = 0; i<NR_CACHES; i++) {
        label[i] = new QLabel(mMapCanvas);
        label[i]->setScaledContents(true);
        newCache(i);
    }
    for (int i = 0; i<NR_CACHES; i++) {
        if (i == 0) cacheArray[i]->fill(Qt::yellow);
        if (i == 1) cacheArray[i]->fill(Qt::yellow);
        if (i == 2) cacheArray[i]->fill(Qt::yellow);
        if (i == 3) cacheArray[i]->fill(Qt::yellow);
        if (i == 4) cacheArray[i]->fill(Qt::yellow);
        if (i == 5) cacheArray[i]->fill(Qt::yellow);
        //if (i==6) cacheArray[i]->fill(Qt::green); // already set in function newcache
        label[i]->setPixmap(QPixmap::fromImage(*cacheArray[i])); // consider QGraphicsScene
        label[i]->setFrameStyle(QFrame::Panel);
        vb->addWidget(label[i]);
    }

    char * title = (char *)malloc(260);
    //  sprintf(title, "%s %d %s %d", "Source: ", sourceCacheIndex,"    Destination: ", destCacheIndex);
    w = new QWidget(mMapCanvas);
    w->setWindowTitle("Pixmaps");
    w->setLayout(vb);
    w->setMaximumWidth(250);
    w->setMaximumHeight(850);
    w->show();
}
//-----------------------------------------------------------------------------

void MyCanvas::DrawEachCaches(void) // debug utility
{
    // Draw each cache separately on the canvas
    for (int i = 0; i<NR_CACHES; i++)
    {
        vb->removeWidget(label[i]);
        label[i]->setPixmap(QPixmap::fromImage(*cacheArray[i])); // consider QGraphicsView
        vb->addWidget(label[i]);
    }
    w->setLayout(vb);
    w->update();
}
