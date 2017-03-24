#ifndef __Graph__
#define __Graph__

#include <sstream>
#include <set>
#include <queue>
#include <limits>

#include "Vertex.h"
#include "FifoQueue.h"

using std::set;

/** Typedef for Vertex containers **/
typedef list<Vertex*>     AdjList;
typedef AdjList::iterator AdjListIt;

static int gt = 0;
/*******************************************************************************
 * Structure expressing an Adjacency Matrix for a graph G TODO:
 ******************************************************************************/
class AdjMatrix
{
  public:

  int n;      //
  Vertex** A; //

  /**
   * Default Constructor
   */
  AdjMatrix(int n)
  {
    A = new Vertex*[n];
    for(int i = 0; i < n; ++i)
      A[i] = new Vertex[n];
  };

  /**
   * Retrieves the vertex at A[i][j]
   */
  Vertex* get(int i, int j){ return &A[i][j];};

  void put(int i, int j, Vertex* v)
  {
    A[i][j] = *v;
  };

  /**
   * Retrieves the verticies at A[i]
   */
  Vertex* allfrom(int i)
  {
    Vertex* r = new Vertex[n];
    for(int idx = 0; idx < n; i++)
      r[idx] = A[i][idx];

    return r;
  };

  /**
   * Destructor
   */
  ~AdjMatrix()
  {
    if(A)
    {
      for(int i = 0; i < n; ++i)
      {
        if(A[i]) delete [] A[i];
      }
      delete [] A;
    }
  }
};

/*******************************************************************************
 * Structure expressing graph edges
 ******************************************************************************/
class Edge
{
  public:

  Vertex* u,       // start vertex
        * v;       // end vertex
  string  id;      // uid
  double  flow,    // flow of the current edge
          cap;     // capacity of the edge

  /**
   * Default construct
   */
  Edge() { u = v = 0; flow = 0; cap = 0; id = ""; };

  /**
   * Construct from verticies
   *
   * @param Vertex* pV1 start vertex, pV2 end vertex
   * @param Vertex* pV2 end vertex
   */
  Edge(Vertex* & pV1, Vertex* & pV2)
  {
    u = &(*pV1);
    v = &(*pV2);
    id = u->id + v->id;
    flow = cap = 0;
  };

  /**
   * Construct from vertex pointer and value
   *
   * @param Vertex* pV1 is the source vertex of edge e
   * @param Vertex* pV2 is the destination vertex of edge e
   * @param int pCap is the capacity of edge e
   */
  Edge(Vertex* pV1, Vertex* pV2, double pCap)
  {
    u = &(*pV1);
    v = &(*pV2);
    cap = pCap;
    flow = 0;
    id = u->id + v->id;
  };

  /**
   * Less than operator overload
   *
   * @param rhs is the edge to be compared
   * @return true if the id of this edge < rhs false otherwise
   */
  bool operator<(const Edge & rhs) const
  {
    return id < rhs.id;
  };

  /**
   * Equivalence operator
   */
  bool operator==(const Edge & rhs) const
  {
    return id == rhs.id;
  }

  /**
   * Inequality operator
   */
  bool operator!=(const Edge & rhs) const
  {
    return id != rhs.id;
  }

  /**
   * Retrieves the residual value of the edge
   *
   * @return int is the value of capacity - flow
   */
  double residual() { return cap - flow; };

  /**
   * Renders an edge
   */
  friend ostream& operator <<(ostream & o, Edge & e)
  {
    if(e.u && e.v)
      o << "Edge: [" << e.u->id << ", " << e.v->id << "]\n";
    return o;
  };
};

/** Typedefs for Vertex Containers **/
typedef map<Vertex, AdjList>  VertexMap;
typedef VertexMap::iterator   VertexMapIt;
typedef VertexMap::value_type VertexMapType;

/** Typedefs for Edge Containers **/
typedef vector<Edge>  EdgeVector;
typedef EdgeVector::iterator EdgeVectorIt;
typedef vector<Edge*> EdgePtrVector;
typedef EdgePtrVector::iterator EdgePtrVectorIt;
typedef pair<Vertex, double> dpair;

/*******************************************************************************
 * Structure for representing a graph G = (V, E)
 ******************************************************************************/
class Graph
{
  public:

  EdgeVector E;   //Collection of Edges
  VertexMap VE;   //Maps a vertex to a list of adjacent verticies
  bool directed,  //indicates whether G is a directed or undirected graph
       weighted,  //
       connected;

