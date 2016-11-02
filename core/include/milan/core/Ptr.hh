#ifndef __MILAN_CORE_PTR_H__
#define __MILAN_CORE_PTR_H__

#include "milan/core/Types.hh"
#include "milan/core/Exception.hh"

#include <type_traits>

namespace milan
{

enum class PtrStorage
{
    OWN,
    SHARE
};

template<class TYPE>
class Ptr
{
    public:

        
    protected:
        TYPE* _data;
        PtrStorage _storage;
        sizetype* _refs;
        
    public:
        Ptr(PtrStorage storage = PtrStorage::SHARE, TYPE* data = nullptr):
            _data(data),
            _storage(storage),
            _refs(new sizetype(1))
        {
        }
        
        inline static Ptr<TYPE> makeShared(TYPE* data = nullptr)
        {
            return Ptr<TYPE>(PtrStorage::SHARE,data);
        }
        
        inline static Ptr<TYPE> makeOwned(TYPE* data = nullptr)
        {
            return Ptr<TYPE>(PtrStorage::OWN,data);
        }
        
        Ptr(Ptr<TYPE>&& ptr):
            _data(ptr._data),
            _storage(ptr._storage),
            _refs(ptr._refs)
        {
            //decouple old Ptr from ref counting. Do not need to increment refs
            ptr._refs=new sizetype(1);
            ptr._data=nullptr;
        }
        
        Ptr& operator=(Ptr<TYPE>&& ptr)
        {
            reset();
            delete _refs;
            _data=ptr._data;
            _storage=ptr._storage;
            _refs=ptr._refs;
            
            //decouple old Ptr from ref counting. Do not need to increment refs
            ptr._refs=new sizetype(1);
            ptr._data=nullptr;
            
            return *this;
        }
        
        Ptr(const Ptr<TYPE>& ptr):
            _data(ptr._data),
            _storage(ptr._storage),
            _refs(ptr._refs)
        {
            ++(*_refs);
        }
        
        Ptr& operator=(const Ptr<TYPE>& ptr)        
        {
            reset();
            delete _refs;
            _data=ptr._data;
            _storage=ptr._storage;
            _refs=ptr._refs;
            ++(*_refs);
            return *this;
        }
        
        inline PtrStorage storage() const
        {
            return _storage;
        }
        
        
        inline TYPE& operator*()
        {
            return *_data;
        }
        
        inline const TYPE& operator*() const
        {
            return *_data;
        }
        
        inline TYPE* operator->()
        {
            return _data;
        }
        
        inline const TYPE* operator->() const
        {
            return _data;
        }

        inline TYPE* get()
        {
            return _data;
        }

        inline const TYPE* get() const
        {
            return _data;
        } 
        
        inline sizetype use_count() const
        {
            return (*_refs);
        }
        
        explicit operator bool() const
        {
            return _data!=nullptr;
        }
        
        inline operator TYPE&()
        {
            return *_data;
        }
        
        inline operator const TYPE&() const
        {
            return *_data;
        }
        
        inline void reset(TYPE* data=nullptr)
        {
            if ((*_refs)==1)
            {
                //if this is the last Ptr managing _data, destroy it
                delete _refs; 
                if (_storage==PtrStorage::OWN and _data!=nullptr)
                {
                    delete _data;
                }
            }
            else if ((*_refs)>1)
            {
                //this is not the last Ptr managing _data, decrement _refs
                --(*_refs);
            }
            //create new _ref to decouple from other Ptrs still managing _data
            _refs = new sizetype(1);
            _data=data;
        }
        
        ~Ptr()
        {
            if ((*_refs)==1)
            {
                //delete _refs before _data to prevent memleak if destructor of data throws
                delete _refs;
                if (_storage==PtrStorage::OWN and _data!=nullptr)
                {
                    delete _data;
                }
            } 
            else if ((*_refs)>1)
            {
                --(*_refs);
            }
        }
};

template<class INTERFACE, class CLASS>
class PtrInterface
{
    public:
        Ptr<const INTERFACE> copy() const
        {
            CLASS* cloneHist = new CLASS(*((CLASS*)this));
            Ptr<const INTERFACE> res(PtrStorage::OWN,cloneHist);
            return res;
        }
        
        Ptr<const INTERFACE> ref() const
        {
            Ptr<const INTERFACE> res(PtrStorage::SHARE,(CLASS*)this);
            return res;
        }
        
        operator Ptr<const INTERFACE>() const
        {
            return copy();
        }
};


}


#endif
