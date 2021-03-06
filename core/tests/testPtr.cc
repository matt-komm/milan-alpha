#include "milan/core/Ptr.hh"

#include "gtest/gtest.h"

TEST(Ptr_shared, access)
{
    using namespace milan;
    int value = 0;
    Ptr<int> ptr(PtrStorage::SHARE,&value);
    
    EXPECT_EQ(bool(ptr),true);
    EXPECT_EQ(ptr.use_count(),1);
    EXPECT_EQ(*ptr,0);
    value = 1;
    EXPECT_EQ(*ptr,1);
    *ptr = 2;
    EXPECT_EQ(*ptr,2);
    EXPECT_EQ(value,2);
}

TEST(Ptr_owned, access)
{
    using namespace milan;
    int* value = new int(0);
    Ptr<int> ptr(PtrStorage::OWN,value);
    
    EXPECT_EQ(bool(ptr),true);
    EXPECT_EQ(ptr.use_count(),1);
    EXPECT_EQ(*ptr,0);
    *value = 1;
    EXPECT_EQ(*ptr,1);
    *ptr = 2;
    EXPECT_EQ(*ptr,2);
    EXPECT_EQ(*value,2);
}

TEST(Ptr_shared, access_constptr)
{
    using namespace milan;
    int value = 0;
    const Ptr<int> ptr(PtrStorage::SHARE,&value);
    
    EXPECT_EQ(bool(ptr),true);
    EXPECT_EQ(ptr.use_count(),1);
    EXPECT_EQ(*ptr,0);
    value = 1;
    EXPECT_EQ(*ptr,1);
}

TEST(Ptr_owned, access_constptr)
{
    using namespace milan;
    int* value = new int(0);
    const Ptr<int> ptr(PtrStorage::OWN,value);
    
    EXPECT_EQ(bool(ptr),true);
    EXPECT_EQ(ptr.use_count(),1);
    EXPECT_EQ(*ptr,0);
    *value = 1;
    EXPECT_EQ(*ptr,1);
}

TEST(Ptr_shared, access_constvalue)
{
    using namespace milan;
    int value = 0;
    Ptr<const int> ptr(PtrStorage::SHARE,&value);
    
    EXPECT_EQ(bool(ptr),true);
    EXPECT_EQ(ptr.use_count(),1);
    EXPECT_EQ(*ptr,0);
    value = 1;
    EXPECT_EQ(*ptr,1);
}

TEST(Ptr_owned, access_constvalue)
{
    using namespace milan;
    int* value = new int(0);
    Ptr<const int> ptr(PtrStorage::OWN,value);
    
    EXPECT_EQ(bool(ptr),true);
    EXPECT_EQ(ptr.use_count(),1);
    EXPECT_EQ(*ptr,0);
    *value = 1;
    EXPECT_EQ(*ptr,1);
}

TEST(Ptr_shared, sharing_copy)
{
    using namespace milan;
    int v1 = 0;
    Ptr<int> p1(PtrStorage::SHARE,&v1);
    
    {
        Ptr<int> p2(p1);
        EXPECT_EQ(bool(p1),true);
        EXPECT_EQ(p1.use_count(),2);
        EXPECT_EQ(bool(p2),true);
        EXPECT_EQ(p2.use_count(),2);
    }
    
    EXPECT_EQ(bool(p1),true);
    EXPECT_EQ(p1.use_count(),1);
}

TEST(Ptr_owned, sharing_copy)
{
    using namespace milan;
    int* v1 = new int(0);
    Ptr<int> p1(PtrStorage::OWN,v1);
    
    {
        Ptr<int> p2(p1);
        EXPECT_EQ(bool(p1),true);
        EXPECT_EQ(p1.use_count(),2);
        EXPECT_EQ(bool(p2),true);
        EXPECT_EQ(p2.use_count(),2);
    }
    
    EXPECT_EQ(bool(p1),true);
    EXPECT_EQ(p1.use_count(),1);
}

TEST(Ptr_shared, sharing_reset)
{
    using namespace milan;
    int v1 = 0;
    Ptr<int> p1(PtrStorage::SHARE,&v1);
    
    {
        Ptr<int> p2(p1);
        EXPECT_EQ(p1.use_count(),2);
        EXPECT_EQ(p2.use_count(),2);
        p2.reset();
        EXPECT_EQ(p1.use_count(),1);
        EXPECT_EQ(p2.use_count(),1);
        int v2 = 2;
        p2.reset(&v2);
        EXPECT_EQ(*p1,0);
        EXPECT_EQ(*p2,2);
    }
    p1.reset();
    EXPECT_EQ(bool(p1),false);
    EXPECT_EQ(p1.use_count(),1);
}

