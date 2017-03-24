#include "dk.h"

using namespace std;

string srcstr,
       tgtstr;

Graph g(false, true);

map<int, Point> cities;

int NV,
    NE;

/**
 *
 */
void get_input()
{
  cout << "Input source city index: ";
  cin >> srcstr;
  cout << "Input destination city index: ";
  cin >> tgtstr;
  cout << "Computing shortest path from: " << srcstr << " to "
       << tgtstr << endl;
}

/**
 *
 */
bool make_graph(string input = "./doc/sample.txt")
{
  int          i  = 0;
  bool         ok = true;
  Point        p;
  string       s;
  stringstream ss;
  Vertex       *u,
               *v;

  cout << "Generating graph from file:  " << input << endl;

  ifstream in(input.c_str(), ios::in);

  if(in.is_open())
  {
    in >> NV >> NE;

    cout << "Map contains " << NV << " verticies (cities) and "
         << NE << " edges" << endl
         << "Generating city coordinates..." << endl;

    for( ; i < NV; ++i)
    {
      Vertex vtx;

      in >> vtx.p.idx;
      in >> vtx.p.x;
      in >> vtx.p.y;

      ss << vtx.p.idx;
      vtx.id = ss.str();
      ss.str(""); ss.clear();

      g.add_vertex(vtx);
    }

    cout << "Generated " << cities.size() << " city coordinates" << endl
         << "Connecting edges between cities in graph..." << endl;

    for(i = 0; i < NE && ok; ++i)
    {
      in >> s;
      u = g.get_vertex(s);
      in >> s;
      v = g.get_vertex(s);

      if(u && v)
      {
        //cout << u->id << "->" << v->id << " | " << u->distance(*v)
        //     << endl;
        g.add_edge(u, v, u->distance(*v));
      }
      else
      {
        cerr << "Error: Could not locate verticies exiting..." << endl;
        ok = false;
      }
    }

    if (ok) cout << "Connected " << g.esize() << " edges" << endl;

    in.close();
    //DEBUG:
    //cout << g << endl;
  }
  else
  {
    cerr << "Could not open: " << input << " for reading" << endl;
    ok = false;
  }

  return ok;
}

/**
 *
 */
void print_q(priority_queue<VertexEntry, vector<VertexEntry>, std::greater<VertexEntry> >& pq)
{
  cout << "Rendering Priority Queue..." << endl;
  priority_queue<VertexEntry, vector<VertexEntry>, std::greater<VertexEntry> > c;
  c = pq;
  while(!c.empty())
  {
    VertexEntry* ve = (VertexEntry*) &c.top();
    Vertex* u = ve->v;
    //cout << "extract-min: " << "u[" << u->id << "]: " << u->d << endl;
    c.pop();
  }
}

/**
 * TODO: MinHeap, MaxHeap, Template Class Wrappers
 */
void Heapify(MinQueue & pq)
{
    make_heap(const_cast<VertexEntry*>(&pq.top()),
              const_cast<VertexEntry*>(&pq.top()) + pq.size(), mcomp);
}

/**
 *
 */
void print_path(Vertex* & tgt, set<VertexEntry> & s)
{
  Vertex* vtgt = g.get_vertex(*tgt);

  cout << "All pairs shortest path" << endl;

  for(set<VertexEntry>::iterator vei = s.begin(); vei != s.end(); ++vei)
  {
    VertexEntry ve = *vei;
    cout << "v[" << ve.v->id << "]: " << ve.v->d << endl;
  }

  if(vtgt)
  {
    cout << vtgt->id << " -> ";
    Vertex* rent = vtgt->pi;
    while(rent)
    {
      cout << rent->id ;
      rent = rent->pi;
      if(rent) cout << " -> ";
    }
    cout << endl;

    VertexEntry ve(tgt);
    set<VertexEntry>::iterator sit = s.find(ve);
    if(sit != s.end())
    {
      VertexEntry ve = *sit;
      cout << "Total Distance: " << ve.v->d << endl;
    }
  }
}

/**
 *
 */
double DIJKSTRA(Graph & g, Vertex* & src, Vertex* & tgt)
{
  double rval = 0;

  Vertex *v, *u;
  Edge* e;
  set<VertexEntry> s;
  float w;

  g.init_single_src(src);
  priority_queue<VertexEntry, vector<VertexEntry>, greater<VertexEntry> > pq;

  cout << "Dijkstra Initialized Using Min Priority Queue..." << endl;

  for(VertexMapIt i = g.VE.begin(); i != g.VE.end(); ++i)
  {
    v = (Vertex*) &i->first;
    VertexEntry ve(v);
    pq.push(ve);
  }

  while(!pq.empty())
  {
    //extract_min(pq, u);
    //DEBUG:
    //print_q(pq);
    VertexEntry* ve = (VertexEntry*) &pq.top();
    u = ve->v;
    s.insert(*ve);
    pq.pop();
    //DEBUG:
    //cout << "extract-min: " << u->id << " : " << u->d << endl;

    AdjListIt ait = u->adj->begin();
    for( ; ait != u->adj->end(); ++ait)
    {
      v = *ait;
      w = g.get_edge(u, v)->cap;
      g.relax(u, v, w);
    }
    Heapify(pq);
    //cout << "Vertex[" << u->id << "] processed" << endl;
  }

  cout << "Rendering path" << endl;
  print_path(tgt, s);

  VertexEntry ve(tgt);
  set<VertexEntry>::iterator sit = s.find(ve);
  if(sit != s.end())
  {
    VertexEntry ve = *sit;
    rval = ve.v->d;
  }

  return rval;
}

/**
 *
 */
int main(int argc, char* argv[])
{
  string mapfile;
  bool ok = true;

  if(argc == 2)
  {
   stringstream s;

    s << argv[1];
    s >> mapfile;
    ok = make_graph(mapfile);
  }
  else
  {
    ok = make_graph();
  }

  if(ok)
  {
    get_input();
    Vertex *src = g.get_vertex(srcstr),
           *tgt = g.get_vertex(tgtstr);
    if(!src)
    {
      cout << "Error: Could not retrive source city " << srcstr << endl;
      ok = false;
    }
    else if(!src)
    {
      cout << "Error: Could not retrive destination city " << tgtstr << endl;
      ok = false;
    }
    else
      DIJKSTRA(g, src, tgt);
  }

  if(!ok) cerr << "Unable to recover from previous errors, exiting..." << endl;

  return 0;
}
