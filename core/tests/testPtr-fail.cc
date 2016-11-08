#include "milan/core/Ptr.hh"

#include "gtest/gtest.h"

int main()
{
    using namespace milan;
    
    class BaseClass
    {
    };
    
    class DerivedClass
    {
    };
    
    Ptr<DerivedClass> ptr(PtrStorage::OWN,new DerivedClass());
    
    #ifdef COPY
    Ptr<BaseClass> ptr1(ptr);
    #endif
    
    #ifdef COPY_ASSIGN
    Ptr<BaseClass> ptr2=ptr;
    #endif
    
    #ifdef MOVE
    Ptr<BaseClass> ptr5(std::move(ptr));
    #endif
    
    #ifdef MOVE_ASSIGN
    Ptr<BaseClass> ptr6=std::move(ptr1);
    #endif
    
    return 0;
}





