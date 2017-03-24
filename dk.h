#ifndef __DKSHP__
#define __DKSHP__

#include <iostream>
#include <string>
#include <set>
#include <queue>
#include <map>
#include <sstream>
#include <string>
#include <fstream>
#include <algorithm> //heapify
#include <float.h>

#include "Graph.h"

#define extract_min(q, u) { u = q.top().v; q.pop(); }

class VertexEntry
{
  public:

  Vertex* v;
  //static int idc;

  VertexEntry()
  {
    v = 0;
  }

  VertexEntry(const Vertex* pV)
  {
    v = (Vertex*) &(*pV);
  }

  /**
   * Shallow copy ...
   */
  VertexEntry& operator=(const VertexEntry & rhs)
  {
    if(this != &rhs)
    {
      //if(rhs.v){ v = new Vertex(*rhs.v); }
      v = (Vertex*) &(*rhs.v);
    }
    return *this;
  }

  bool operator==(const VertexEntry & rhs)
  {
    return *v == *(rhs.v);
  }

  bool operator<(const VertexEntry & rhs)
  {
    return v->d < rhs.v->d;
  }

  bool operator>(const VertexEntry & rhs)
  {
    //return !(*this < rhs);
    return v->d > rhs.v->d;
  }

};

//
//
bool operator==(VertexEntry lhs, VertexEntry rhs) { return *lhs.v == *rhs.v;    };
//
bool operator>(VertexEntry lhs, VertexEntry rhs)  { return lhs.v->d > rhs.v->d; };
//
bool operator<(VertexEntry lhs, VertexEntry rhs)  { return lhs.v->d < rhs.v->d; };
//
bool mcomp(VertexEntry lhs, VertexEntry rhs)      { return lhs.v->d > rhs.v->d; };

//
typedef priority_queue<VertexEntry, vector<VertexEntry>, greater<VertexEntry> > MinQueue;

#endif//__DKSHP__
