#include "graph.h"
/**************************************************************************//**
 * @author Dr. Hinker and Jennifer Kulich
 *
 * @par Description:
 * Default constructor for the graph class and sets the number of edges and
 * vertices to 0
 *
 *****************************************************************************/
Graph::Graph()
{
  numVertices = 0;
  numEdges = 0;
}


/**************************************************************************//**
 * @author Dr. Hinker
 *
 * @par Description:
 * Constructor for the graph class that takes in a grid and its width and height
 * and connect nodes if possible, keeping track of the number of vertices
 *
 * @param[in] grid - The playfield
 * @param[in] width - width of the grid
 * @param[int] height - height of the grid
 *
 *****************************************************************************/
Graph::Graph(const int *grid, int width, int height)
{
  //create all the nodes
  for(int i = 0; i < width * height; i++)
  {
    //passing the same number twice will create a node with no edges
    addEdge(i,i);
  }

/*
     //add all edges that should exist
     for(int i = 0; i < width*height; i++)
     {
        //check the node above the node we are looking at
        if((grid[i+width]%2 == 0) && (grid[i] == 0) && ((i/width) != (width-1)))
        {
           addEdge(i, i+width);
        }

        //check the node to the left of the node we are looking at
        if((grid[i-1]%2 == 0) && (grid[i] == 0) && ((i%width) != 0))
        {
           addEdge(i, i-1);
        }

        //check the node bellow the node we are looking at
        if((grid[i-width]%2 == 0) && (grid[i] == 0) && ((i/width) != 0))
        {
           addEdge(i, i-width);
        }

        //check the node to the right of the node we are looking at
        if((grid[i+1]%2 == 0) && (grid[i] == 0) && ((i%width) != (width-1)))
        {
           addEdge(i, i+1);
        }
     }
     */

  for(int column = 0; column < width; column++)
  {
    for(int row = 0; row < height; row++)
    {

      int index = (row * width) + column;
      //check to make sure the spots are open
      if(grid[index] != CLEAR_VALUE && grid[index] != FOOD_VALUE && grid[index] != HEAD_VALUE)
      {
        continue;
      }
      //  continue;
      //check if in corner
      //bottom left corner
      else if(column == 0 && row == 0)
      {
          //add edge up and right
          if(grid[index + width] == CLEAR_VALUE || grid[index + width] == FOOD_VALUE)
            addEdge(index, index + width);
          if(grid[index + 1] == CLEAR_VALUE || grid[index + 1] == FOOD_VALUE)
            addEdge(index, index + 1);
          continue;
      }
      //bottom right corner
      else if(row == 0 && column == width - 1)
      {
          //add edge up and left
          if(grid[index + width] == CLEAR_VALUE || grid[index + width] == FOOD_VALUE)
            addEdge(index, index + width);
          if(grid[index - 1] == CLEAR_VALUE || grid[index - 1] == FOOD_VALUE)
            addEdge(index, index - 1);
          continue;
      }
      //top left corner
      else if(column == 0 && row == height - 1)
      {
        //add edge down and right
        if(grid[index - width] == CLEAR_VALUE || grid[index - width] == FOOD_VALUE)
          addEdge(index, index - width);
        if(grid[index + 1] == CLEAR_VALUE || grid[index + 1] == FOOD_VALUE)
          addEdge(index, index + 1);
        continue;
      }
      //top right corner
      else if(column == width - 1 && row == height - 1 )
      {
        //add edge down and left
        if(grid[index - width] == CLEAR_VALUE || grid[index - width] == FOOD_VALUE)
          addEdge(index, index - width);
        if(grid[index - 1] == CLEAR_VALUE || grid[index - 1] == FOOD_VALUE)
          addEdge(index, index - 1);
        continue;
      }
      //check if in edge
      //bottom edge
      else if(row == 0 && (column > 0 && column < width) )
      {
        //add edge, up right and left
        if(grid[index + width] == CLEAR_VALUE || grid[index + width] == FOOD_VALUE)
          addEdge(index, index + width);
        if(grid[index - 1] == CLEAR_VALUE || grid[index - 1] == FOOD_VALUE)
          addEdge(index, index - 1);
        if(grid[index + 1] == CLEAR_VALUE || grid[index + 1] == FOOD_VALUE)
          addEdge(index, index + 1);
        continue;
      }
      //top edge
      else if(row == height - 1 && (column > 0 && column < width))
      {
        //add edge down right and left
        if(grid[index - width] == CLEAR_VALUE || grid[index - width] == FOOD_VALUE)
          addEdge(index, index - width);
        if(grid[index + 1] == CLEAR_VALUE || grid[index + 1] == FOOD_VALUE)
          addEdge(index, index + 1);
        if(grid[index - 1] == CLEAR_VALUE || grid[index - 1] == FOOD_VALUE)
          addEdge(index, index - 1);
        continue;
      }
      //left edge
      else if(column == 0 && (row > 0 && row < height))
      {
        //add edge up down and right
        if(grid[index + width] == CLEAR_VALUE || grid[index + width] == FOOD_VALUE)
          addEdge(index, index + width);
        if(grid[index - width] == CLEAR_VALUE || grid[index - width] == FOOD_VALUE)
          addEdge(index, index - width);
        if(grid[index + 1] == CLEAR_VALUE || grid[index + 1] == FOOD_VALUE)
          addEdge(index, index + 1);
        continue;
      }
      //right edge
      else if(column == width - 1 && (row > 0 && row < height) )
      {
        //add edge up down and left
        if(grid[index + width] == CLEAR_VALUE || grid[index + width] == FOOD_VALUE)
          addEdge(index, index + width);
        if(grid[index - width] == CLEAR_VALUE || grid[index - width] == FOOD_VALUE)
          addEdge(index, index - width);
        if(grid[index - 1] == CLEAR_VALUE || grid[index - 1] == FOOD_VALUE)
          addEdge(index, index - 1);
        continue;
      }
      //if in middle, do all of the add edges
      else
      {
        //add edge up down right and left
        if(grid[index + width] == CLEAR_VALUE || grid[index + width] == FOOD_VALUE)
          addEdge(index, index + width);
        if(grid[index - width] == CLEAR_VALUE || grid[index - width] == FOOD_VALUE)
          addEdge(index, index - width);
        if(grid[index + 1] == CLEAR_VALUE || grid[index + 1] == FOOD_VALUE)
          addEdge(index, index + 1);
        if(grid[index - 1] == CLEAR_VALUE || grid[index - 1] == FOOD_VALUE)
          addEdge(index, index - 1);
      }

    }
  }
  numVertices = vertices.size();

}


