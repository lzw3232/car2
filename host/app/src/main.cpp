#include <iostream>

#include "../../framework/include/io/JPGImageLoader.h"
#include "../../framework/include/fliter/GrayFliter.h"
#include "../../framework/include/fliter/BinaryValFliter.h"
#include "../../framework/include/window/Window.h"
#include "../../framework/include/window/WindowManager.h"
#include "../../framework/include/fliter/DenoiseFliter.h"
#include "../../framework/include/cut/Cut.h"
#include "../../framework/include/location/Locate.h"
#include "../../framework/include/cnn/ConvertionKernel.h"
#include "../../framework/include/cnn/common/Activation.h"
#include "../../framework/include/cnn/Cnn.h"
#include "../../framework/include/scale/Scale.h"
#include "../../framework/include/correct/Edge.h"
#include <vector>


int main(int argc,char *argv[]){
    
    lzw::ImageLoader *jpgLoader = new lzw::JPGImageLoader();
    //lzw::PixelMtrix *pixels1 = jpgLoader->loadImage("/home/lzw/study/car/img/1.jpg");
    //lzw::PixelMtrix *pixels2 = jpgLoader->loadImage("/home/lzw/study/car/img/1.jpg");
    //lzw::PixelMtrix *pixels3 = jpgLoader->loadImage("/home/root/lzw/img/1.jpg");
    lzw::PixelMtrix *pixels3 = jpgLoader->loadImage("/home/lzw/study/car/img/009.jpg");
    //std::cout<<pixels->getWidth()<<std::endl;
    //std::cout<<pixels->getHeight()<<std::endl;
    // lzw::Window *defaultWindow=new lzw::Window(pixels3->getWidth(),pixels3->getHeight(),"原图");
    // defaultWindow->SetPixelMtrix(pixels3);

    lzw::Locate *locate = new lzw::Locate(pixels3);
    pixels3 = locate->doLocate();

    lzw::Scale *scale = new lzw::Scale(192,64,pixels3);
    pixels3 = scale->doScale(); 

    // lzw::Window *locateWindow=new lzw::Window(pixels3->getWidth(),pixels3->getHeight(),"定位");
    // locateWindow->SetPixelMtrix(pixels3);


    lzw::Window *GrayWindow=new lzw::Window(pixels3->getWidth(),pixels3->getHeight(),"灰度化");
    lzw::Fliter *grayFliter = new lzw::GrayFliter(pixels3);
    pixels3 = grayFliter->doFliter();
    GrayWindow->SetPixelMtrix(pixels3);

    // lzw::Window *EdgeWindow=new lzw::Window(pixels3->getWidth(),pixels3->getHeight(),"sobel算子");
    // lzw::Edge *edge = new lzw::Edge(pixels3);
    // pixels3 = edge->doEdge();
    // EdgeWindow->SetPixelMtrix(pixels3);

    lzw::Window *BinaryWindow=new lzw::Window(pixels3->getWidth(),pixels3->getHeight(),"二值化");
    lzw::Fliter *binaryValFliter = new lzw::BinaryValFliter(pixels3);
    pixels3 = binaryValFliter->doFliter();
    BinaryWindow->SetPixelMtrix(pixels3);



    lzw::Window *denoiseWindow=new lzw::Window(pixels3->getWidth(),pixels3->getHeight(),"去噪");
    lzw::Fliter *denoiseFliter = new lzw::DenoiseFliter(pixels3);
    pixels3 = denoiseFliter->doFliter();
    denoiseWindow->SetPixelMtrix(pixels3);


    lzw::Cut *cut = new lzw::Cut(pixels3);
    cut->doCut();
    std::vector<lzw::Window *> windows;

    for(int i=0;i<cut->getArray().size();i++){
        lzw::PixelMtrix *pixels4 = cut->clip(pixels3,cut->getArray().at(i));
        lzw::Window *cutWindow_piece = new lzw::Window(pixels4->getWidth(),pixels4->getHeight(),"切"+i);
        cutWindow_piece->SetPixelMtrix(pixels4);
        windows.push_back(cutWindow_piece);
    }

    // std::vector<lzw::PixelMtrix *> picture;

    // for(int i=0;i<cut->getArray().size();i++){
    //     lzw::PixelMtrix *pixels4 = cut->clip(pixels3,cut->getArray().at(i));
    //     lzw::Scale *scale = new lzw::Scale(64,64,pixels4);
    //     lzw::PixelMtrix *pixels5 = scale->doScale();    
    //     picture.push_back(pixels5);
    // }

    // lzw::cnn::CNN *cnn = new lzw::cnn::CNN();
    
    // lzw::Vector *v1 = new lzw::Vector(3,3);
    // std::vector<lzw::Vector *> vs;
    // vs.push_back(v1);
    // lzw::Vector *v2 = new lzw::Vector(5,5);
    // vs.push_back(v2);
    // lzw::Vector *v3 = new lzw::Vector(3,3);
    // vs.push_back(v3);

    // lzw::cnn::ConvertionKernel *kernel = new lzw::cnn::ConvertionKernel();
    // lzw::cnn::ConvertionLayer *layer = new lzw::cnn::ConvertionLayer(64,64,picture.size(),picture,kernel,lzw::ActivationFunctionType::ACTIVATION_FUNCTION_SIGMOID);
    // cnn->input(layer,3,1,picture.size(),1,lzw::cnn::KernelGenerateMode::RANDOM,vs,lzw::ActivationFunctionType::ACTIVATION_FUNCTION_SIGMOID);
    // cnn->train();
    

    lzw::WindowManager *manager=lzw::WindowManager::GetWindowManagerInstance();



    // for(int rows=0; rows<pixels2->getHeight();rows++){
    //     for(int cols=0; cols<pixels2->getWidth();cols++){
    //         std::cout<<pixels2->getPixel()[rows][cols].getColor()->getR()<<" ";
    //     }
    //     std::cout<<std::endl;
    // }
    // manager->createWindow(defaultWindow);
    // manager->createWindow(locateWindow);
     manager->createWindow(GrayWindow);
    //manager->createWindow(EdgeWindow);
    manager->createWindow(BinaryWindow);
    manager->createWindow(denoiseWindow);
    for(int i = 0;i<cut->getArray().size();i++){ 
        manager->createWindow(windows.at(i));
    }

    manager->renderWindow();
    
    return 0;

}