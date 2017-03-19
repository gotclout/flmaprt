#ifndef __Vertex__
#define __Vertex__

#define NIL NULL

#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <climits>
#include <cmath>
#include <float.h>

using std::map;
using std::vector;
using std::list;
using std::string;
using std::iostream;
using std::ostream;
using std::endl;

/** Vertex Color Enumeration **/
enum EColor
{
  eWhite = 0,
  eGray,
  eBlack,
  eUndefined
};

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
   * Dist
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

/*******************************************************************************
 * Structure for expressing graph vertex
 ******************************************************************************/
struct Vertex
{

  typedef list<Vertex*>     AdjList;
  typedef AdjList::iterator AdjListIt;

  AdjList* adj;   //adjacent verticies
  Vertex*  pi;    //parent
  EColor   color; //state
  int      d,     //discovery time
           f,     //finish time
           l,     //lowest
           mcap;  //min capacity of path to vertex
  double  maxcap,
          mincap;
  string   id;    //uid
  static int ic;
  int aid;
  Point p;
  bool visited;

  /**
   * Default construct
   */
  Vertex(int ppid = -1)
  {
    adj   = 0;
    pi    = 0;
    color = eUndefined;
    d     = l = f = -1;
    id    = "DEFAULT";
    mcap  = INT_MAX;
      aid = ppid;
    if(ppid == -1)
    {
      aid++ ;
    }

  };

  /**
   * Construct from id
   *
   * @param string pId is the unique identifier for this vertex
   */
  Vertex(string pId, int ppid = -1)
  {
    adj   = 0;
    pi    = 0;
    color = eUndefined;
    id    = pId;
    d     = l = f = -1;
    mcap  = INT_MAX;
      aid = ppid;
    if(ppid == -1)
    {
      aid++ ;
    }
  };

  /**
   *
   */
  size_t num_adj() { return adj ? adj->size() : 0; }

  /**
   * Adds a vertex to this vertexes adjaceny list
   *
   * @param Vertex* v is the vertex to be added
   * @return bool true if the vertex is successfully added, false otherwise
   */
  bool add_adj(Vertex* & v)
  {
    bool ret = true;

    if(v) adj->push_back(v);
    else ret = false;

    return ret;
  };

  /**
   * Remove the specified vertex from this vertexes adjaceny list
   *
   * @param Vertex* v is the vertex to be removed
   * @return bool true if the vertex is successfully removed, false otherwise
   */
  bool remove_adj(Vertex* & v)
  {
    bool ret = false;

    if(adj)
    {
      AdjListIt it = adj->begin();

      while(it != adj->end() && !ret)
      {
        if(*it == v)
        {
          adj->erase(it);
          ret = true;
        }
      }
    };

    return ret;
  }

  /**
   *
   */
  double distance(const Point & pStart)
  {
    double dx = p.x - pStart.x,
           dy = p.y - pStart.y,
           d  = sqrt((dx*dx) + (dy*dy));

    return d;
  }

  /**
   * TODO: Copy/Assignment
   */
  Vertex& operator=(const Vertex & rhs)
  {
    if(this != &rhs)
    {
      id = rhs.id;
      adj = new AdjList;
      if(rhs.adj) *(adj = rhs.adj);
      aid = rhs.aid;
      pi = rhs.pi;
      color = rhs.color;
      p = rhs.p;
      maxcap = rhs.maxcap;
      mincap = rhs.mincap;
      mcap   = rhs.mcap;
    }

    return *this;
  }

  Vertex(const Vertex & src)
  {
    id = src.id;
    adj = new AdjList;
    if(src.adj) *(adj = src.adj);
    aid = src.aid;
    pi = src.pi;
    color = src.color;
    p = src.p;
    maxcap = src.maxcap;
    mincap = src.mincap;
    mcap   = src.mcap;
  }

  /**
   * Equivalence operator overload
   * 
   * @param Vertex rhs is a const reference to the Vertex to be compared to 
   *  this vertex
   * @return bool true if the id of this Vertex is equivalent to the id
   *  of the rhs Vertex id
   */
  bool operator==(const Vertex & rhs) const
  {
    return id == rhs.id;
  };

  /**
   * Inequivalence operator overload
   * 
   * @param Vertex rhs is a const reference to the Vertex to be compared to
   *  this vertex
   * @return bool true if the id of this Vertex is not equivalent to the id
   *  of the rhs Vertex id
   */
  bool operator !=(const Vertex & rhs) const {return !(*this == rhs); };

  /**
   * Less than operator overload for container comparators
   *
   * @param Vertex is the right hand side vertex
   */
  bool operator<(const Vertex & rhs) const { return id < rhs.id; };

  /**
   * Converts EColour enum to string
   */
  string get_color(const EColor & c)
  {
    string ret = "UNDEFINED";
    switch(c)
    {
      case eWhite: ret = "WHITE";
      case eGray:  ret = "GRAY";
      case eBlack: ret = "BLACK";
      default    :              ; //Undefined
    }

    return ret;
  };

  /**
   * Renders a vertex
   *
   * @param ostream outputstream for rendering
   * @param Vertex is the vertex to be rendered
   */
  friend ostream& operator<< (ostream & o, const Vertex & pV)
  {
    Vertex v = (Vertex) pV;
    return operator<<(o, v);
  };

  friend ostream& operator<< (ostream & o, Vertex & v)
  {
    size_t sz = v.num_adj();

    if(sz > 0)
    {
      o << v.id << " Adjacent to Verticies: ";
      AdjListIt it = v.adj->begin();
      for(size_t i = 0; it != v.adj->end(); ++it, ++i)
      {
        o << (*it)->id;
        if(i+1 != sz) o << ", ";
      }
    }
    else o << "Vertex: " << v.id << " Has No Adjacent Verticies";

    o << endl;

    if(v.pi)    o << "                  pi[" << v.id << "]:" << v.pi->id;
    else        o << "                  pi[" << v.id << "]:" << "NIL";
    if(v.d < 0) o << ", d[" << v.id << "]:"  << "UNDEFINED";
    else        o << ", d[" << v.id << "]:"  << v.d;
    if(v.f < 0) o << ", f[" << v.id << "]:"  << "UNDEFINED";
    else        o << ", f[" << v.id << "]:"  << v.f;
                o << ", Color:"     << v.get_color(v.color);
    o << "p(x, y, z): " << v.p;

    return o;
  };
};
int Vertex::ic = 0;
#endif//__Vertex__
