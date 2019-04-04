#include <iostream>
#include <algorithm>
#include "heap.h"

using namespace std;

void HeapSort::percolateDown(int hole)
{
   int child;
   int tmp = std::move(a[hole]);

   for (; hole * 2 <= currentSize ; hole = child)
   {
      child = hole * 2;
      if (child != currentSize && a[child + 1] < a[child])
         ++child;

      if (a[child] < tmp)
         a[hole] = std::move(a[child]);
      else
         break;
   }
   a[hole] = std::move(tmp);
}

void HeapSort::buildHeap()
{
   for (int i = currentSize / 2 ; i > 0 ; --i)
      percolateDown(i);
}

void HeapSort::deleteMin(int &item)
{
    item = std::move(a[1]);
    a[1] = std::move(a[currentSize--]);
    percolateDown(1);
}

void HeapSort::sort()
{
   a = new int[size+2];

   currentSize = size;

   for (int i = size+1 ; i >= 1 ; i--)
      a[i] = data[i-1];

   buildHeap();

   int index = 0;
   for (int i = 0 ; i < size ; i++)
      deleteMin(data[index++]);

   delete a;
}
