/***************************************************************************//**
 * @file
 ******************************************************************************/
#ifndef __HEAP_H
#define __HEAP_H

/***************************************************************************//**
 * @class HeapSort
 ******************************************************************************/
class HeapSort : public Sort
{
   int *a;
   int currentSize;
   public:
      HeapSort(int *d, int s) : Sort(d, s, "HeapSort", false) {}
      void sort();

   private:
      void buildHeap();
      void percolateDown(int);
      void deleteMin(int&);
};
#endif