  /**
   * Retrieves the number of verticies |V|
   */
  size_t vsize() { return VE.size(); };

  /**
   * Retrieves the number of edges |E|
   */
  size_t esize() { return E.size(); };

  /**
   * Default construct
   *
   * @param bool pDirected is true if G is directed, false otherwise
   */
  //Graph(bool d) { directed = d; weighted = false; };
  Graph(bool pDirected = false, bool pWeighted = false)
  {
    directed = pDirected;
    weighted = pWeighted;
  };

  /**
   * Retrieves a pointer to the vertex v in G
   *
   * @param Vertex v is the vertex to be retrieved
   */
  Vertex* get_vertex(Vertex v)
  {
    return VE.find(v) == VE.end() ? 0 : (Vertex*) &VE.find(v)->first;
  };

  /**
   * Sets the parent value of all verticies to nil
   */
  void nilpi()
  {
    for(VertexMapIt i = VE.begin(); i != VE.end(); ++i)
    {
      Vertex* vptr = (Vertex*) &i->first;
      vptr->pi = NIL;
    }
  };

  /**
   *
   */
  void init_single_src(Vertex* s)
  {
    Vertex* v;

    for(VertexMapIt i = VE.begin(); i != VE.end(); ++i)
    {
      v     = (Vertex*) &i->first;
      v->d  = std::numeric_limits<double>::infinity();
      v->pi = NIL;
    }

    s->d = 0;
  };

  /**
   *
   */
  void init(Vertex* v = NIL)
  {
    for(VertexMapIt i = VE.begin(); i != VE.end(); ++i)
    {
      Vertex* vptr = (Vertex*) &i->first;
      vptr->pi      = NIL;
      vptr->visited = false;
      vptr->color   = eWhite;
    }
    if(v) v->d = 0; //if single source
  };

  /**
   *
   */
  bool add_vertex(Vertex & vtx)
  {
    bool rval = true;

    VertexMapIt vit = VE.find(Vertex(vtx.id)),
                beg = VE.begin();
    if(vit == VE.end())
    {
      vit = VE.insert(beg, VertexMapType(vtx, AdjList()));
    }
    else
    {
      rval = false;
    }

    return rval;
  };

  /**
   *
   */
  void add_edge(Vertex* u, Vertex* v, double w = -1)
  {
    return add_edge(*u, *v, w);
  }
  void add_edge(Vertex & u, Vertex & v, double w = -1)
  {
    VertexMapIt uit, vit, beg = VE.begin();
    Vertex* uvt, //u vertex ptr
           *vvt; //v vertex ptr

    uit = VE.find(Vertex(u.id));
    if(uit == VE.end())
      uit = VE.insert(beg, VertexMapType(u, AdjList()));
    uvt = (Vertex*)&(uit->first);

    if(!uvt->adj) uvt->adj = (AdjList*)& uit->second;

    vit = VE.find(Vertex(v.id));
    if(vit == VE.end())
      vit = VE.insert(beg, VertexMapType(v, AdjList()));
    vvt = (Vertex*)&(vit->first);

    if(!vvt->adj) vvt->adj = (AdjList*)& vit->second;

    uvt->add_adj(vvt);                      //add uv by default
    E.push_back(Edge(&(*uvt), &(*vvt), w));

    if(!directed)                           //add vu if undirected
    {
      vvt->add_adj(uvt);
      E.push_back(Edge(&(*vvt), &(*uvt), w));
    }
  };

  /**
   * Adds an edge to this graph
   *
   * @param int u is the identifier for the first vertex
   * @param int v is the identifier for the second vertex
   */
  void add_edge(string u, string v, double w = -1)
  {
    VertexMapIt uit, vit, beg = VE.begin();
    Vertex* uvt, //u vertex ptr
           *vvt; //v vertex ptr

    uit = VE.find(Vertex(u));
    if(uit == VE.end())
      uit = VE.insert(beg, VertexMapType(Vertex(u), AdjList()));
    uvt = (Vertex*)&(uit->first);

    if(!uvt->adj) uvt->adj = (AdjList*)& uit->second;

    vit = VE.find(Vertex(v));
    if(vit == VE.end())
      vit = VE.insert(beg, VertexMapType(Vertex(v), AdjList()));
    vvt = (Vertex*)&(vit->first);

    if(!vvt->adj) vvt->adj = (AdjList*)& vit->second;

    uvt->add_adj(vvt);                      //add uv by default
    E.push_back(Edge(&(*uvt), &(*vvt), w));

    if(!directed)                           //add vu if undirected
    {
      vvt->add_adj(uvt);
      E.push_back(Edge(&(*vvt), &(*uvt), w));
    }
  };

