#ifndef MEMORYORDER_H
#define MEMORYORDER_H
#include <iostream>
#include <vector>

class MemoryOrder{
    public:
      std::vector<std::string> getMemoryOrder();
      bool isStackGrowingUpwards(int*); 
      bool isStackGrowingUpwards();
      bool isHeapGrowingUpwards();
};

#endif // MEMORYORDER_H
