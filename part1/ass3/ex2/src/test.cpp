#include <Iterators.h>
#define  CATCH_CONFIG_MAIN
#include "catch.hpp"
#include<vector>
#include<list>
#include<iostream>

SCENARIO("Test Scenario for an empty Iterators"){
  std::list<double> list {};
  std::vector<double> vector {};

  VectorIterator vectorIt = VectorIterator(vector);
  ListIterator listIt = ListIterator(list);

  REQUIRE(listIt.getSize() == 0);
  REQUIRE(vectorIt.getSize() == 0);

  REQUIRE(listIt.getMember(0) == -1);
  REQUIRE(vectorIt.getMember(0) == -1);

  REQUIRE(listIt.next() == nullptr);
  REQUIRE(vectorIt.next() == nullptr);
}

SCENARIO("Test Scenario for Iterators"){
  std::list<double> list {1.5, 2.5, 3.5};
  std::vector<double> vector {5.5, 6.5, 7.5};

  VectorIterator vectorIt = VectorIterator(vector);
  ListIterator listIt = ListIterator(list);

  REQUIRE(listIt.getSize() == 3);
  REQUIRE(vectorIt.getSize() == 3);

  REQUIRE(listIt.getMember(0) == 1.5);
  REQUIRE(vectorIt.getMember(0) == 5.5);

  REQUIRE(*listIt.next() == 1.5);
  REQUIRE(*vectorIt.next() == 5.5);

  REQUIRE(*listIt.next() == 2.5);
  REQUIRE(*vectorIt.next() == 6.5);

  REQUIRE(*listIt.next() == 3.5);
  REQUIRE(*vectorIt.next() == 7.5);

  REQUIRE(listIt.next() == nullptr);
  REQUIRE(vectorIt.next() == nullptr);

  std::ostream ostream(nullptr);
  ostream.rdbuf(std::cout.rdbuf());

  //must write "5.5\n6.5\n7.5\n"
  print(vectorIt, ostream);
  //must write "1.5\n2.5\n3.5\n"
  print(listIt, ostream);
}
