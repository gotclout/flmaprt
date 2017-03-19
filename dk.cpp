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

#include "dk.h"

using namespace std;

int src, tgt;
int NV, NE;

map<int, Point> cities;
double** g;
double** c;

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

void make_graph()
{
  int u, v, d, i, j;


  ifstream in("./doc/sample.txt", ios::in);

  //if(in.is_open(), ios::in)
  if(in.is_open())
  {
    in >> NV >> NE;

    for(i = 0; i < NV; ++i)
    {
      Point p;
      in >> p.idx;
      in >> p.x;
      in >> p.y;

      cities[p.idx] = p;
      cout << "Add City: " << p << endl;
    }

    g = new double*[NV];
    c = new double*[NV];
    for(i = 0; i < NV; ++i)
    {
      g[i] = new double[NV];
      c[i] = new double[NV];
    }

    for(i = 0; i < NV; ++i)
    {
      for(j = 0; j < NV; ++j)
      {
        g[i][j] = 0;
        c[i][j] = 0;
      }
    }
    for(i = 0; i < NE; ++i)
    {
      in >> u;
      in >> v;

      g[u][v] = g[v][u] = cities[u].distance(cities[v]);
    }

    for(i = 0; i < NV; ++i)
    {
      for(j = 0; j < NV; ++j)
      {
        cout << g[i][j] << "\t";
      }
      cout << endl;
    }
  }
}

// Number of vertices in the graph
#define V 9
// A utility function to find the vertex with minimum distance value, from
// the set of vertices not yet included in shortest path tree
int minDistance(int dist[], bool sptSet[])
{
   // Initialize min value
   int min = INT_MAX, min_index;
  
   for (int v = 0; v < V; v++)
     if (sptSet[v] == false && dist[v] <= min)
         min = dist[v], min_index = v;
  
   return min_index;
}
  
int mindk(double dist[], bool sptSet[])
{
   // Initialize min value
   double min = DBL_MAX, min_index;
  
   for (int v = 0; v < NV; v++)
     if (sptSet[v] == false && dist[v] <= min)
         min = dist[v], min_index = v;
  
   return min_index;
}

// A utility function to print the constructed distance array
int printSolution(int dist[], int n)
{
   printf("Vertex   Distance from Source\n");
   for (int i = 0; i < V; i++)
      printf("%d \t\t %d\n", i, dist[i]);
}
  
int printdk(double dist[], int n)
{
   printf("Vertex   Distance from Source\n");
   for (int i = 0; i < NV; i++)
      printf("%d \t\t %f\n", i, dist[i]);
}
void printc()
{
  for (int i = 0; i < NV; ++i)
  {
    for(int j = 0; j < NV; ++j)
    {
      cout << c[i][j]<< "\t";
    }
    cout << endl;
  }
}

#include <set>
using std::set;

set<int> dk(double** & dk, int src, int tgt)
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
     //dist[src] = 0;
 
     int cc = 0, nc = 0;
     // Find shortest path for all vertices
     for (int count = 0; count < NV-1; count++)
     {
       // Pick the minimum distance vertex from the set of vertices not
       // yet processed. u is always equal to src in first iteration.
       int u = mindk(dist, sptSet);
 
       if (count == tgt)
         cout << "u: " << u << endl;
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
           //cout << count << ",";
           //cout << cc++ << endl;
           //nc = cc;
           dist[v] = dist[u] + g[u][v];
           //c[u+1][v] = dist[v];
           //c[u][v+1] = dist[v];
         }
         else
         { //if(dist[v] == DBL_MAX)
           //  c[u+1][v] = 0;
           //c[u+1][v-1] = dist[v];
         }
         
       }
     }
     // print the constructed distance array
     printdk(dist, NV);
     //printc();
    return path;
}

// Funtion that implements Dijkstra's single source shortest path algorithm
// for a graph represented using adjacency matrix representation
void dijkstra(int graph[V][V], int src)
{
     int dist[V];     // The output array.  dist[i] will hold the shortest
                      // distance from src to i
  
     bool sptSet[V]; // sptSet[i] will true if vertex i is included in shortest
                     // path tree or shortest distance from src to i is finalized
  
     // Initialize all distances as INFINITE and stpSet[] as false
     for (int i = 0; i < V; i++)
        dist[i] = INT_MAX, sptSet[i] = false;
  
     // Distance of source vertex from itself is always 0
     dist[src] = 0;
  
     // Find shortest path for all vertices
     for (int count = 0; count < V-1; count++)
     {
       // Pick the minimum distance vertex from the set of vertices not
       // yet processed. u is always equal to src in first iteration.
       int u = minDistance(dist, sptSet);
  
       // Mark the picked vertex as processed
       sptSet[u] = true;
  
       // Update dist value of the adjacent vertices of the picked vertex.
       for (int v = 0; v < V; v++)
  
         // Update dist[v] only if is not in sptSet, there is an edge from 
         // u to v, and total weight of path from src to  v through u is 
         // smaller than current value of dist[v]
         if (!sptSet[v] && graph[u][v] && dist[u] != INT_MAX 
                                       && dist[u]+graph[u][v] < dist[v])
            dist[v] = dist[u] + graph[u][v];
     }
  
     // print the constructed distance array
     printSolution(dist, V);
}
  
// driver program to test above function
int main()
{
  make_graph();
  set_input();
  
   /* Let us create the example graph discussed above */
   int graph[V][V] = {{0, 4, 0, 0, 0, 0, 0, 8, 0},
                      {4, 0, 8, 0, 0, 0, 0, 11, 0},
                      {0, 8, 0, 7, 0, 4, 0, 0, 2},
                      {0, 0, 7, 0, 9, 14, 0, 0, 0},
                      {0, 0, 0, 9, 0, 10, 0, 0, 0},
                      {0, 0, 4, 14, 10, 0, 2, 0, 0},
                      {0, 0, 0, 0, 0, 2, 0, 1, 6},
                      {8, 11, 0, 0, 0, 0, 1, 0, 7},
                      {0, 0, 2, 0, 0, 0, 6, 7, 0}
                     };
  
    //dijkstra(graph, 7);
 
    set<int> p = dk(g, src, tgt);
    set<int>::iterator i = p.begin();
    for( ; i != p.end(); ++i)
      cout << *i << " -> ";
    cout << tgt << endl;
    return 0;
}
