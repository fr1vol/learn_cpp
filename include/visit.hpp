#pragma once
#include <iostream>

namespace ez
{

namespace normal{

struct dog;
struct bird;

struct visitor{
  virtual void visit(dog*) = 0;
  virtual void visit(bird*) = 0;
};

struct animal{
  
  virtual int move() = 0;
  virtual void accept(visitor*) = 0;
  virtual ~animal() = default;
};

template<typename T>
struct visitable : public animal
{ 
  void accept(visitor* v) override{
    v->visit(static_cast<T*>(this));
  }
};
struct dog : public visitable<dog>
{
    int move() override
    {
      return 4;
    }


    void swim(){
      std::cout<<"swim"<<std::endl;
    }
};

struct bird : public visitable<bird>
{
    int move() override
    {
      return 2;
    }

    void fly()  {
        std::cout<<"fly"<<std::endl;
    }
};

struct visitor_impl : public visitor{
  void visit(dog* d)override{
    d->swim();
  }
  void visit(bird* b) override{
    b->fly();
  }
};


void test() { 

  animal* a = new dog;
  animal* b = new bird;

  visitor* v = new visitor_impl;

  a->accept(v);
  b->accept(v);

}
}

namespace hard
{
    
template<typename T>
struct visitor{
  virtual void visit(T*) = 0;
};

struct token {
    virtual ~token() = default;
};

struct animal{
  
  virtual int move() = 0;
  virtual void accept(token*) = 0;
  virtual ~animal() = default;
};

template<typename T>
struct visitable : public animal
{ 
  void accept(token* v) override{
      if(auto p = dynamic_cast<visitor<T>*>(v) ; p){
          p->visit(static_cast<T*>(this));
      }
  }
};
struct dog : public visitable<dog>
{
    int move() override
    {
      return 4;
    }


    void swim(){
      std::cout<<"swim"<<std::endl;
    }
};

struct bird : public visitable<bird>
{
    int move() override
    {
      return 2;
    }

    void fly()  {
        std::cout<<"fly"<<std::endl;
    }
};

struct fish : public visitable<fish>
{
    int move() override
    {
      return 1;
    }

    void dive()  {
        std::cout<<"dive"<<std::endl;
    }
};

template<typename... T>
struct Mult_visitor : public token ,public visitor<T>...
{

};

using V1 = Mult_visitor<dog,bird>;
using V2 = Mult_visitor<fish>;


struct impl_1 : public V1{
  void visit(dog* d)override{
    d->swim();
  }
  void visit(bird* b) override{
    b->fly();
  }
};

struct impl_2 : public V2{
    void visit(fish* f) override{
        f->dive();
    }
};


void test() { 

  animal* a = new dog;
  animal* b = new bird;
  animal* c = new fish;

  token* v1 = new impl_1;
  token* v2 = new impl_2;
  a->accept(v1);
  b->accept(v1);
  c->accept(v2);

}
}

}