  /**
   * Updates an edge flow and its reverse edge by m
   */
  void update_edge(Vertex u, Vertex v, double m)
  {
    for(size_t i = 0; i < E.size(); ++i)
    {
      if(E[i].u->id == u.id && E[i].v->id == v.id)E[i].flow += m;
      else if(E[i].u->id == v.id &&  E[i].v->id == u.id) E[i].cap -= m;
    }
  };

  /**
   * Updates an edge flow and its reverse edge by m
   */
  void update_edge2(Vertex u, Vertex v, int txp, int rxp)
  {
    for(size_t i = 0; i < E.size(); ++i)
    {
      if(E[i].u->id == u.id && E[i].v->id == v.id)E[i].flow += txp;//no no
      if(E[i].u->id == v.id &&  E[i].v->id == u.id) E[i].cap -= rxp;
    }
  };

  /**
   * Relaxes vertex
   *
   * @param Vertex u
   * @param Vertex v
   * @param int w
   * @return bool
   */
  bool relax(Vertex* & u, Vertex* & v, double w)
  {
    bool ret = false;

    if(weighted)
    {
      if(u->d + w < v->d)
      {
        v->d = u->d + w;
        v->pi = u;
        ret = true;
      }
    }

    return ret;
  }

  bool relax(Vertex & u, Vertex & v, double w)
  {
    bool ret = false;

    if(weighted)
    {
      if(u.d + w < v.d)
      {
        v.d = u.d + w;
        v.pi = &u;
        ret = true;
      }
    }

    return ret;
  }

  /**
   * Retrieves the edges connecting v to e in G
   */
  EdgePtrVector adjacent_edges(Vertex v)
  {
    EdgePtrVector ev;
    for(size_t i = 0; i < E.size(); ++i)
      if(E[i].u->id == v.id) ev.push_back(&E[i]);
    return ev;
  };

  /**
   * Retrieves the vertex adjacent to v on edge e
   *
   * @param Vertex v is the specified vertex
   * @param Edge e is the specified edge
   * @return Vertex* is the vertex adjacent to v on edge e
   */
  Vertex* adjacent_vertex(Vertex v, Edge e) {return *e.u == v ? e.v : e.u;};

  /**
   * Removes the specified edge from E in G = VE
   */
  bool remove_edge(Vertex* u, Vertex* v)
  {
    Edge e(u, v);
    return remove_edge(e);
  }

  /**
   * Removes the specified edge from E in G = VE
   */
  bool remove_edge(Edge & e)
  {
    bool ret = false;

    EdgeVectorIt it = E.begin();
    for( ; it != E.end() && !ret; ++it)
    {
      if(e == *it)
      {
        //remove adjacent vertices
        VertexMapIt vit = VE.find(*(e.u));
        Vertex* v = (Vertex*) &vit->first;
        if(vit != VE.end()) v->remove_adj(e.v);
        if(!directed)
        {
          vit = VE.find(*(e.v));
          v = (Vertex*) &vit->first;
          if(vit != VE.end()) v->remove_adj(e.u);
        }
        E.erase(it);
        ret = true;
      }
    }

    return ret;
  };

  /**
   * Construct and retreives G^T of G
   */
  Graph* get_transpose()
  {
    Graph* gt = new Graph(directed, weighted);

    size_t sz = E.size(), i = 0;

    for( ; i < sz; ++i)
    {

    }

    return gt;
  };

  /**
   * Retrieves edge at index i
   */
  Edge* get_edge(int i)
  {
    return i < E.size() && i > -1 ? &E[i] : 0;
  };

  Edge* get_edge(Vertex* u, Vertex* v)
  {
    for(size_t i = 0; i < E.size(); ++i)
    {
      if(E[i].u->id == u->id && E[i].v->id == v->id)return &E[i];
    }

    return 0;
  }

  /**
   * Retrieve vertex with the specified id
   */
  Vertex* get_vertex(string id)
  {
    VertexMapIt vit = VE.find(Vertex(id));
    return vit != VE.end() ? (Vertex*)&vit->first : 0;
  };

