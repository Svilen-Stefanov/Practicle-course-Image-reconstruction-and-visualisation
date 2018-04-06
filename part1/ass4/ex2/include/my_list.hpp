#pragma once
#include <iostream>
#include<iterator>
#include<memory>

template <typename T, class Allocator = std::allocator<T>>
class MyList {

private:
    /*element in MyList*/
    class Link {
      public:
        Link* prev;
        Link* next;
        T element;

        Link() : next(nullptr), prev(nullptr) {}
        Link(T val, Link *next, Link *prev) : element(val), next(next), prev(prev) {}
  		  Link(Link &link) : element(link->element), next(link->next), prev(link->prev) {} // copy constructor
    		Link(Link &&link) : element(std::move(link->element)), next(std::move(link->next)), prev(std::move(link->prev)) {} // move constructor
    };

    Link* head;
    Link* last;
    Link* afterLast;
    int sizeOfList = 0;
    bool afterLastCreated = false;
public:

    /*used to iterate through the MyList*/
    class iterator : public std::iterator<std::bidirectional_iterator_tag, T> {

      public:
        typedef std::ptrdiff_t difference_type;
        typedef T value_type;
        typedef T& reference;
        typedef T* pointer;
        typedef std::bidirectional_iterator_tag iterator_category;
        Link* ptr;

        iterator() : ptr(nullptr) {}
        iterator(Link* val) : ptr(val) {}
        iterator(const iterator& copyVal) : ptr(copyVal.ptr) {}
    		~iterator() {ptr = nullptr;}

        iterator& operator++() {ptr = ptr->next; return *this;}
        iterator operator++(int) {iterator iter(*this); operator++(); return iter;}
        iterator& operator--() {ptr = ptr->prev; return *this;}
        iterator operator--(int) {iterator iter(*this); operator--(); return iter;}
        inline bool operator==(const iterator& cmp) {return ptr==cmp.ptr;}
        inline bool operator!=(const iterator& cmp) {return ptr!=cmp.ptr;}
        T& operator*() {return ptr->element;}
        Link* getLink() const {return ptr;}
    };


    MyList() : head(nullptr), last(nullptr), afterLast(nullptr), sizeOfList(0), afterLastCreated(false) {}

    MyList(std::initializer_list<T> l) {
        init(l);
    }

    MyList(const MyList<T> &lst) : head(nullptr), last(nullptr), afterLast(nullptr), sizeOfList(0){    //copy constructor
      if(!lst.empty()){
        // Create all links;
        auto itr = begin();
        afterLast = new Link(lst.afterLast->element, nullptr, nullptr);
        afterLastCreated = true;
        for (auto it = lst.begin(); it != lst.end(); it++) {
          insert(itr, *it);
          itr = end();
        }
      }
    }

    MyList(MyList<T> &&lst) : head(nullptr), last(nullptr), afterLast(nullptr), sizeOfList(0){   //move constructor
      if(!lst.empty()){
        auto itr = begin();
        afterLast = new Link(lst.afterLast->element, nullptr, nullptr);
        afterLastCreated = true;
        for (auto it = lst.begin(); it != lst.end(); it++) {
          insert(itr, *it);
          itr = end();
        }
        lst.erase(lst.begin(), lst.end());
      }
    }

    MyList& operator=(std::initializer_list<T> l) {   //list assignment
      if(!empty()){     //delete elements in the old list before the new initialization
        erase(begin(), end());
        if(afterLastCreated) delete afterLast;
      }
      init(l);
      return *this;
    }

    MyList& operator=(const MyList<T> &lst) {     //copy assignment operator
      if (this != &lst) {
        if (!empty()){        //delete elements in the old list before the new initialization
            erase(begin(), end());
            if(afterLastCreated) delete afterLast;
        }
        setNull();
        if(!lst.empty()){
          auto itr = begin();
          afterLast = new Link(lst.afterLast->element, nullptr, nullptr);
          afterLastCreated = true;
          for (auto it = lst.begin(); it != lst.end(); it++) {
            insert(itr, *it);
            itr = end();
          }
        }
      }
      return *this;
    }

    MyList& operator=(MyList<T> &&lst) {    //move assignment operator
      if (this != &lst) {
        if (!empty()){    //delete elements in the old list before the new initialization
            erase(begin(), end());
            if(afterLastCreated) delete afterLast;
        }
        setNull();
        if(!lst.empty()){
          auto itr = begin();
          afterLast = new Link(lst.afterLast->element, nullptr, nullptr);
          afterLastCreated = true;
          for (auto it = lst.begin(); it != lst.end(); it++) {
            insert(itr, *it);
            itr = end();
          }
          lst.erase(lst.begin(), lst.end());
        }
      }
      return *this;
    }

    ~MyList() {
        if(!empty()) erase(begin(), end());
        if(afterLastCreated) delete afterLast;
    }

    iterator begin() {
      return iterator(head);
    }

