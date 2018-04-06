#include "catch.hpp"

#include <string>
#include <iostream>
#include <vector>
#include <Eigen/Dense>
#include <Eigen/Geometry>

#include <math/Volume.h>
#include <math/Detector.h>
#include <math/Raytracer.h>


SCENARIO("The Raytracer class behaves like it should") {
    //setup volume
    Eigen::Vector3f min(10,10,0);
    Eigen::Vector3f max(20,20,10);
    Eigen::Vector3i resolution(10,10,10);
    Eigen::Vector3f sp(1,1,1);
    Volume_ptr vol = std::make_shared<Volume>(min,max,sp,resolution);
    Eigen::Vector3f source(-5,5,5);
    Raytracer actrace(vol, Detector::Ray_t(Eigen::Vector3f(0,0,0), Eigen::Vector3f(1,0,0))); //dummy ray
    
    GIVEN("A ray that does intersect the volume") {
        Eigen::Vector3f target1(10,10,5);
        Eigen::Vector3f normalizedDirection1 = target1-source;
        normalizedDirection1.normalize();
        normalizedDirection1 = Detector::checkForZero(normalizedDirection1);
        
        Eigen::ParametrizedLine<float, 3> ray2 = Eigen::ParametrizedLine<float,3>(source,normalizedDirection1);

        Eigen::Vector2f result2 = actrace.rayBoxIntersection(ray2);
        THEN("the Raytracer should return the distance") {
            REQUIRE(result2[0] == Approx(15.81139));
            REQUIRE(result2[1] == Approx(26.35231));
        }
}

    GIVEN("A ray that doesn't intersect the volume") {
        Eigen::Vector3f target1(-2,-2,-5);
        Eigen::Vector3f normalizedDirection1 = target1-source;
        normalizedDirection1.normalize();
        normalizedDirection1 = Detector::checkForZero(normalizedDirection1);
        
        Eigen::ParametrizedLine<float, 3> ray2 = Eigen::ParametrizedLine<float,3>(source,normalizedDirection1);

        Eigen::Vector2f result2 = actrace.rayBoxIntersection(ray2);
        THEN("the Raytracer should return -1,-1") {
            REQUIRE(result2[0] == Approx(-1));
            REQUIRE(result2[1] == Approx(-1));
        }
}

    GIVEN("A ray that touches the volume") {
        //ray is touching
        WHEN("a ray is touching") {
           Eigen::Vector3f target1(10,10,10);
            Eigen::Vector3f normalizedDirection1 = target1-source;
            normalizedDirection1.normalize();
            normalizedDirection1 = Detector::checkForZero(normalizedDirection1);
        
            Eigen::ParametrizedLine<float, 3> ray2 = Eigen::ParametrizedLine<float,3>(source,normalizedDirection1);

            Eigen::Vector2f result2 = actrace.rayBoxIntersection(ray2);
            THEN("the Raytracer should return -1,-1") {
                REQUIRE(result2[0] == Approx(-1));
                REQUIRE(result2[1] == Approx(-1));
            }
        }
    
    GIVEN("A ray that intersects the volume") {
        THEN("the acraytracer should not return -1")
        for (int z = 1; z < 9; z++) {
            Eigen::Vector3f target(10,10,z);
            Eigen::Vector3f normalizedDirection = target-source;
            normalizedDirection.normalize();
            normalizedDirection = Detector::checkForZero(normalizedDirection);
            Eigen::ParametrizedLine<float, 3> ray1 = Eigen::ParametrizedLine<float,3>(source,normalizedDirection);
            Eigen::Vector2f result1 = actrace.rayBoxIntersection(ray1);
            //std::cout << "result for x" << x << " and y" << ymin+1 << "and z " << z << " is : "<< result1 << std::endl;
            REQUIRE(result1[0] != Approx(-1));
            REQUIRE(result1[1] != Approx(-1));
            }
        }
    }
}

