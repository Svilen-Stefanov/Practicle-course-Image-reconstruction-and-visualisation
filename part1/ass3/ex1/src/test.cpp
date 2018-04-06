#include <MemoryOrder.h>
#define  CATCH_CONFIG_MAIN
#include "catch.hpp"

SCENARIO("Test Scenario for an empty stack"){
    MemoryOrder mem;

    //on my machine the stack is going backwards and the heap is going upwards
    REQUIRE(mem.isStackGrowingUpwards() == false);
    REQUIRE(mem.isHeapGrowingUpwards() == true);

    //on my machine they are in order static, heap, stack
    REQUIRE(mem.getMemoryOrder().at(0) == "static");
    REQUIRE(mem.getMemoryOrder().at(1) == "heap");
    REQUIRE(mem.getMemoryOrder().at(2) == "stack");
}