    iterator begin() const {
 		   return iterator(head);
 	  }

    iterator end() {
      return iterator(afterLast);
    }

    iterator end() const {
      return iterator(afterLast);
 	  }

    void init(std::initializer_list<T> l){
      setNull();
      if(l.size() != 0){
        for (auto it : l){
            push_back(it);
        }
      }
    }

    /*makes sure that MyList is in initial state*/
    void setNull(){
      head = nullptr;
      last = nullptr;
      afterLast = nullptr;
      sizeOfList = 0;
    }

    void push_back(const T& t) {
      insert(end(), t);
    }

    void push_back(T&& t) {
      insert(end(), t);
    }

    void push_front(const T& t) {
      insert(begin(), t);
    }

    void push_front(T&& t) {
      insert(begin(), t);
    }

    void pop_back() {
      if (empty()) {
        throw std::runtime_error("List is empty");
      } else {
        erase(end());
      }
    }

    void pop_front() {
      if (empty()) {
        throw std::runtime_error("List is empty");
      } else {
        erase(begin());
      }
    }

    T& front() {
      if (empty()) {
   			throw std::runtime_error("List is empty");
   		} else {
   			return head->element;
   		}
    }

    const T& front() const {
      if (empty()) {
   			throw std::runtime_error("List is empty");
   		} else {
   			return head->element;
   		}
    }

    T& back() {
      if (empty()) {
   			throw std::runtime_error("List is empty");
   		} else {
   			return last->element;
   		}
    }

    const T& back() const {
      if (empty()) {
   			throw std::runtime_error("List is empty");
   		} else {
   			return last->element;
   		}
    }

    std::size_t size() const {
        return sizeOfList;
    }

    bool empty() const {
      return sizeOfList == 0;
    }

    /*Insert element at the specified position*/
    iterator insert(const iterator position, const T &value) {
      Link *nextElem = position.getLink();
      Link *newLink = nextElem;

      if (nextElem == nullptr){           //insert first element in the list
        newLink = new Link(value, nullptr, nullptr);
        head = newLink;
        last = head;
        if (!afterLastCreated){
             afterLast = new Link(value, nullptr, last);
             afterLastCreated = true;
        } else afterLast->prev = last;
        last->next = afterLast;
      } else {               //elements present
        Link *prevElem = nextElem->prev;
        Link *newLink = new Link(value, nextElem, prevElem);
        nextElem->prev = newLink;
        if (prevElem != nullptr)        //if prev exists
          prevElem->next = newLink;
        if (nextElem == head){           //set head
          head = newLink;
        } else if (nextElem == afterLast){      //set last
          last = newLink;
        }
      }
      sizeOfList++;

      return iterator(newLink);
    }

    /*insert element with value n times at position*/
    iterator insert(const iterator position, unsigned int n, const T &value) {
      //I assume n>=1
      for (size_t i = 0; i < n; i++) {
        insert(position, value);
      }
      return position;
    }

    /*insert elements from first to last of the first list at the position in the second list*/
    iterator insert(const iterator position, const iterator first, const iterator last) {
        //insert from position position the values from first to last
        auto it = first;
        Link *currElem;
        while(it != last){
          currElem = it.getLink();
          insert(position, currElem->element);
          it++;
        }
        return position;
    }

    /*erase element at the specified position*/
    iterator erase(const iterator position) {
      if(!empty()){
        Link* posElem = position.getLink();
        Link* nextElem = posElem->next;

        if (head == last){
          delete head;
          delete afterLast;
          afterLastCreated = false;
          setNull();
          return iterator();
        } else {
          Link *prevElem = posElem->prev;
          if (nextElem != nullptr)
            nextElem->prev = prevElem;
          if (prevElem != nullptr)              //if prevElem exists
            prevElem->next = nextElem;
          if (posElem == head){           //set head
            head = nextElem;
          } else if (posElem == afterLast){      //set last
            last = prevElem->prev;
            afterLast = prevElem;
            nextElem = afterLast;
          }
        }

        sizeOfList--;
        delete posElem;
        return iterator(nextElem);
      }
      return iterator();
    }

    /*erase element from first to last*/
    iterator erase(const iterator first, const iterator last) {
      auto it = first;
        while(it!=last && it!=nullptr){
          it = erase(it);
        }
      return it;
    }

    /*prints the elements of the list*/
    friend std::ostream& operator<<(std::ostream &out, const MyList<T> &ml) {
      if(!ml.empty()){
        for (auto it = ml.begin(); it != ml.end(); it++) {
          if(it!=--ml.end())
            out << *it << ",";
          else out << *it;
        }
      }
      return out;
    }

    /*insert elements in the list from input*/
    friend std::istream& operator>>(std::istream &input, MyList<T> &ml) {
      //read from  the input
      T data;
      std::istream::sentry s(input);
      while (input >> data) {
      	ml.push_back(data);
      	if (input.peek() == ',') {
      		input.ignore();
      	}
      }
      return input;
    }
};
