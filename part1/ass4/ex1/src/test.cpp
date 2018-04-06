#include <my_list.hpp>
#define  CATCH_CONFIG_MAIN
#include "catch.hpp"
#include<iostream>
#include <istream>
#include <sstream>

SCENARIO("Test Scenario for borderfalls in Doubly Linked List"){
  MyList<int> list;

  WHEN("Push and pop one element in the list"){
    REQUIRE(list.empty() == true);
    list.push_back(5);
    REQUIRE(list.empty() == false);
    REQUIRE(list.back() == 5);
    REQUIRE(list.front() == 5);
    list.pop_back();
    REQUIRE(list.empty() == true);
  }


  WHEN("Insert and erase one element in the list"){
    REQUIRE(list.empty() == true);
    auto it {list.begin()};
    it = list.insert(it, 5);
    REQUIRE(list.empty() == false);
    REQUIRE(list.back() == 5);
    REQUIRE(list.front() == 5);
    it = list.erase(it);
    REQUIRE(list.empty() == true);

    it = list.insert(it, 5);
    REQUIRE(list.empty() == false);
    REQUIRE(list.back() == 5);
    REQUIRE(list.front() == 5);
    it = list.erase(it);
    REQUIRE(list.empty() == true);
  }

  WHEN("Insert and erase one element in the list"){
    REQUIRE(list.empty() == true);
    auto it {list.begin()};
    it = list.insert(it, 5);
    REQUIRE(list.empty() == false);
    REQUIRE(list.back() == 5);
    REQUIRE(list.front() == 5);
    list.erase(it);
    REQUIRE(list.empty() == true);
  }

  WHEN("Makes sure that afterLast is correctrly allocated"){
    REQUIRE(list.empty() == true);
    auto it {list.begin()};
    it = list.insert(it, 5);
    REQUIRE(list.empty() == false);
    REQUIRE(list.back() == 5);
    REQUIRE(list.front() == 5);
    it = list.erase(it);
    REQUIRE(list.empty() == true);
    it = list.insert(it, 5);
    REQUIRE(list.empty() == false);
    list.pop_back();
    REQUIRE(list.empty() == true);
  }

  WHEN("Test iterator insert at the end"){
    auto it {list.begin()};
    it = list.insert(it, 999);
    REQUIRE(list.front() == 999);
    it = list.end();
    list.insert(it, 11);
    REQUIRE(list.back() == 11);
    std::cout << "After insert of 11 and 999: " << list << std::endl;
  }
}

