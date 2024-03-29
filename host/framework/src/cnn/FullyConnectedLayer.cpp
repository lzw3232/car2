#include "../../include/cnn/FullyConnectedLayer.h"
#include<iostream>
#include<stdlib.h>
#include<math.h>

using namespace lzw;
using namespace lzw::cnn;


FullyConnectedLayer::FullyConnectedLayer()
{
}

FullyConnectedLayer::FullyConnectedLayer(int width, 
                            int height, 
                            int batchSize, 
                            std::vector<double **> values 
                            ):Layer(width,height,batchSize,values)
{
}

ConvertionKernel *FullyConnectedLayer::generateConvertionKernel(Vector *v, KernelGenerateMode kernelMode){
    ConvertionKernel *kernel = new ConvertionKernel(v->getX(),v->getY());
    switch (kernelMode)
    {
        case RANDOM:{
            /* code */
            random_gen:
            double **mtrix=new double*[v->getY()];
            for(int i=0; i<v->getY();i++){
                mtrix[i] = new double[v->getX()];
                for(int j=0;j<v->getX();j++){
                    mtrix[i][j]=rand()%10;
                }
            }
            kernel->setW(mtrix);
            return kernel;
            break;
        }
        default:
            goto random_gen;
            break;
    }
}

void FullyConnectedLayer::convolution(bool isFirstEpouch,Vector *v, KernelGenerateMode kernelMode){
    if(isFirstEpouch){
        Vector *fully_v=new Vector(1,this->getHeight()*this->getWidth());
        this->setw(this->generateConvertionKernel(fully_v,kernelMode));
    }
    std::vector<double **> values;
    for(int size=0;size<this->getbatchSize();size++){
        double **mtrix=new double*[1];
        double sum = 0;
        mtrix[0]=new double[1];
        for(int row=0;row<this->getHeight(); row++){
            for(int col=0; col<this->getWidth();col++){
                sum += this->getvalues().at(size)[row][col]*this->getw()->getW()[row*this->getWidth()+col][1];   
            }
        }
        mtrix[0][0]=sum;
        values.push_back(mtrix);
    }
    for(int size=0;size<values.size();size++){
        std::cout<<values.at(size)[0][0]<<" ";
    }
    std::cout<<std::endl;

    int w=1;
    int h=1;


    if(isFirstEpouch){
        this->afterFullyConnectedLayer = new FullyConnectedLayer(w,h,this->getbatchSize(),values);
    }else{
        this->afterFullyConnectedLayer->setvalues(values);
    }
    //this->afterFullyConnectedLayer->print();
};


void FullyConnectedLayer::print(){
    std::cout<<this->getWidth()<<" "<<this->getHeight()<<std::endl;
}

FullyConnectedLayer::~FullyConnectedLayer()
{
}

