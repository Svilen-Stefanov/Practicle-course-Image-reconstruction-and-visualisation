#include <namePairs.h>
#define  CATCH_CONFIG_MAIN
#include "catch.hpp"

SCENARIO("Test Scenario for initialising namePairs"){
    NamePairs namePairs;
    namePairs.setPair("Julian", 22.0);
    namePairs.setPair("Svilen", 20.0);
    namePairs.setPair("Anna", 12.0);
    WHEN("Short list of NamePair"){
      REQUIRE(namePairs.getNamePairs().find("Julian")->second == 22.0);
      REQUIRE(namePairs.getNamePairs().find("Svilen")->second == 20.0);
      REQUIRE(namePairs.getNamePairs().find("Anna")->second == 12.0);
    }

    WHEN("Is sorted"){
      /*Maps are not ment to be used if the user
      might want to get an element at a specified postion,
      but such can be implementet as a next feature*/
      REQUIRE(namePairs.getNamePairs().begin()->first == "Anna");
      REQUIRE(namePairs.getNamePairs().begin()->second == 12.0);
      REQUIRE((++namePairs.getNamePairs().begin())->first == "Julian");
      REQUIRE((++namePairs.getNamePairs().begin())->second == 22.0);
      REQUIRE((++++namePairs.getNamePairs().begin())->first == "Svilen");
      REQUIRE((++++namePairs.getNamePairs().begin())->second == 20.0);
    }
}

SCENARIO("Test Scenario for printing the NamePairs"){
    WHEN("Having different names"){
      NamePairs namePairs;
      namePairs.setPair("Tom", 40.3);
      namePairs.setPair("Kevin", 33.6);
      namePairs.setPair("Anna", 16.7);

      std::ostringstream printStream;
      std::string compareStr;
      compareStr = "(Anna, 16.7)\n(Kevin, 33.6)\n(Tom, 40.3)\n";
      printStream << namePairs;

      std::string compare = printStream.str();
      REQUIRE(compareStr == compare);
    }

    WHEN("Having same names"){
      NamePairs namePairs;
      namePairs.setPair("Tom", 60.3);
      namePairs.setPair("Tom", 50.7);
      namePairs.setPair("Kevin", 20.6);

      std::ostringstream printStream;
      std::string compareStr;
      compareStr = "(Kevin, 20.6)\n(Tom, 60.3)\n(Tom, 50.7)\n";
      printStream << namePairs;

      std::string compare = printStream.str();
      REQUIRE(compareStr == compare);
    }
}
