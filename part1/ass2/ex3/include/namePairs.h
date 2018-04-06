#ifndef NAMEPAIRS_H
#define NAMEPAIRS_H
#include<map>
#include<iostream>

class NamePairs
{
    public:
        void setPair(std::string, double);
        //sort isn't needed, because a map sorts the keys automatically (can be implemented if values also need to be sorted)
        std::multimap<std::string, double> getNamePairs() const;
        /*Maps are not ment to be used if the user
        might want to get an element at a specified postion,
        but such can be implementet as a next feature*/

    private:
        std::multimap<std::string, double> namePairs;
};

    std::ostream& operator<<(std::ostream& ostream, NamePairs& namePairs);;

#endif // NAMEPAIRS_H
