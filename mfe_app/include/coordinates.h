#ifndef COORDINATES_H_INCLUDED
#define COORDINATES_H_INCLUDED

#include <utility>
#include <math.h>

typedef double coordinate;
typedef std::pair<double,double> coordinates;
typedef double distance;
typedef double distance_sq; // Squared distance. Useful in searching minimum distances

inline distance_sq squared_distance( coordinates const& P,
                                     coordinates const& Q )
{
    coordinate dx = P.first - Q.first ;
    coordinate dy = P.second - Q.second ;
    return dx*dx + dy*dy;
}
// calculate the cosine of angle abc
inline double cos( coordinates a, coordinates b, coordinates c )
{
    double dot_product = (b.first-a.first)*(b.first-c.first) + (b.second-a.second)*(b.second-c.second);
    return dot_product / sqrt( squared_distance( a,b )* squared_distance( b, c ) );
}
#endif // COORDINATES_H_INCLUDED
