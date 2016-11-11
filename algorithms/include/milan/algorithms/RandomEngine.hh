#ifndef __MILAN_ALGORITHMS_RANDOMENGINE_H__
#define __MILAN_ALGORITHMS_RANDOMENGINE_H__

#include "milan/core/Types.hh"
#include "milan/core/Math.hh"

#include <limits>

namespace milan
{


class RandomEngine
{
    protected:
        uint64 _xx, _yy, _zz;
    public:
        RandomEngine(uint64 seed)
        {
           unsigned n=0;
           _xx = 2257535ULL;
           _yy = 821507ULL;
           _zz = 819103680ULL;
           for (n=((seed>>22)&0x3ff)+20; n>0; n--) _xx = milan::rotl<uint64>(_xx,52) - milan::rotl<uint64>(_xx, 9);
           for (n=((seed>>11)&0x7ff)+20; n>0; n--) _yy = milan::rotl<uint64>(_yy,24) - milan::rotl<uint64>(_yy,45);
           for (n=((seed    )&0x7ff)+20; n>0; n--) _zz -= milan::rotl<uint64>(_zz,38);
        }

        uint64 operator()()
        {
            //see http://www.drdobbs.com/tools/fast-high-quality-parallel-random-number/229625477?pgno=2
            _xx = milan::rotl<uint64>(_xx,52) - milan::rotl<uint64>(_xx, 9);
            _yy = milan::rotl<uint64>(_yy,24) - milan::rotl<uint64>(_yy,45);
            _zz -= milan::rotl<uint64>(_zz,38);
            return _xx ^ _yy ^ _zz;
        }
        
        static constexpr uint64 min()
        {
            return std::numeric_limits<uint64>::lowest();
        }
        
        static constexpr uint64 max()
        {
            return std::numeric_limits<uint64>::max();
        }
        
};

}

#endif
