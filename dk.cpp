// A C / C++ program for Dijkstra's single source shortest path algorithm.
// The program is for adjacency matrix representation of the graph
  
#include <stdio.h>
#include <limits.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <new>


#include "dk.h"

using namespace std;

string mapfile = "";

int src, tgt;
int NV, NE;

map<int, Point> cities;
float** g;
//vector< vector<float> > g;
Graph gg;
void set_input()
{
  src = 0; tgt = 5;
  cout << "Input source city index: " << src << endl;
  cout << "Input destination city index: " << tgt << endl;
  cout << "Computing shortest path from: " << src << " to "
       << tgt << endl;
}

void get_input()
{
  cout << "Input source city index: ";
  cin >> src;
  cout << "Input destination city index: ";
  cin >> tgt;
  cout << "Computing shortest path from: " << src << " to "
       << tgt << endl;
}

void make_graph(string input = "./doc/sample.txt")
{
  int u, v, d, i, j;

  ifstream in(input.c_str(), ios::in);

  stringstream ss;

  cout << "Generating graph from file:  " << input << endl;
  if(in.is_open())
  {
    in >> NV >> NE;

    cout << "Map contains " << NV << " verticies (cities) and "
         << NE << " edges" << endl;

    cout << "Adding cities to graph..." << endl;
    Point p;
    for(i = 0; i < NV; ++i)
    {
      in >> p.idx;
      in >> p.x;
      in >> p.y;

      cities[p.idx] = p;
    }

    cout << "last" << endl << p << endl;
    cout << "Cities added to graph" << endl;

    g = new float*[NV];
    for(i = 0; i < NV; ++i)
    {
      g[i] = new (std::nothrow)float[NV];
      for(j = 0; j <NV; ++j)
        g[i][j] = 0.0;
      //vector <float> vf(NV, 0);
      //g.push_back(vf);
    }

    cout << "Adding edges to graph..." << endl;
    u = v = 0;
    for(i = 0; i < NE; ++i)
    {
      in >> u;
      in >> v;

      ss << u;
      Vertex vu(ss.str());
      ss.str(""); ss.clear();
      ss << v;
      Vertex vv(ss.str());
      ss.str(""); ss.clear();
      vu.p = cities[u];
      vv.p = cities[v];

      g[u][v] = g[v][u] = cities[u].distance(cities[v]);
      gg.add_edge(vu, vv, g[u][v]);
    }

    cout << "Edges added to graph" << endl;

    /*
    for(i = 0; i < NV; ++i)
    {
      for(j = 0; j < NV; ++j)
      {
        cout << g[i][j] << "\t";
      }
      cout << endl;
    }
    cout << gg;
    */
    in.close();
  }
  else
  {
    cout << "Could not open: " << input << " for reading" << endl;
  }
}

// Number of vertices in the graph

int mindk(double dist[], bool sptSet[])
{
   // Initialize min value
   double min = DBL_MAX, min_index;

   for (int v = 0; v < NV; v++)
     if (sptSet[v] == false && dist[v] <= min)
         min = dist[v], min_index = v;

   return min_index;
}

int printdk(double dist[], int n)
{
   printf("Vertex   Distance from Source\n");
   for (int i = 0; i < NV; i++)
      printf("%d \t\t %f\n", i, dist[i]);
}

set<int> dk(int src, int tgt)
{
     set<int> path;

     double dist[NV];     // The output array.  dist[i] will hold the shortest
                      // distance from src to i

     bool sptSet[NV]; // sptSet[i] will true if vertex i is included in shortest
                     // path tree or shortest distance from src to i is finalized

     // Initialize all distances as INFINITE and stpSet[] as false
     for (int i = 0; i < NV; i++)
        dist[i] = DBL_MAX, sptSet[i] = false;

     // Distance of source vertex from itself is always 0
     dist[src] = 0;

     int cc = 0, nc = 0;
     // Find shortest path for all vertices
     for (int count = 0; count < NV-1; count++)
     {
       // Pick the minimum distance vertex from the set of vertices not
       // yet processed. u is always equal to src in first iteration.
       int u = mindk(dist, sptSet);

       if (count == tgt)
         cout << "Generating Path " << src << " " << tgt << endl; //u
       // Mark the picked vertex as processed
       sptSet[u] = true;

       //cout << "p: " << u << endl;
       // Update dist value of the adjacent vertices of the picked vertex.
       for (int v = 0; v < NV; v++)
       { 
         // Update dist[v] only if is not in sptSet, there is an edge from 
         // u to v, and total weight of path from src to  v through u is 
         // smaller than current value of dist[v]
         if (!sptSet[v] && g[u][v] && dist[u] != DBL_MAX
                        && dist[u] + g[u][v] < dist[v])
         {
           path.insert(count);
           dist[v] = dist[u] + g[u][v];
         }
       }
     }
     // print the constructed distance array
    //printdk(dist, NV);
    cout << "SP Cost: " << dist[tgt] << endl;
    return path;
}

int main(int argc, char* argv[])
{

  if (argc == 2)
  {
    stringstream s;

    s << argv[1];
    s >> mapfile;
    make_graph(mapfile);
  }
  else
  {
    make_graph();
  }

  get_input();

    set<int> p = dk(src, tgt);
    for(int i = 0; i < NV; ++i)
      delete  [] g[i];
    delete []g;
    set<int>::iterator i = p.begin();
    cout << "Generic Dijkstra SP" << endl;
    for( ; i != p.end(); ++i)
      cout << *i << " -> ";
    cout << tgt << endl;

    stringstream ss;
    string srcstr, tgtstr;
    ss << src; ss >> srcstr;
    ss.str(""); ss.clear();
    ss << tgt; ss >> tgtstr;
    ss.str(""); ss.clear();

    set<string> pp = gg.dk_spath(srcstr, tgtstr, tgt);
    cout << "Generic Dijkstra SP With PQ" << endl;
    set<string>::iterator ii = pp.begin();
    for( ; ii != pp.end(); ++ii)
      cout << *ii << " -> ";
    cout << tgt << endl;

    return 0;
}
