#include "../../include/scale/Scale.h"
#include <iostream>
#include <math.h>
#include <stdlib.h>

using namespace lzw;

Scale::Scale(int w, int h){
    this->distW=w;
    this->distH=h;
}

Scale::Scale(int w, int h ,PixelMtrix *pixel){
    this->distW=w;
    this->distH=h;
    this->pixel=pixel;
}

//采样公式
double S(int x){
    double a =-0.5;
    int u = abs(x);
    if(u>=0 && u<1){
        return (double)(1-(a+3)*u*u+(a+2)*u*u*u);
    }
    else{
        return (double)(-4*a+8*a*u-5*a*u*u+a*u*u*u);
    }
}

int Scale::F(int p,int q,int w){
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

PixelMtrix *Scale::doScale(){
    int srcW=this->pixel->getWidth();
    int srcH=this->pixel->getHeight();
    
    double scaleW= (double)srcW / this->distW;
    double scaleH= (double)srcH / this->distH;

    PixelMtrix *pixelmtrix = new PixelMtrix(this->distW,this->distH);

    Pixel **pixels = new Pixel*[this->distH];
    for(int row=0;row<this->distH;row++){
        pixels[row]=new Pixel[this->distW];
        for(int col=0;col<this->distW;col++){
            int distI = (double)row *scaleH;
            int distJ = (double)col *scaleW;
            int i = floor((double)row *scaleH);
            int j = floor((double)col *scaleW);
            int v = distI-i;
            int u = distJ-j;

            int resR=0;
            int resG=0;
            int resB=0;
            for(int p=-1;p<=2;p++){
                for(int q=-1;q<=2;q++){
                    resR+= (int)(F(i+p,j+q,1)*S(p-v)*S(q-u));
                    resG+= (int)(F(i+p,j+q,2)*S(p-v)*S(q-u));
                    resB+= (int)(F(i+p,j+q,3)*S(p-v)*S(q-u));
                }
            }
            pixels[row][col] = new Color(resR , resG , resB);
            
        }
    }
    pixelmtrix->setPixels(pixels);

    return pixelmtrix;
}

Scale::~Scale(){

}