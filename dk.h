#ifndef __DKSHP__
#define __DKSHP__

#include <cmath>
#include <float.h>
#include <iostream>
#include <string>

using std::endl;
using std::ostream;
using std::iostream;

/*******************************************************************************
 * Structure for expressing geospatial coordinates
 ******************************************************************************/
class Point
{
  public:

     int idx;

  double x,
         y,
         z;

  /**
   * Default constructor
   */
  Point(double pX = 0, double pY = 0, double pZ = 0)
  {
    x = pX;
    y = pY;
    z = pZ;
  };

  /**
   *
   */
  double distance(const Point & pStart)
  {
    double dx = x - pStart.x,
           dy = y - pStart.y,
           d  = sqrt((dx*dx) + (dy*dy));

    return d;
  };

  /**
   * Output operator
   */
   friend ostream & operator<< (ostream & o, Point & p)
   {
     o << "(" << p.x << ", " << p.y << ", " << p.z << ")" << endl;

     return o;
   };

};

#endif//__DKSHP__
