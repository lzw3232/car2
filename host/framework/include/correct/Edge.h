#ifndef __FRAMEWORK_EDGE_H__
#define __FRAMEWORK_EDGE_H__

#include "../../include/image/PixelMtrix.h"

namespace lzw{

    class Edge
    {
    private:
        /* data */
        PixelMtrix *pixel;

    public:
        Edge(PixelMtrix *pixelMtrix);
        ~Edge();

        PixelMtrix *doEdge();
        int F(int p,int q,int w);

        PixelMtrix *getPixelMtrix(){return this->pixel;};
    };
    
    
}

#endif //! __FRAMEWORK_EDGE_H__