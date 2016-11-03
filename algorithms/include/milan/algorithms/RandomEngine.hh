#ifndef __MILAN_ALGORITHMS_RANDOMENGINE_H__
#define __MILAN_ALGORITHMS_RANDOMENGINE_H__

#include "milan/core/Types.hh"

#include <limits>

namespace milan
{

#define rotl(r,n) (((r)<<(n)) | ((r)>>((8*sizeof(r))-(n))))

class RandomEngine
{
    protected:
        uint64 _xx, _yy, _zz;
    public:
        RandomEngine(uint64 seed)
        {
           unsigned n;
           _xx = 2257535ULL;
           _yy = 821507ULL;
           _zz = 819103680ULL;
           for (n=((seed>>22)&0x3ff)+20; n>0; n--) _xx = rotl(_xx,52) - rotl(_xx, 9);
           for (n=((seed>>11)&0x7ff)+20; n>0; n--) _yy = rotl(_yy,24) - rotl(_yy,45);
           for (n=((seed    )&0x7ff)+20; n>0; n--) _zz -= rotl(_zz,38);
        }

        uint64 operator()()
        {
            //see http://www.drdobbs.com/tools/fast-high-quality-parallel-random-number/229625477?pgno=2
            _xx = rotl(_xx,52) - rotl(_xx, 9);
            _yy = rotl(_yy,24) - rotl(_yy,45);
            _zz -= rotl(_zz,38);
            return _xx ^ _yy ^ _zz;
        }
        
        static constexpr uint64 min()
        {
            return 0;
        }
        
        static constexpr uint64 max()
        {
            return std::numeric_limits<uint64>::max();
        }
        
};

}

#endif
