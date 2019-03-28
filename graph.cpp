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
  for(int column = 0; column < width; column++)
  {
    for(int row = 0; row < height; row++)
    {

      int index = (row * width) + column;
      /*
      std::cout << std::endl << std::endl;
      std::cout << "column: " << column << std::endl;
      std::cout << "row: " << row << std::endl;
      std::cout << "width: " << width << std::endl;
      std::cout << "height: "<< height << std:: endl;
      std::cout << "index: " << index << std::endl;
      std::cout << "Num in square: " << grid[index] << std::endl;
      */
      //check to make sure the spots are open
      if(grid[index] != CLEAR_VALUE)
        continue;
      //check if in corner
      //bottom left corner
      else if(column == 0 && row == 0 && grid[index] == CLEAR_VALUE)
      {
          //add edge up and right
          addEdge(index, index + width);
          addEdge(index, index + 1);
          continue;
      }
      //bottom right corner
      else if(row == 0 && column == width - 1 && grid[index] == CLEAR_VALUE)
      {
          //add edge up and left
          addEdge(index, index + width);
          addEdge(index, index - 1);
          continue;
      }
      //top left corner
      else if(column == 0 && row == height - 1 && grid[index] == CLEAR_VALUE )
      {
        //add edge down and right
        addEdge(index, index - width);
        addEdge(index, index + 1);
        continue;
      }
      //top right corner
      else if(column == width - 1 && row == height - 1 && grid[index] == CLEAR_VALUE)
      {
        //add edge down and left
        addEdge(index, index - width);
        addEdge(index, index - 1);
        continue;
      }
      //check if in edge
      //bottom edge
      else if(row == 0 && (column > 0 && column < width) && grid[index] == CLEAR_VALUE)
      {
        //add edge, up right and left
        addEdge(index, index + width);
        addEdge(index, index - 1);
        addEdge(index, index + 1);
        continue;
      }
      //top edge
      else if(row == height - 1 && (column > 0 && column < width) && grid[index] == CLEAR_VALUE)
      {
        //add edge down right and left
        addEdge(index, index - width);
        addEdge(index, index + 1);
        addEdge(index, index - 1);
        continue;
      }
      //left edge
      else if(column == 0 && (row > 0 && row < height) && grid[index] == CLEAR_VALUE)
      {
        //add edge up down and right
        addEdge(index, index + width);
        addEdge(index, index - width);
        addEdge(index, index + 1);
        continue;
      }
      //right edge
      else if(column == width - 1 && (row > 0 && row < height) && grid[index] == CLEAR_VALUE)
      {
        //add edge up down and left
        addEdge(index, index + width);
        addEdge(index, index - width);
        addEdge(index, index - 1);
        continue;
      }
      //if in middle, do all of the add edges
      //
      else
      {
        //add edge up down right and left
        addEdge(index, index + width);
        addEdge(index, index - width);
        addEdge(index, index + 1);
        addEdge(index, index - 1);
      }

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
    //std::cout << "adding edge[" << one << ", " << two << "]\n" ;
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