TEST(Ptr_owned, sharing_reset)
{
    using namespace milan;
    int* v1 = new int(0);
    Ptr<int> p1(PtrStorage::OWN,v1);
    
    {
        Ptr<int> p2(p1);
        EXPECT_EQ(p1.use_count(),2);
        EXPECT_EQ(p2.use_count(),2);
        p2.reset();
        EXPECT_EQ(p1.use_count(),1);
        EXPECT_EQ(p2.use_count(),1);
        int* v2 = new int(2);
        p2.reset(v2);
        EXPECT_EQ(*p1,0);
        EXPECT_EQ(*p2,2);
    }
    p1.reset();
    EXPECT_EQ(bool(p1),false);
    EXPECT_EQ(p1.use_count(),1);
}

TEST(Ptr_shared, copy)
{
    using namespace milan;
    int value = 0;
    Ptr<int> ptr1(PtrStorage::SHARE,&value);
    Ptr<int> ptr2(ptr1);
    EXPECT_EQ(ptr1.get(),ptr2.get());
}

TEST(Ptr_owned, copy)
{
    using namespace milan;
    int* value = new int(0);
    Ptr<int> ptr1(PtrStorage::OWN,value);
    Ptr<int> ptr2(ptr1);
    EXPECT_EQ(ptr1.get(),ptr2.get());
}

TEST(Ptr_shared, assign)
{
    using namespace milan;
    int value1 = 1;
    int value2 = 2;
    Ptr<int> ptr1(PtrStorage::SHARE,&value1);
    Ptr<int> ptr2(PtrStorage::SHARE,&value2);
    ptr1=ptr2;
    EXPECT_EQ(ptr1.get(),ptr2.get());
}

TEST(Ptr_owned, assign)
{
    using namespace milan;
    Ptr<int> ptr1(PtrStorage::OWN,new int(1));
    Ptr<int> ptr2(PtrStorage::OWN,new int(2));
    ptr1=ptr2;
    EXPECT_EQ(ptr1.get(),ptr2.get());
    EXPECT_EQ(ptr1.storage(),ptr2.storage());
}

TEST(Ptr_mixed, assign)
{
    using namespace milan;
    int value1 = 1;
    Ptr<int> ptr1(PtrStorage::SHARE,&value1);
    Ptr<int> ptr2(PtrStorage::OWN,new int(2));
    Ptr<int> ptr3(PtrStorage::SHARE,&value1);
    ptr1=ptr2;
    EXPECT_EQ(ptr1.get(),ptr2.get());
    EXPECT_EQ(ptr1.storage(),ptr2.storage());
    EXPECT_EQ(ptr1.storage(),PtrStorage::OWN);
    
    ptr1=ptr3;
    EXPECT_EQ(ptr1.get(),ptr3.get());
    EXPECT_EQ(ptr1.storage(),ptr3.storage());
    EXPECT_EQ(ptr1.storage(),PtrStorage::SHARE);
}

TEST(Ptr_shared, move)
{
    using namespace milan;
    int value1 = 1;
    int value2 = 2;
    Ptr<int> ptr1(PtrStorage::SHARE,&value1);
    {
        Ptr<int> ptr2(PtrStorage::SHARE,&value2);
        EXPECT_EQ(ptr1.get(),&value1);
        EXPECT_EQ(ptr2.get(),&value2);
        
        ptr1=std::move(ptr2);
        EXPECT_EQ(ptr1.get(),&value2);
        EXPECT_EQ(ptr2.get(),nullptr);
        EXPECT_EQ(ptr1.use_count(),1);
        EXPECT_EQ(ptr2.use_count(),1);
        
        Ptr<int> ptr3(ptr1);
        EXPECT_EQ(ptr1.use_count(),2);
        EXPECT_EQ(ptr2.use_count(),1);
    }
    EXPECT_EQ(bool(ptr1),true);
    EXPECT_EQ(ptr1.get(),&value2);
    EXPECT_EQ(ptr1.use_count(),1);
}

TEST(Ptr_owned, move)
{
    using namespace milan;
    int* value1 = new int(1);
    int* value2 = new int(2);
    Ptr<int> ptr1(PtrStorage::OWN,value1);
    {
        Ptr<int> ptr2(PtrStorage::OWN,value2);
        EXPECT_EQ(ptr1.get(),value1);
        EXPECT_EQ(ptr2.get(),value2);
        
        ptr1=std::move(ptr2);
        EXPECT_EQ(ptr1.get(),value2);
        EXPECT_EQ(ptr2.get(),nullptr);
        EXPECT_EQ(ptr1.use_count(),1);
        EXPECT_EQ(ptr2.use_count(),1);
        
        Ptr<int> ptr3(ptr1);
        EXPECT_EQ(ptr1.use_count(),2);
        EXPECT_EQ(ptr2.use_count(),1);
    }
    EXPECT_EQ(bool(ptr1),true);
    EXPECT_EQ(ptr1.get(),value2);
    EXPECT_EQ(ptr1.use_count(),1);
}