  /**
   *
   */
  bool bfs_connected()
  {
    bool rval = true;
    VertexMapIt vmi = VE.begin();
    init();

    Vertex s = vmi->first;
    Queue<Vertex> q;
    s.pi = &s;
    q.enqueue(s);

    while(!q.empty())
    {
      Vertex* u = (Vertex*) &VE.find(q.dequeue())->first;
      cout << "visiting " << u->id << endl;
      u->visited = true;
      AdjListIt ait = u->adj->begin();

      for( ; ait != u->adj->end(); ++ait)
      {
        Vertex* v = *ait;
        if(v->pi == NIL && v->visited == false)
        {
          v->pi = get_vertex(*u);
          q.enqueue(*v);
        }
        else if(v->pi == NIL) v->pi = get_vertex(*u);
      }
    }

    for (vmi = VE.begin(); rval && vmi != VE.end(); ++vmi)
    {
      if(vmi->first.visited == false) rval = false;
    }

    return rval;
  };

  /**
   *
   */
  bool dfs_connected()
  {
    bool rval = true;

    dfs();

    for(VertexMapIt i = VE.begin(); i != VE.end() && rval; ++i)
    {
      if(i->first.color != eBlack) rval = false;
    }

    return rval;
  }


  /**
   *
   */
  void dfs()
  {
    init();

    gt = 0;

    for(VertexMapIt i = VE.begin(); i != VE.end(); ++i)
    {
      Vertex *u = (Vertex*) &i->first;
      if(u->color  == eWhite)
        dfs_visit(u);
    }
  }

  /**
   *
   */
  void dfs_visit(Vertex* & u)
  {
    gt += 1;
    u->d = gt;
    u->color = eGray;
    Vertex* v = 0;

    for(AdjListIt uit = u->adj->begin(); uit != u->adj->end(); ++uit)
    {
      v = (*uit);
      if(v->color == eWhite)
      {
        v->pi = &(*u);
        dfs_visit(v);
      }
    }

    u->color = eBlack;
    gt += 1;
    u->f = gt;
  }

  /**
   * Compute shortest path via Dijkstras SP with PQ
   */
  set<string> dk_spath(string pu, string pv, double pw)
  {
    set <string> path;

    priority_queue< dpair > pq;

    vector<double> dist( vsize(), DBL_MAX);

    Vertex* sv = (Vertex*) &VE.begin()->first;

    pq.push(make_pair(*sv, 0));

    path.insert(pu);
    dist[sv->p.idx] = 0;
    int count = 0;
    double pd = -1;
    double cost = 0;

    stringstream ss;
    ss << pv;
    int pvi;  ss >> pvi;

    while(!pq.empty())
    {
      Vertex u = pq.top().first;
      pq.pop();

      EdgePtrVector epv = adjacent_edges(u);
      size_t sz = epv.size(), i = 0;
      for ( ; i < sz; ++i)
      {
        Vertex v  = *(epv[i]->v);
        string id = v.id;
        double w  = epv[i]->cap;
        if (count == pw)
        {
          if(u.p.idx < v.p.idx )
          {
            path.insert(u.id);
            if (pd == -1)
            {
              pd = dist[v.p.idx];
              path.insert(v.id);
            }
            else
            {
              if(dist[v.p.idx] < pd )
              {
                path.insert(v.id);
                pd = dist[v.p.idx];
              }
            }
          }
        }
        if(dist[v.p.idx] > dist[u.p.idx] + w)
        {
          dist[v.p.idx] = dist[u.p.idx] + w;
          pq.push(make_pair(v, dist[v.p.idx]));
        }
      }
      count++;
    }

    for(size_t i = 0; i < vsize(); ++i)
    {
      if( i == pvi)
        cost = dist[i];
    }

    cout << "SP Cost: " << cost << endl;
    return path;
  }

  /**
   * Renders a graph
   *
   * @param ostream is the output stream for rendering
   * @param Graph G is the graph to be rendered
   */
  friend ostream& operator << (ostream & o, Graph & G)
  {
    size_t i = 0, sz = G.vsize();

    o << "Rendering Graph...\n\n" << "Num Verticies: " << sz << endl;

    VertexMapIt vmi = G.VE.begin();

    for(; vmi != G.VE.end(); ++vmi) o << vmi->first << endl;
    o << "Rendering Edges\nNum Edges: " << G.E.size() << endl;
    for(i = 0; i < G.E.size(); ++i) o << G.E[i];
    o << "\nRendering Complete\n";

    return o;
  };
};

#endif//__Graph__
