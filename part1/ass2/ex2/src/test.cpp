#include <NamePairs.h>
#define  CATCH_CONFIG_MAIN
#include "catch.hpp"

NamePairs namePairs;
SCENARIO("Test Scenario for one NamePair"){

  std::vector<std::string> newNames {"Tom"};
  std::vector<double> newAges {60};

  namePairs.setNames(newNames);
  namePairs.setAges(newAges);
  GIVEN("Initialisation of a NamePair"){
    REQUIRE(namePairs.getNames() == newNames);
    REQUIRE(namePairs.getAges() == newAges);
  }

  namePairs.sort();
  GIVEN("Sorting the pair"){
    REQUIRE(namePairs.getNames() == newNames);
    REQUIRE(namePairs.getAges() == newAges);
  }
}

SCENARIO("Test Scenario for short NamePairs"){
    std::vector<std::string> newNames {"Tom","Kevin", "Anna", "David"};
    std::vector<double> newAges {60,12,10,5};
    std::vector<std::string> sortedNames {"Anna", "David", "Kevin", "Tom"};
    std::vector<double> sortedAges {10,5,12,60};

    GIVEN("Initialisation of a NamePair"){
      namePairs.setNames(newNames);
      REQUIRE(namePairs.getNames() == newNames);
      namePairs.setAges(newAges);
      REQUIRE(namePairs.getAges() == newAges);
    }

    namePairs.setNames(newNames);
    namePairs.setAges(newAges);
    GIVEN("Sorting of a NamePair"){
      namePairs.sort();
      REQUIRE(namePairs.getNames() == sortedNames);
      REQUIRE(namePairs.getAges() == sortedAges);

    }
}

SCENARIO("Test Scenario for large NamePairs"){
  std::vector<std::string> newNames { "Camie", "Gipe", "Tom", "Alice", "Dale", "Michal", "Araujo", "Steuck", "Martina", "Kevin", "Anna", "David"};
  std::vector<double> newAges {13, 16, 21, 11, 14, 19, 12, 20, 18, 17, 10, 15};

  std::vector<std::string> sortedNames {"Alice", "Anna", "Araujo", "Camie", "Dale", "David", "Gipe", "Kevin", "Martina", "Michal", "Steuck", "Tom"};
  std::vector<double> sortedAges {11, 10, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21};

  GIVEN("Initialisation of a NamePair"){
    namePairs.setNames(newNames);
    REQUIRE(namePairs.getNames() == newNames);
    namePairs.setAges(newAges);
    REQUIRE(namePairs.getAges() == newAges);
  }

  namePairs.setNames(newNames);
  namePairs.setAges(newAges);
  GIVEN("Sorting of a NamePair"){
    namePairs.sort();
    REQUIRE(namePairs.getNames() == sortedNames);
    REQUIRE(namePairs.getAges() == sortedAges);
  }
}

/* Vectors with same name*/
SCENARIO("Scenario for list with same names"){
    std::vector<std::string> newSameNames {"Tom","Tom", "Tom", "Anna"};
    std::vector<double> newSameAges {60,12,10,5};
    std::vector<std::string> sortedSameNames {"Anna", "Tom", "Tom", "Tom"};
    std::vector<double> sortedSameAges {5,60,12,10};

    namePairs.setNames(newSameNames);
    namePairs.setAges(newSameAges);
    namePairs.sort();
    GIVEN("Sorting NamePairs with same names and different ages"){
      REQUIRE(namePairs.getNames() == sortedSameNames);
      REQUIRE(namePairs.getAges() == sortedSameAges);
    }
}


SCENARIO("Test Scenario for printing the NamePairs"){
  std::vector<std::string> names {"Tom","Kevin"};
  std::vector<double> ages {60,12};
  namePairs.setNames(names);
  namePairs.setAges(ages);

  std::ostringstream printStream;
  std::string compareStr;
  compareStr = "(Tom, 60)\n(Kevin, 12)\n";
  printStream << namePairs;

  std::string compare = printStream.str();

  REQUIRE(compareStr == compare);
}

SCENARIO("Test Scenario for error handling"){
  GIVEN("An emthy class"){
      std::vector<std::string> newNames {};
      std::vector<double> newAges {};
      namePairs.setNames(newNames);
      namePairs.setAges(newAges);
      namePairs.sort();
      /*The class should remain empthy and should be able to recognise that there is nothing to be sorted*/
      REQUIRE(namePairs.getNames() == newNames);
      REQUIRE(namePairs.getAges() == newAges);
  }

  std::vector<std::string> newNames {"Tom","Kevin", "Anna"};
  std::vector<double> newAges {60,12,10,5};

  namePairs.setNames(newNames);
  namePairs.setAges(newAges);
  GIVEN("Try to sort a NamePairs with different size of names and ages"){
      REQUIRE_THROWS_AS(namePairs.sort(), std::logic_error);
  }

  GIVEN("Try to print NamePairs with different size of names and ages"){
      std::vector<std::string> names {"Tom","Kevin"};
      std::vector<double> ages {60,12, 20};
      namePairs.setNames(names);
      namePairs.setAges(ages);

      std::ostringstream printStream;
      /*throws exception when not of equal size*/;
      REQUIRE_THROWS_AS(printStream << namePairs, std::logic_error);
  }
}
