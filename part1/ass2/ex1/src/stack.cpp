#include "stack.h"
#include <iostream>

    bool Stack::isEmpty(){
        if(stackList.empty())
            return true;
        else return false;
    };

    void Stack::push(int element){
        stackList.push_back(element);
    };

    int Stack::pop(){
        if(stackList.empty()){
          throw std::logic_error("The stack is empty!");
        }
        int popElement = stackList.back();
        stackList.pop_back();
        return popElement;
    };

    int Stack::top(){
        if(stackList.empty()){
          throw std::logic_error("The stack is empty!");
        }
        return stackList.back();
    };
