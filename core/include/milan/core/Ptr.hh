#ifndef __MILAN_CORE_PTR_H__
#define __MILAN_CORE_PTR_H__

#include "milan/core/Types.hh"
#include "milan/core/Exception.hh"

namespace milan
{

template<class TYPE>
class Ptr
{
    public:
        enum Storage
        {
            OWN,
            SHARE
        };
        
    protected:
        TYPE* _data;
        Storage _storage;
        sizetype* _refs;
        
    public:
        Ptr(Storage storage, TYPE* data=nullptr):
            _data(data),
            _storage(storage),
            _refs(new sizetype(1))
        {
        }
        
        inline static Ptr<TYPE>&& makeShared(TYPE* data=nullptr)
        {
            return Ptr(SHARE,data);
        }
        
        static Ptr<TYPE>&& makeOwned(TYPE* data=nullptr)
        {
            return Ptr(OWN,data);
        }
        
        Ptr(const Ptr<TYPE>& ptr):
            _data(ptr._data),
            _storage(ptr._storage),
            _refs(ptr._refs)
        {
            ++(*_refs);
        }
        
        inline TYPE& operator*()
        {
            return *_data;
        }
        
        inline const TYPE& operator*() const
        {
            return *_data;
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
        
        inline void reset(TYPE* data=nullptr)
        {
            if ((*_refs)==1)
            {
                delete _refs;
                if (_storage==OWN and _data!=nullptr)
                {
                    delete _data;
                }
            } 
            else if ((*_refs)>1)
            {
                --(*_refs);
            }
            _refs = new sizetype(1);
            _data=data;
        }
        
        ~Ptr()
        {
            if ((*_refs)==1)
            {
                delete _refs;
                if (_storage==OWN and _data!=nullptr)
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

}


#endif
