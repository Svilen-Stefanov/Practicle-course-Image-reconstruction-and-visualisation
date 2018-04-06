#ifndef NAMEPAIRS_H
#define NAMEPAIRS_H
#include<vector>
#include<iostream>

class NamePairs
{
    public:
        void setNames(std::vector<std::string>);
        void setAges(std::vector<double>);
        std::vector<std::string> getNames() const;
        std::vector<double> getAges() const;
        void sort();

    private:
        std::vector<std::string> Names;
        std::vector<double> Ages;

};
    
    std::ostream& operator<<(std::ostream& ostream, NamePairs& namePairs);;

#endif // NAMEPAIRS_H
