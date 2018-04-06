#include <Rational.h>
#define  CATCH_CONFIG_MAIN
#include "catch.hpp"

SCENARIO("Test Scenario for initializing a constructor"){

    GIVEN("Initializing a new constructor with values"){
      Rational r = Rational(2, 3);

      REQUIRE(r.getNum() == 2);
      REQUIRE(r.getDenom() == 3);
    }

    GIVEN("Initializing a construcor with setters"){
      //the previous test show that get works
      Rational r;
      r.setNum(2);
      r.setDenom(3);

      REQUIRE(r.getNum() == 2);
      REQUIRE(r.getDenom() == 3);

      r = Rational(4, 5);
      REQUIRE(r.getNum() == 4);
      REQUIRE(r.getDenom() == 5);
    }
}

SCENARIO("Test Scenario for Operators"){

  Rational r1 = Rational(2, 3);
  Rational r2 = Rational(3, 4);
  Rational r3;

  GIVEN("Assignment of a rational number"){
    r3 = r1;
    REQUIRE(r3.getNum() == r1.getNum());
    REQUIRE(r3.getDenom() == r1.getDenom());
  }

  GIVEN("Arithmetic operations with assignment"){
    WHEN("Addition of two rational numbers"){
      //the proper function of assignment is quarantied with the previous test
      r3 = r1+r2;
      REQUIRE(r3.getNum() == 17);
      REQUIRE(r3.getDenom() == 12);
    }

    WHEN("Subtraction of two rational numbers"){
      r3 = r2-r1;
      REQUIRE(r3.getNum() == 1);
      REQUIRE(r3.getDenom() == 12);
    }

    WHEN("Multiplication of two rational numbers"){
      r3 = r1*r2;
      REQUIRE(r3.getNum() == 6);
      REQUIRE(r3.getDenom() == 12);
    }

    WHEN("Devision of two rational numbers"){
      r3 = r2/r1;
      REQUIRE(r3.getNum() == 9);
      REQUIRE(r3.getDenom() == 8);
    }
  }

  GIVEN("Equality operators"){
      Rational testEqual = Rational (2,3);

      WHEN("Equal"){
        REQUIRE(r1==testEqual);
      }

      WHEN("Not equal"){
        REQUIRE(r1!=r2);
      }
  }

  GIVEN("Double operator overloading"){
      Rational test = Rational(1, 2);
      double result = (double)test;
      REQUIRE(result == 0.5);
      REQUIRE(result != 1);

      Rational test2 = Rational(5, 6);
      double result2 = double(test2);
      double actResult = 5.0/6;
      REQUIRE(result2 == actResult);
  }
}

SCENARIO("Test Scenario for error handling"){
    Rational test2 = Rational (11, 12);
    Rational test3 = Rational (0, 15);

    //TODO
    GIVEN("Initialization of constructor with denominator 0"){
        REQUIRE_THROWS_AS(Rational(1, 0), std::logic_error);
    }

    GIVEN("Allowed operations with 0"){
      WHEN("Addition"){
        REQUIRE(test2+test3 == test2);
      }

      WHEN("Subtraction"){
        REQUIRE(test2-test3 == test2);
      }

      WHEN("Multiplication"){
        REQUIRE(test2*test3==0);
      }
    }

    GIVEN("The denominator by devision of 2 rational numbers is 0"){
        REQUIRE_THROWS_AS(test2/test3, std::logic_error);
    }
}