SCENARIO("Test Scenario for Doubly Linked List with empty constructor"){
    MyList<int> list = MyList<int>();

    WHEN("Test push_front"){
      list.push_front(10);
      REQUIRE(list.back() == 10);
      list.push_front(7);
      REQUIRE(list.front() == 7);
    }

    WHEN("Test push backoperations"){
      list.push_back(5);
      REQUIRE(list.back() == 5);
      list.push_back(15);
      list.push_back(25);
      list.push_back(35);
      REQUIRE(list.back() == 35);
      list.push_back(45);
      list.push_back(55);
      REQUIRE(list.back() == 55);
      list.pop_back();
      REQUIRE(list.back() == 45);

      MyList<int> list2 = MyList<int>();
      list2.push_back(1);
      REQUIRE(list2.back() == 1);
      list2.push_back(2);
      REQUIRE(list2.back() == 2);
      list2.push_back(3);
      list2.push_back(4);
      REQUIRE(list2.back() == 4);
      REQUIRE(list2.front() == 1);

    }

    WHEN("Push, front and back operations"){
      list.push_back(5);
      list.push_back(6);
      REQUIRE(list.back() == 6);
      list.pop_back();
      REQUIRE(list.empty() == false);
      list.push_front(10);
      REQUIRE(list.front() == 10);
      REQUIRE(list.back() == 5);
      list.push_front(25);
      list.push_front(27);
      REQUIRE(list.front() == 27);
      list.push_front(101);
      list.push_front(121);
      REQUIRE(list.back() == 5);
      REQUIRE(list.size() == 6);
    }

    WHEN("Check whether pop_Front and pop_back work"){
      REQUIRE(list.size() == 0);
      list.push_back(5);
      list.push_back(6);
      REQUIRE(list.size() == 2);
      list.pop_back();
      REQUIRE(list.size() == 1);
      list.pop_front();
      REQUIRE(list.size() == 0);
    }

    WHEN("Test printing"){
      MyList<int> list2 = MyList<int>();
      list2.push_back(5);
      list2.push_back(6);
      list2.push_back(1);
      list2.push_back(2);
      list2.push_back(3);
      list2.push_back(4);
      std::cout << "Print list with push_back: " << list2 << std::endl;
    }

    WHEN("Test printing with push_front"){
      list.push_front(5);
      list.push_front(6);
      list.push_front(1);
      list.push_front(2);
      list.push_front(3);
      list.push_front(4);
      std::cout << "Print list with push_front: " << list << std::endl;
    }

    WHEN("Test iterator insert with one position"){
      list.push_back(20);
      list.push_back(100);
      auto it {list.begin()};
      it = list.insert(it, 6);
      std::cout << "After insert of 6: " << list << std::endl;
      list.insert(++it, 12);
      std::cout << "After insert of 12: " << list << std::endl;
    }

    WHEN("Test iterator insert at the end"){
      list.push_back(19);
      list.push_back(99);
      REQUIRE(list.back() == 99);
      auto it {list.end()};
      list.insert(it, 999);
      REQUIRE(list.front() == 19);
      REQUIRE(list.back() == 999);
      std::cout << "After insert of 999: " << list << std::endl;
    }

    WHEN("Test iterator insert at the beginning/end"){
      auto it {list.begin()};
      list.insert(it, 11);
      REQUIRE(list.back() == 11);
      std::cout << "After insert of 11: " << list << std::endl;

      MyList<int> list2 {1, 2, 4};
      auto it2 {list2.end()};
      it2 = list2.insert(it2, 1);
      REQUIRE(list2.back() == 1);
      std::cout << "After insert of 1: " << list2 << std::endl;
      list2.insert(it2, 12);
      std::cout << "After insert of 12: " << list2 << std::endl;
    }

    WHEN("Test iterator insert with many positions"){
      unsigned int n (2);
      list.push_back(10);
      list.push_back(1);
      auto it {list.begin()};
      list.insert(it, n, 7);
      std::cout << "After insert of 7, 2 times: " << list << std::endl;
    }

    WHEN("Test third iterator method using three positions"){
      MyList<int> l1, l2 = MyList<int>();
      l1.push_back(1);
      REQUIRE(l1.back() == 1);
      l1.push_back(2);
      REQUIRE(l1.back() == 2);
      l1.push_back(3);
      REQUIRE(l1.back() == 3);
      l1.push_back(4);
      REQUIRE(l1.back() == 4);
      l2.push_back(11);
      l2.push_back(22);
      l2.push_back(33);
      l2.push_back(44);

      std::cout << "L1 before insert of 22 and 33: " << l1 << std::endl;
      std::cout << "L2 before insert of 22 and 33: " << l2 << std::endl;
      l1.insert(++l1.begin(), ++l2.begin(), --l2.end());
      std::cout << "After insert of 22 and 33: " << l1 << std::endl;
    }

    WHEN("Test erase iterator"){
      list.push_back(356);
      list.push_back(121);
      auto it {list.begin()};
      std::cout << "Before erase of 356: " << list << std::endl;
      list.erase(it);
      std::cout << "After erase of 356: " << list << std::endl;
      REQUIRE(list.back() == 121);

      list.push_back(1);
      REQUIRE(list.front() == 121);
      REQUIRE(list.back() == 1);
      std::cout << "After insert of 1: " << list << std::endl;
      auto itr{list.end()};
      REQUIRE(list.front() == 121);
      REQUIRE(list.back() == 1);
      list.erase(itr);
      std::cout << "After erase of 1: " << list << std::endl;
      REQUIRE(list.front() == 121);
    }
}

  SCENARIO("Test Scenario for Doubly Linked List with copy constructor"){
      std::cout << "___________Copy constructor_______________" << std::endl;
      MyList<int> list = MyList<int>();
      list.push_back(356);
      list.push_back(121);
      std::cout << "L1: " << list << std::endl;

      MyList<int> listTest {list};
      REQUIRE(listTest.front() == 356);
      REQUIRE(listTest.back() == 121);
      REQUIRE(listTest.size() == 2);
      std::cout << "L2: " << listTest << std::endl;
    }

  SCENARIO("Test Scenario for Doubly Linked List with move constructor"){
      std::cout << "___________Move constructor_______________" << std::endl;
      MyList<int> list = MyList<int>();
      list.push_back(7);
      list.push_back(11);
      list.push_front(3);
      list.push_front(50);
      std::cout << "L1 before move: " << list << std::endl;

      MyList<int> listTest {std::move(list)};
      REQUIRE(listTest.front() == 50);
      REQUIRE(listTest.back() == 11);
      REQUIRE(listTest.size() == 4);
      REQUIRE(list.size() == 0);
      REQUIRE(list.empty() == true);
      std::cout << "L1 after move: " << list << std::endl;
      std::cout << "L2 after move: " << listTest << std::endl;
  }

  SCENARIO("Test Scenario for Doubly Linked List with list assignment constructor"){
      std::cout << "____________Assignment constructor______________" << std::endl;
      MyList<int> list {5, 6, 7, 10};
      REQUIRE(list.front() == 5);
      REQUIRE(list.back() == 10);
      REQUIRE(list.size() == 4);

      std::cout << "L: " << list << std::endl;

      WHEN("Test erase"){
        MyList<int> list2 {1, 2, 3, 4};
        auto it {list2.end()};
        it = list2.erase(it);
        REQUIRE(list2.empty() == false);
        REQUIRE(list2.back() == 3);
        std::cout << "L2: " << list2 << std::endl;
        it = list2.erase(it);
        REQUIRE(list2.back() == 2);
        std::cout << "L2: " << list2 << std::endl;
        it = list2.erase(it);
        REQUIRE(list2.back() == 1);
        it = list2.erase(it);
        REQUIRE(list2.size() == 0);
      }
  }

  SCENARIO("Test Scenario for Doubly Linked List with copy assignment operator"){
    std::cout << "____________Copy assignment operator______________" << std::endl;
    MyList<int> list{1, 2}, list2{3,4};
    REQUIRE(list.front() == 1);
    REQUIRE(list.back() == 2);
    REQUIRE(list2.front() == 3);
    REQUIRE(list2.back() == 4);

    std::cout << "List 1 before the copy assignment: " << list << std::endl;
    std::cout << "List 2 before the copy assignment: " << list2 << std::endl;

    list = list2;

    REQUIRE(list.front() == 3);
    REQUIRE(list.back() == 4);
    REQUIRE(list2.front() == 3);
    REQUIRE(list2.back() == 4);

    std::cout << "List 1 after the copy assignment: " << list << std::endl;
    std::cout << "List 2 after the copy assignment: " << list2 << std::endl;

    WHEN("Not equally sized lists"){
      MyList<int> list3{1, 10}, list4{3, 4, 10, 25, 11, 100};
      REQUIRE(list3.front() == 1);
      REQUIRE(list3.back() == 10);
      REQUIRE(list3.size() == 2);
      REQUIRE(list4.front() == 3);
      REQUIRE(list4.back() == 100);
      REQUIRE(list4.size() == 6);

      std::cout << "List 3 before the copy assignment: " << list3 << std::endl;
      std::cout << "List 4 before the copy assignment: " << list4 << std::endl;

      list3 = list4;

      REQUIRE(list3.front() == 3);
      REQUIRE(list3.back() == 100);
      REQUIRE(list3.size() == 6);
      REQUIRE(list4.front() == 3);
      REQUIRE(list4.back() == 100);
      REQUIRE(list4.size() == 6);

      std::cout << "List 3 after the copy assignment: " << list3 << std::endl;
      std::cout << "List 4 after the copy assignment: " << list4 << std::endl;
    }
  }

  SCENARIO("Test Scenario for Doubly Linked List with move assignment operator"){
    std::cout << "____________Move assignment operator______________" << std::endl;
    MyList<int> list{4, 14, 44}, list2{121, 9, 3, 27, 16};
    REQUIRE(list.front() == 4);
    REQUIRE(list.back() == 44);
    REQUIRE(list.size() == 3);
    REQUIRE(list2.front() == 121);
    REQUIRE(list2.back() == 16);
    REQUIRE(list2.size() == 5);

    std::cout << "List 1 before the copy assignment: " << list << std::endl;
    std::cout << "List 2 before the copy assignment: " << list2 << std::endl;

    list = std::move(list2);

    REQUIRE(list.front() == 121);
    REQUIRE(list.back() == 16);
    REQUIRE(list.size() == 5);
    REQUIRE(list2.size() == 0);

    std::cout << "List 1 after the copy assignment: " << list << std::endl;
  }

  SCENARIO("Test Scenario for Doubly Linked List for input"){
    std::cout << "____________Input test______________" << std::endl;
    MyList<int> list;
    REQUIRE(list.size() == 0);

    std::cout << "List before the input: " << list << std::endl;

    //string should contain data
    std::string a{"4, 14"};
    std::istringstream iss(a);
    iss >> list;

     std::cout << "List after the input: " << list << std::endl;
  }
