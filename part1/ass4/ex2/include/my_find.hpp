#include <iostream>
#include <iterator>

template <typename Iterator, typename T>
Iterator myfind(Iterator begin, Iterator end, T val){
   Iterator it;
   for(it = begin; it != end; ++it) {
     if(*it == val)
       break;
   }
   return it;
}
