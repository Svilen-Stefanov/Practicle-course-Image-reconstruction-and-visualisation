#include <stack.h>
#define  CATCH_CONFIG_MAIN
#include "catch.hpp"

SCENARIO("Test Scenario for an empty stack"){
    Stack newStack;

    GIVEN("new empty stack"){
      REQUIRE(newStack.isEmpty() == true);
    }

    GIVEN("pop from an empty stack"){
      REQUIRE_THROWS_AS(newStack.pop(), std::logic_error);
    }

    GIVEN("top from an empty stack"){
      REQUIRE_THROWS_AS(newStack.pop(), std::logic_error);
    }

    GIVEN("push a new element into the stack"){
      newStack.push(5);
      REQUIRE(newStack.isEmpty() == false);
      REQUIRE(newStack.top() == 5);
    }
}

SCENARIO("Test Scenario for a non-empty stack"){
  Stack newStack;
  newStack.push(5);

  GIVEN("pop an element from the stack"){
    REQUIRE(newStack.pop() == 5);
    REQUIRE(newStack.isEmpty() == true);
  }

  newStack.push(2);
  GIVEN("push and pop elements in the stack"){
    REQUIRE(newStack.isEmpty() == false);
    REQUIRE(newStack.top() == 2);
    newStack.push(3);
    REQUIRE(newStack.isEmpty() == false);
    REQUIRE(newStack.top() == 3);
    newStack.push(4);
    REQUIRE(newStack.isEmpty() == false);
    REQUIRE(newStack.top() == 4);

    REQUIRE(newStack.pop() == 4);
    REQUIRE(newStack.pop() == 3);
    REQUIRE(newStack.pop() == 2);
    REQUIRE(newStack.pop() == 5);
  }
}
