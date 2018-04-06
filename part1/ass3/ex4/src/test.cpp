#include <FileSystemObject.h>
#define  CATCH_CONFIG_MAIN
#include "catch.hpp"
#include<vector>
#include<memory>
#include<iostream>

SCENARIO("Test Scenario for a FileSystemObject"){
    Directory dir1 ("file1");
    File file1 ("file1.txt");
    File file2 ("file2.txt");
    Directory dir2 ("dir3");
    std::unique_ptr<File> ptr = std::make_unique<File>("file3.txt");
    std::unique_ptr<File> ptr2 = std::make_unique<File>("file4.txt");
    std::unique_ptr<Directory> ptr3 = std::make_unique<Directory>("dir2");
    std::unique_ptr<File> ptr4 = std::make_unique<File>("file10.txt");
    std::unique_ptr<File> ptr5 = std::make_unique<File>("file11.txt");

    dir1.addChild(std::move(ptr));
    dir1.addChild(std::move(ptr2));
    ptr3->addChild(std::move(ptr4));
    ptr3->addChild(std::move(ptr5));
    dir1.addChild(std::move(ptr3));

    std::ostream ostream(nullptr);
    ostream.rdbuf(std::cout.rdbuf());

    dir1.printInfo(ostream);
}
