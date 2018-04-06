#include <Tracer.h>
#define  CATCH_CONFIG_MAIN
#include "catch.hpp"
#include<string>

Tracer test ("Initializing a global constructor!");

SCENARIO("Test Scenario for Tracer"){
      REQUIRE(test.string == "Initializing a global constructor!");
      std::cout << std::endl << "______________Test Scenario for Tracer____________________" << std::endl << std::endl;
      Tracer tracer1 ("This is a normal constructor!");
      Tracer tracer2 = tracer1;       //copy constructor
      REQUIRE(tracer2.string == tracer1.string);

      Tracer* tracer4 = new Tracer ("Constructor with memory allocation!");

      Tracer tracer3;
      //empty constructor
      REQUIRE(tracer3.string == "");
      tracer3 = tracer1;        //copy assignment operator
      REQUIRE(tracer3.string == tracer1.string);

      std::string tracer3String = tracer3.string;
      //tracer3 before move
      REQUIRE(tracer3.string == tracer3String);
      Tracer tracer5 = std::move(tracer3);      //move constructor
      REQUIRE(tracer5.string == tracer3String);
      //tracer3 after move
      REQUIRE(tracer3.string == "");

      Tracer tracer6;
      std::string tracer5String = tracer5.string;
      //tracer5 before move
      REQUIRE(tracer5.string == tracer5String);
      tracer6 = std::move(tracer5);   //move assignment operator

      REQUIRE(tracer6.string == tracer5String);
      //tracer5 before move
      REQUIRE(tracer5.string == "");

      delete tracer4;

      std::cout << std::endl << "_______Objects, which are deleted after the termination of the test_______" << std::endl << std::endl;
}

SCENARIO("Test Scenario for Tracer with exceptions"){
      std::cout << std::endl << "______________Test Scenario for Tracer with exceptions____________________" << std::endl << std::endl;

      Tracer tracer1 ("This is a normal constructor!");
      REQUIRE(tracer1.string == "This is a normal constructor!");
      Tracer tracer2 = tracer1;

      Tracer* tracer4 = new Tracer ("Constructor with memory allocation!");
      std::string tracer4String = tracer4->string;
      Tracer tracer3;
      try {
        throw std::logic_error("Throw an exception to see how object respond!");

      //test4 won't be deleted -> it leads to memory leaps
      delete tracer4;

      tracer3 = tracer1;

      Tracer tracer5 = std::move(tracer3);

      Tracer tracer6;
      tracer6 = std::move(tracer5);
    } catch(std::logic_error ex){
        //tracer 4 is still alive
        REQUIRE(tracer4->string == tracer4String);
    }
    //tracer3 didn't change its value
    REQUIRE(tracer3.string == "");
}

SCENARIO("Test Scenario for Tracer with exceptions with deletion after the try/catch block"){
      std::cout << std::endl << "_________Test Scenario for Tracer with exceptions (smaller try block)___________" << std::endl << std::endl;

      Tracer tracer1 ("This is a normal constructor!");
      Tracer tracer2 = tracer1;
      REQUIRE(tracer2.string == tracer1.string);

      Tracer* tracer4 = new Tracer ("Constructor with memory allocation!");
      std::string tracer4String = tracer4->string;
      try {
        throw std::logic_error("Throw an exception to see how object respond!");
        Tracer tracer5 = std::move(tracer2);
      } catch(std::logic_error ex){
        //move didn't happen
        REQUIRE(tracer2.string == tracer1.string);
      }

      REQUIRE(tracer4->string == tracer4String);
      delete tracer4;
}
