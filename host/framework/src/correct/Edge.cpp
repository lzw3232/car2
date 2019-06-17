#include "../../include/correct/Edge.h"
#include <math.h>

using namespace lzw;

Edge::Edge(PixelMtrix *pixel){
    this->pixel=pixel;
}

int Edge::F(int p,int q,int w){
    if(p<0){
        p=0;
    }
    else if(p>(this->pixel->getHeight()-1)){
        p=this->pixel->getHeight()-1;
    }

    if(q<0){
        q=0;
    }
    else if(q>(this->pixel->getWidth()-1)){
        q=this->pixel->getWidth()-1;
    }
    if(w==1){
        return this->pixel->getPixel()[p][q].getColor()->getR();
    }else if(w==2){
        return this->pixel->getPixel()[p][q].getColor()->getG();
    }else{
        return this->pixel->getPixel()[p][q].getColor()->getB();
    }
}

PixelMtrix *Edge::doEdge(){
        double sobel[3][3] =
        {
            {-1,0,1},
            {-2,0,2},
            {-1,0,1},
        };
        double sobel2[3][3] =
        {
            {1,2,1},
            {0,0,0},
            {-1,-2,-1},
        };
        Pixel **pixels=new Pixel*[this->pixel->getHeight()];
        for(int row=0;row<this->pixel->getHeight(); row++){
            pixels[row]=new Pixel[this->pixel->getWidth()];
            for(int col=0; col<this->pixel->getWidth();col++){
                int resR=0;
                int resG=0;
                int resB=0;
                int resR2=0;
                int resG2=0;
                int resB2=0;
                for(int i=0; i<3;i++){
                    for(int j=0;j<3;j++){
                        resR+=F(row-i-1,col-j-1,1)*sobel[i][j];
                        resG+=F(row-i-1,col-j-1,2)*sobel[i][j];
                        resB+=F(row-i-1,col-j-1,3)*sobel[i][j];

                        // resR2+=F(row-i-1,col-j-1,1)*sobel2[i][j];
                        // resG2+=F(row-i-1,col-j-1,2)*sobel2[i][j];
                        // resB2+=F(row-i-1,col-j-1,3)*sobel2[i][j];
                    }
                }
                // resR=sqrt(resR*resR+resR2*resR2);
                // resG=sqrt(resG*resG+resG2*resG2);
                // resB=sqrt(resB*resB+resB2*resB2);

                resR = (resR>=150)?255:0;
                resG = (resG>=150)?255:0;
                resB = (resB>=150)?255:0;
                pixels[row][col] = new Color(resR , resG , resB);
            }
        }
        this->pixel->setPixels(pixels);

        return this->pixel;
}

Edge::~Edge(){
}