TEST(Ptr_mixed, move)
{
    using namespace milan;
    int value1 = 1;
    Ptr<int> ptr1(PtrStorage::SHARE,&value1);
    Ptr<int> ptr2(PtrStorage::OWN,new int(2));
    Ptr<int> ptr3(PtrStorage::SHARE,&value1);
    ptr1=std::move(ptr2);
    EXPECT_EQ(*ptr1,2);
    EXPECT_EQ(ptr1.storage(),PtrStorage::OWN);
    EXPECT_EQ(ptr2.get(),nullptr);
    EXPECT_EQ(ptr1.use_count(),1);
    EXPECT_EQ(ptr2.use_count(),1);
    
    ptr1=std::move(ptr3);
    EXPECT_EQ(*ptr1,1);
    EXPECT_EQ(ptr1.storage(),PtrStorage::SHARE);
}


TEST(Ptr_share, upcast)
{
    using namespace milan;
    
    class BaseClass
    {
    };
    
    class DerivedClass: public BaseClass
    {
    };
    
    DerivedClass dobj;
    Ptr<DerivedClass> ptr(PtrStorage::SHARE,&dobj);
    EXPECT_EQ(ptr.use_count(),1);
    
    Ptr<BaseClass> ptr1(ptr);
    EXPECT_EQ(ptr.use_count(),2);
    EXPECT_EQ(ptr1.use_count(),2);
    Ptr<BaseClass> ptr2=ptr;
    EXPECT_EQ(ptr.use_count(),3);
    EXPECT_EQ(ptr1.use_count(),3);
    EXPECT_EQ(ptr2.use_count(),3);
    {
        Ptr<BaseClass> ptr4=ptr;
        EXPECT_EQ(ptr.use_count(),4);
        EXPECT_EQ(ptr1.use_count(),4);
        EXPECT_EQ(ptr2.use_count(),4);
    }
    EXPECT_EQ(ptr.use_count(),3);
    EXPECT_EQ(ptr1.use_count(),3);
    EXPECT_EQ(ptr2.use_count(),3);
    
    Ptr<BaseClass> ptr5(std::move(ptr));
    EXPECT_EQ(ptr5.use_count(),3);
    EXPECT_EQ(ptr1.use_count(),3);
    Ptr<BaseClass> ptr6=std::move(ptr1);
    EXPECT_EQ(ptr6.use_count(),3);
    EXPECT_EQ(ptr5.use_count(),3);
    EXPECT_EQ(ptr2.use_count(),3);
}

TEST(Ptr_own, upcast)
{
    using namespace milan;
    
    class BaseClass
    {
    };
    
    class DerivedClass: public BaseClass
    {
    };
    
    Ptr<DerivedClass> ptr(PtrStorage::OWN,new DerivedClass());
    EXPECT_EQ(ptr.use_count(),1);
    
    Ptr<BaseClass> ptr1(ptr);
    EXPECT_EQ(ptr.use_count(),2);
    EXPECT_EQ(ptr1.use_count(),2);
    Ptr<BaseClass> ptr2=ptr;
    EXPECT_EQ(ptr.use_count(),3);
    EXPECT_EQ(ptr1.use_count(),3);
    EXPECT_EQ(ptr2.use_count(),3);
    {
        Ptr<BaseClass> ptr4=ptr;
        EXPECT_EQ(ptr.use_count(),4);
        EXPECT_EQ(ptr1.use_count(),4);
        EXPECT_EQ(ptr2.use_count(),4);
    }
    EXPECT_EQ(ptr.use_count(),3);
    EXPECT_EQ(ptr1.use_count(),3);
    EXPECT_EQ(ptr2.use_count(),3);
    
    Ptr<BaseClass> ptr5(std::move(ptr));
    EXPECT_EQ(ptr5.use_count(),3);
    EXPECT_EQ(ptr1.use_count(),3);
    Ptr<BaseClass> ptr6=std::move(ptr1);
    EXPECT_EQ(ptr6.use_count(),3);
    EXPECT_EQ(ptr5.use_count(),3);
    EXPECT_EQ(ptr2.use_count(),3);
}





