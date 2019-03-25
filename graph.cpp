#include "graph.h"

Graph::Graph()
{
  numVertices = 0;
  numEdges = 0;
}
Graph::Graph(std::istream &fin) : numVertices(0), numEdges(0)
{
   int v, w;

   while (!fin.eof())
   {
      fin >> v >> w;
      addEdge(v, w);
   }

   numVertices = vertices.size();
//   for (int u : Vertices())
//      numEdges += adj(u).size();
}


Graph::Graph(const int *grid, int width, int height)
{
  for(int i = 0; i < width; i++)
  {
    for(int j = 0; j < height; j++)
    {
      if(grid[(i * height) + j] == 0)
          addEdge(i + 1, j + 1);
    }
  }
  numVertices = vertices.size();
}


std::set<int> Graph::adj(int v)
{
  return vertices.at(v);
}



void Graph::addEdge(int one, int two)
{
  if(vertices[one].find(two) == vertices[one].end())
  {
    numEdges++;
    (vertices[one]).insert(two);
    (vertices[two]).insert(one);
    numVertices = vertices.size();

  }
}


int Graph::V() const
{
	return numVertices;
}

int Graph::E() const
{
	return numEdges;
}

std::set<int> Graph::Vertices() const
{
  std::set<int>all;

  for(auto x : vertices)
    all.insert(x.first);

  return all;
}
