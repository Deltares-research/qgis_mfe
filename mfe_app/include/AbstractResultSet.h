// RUN MLSupportCppHParser.exe
// corresponding multi language support files are generated

#ifndef _INC_ABSTRACTRESULTSET
#define _INC_ABSTRACTRESULTSET



class AbstractResultSet {
    public:

    enum SqlType {
        BIT_FIELD             =  -7,
        TINYINT_FIELD         =  -6,
        SMALLINT_FIELD        =   5,
        INTEGER_FIELD         =   4,
        BIGINT_FIELD         =  -5,
        FLOAT_FIELD         =   6,
        REAL_FIELD             =   7,
        DOUBLE_FIELD         =   8,
        NUMERIC_FIELD         =   2,
        DECIMAL_FIELD        =   3,
        CHAR_FIELD            =   1,
        VARCHAR_FIELD         =  12,
        LONGVARCHAR_FIELD   =  -1,
        DATE_FIELD             =  91,
        TIME_FIELD             =  92,
        TIMESTAMP_FIELD     =  93,
        BINARY_FIELD        =  -2,
        VARBINARY_FIELD     =  -3,
        LONGVARBINARY_FIELD =  -4,
        BOOLEAN_FIELD       =  16,
        OTHER_FIELD            = 1111
    };

    enum ShapeType {
        POINT  =  0,
        LINE  = 1,
        POLYGON = 2,
        NONE = 3
    };




    virtual ~AbstractResultSet() {};

    virtual bool isExceptionOccured() = 0;
    virtual const char* getLastExceptionMessage() = 0;



    /**
     * Moves the cursor down one row from its current position.
     * A <code>ResultSet</code> cursor is initially positioned
     * before the first row; the first call to the method
     * <code>next</code> makes the first row the current row; the
     * second call makes the second row the current row, and so on.
     *
     * <P>If an input stream is open for the current row, a call
     * to the method <code>next</code> will
     * implicitly close it. A <code>ResultSet</code> object's
     * warning chain is cleared when a new row is read.
     *
     * @return <code>true</code> if the new current row is valid;
     * <code>false</code> if there are no more rows
     * @exception SQLException if a database access error occurs
     */
    virtual bool goToNext() = 0;

     /**
     * Retrieves the shape type connected to the current row
     * of this <code>ResultSet</code>
     */
    virtual ShapeType getShapeType() = 0;

     /**
     * Retrieves the number of points in the shape connected to the current row
     * of this <code>ResultSet</code>
     */
    virtual int getPointCount() = 0;

     /**
     * Retrieves the x and y coordinates of all the points in the shape connected to the current row
     * of this <code>ResultSet</code>
     */
    virtual bool getCoors(int bufsize, double* xs/*[bufsize]*/, double* ys/*[bufsize]*/) = 0;

    // Maps the given ResultSet column name to its ResultSet column index.
    virtual int findColumn(const char* columnName) = 0;

    // Returns the number of columns in this ResultSet object for the current row
    virtual int getColumnCount() = 0;


    // Indicates the designated column's normal maximum width in characters
    virtual int getColumnDisplaySize() = 0;

    // Get the designated column's type.
    virtual SqlType getColumnType(int columnType) = 0;

    /**
     * Get the designated column's name.
     *
     * @param column the first column is 1, the second is 2, ...
     * @return column name
     * @exception SQLException if a database access error occurs
     */
    virtual const char* getColumnName(int column) = 0;


    // Return false if it is needed to call findColumn, getColumnCount, getColumnType and getColumnName again to
    // get the actual column information for the current row
    virtual bool hasSameColumnsAsPreviousRow() = 0;


    // Retrieves the value of the designated column in the current row.
    // It is a Date column and the date is given as a string.
    // the date format is given according Visual Basic rules of the VB Format function
    // e.g. "yyyy-mmm-dd"  ; dd-mm-yyyy" ; "yyyy/mm/dd hh:mm:ss"
    // If time included in the format, the time was given back, at least as 00:00:00
    virtual const char* getDate(int columnIndex, const char* dateFormat) = 0;


    /**
     * Retrieves the value of the designated column in the current row
     * of this <code>ResultSet</code> object as
     * a <code>String</code> in the Java programming language.
     *
     * @param columnIndex the first column is 1, the second is 2, ...
     * @return the column value; if the value is SQL <code>NULL</code>, the
     * value returned is <code>null</code>
     * @exception SQLException if a database access error occurs
     */
    virtual const char* getString(int columnIndex) = 0;

    /**
     * Retrieves the value of the designated column in the current row
     * of this <code>ResultSet</code> object as
     * a <code>boolean</code> in the Java programming language.
     *
     * @param columnIndex the first column is 1, the second is 2, ...
     * @return the column value; if the value is SQL <code>NULL</code>, the
     * value returned is <code>false</code>
     * @exception SQLException if a database access error occurs
     */
    virtual bool getBoolean(int columnIndex) = 0;

    /**
     * Retrieves the value of the designated column in the current row
     * of this <code>ResultSet</code> object as
     * a <code>byte</code> in the Java programming language.
     *
     * @param columnIndex the first column is 1, the second is 2, ...
     * @return the column value; if the value is SQL <code>NULL</code>, the
     * value returned is <code>0</code>
     * @exception SQLException if a database access error occurs
     */
    virtual int getByte(int columnIndex) = 0;

    /**
     * Retrieves the value of the designated column in the current row
     * of this <code>ResultSet</code> object as
     * an <code>int</code> in the Java programming language.
     *
     * @param columnIndex the first column is 1, the second is 2, ...
     * @return the column value; if the value is SQL <code>NULL</code>, the
     * value returned is <code>0</code>
     * @exception SQLException if a database access error occurs
     */
    virtual int getInt(int columnIndex) = 0;

    /**
     * Retrieves the value of the designated column in the current row
     * of this <code>ResultSet</code> object as
     * a <code>float</code> in the Java programming language.
     *
     * @param columnIndex the first column is 1, the second is 2, ...
     * @return the column value; if the value is SQL <code>NULL</code>, the
     * value returned is <code>0</code>
     * @exception SQLException if a database access error occurs
     */
    virtual float getFloat(int columnIndex) = 0;

    /**
     * Reports whether
     * the last column read had a value of SQL <code>NULL</code>.
     * Note that you must first call one of the getter methods
     * on a column to try to read its value and then call
     * the method <code>wasNull</code> to see if the value read was
     * SQL <code>NULL</code>.
     *
     * @return <code>true</code> if the last column value read was SQL
     *         <code>NULL</code> and <code>false</code> otherwise
     * @exception SQLException if a database access error occurs
     */

    virtual double getDouble(int columnIndex) = 0;

    // Get a quantity, compute from a shape/feature in the resultSet.
    // Use the key, e.g. "area"
    virtual double getQuantity(const char* key) = 0;

    virtual bool wasNull() = 0;

};

#endif  /* _INC_ABSTRACTRESULTSET */