#ifndef FILESYSTEMOBJECT_H
#define FILESYSTEMOBJECT_H
#include<iostream>
#include<vector>
#include<memory>

class FileSystemObject {
    public:
      FileSystemObject(){}
      virtual void printInfo(std::ostream&) = 0;
      virtual void printInfo(std::ostream&, int) = 0;
};

class File : public FileSystemObject {
    public:
      File (std::string);
      void printInfo(std::ostream&);
      void printInfo(std::ostream&, int);

    private:
      std::string name;

};

class Directory : public FileSystemObject {
    public:
      Directory (std::string);
      void addChild(std::unique_ptr<FileSystemObject>);
      void printInfo(std::ostream&);
      void printInfo(std::ostream&, int);

    private:
      std::string name;
      std::vector<std::unique_ptr<FileSystemObject>> vectorOfPointers;
};



#endif // FILESYSTEMOBJECT_H
