#ifndef STACK_H
#define STACK_H
#include <list>

class Stack{
    public:
        bool isEmpty();
        void push(int element);
        int pop();
        int top();

    private:
        std::list<int> stackList;
};

#endif // STACK_H
