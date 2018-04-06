#include <my_find.hpp>
#include <my_list.hpp>
#include <vector>
#include <list>
#define  CATCH_CONFIG_MAIN
#include "catch.hpp"

SCENARIO("Test Scenario for my_find"){

  WHEN("Testing my_find with my_list"){
    std::cout << "__________Test my_list_________" << std::endl;
    std::list<int> MyList{57, 33, 21, 1501};

    //existing element in the list
    auto it = myfind(MyList.begin(), MyList.end(), 1501);
    std::cout << "Looking for 1501 in my_list: ";
    if(it == MyList.end())
      std::cout << "Such element was not found!" << std::endl;
    else
      std::cout << "Found! The element you have searched for: " << *it << std::endl;

    //non-existing element in the list
    auto it2 = myfind(MyList.begin(), MyList.end(), 1);
    std::cout << "Looking for 1 in my_list: ";
    if(it2 == MyList.end())
      std::cout << "Such element was not found!" << std::endl;
    else
      std::cout << "Found! The element you have searched for: " << *it2 << std::endl;

  }

  WHEN("Testing my_find with a list"){
    std::cout << "__________Test list_________" << std::endl;
    std::list<std::string> list{"Svilen", "Plamen", "Eliza", "Krassen"};

    //existing element in the list
    auto it = myfind(list.begin(), list.end(), "Eliza");
    std::cout << "Looking for \"Eliza\" in the list: ";
    if(it == list.end())
      std::cout << "Such element was not found!" << std::endl;
    else
      std::cout << "Found! The element you have searched for: " << *it << std::endl;

    //non-existing element in the list
    auto it2 = myfind(list.begin(), list.end(), "John");
    std::cout << "Looking for \"John\" in the list: ";
    if(it2 == list.end())
      std::cout << "Such element was not found!" << std::endl;
    else
      std::cout << "Found! The element you have searched for: " << *it2 << std::endl;
  }

  WHEN("Testing my_find with a vector"){
    std::cout << "__________Test list_________" << std::endl;
    std::vector<double> vector{7, 5, 11, 101, 121};

    //existing element in the vector
    auto it = myfind(vector.begin(), vector.end(), 5);
    std::cout << "Looking for 5 in the list: ";
    if(it == vector.end())
      std::cout << "Such element was not found!" << std::endl;
    else
      std::cout << "Found! The element you have searched for: " << *it << std::endl;

    //non-existing element in the vector
    auto it2 = myfind(vector.begin(), vector.end(), 6);
    std::cout << "Looking for 6 in the list: ";
    if(it2 == vector.end())
      std::cout << "Such element was not found!" << std::endl;
    else
      std::cout << "Found! The element you have searched for: " << *it2 << std::endl;
  }
}
