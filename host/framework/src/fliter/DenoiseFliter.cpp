#include "../../include/fliter/DenoiseFliter.h"

using namespace lzw;

DenoiseFliter::DenoiseFliter(PixelMtrix *pixelMtrix):Fliter(pixelMtrix,DENOISE_FLITER)
{
}

DenoiseFliter::~DenoiseFliter()
{
}

int DenoiseFliter::F(int p,int q,int w){
    if(p<0){
        p=0;
    }
    else if(p>(this->getPixelMtrix()->getHeight()-1)){
        p=this->getPixelMtrix()->getHeight()-1;
    }

    if(q<0){
        q=0;
    }
    else if(q>(this->getPixelMtrix()->getWidth()-1)){
        q=this->getPixelMtrix()->getWidth()-1;
    }
    if(w==1){
        return this->getPixelMtrix()->getPixel()[p][q].getColor()->getR();
    }else if(w==2){
        return this->getPixelMtrix()->getPixel()[p][q].getColor()->getG();
    }else{
        return this->getPixelMtrix()->getPixel()[p][q].getColor()->getB();
    }
}

PixelMtrix *DenoiseFliter::doFliter(){
    // for(int rows=0; rows<this->getPixelMtrix()->getHeight();rows++){
    //     for(int cols=0; cols<this->getPixelMtrix()->getWidth();cols++){
    //         Pixel pixel = this->getPixelMtrix()->getPixel()[rows][cols];
    //         int isNoise = 0;
    //         if(pixel.getColor()->getR() == 0x0){
    //             if(rows == 0 && cols == 0){
    //                 Pixel rightPixel = this->getPixelMtrix()->getPixel()[rows][cols+1];
    //                 Pixel bottomPixel = this->getPixelMtrix()->getPixel()[rows+1][cols];
    //                 if(rightPixel.getColor()->getR() == 0xFF && bottomPixel.getColor()->getR()== 0xFF){
    //                     isNoise = 1;
    //                 }
    //             }
    //             else if (rows == 0 && cols == (this->getPixelMtrix()->getWidth()-1)) {
    //                 Pixel leftPixel = this->getPixelMtrix()->getPixel()[rows][cols-1];
    //                 Pixel bottomPixel = this->getPixelMtrix()->getPixel()[rows+1][cols];
    //                 if(leftPixel.getColor()->getR() == 0xFF && bottomPixel.getColor()->getR() == 0xFF){
    //                     isNoise = 1;
    //                 }
    //             }
    //             else if (rows == (this->getPixelMtrix()->getHeight()-1) && cols == 0) {
    //                 Pixel rightPixel = this->getPixelMtrix()->getPixel()[rows][cols+1];
    //                 Pixel topPixel = this->getPixelMtrix()->getPixel()[rows-1][cols];
    //                 if(rightPixel.getColor()->getR() == 0xFF && topPixel.getColor()->getR() == 0xFF){
    //                     isNoise = 1;
    //                 }
    //             }
    //             else if (rows == (this->getPixelMtrix()->getHeight()-1) &&cols == (this->getPixelMtrix()->getWidth()-1)) {
    //                 Pixel leftPixel = this->getPixelMtrix()->getPixel()[rows][cols-1];
    //                 Pixel topPixel = this->getPixelMtrix()->getPixel()[rows-1][cols];
    //                 if(leftPixel.getColor()->getR() == 0xFF && topPixel.getColor()->getR()== 0xFF){
    //                     isNoise = 1;
                    
    //             }
    //             }
    //             else if (rows == 0) {
    //                 Pixel leftPixel = this->getPixelMtrix()->getPixel()[rows][cols-1];
    //                 Pixel bottomPixel = this->getPixelMtrix()->getPixel()[rows+1][cols];
    //                 Pixel rightPixel = this->getPixelMtrix()->getPixel()[rows][cols+1];
    //                 if(leftPixel.getColor()->getR() == 0xFF && bottomPixel.getColor()->getR()== 0xFF && rightPixel.getColor()->getR()== 0xFF){
    //                     isNoise = 1;
    //                 }
    //             }
    //             else if (cols == 0) {
    //                 Pixel topPixel = this->getPixelMtrix()->getPixel()[rows-1][cols];
    //                 Pixel bottomPixel = this->getPixelMtrix()->getPixel()[rows+1][cols];
    //                 Pixel rightPixel = this->getPixelMtrix()->getPixel()[rows][cols+1];
    //                 if(topPixel.getColor()->getR() == 0xFF && bottomPixel.getColor()->getR()== 0xFF && rightPixel.getColor()->getR()== 0xFF){
    //                     isNoise = 1;
    //                 }
    //             }
    //             else if (rows == (this->getPixelMtrix()->getHeight()-1)) {
    //                 Pixel topPixel = this->getPixelMtrix()->getPixel()[rows-1][cols];
    //                 Pixel leftPixel = this->getPixelMtrix()->getPixel()[rows][cols-1];
    //                 Pixel rightPixel = this->getPixelMtrix()->getPixel()[rows][cols+1];
    //                 if(topPixel.getColor()->getR() == 0xFF && leftPixel.getColor()->getR()== 0xFF && rightPixel.getColor()->getR()== 0xFF){
    //                     isNoise = 1;
    //                 }
    //             }
    //             else if (cols == (this->getPixelMtrix()->getWidth()-1)) {
    //                 Pixel topPixel = this->getPixelMtrix()->getPixel()[rows-1][cols];
    //                 Pixel leftPixel = this->getPixelMtrix()->getPixel()[rows][cols-1];
    //                 Pixel bottomPixel = this->getPixelMtrix()->getPixel()[rows+1][cols];
    //                 if(topPixel.getColor()->getR() == 0xFF && leftPixel.getColor()->getR()== 0xFF && bottomPixel.getColor()->getR()== 0xFF){
    //                     isNoise = 1;
    //                 }
    //             }
    //             else
    //             {
    //                 Pixel topPixel = this->getPixelMtrix()->getPixel()[rows-1][cols];
    //                 Pixel leftPixel = this->getPixelMtrix()->getPixel()[rows][cols-1];
    //                 Pixel bottomPixel = this->getPixelMtrix()->getPixel()[rows+1][cols];
    //                 Pixel rightPixel = this->getPixelMtrix()->getPixel()[rows][cols+1];
    //                 if(topPixel.getColor()->getR() == 0xFF && leftPixel.getColor()->getR()== 0xFF && bottomPixel.getColor()->getR()== 0xFF&& rightPixel.getColor()->getR()== 0xFF){
    //                     isNoise = 1;
    //                 }
    //             }
    //         }

    //         if(isNoise==1){
    //             this->getPixelMtrix()->setPixel(
    //                 rows,
    //                 cols,
    //                 new Pixel(new Color(
    //                     0xFF,
    //                     0xFF,
    //                     0xFF
    //                 )));
    //         }

    //     }
    // }
    // return this->getPixelMtrix();
    // double sobel[3][3] =
    //     {
    //         {1,1,1},
    //         {1,1,1},
    //         {1,1,1},
    //     };
    double sobel[3][3] =
        {
            {0.0585,0.0965,0.0585},
            {0.0965,0.1529,0.0965},
            {0.0585,0.0965,0.0585},
        };
        Pixel **pixels=new Pixel*[this->getPixelMtrix()->getHeight()];
        for(int row=0;row<this->getPixelMtrix()->getHeight(); row++){
            pixels[row]=new Pixel[this->getPixelMtrix()->getWidth()];
            for(int col=0; col<this->getPixelMtrix()->getWidth();col++){
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

                // resR = resR/9;
                // resG = resG/9;
                // resB = resB/9;
                resR = (resR>=150)?255:0;
                resG = (resG>=150)?255:0;
                resB = (resB>=150)?255:0;
                pixels[row][col] = new Color(resR , resG , resB);
            }
        }
        this->getPixelMtrix()->setPixels(pixels);

        return getPixelMtrix();
}