/**************************************************************************//**
 * @author Jennifer Kulich
 *
 * @par Description:
 * Gets the adjacency list of the vertices
 *
 * @param[in] v - the vertice to get the adjacency list from
 *
 *****************************************************************************/
std::set<int> Graph::adj(int v)
{
  return vertices.at(v);
}


/**************************************************************************//**
 * @author Dr. Hinker
 *
 * @par Description:
 * Will add the edges from one node to another and add to the number of edges
 *
 * @param[in] one - the one node to add a connection to
 * @param[in] two - the second node to add a connection to
 *
 *****************************************************************************/
void Graph::addEdge(int one, int two)
{
  //if they are the same, create a single node
  if(two == one)
    vertices[one];
  else
  {
    //add each node to the other nodes list of connected nodes
    vertices[one].insert(two);
    vertices[two].insert(one);
  }

  //recalculat the number of vertices
  numVertices = vertices.size();
  numEdges = 0;

  //recalculate the number of edges
  for(int x : Vertices())
    numEdges += adj(x).size();

  numEdges = numEdges / 2;
}


/**************************************************************************//**
 * @author Jennifer Kulich
 *
 * @par Description:
 * Return the number of vertices in the graph
 *
 * @return - the number of vertices in the graph
 *
 *****************************************************************************/
int Graph::V() const
{
	return numVertices;
}


/**************************************************************************//**
 * @author Jenifer Kulich
 *
 * @par Description:
 * Return the number of edges in the graph
 *
 * @return - the number of edges in the graph
 *
 *****************************************************************************/
int Graph::E() const
{
	return numEdges;
}


/**************************************************************************//**
 * @author Dr. Hinker
 *
 * @par Description:
 * Will return the set of vertices in the graph
 *
 * @return - the set of all of the vertices
 *
 *****************************************************************************/
std::set<int> Graph::Vertices() const
{
  std::set<int>all;

  for(auto x : vertices)
    all.insert(x.first);

  return all;
}
