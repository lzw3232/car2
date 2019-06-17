#ifndef __FRAMEWORK_DENOISEFLITER_H__
#define __FRAMEWORK_DENOISEFLITER_H__

#include "../../include/fliter/Fliter.h"

namespace lzw{

    class DenoiseFliter:public Fliter
    {
    private:
        /* data */
    public:
        DenoiseFliter(PixelMtrix *pixelMtrix);
        ~DenoiseFliter();
        int F(int p,int q,int w);

        PixelMtrix *doFliter();
    };

    
    
}

#endif //! __FRAMEWORK_DENOISEFLITER_H__