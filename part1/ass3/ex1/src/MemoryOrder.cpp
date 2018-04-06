#include "MemoryOrder.h"
#include<vector>


    std::vector<std::string> MemoryOrder::getMemoryOrder(){
        int a = 5;
        int *testStack = &a;
        int *testHeap = new int(10);
        static int c = 15;
        int *testStatic = &c;

        std::vector<std::string> returnVector;
        if (testStack > testHeap){ //stack > heap ----> heap, stack
            if(testStatic > testStack) //static > stack ---> heap, stack, static
                  returnVector = std::vector<std::string> {"heap", "stack", "static"};
            else {
                if(testStatic > testHeap)  //static > heap ---> heap, static, stack
                    returnVector = std::vector<std::string> {"heap", "static", "stack"};
                else    //static < heap ---> static, heap, stack
                    returnVector = std::vector<std::string> {"static", "heap", "stack"};
            }
        } else {  //stack < heap ----> stack, heap
            if(testStatic > testHeap)   //static > stack ---> heap, stack, static
                returnVector = std::vector<std::string>{"stack", "heap", "static"};
            else {
                if(testStatic > testStack)   //static > heap ---> heap, static, stack
                    returnVector = std::vector<std::string>{"stack", "static", "heap"};
                else //static < heap ---> static, heap, stack
                    returnVector = std::vector<std::string>{"static", "stack", "heap"};
            }
        }
        delete testHeap;

        return returnVector;
    }

    bool MemoryOrder::isStackGrowingUpwards(int *p) {
    int i;
    if (!p)
        isStackGrowingUpwards(&i);
    else if (p < &i)
        return true;
    else
        return false;
    }

    bool MemoryOrder::isStackGrowingUpwards(){
        return isStackGrowingUpwards(nullptr);
    }

    bool MemoryOrder::isHeapGrowingUpwards(){
        int *a = new int(5);
        int *b = new int(6);
        int *c = new int(7);
        bool isGrowingUpwards = (b < c) ? true : false;
        delete a;
        delete b;
        delete c;
        return isGrowingUpwards;
    }
