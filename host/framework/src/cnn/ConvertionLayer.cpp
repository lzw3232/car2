#include "../../include/cnn/ConvertionLayer.h"
#include<iostream>
#include "CL/opencl.h"
#include "AOCLUtils/aocl_utils.h"
#include<stdlib.h>
#include<math.h>

using namespace aocl_utils;

cl_program program;
cl_context context;
cl_command_queue queue;
cl_kernel tKernel;
cl_kernel mKernel;

using namespace lzw;
using namespace lzw::cnn;

void PixelMtrixtoDouble(){
   
}

void ConvertionLayer::set(ActivationFunctionType activationFunctionType){
    this->setActivationFunctionType(activationFunctionType);

        // 激活函数
        switch (this->getActivationFunctionType())
        {
        case ACTIVATION_FUNCTION_CUSTOMIZE:
            /* code */
            break;
        
        case ACTIVATION_FUNCTION_SIGMOID:
            this->setActivationFunction(new SigMoidActivation());
            break;
        default:
            break;
        }

    this->setb(10.5);
}

ConvertionLayer::ConvertionLayer(int width, int height, int batchSize, std::vector<double **> values, ConvertionKernel *w,ActivationFunctionType activationFunctionType):Layer(width,height,batchSize,values,w)
{
    this->set(activationFunctionType);
}

ConvertionLayer::ConvertionLayer(int width, int height, int batchSize, std::vector<PixelMtrix *> values, ConvertionKernel *w,ActivationFunctionType activationFunctionType):Layer(width,height,batchSize,values,w)
{
    this->set(activationFunctionType);
}

ConvertionLayer::~ConvertionLayer()
{
}

ConvertionKernel *ConvertionLayer::generateConvertionKernel(Vector *v, KernelGenerateMode kernelMode){
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


void ConvertionLayer::convolution(bool isFirstEpouch,Vector *v, KernelGenerateMode kernelMode){
    if(isFirstEpouch){
        this->setw(this->generateConvertionKernel(v,kernelMode));
    }
    std::vector<double **> values;
    for(int size=0;size<this->getbatchSize();size++){
        double **mtrix=this->fpga(this->getWidth(),this->getw()->getHeight(),this->getvalues().at(size),this->getw()->getW());
        this->getActivationFunction()->doActive(this->getWidth()-(int)(2*floor(this->getw()->getWidth()/2)),
                                    this->getHeight()-(int)(2*floor(this->getw()->getHeight()/2)),
                                    mtrix);
        values.push_back(mtrix);
    }

    
    int w=this->getHeight()-(int)(2*floor(this->getw()->getHeight()/2));
    int h=this->getWidth()-(int)(2*floor(this->getw()->getWidth()/2));
    if(isFirstEpouch){
        this->afterConvertionLayer = new ConvertionLayer(w,h,this->getbatchSize(),values,this->getw(),this->getActivationFunctionType());
    }else{
        this->afterConvertionLayer->setvalues(values);
    }
    this->afterConvertionLayer->print();
}

void ConvertionLayer::print(){
    std::cout<<this->getWidth()<<" "<<this->getHeight()<<std::endl;
    //     for(int row=0;row<this->getHeight(); row++){
    //         for(int col=0; col<this->getWidth();col++){
    // std::cout<<this->getvalues().at(0)[row][col]<<" ";
    //         }
    //         std::cout<<std::endl;
    //     }
    //         std::cout<<"-------------------------------------------------"<<std::endl;
}

PoollingLayer *ConvertionLayer::castToPoollingLayer(){
    PoollingLayer *p_layer = new PoollingLayer(this->getWidth(),
                                                this->getHeight(),
                                                this->getbatchSize(),
                                                this->getvalues());
    // p_layer->setvalues = this->getvalues();
    // p_layer->setWidth = this->getWidth();
    // p_layer->setHeight = this->getHeight();
    // p_layer->setbatchSize = this->getbatchSize();
    return p_layer;
}

double **ConvertionLayer::fpga(int size,int size2,double **values,double **kernel){
	double *values1=new double[size*size];
	for(int i=0;i<size;i++){
		for(int j=0;j<size;j++){
			values1[i*size+j]=values[i][j];
		}
	}
	double *kernel2=new double[size2*size2];
	for(int i=0;i<size2;i++){
		for(int j=0;j<size2;j++){
			kernel2[i*size+j]=kernel[i][j];
		}
	}
	int *s1=&size;
	int *s2=&size2;

    cl_int error;
	cl_platform_id platforms;
	cl_device_id devices;
	size_t program_size;
	char *program_buffer;
	double *output = NULL;


	output = (double*)alignedMalloc(sizeof(double) * (size-(int)(2*floor(size2/2)))*(size-(int)(2*floor(size2/2))));

	//char * A ;
	//cin >> A;
	
	
	//获取平台
	error = clGetPlatformIDs(1, &platforms, NULL);
	if (error != 0) {
		printf("Get platform failed!");
		return 0;
    }
	error = clGetDeviceIDs(platforms, CL_DEVICE_TYPE_ALL, 1, &devices, NULL);
	if (error != 0) {
		printf("Get device failed!");
		return 0;
	}
	//创建上下文
	context = clCreateContext(NULL,1,&devices,NULL,NULL,&error);
	if (error != 0) {
		printf("Creat context failed!");
		return 0;
	}
	//创建程序
	std::string binary_file = getBoardBinaryFile("convertion", devices);
  	printf("Using AOCX: %s\n", binary_file.c_str());
 	program = createProgramFromBinary(context, binary_file.c_str(), &devices, 1);

  	// Build the program that was just created.
  	error = clBuildProgram(program, 0, NULL, "", NULL, NULL);
  	checkError(error, "Failed to build program");
	//创建命令队列
	queue = clCreateCommandQueue(context, devices, CL_QUEUE_PROFILING_ENABLE, &error);
	if (error < 0) {
		printf("Coudn't create the command queue");
		return 0;
	}


	//创建内核
	const char * kernel1 = "ConvertionDetector";
	tKernel = clCreateKernel(program,kernel1,&error);
	if (tKernel==NULL) {
		printf("Couldn't create kernel!\n");
		return 0;
	}

	//创建缓存对象
	cl_mem memObject1 = clCreateBuffer(context, CL_MEM_READ_ONLY,sizeof(double) * size*size,NULL,&error);
	if (error < 0) {
		printf("Creat memObject1 failed!\n");
		return 0;
	}

	cl_mem memObject2 = clCreateBuffer(context, CL_MEM_READ_ONLY,sizeof(double) * size2*size2, NULL, &error);
	if (error < 0) {
		printf("Creat memObject1 failed!\n");
		return 0;
	}	
	cl_mem memObject3 = clCreateBuffer(context, CL_MEM_READ_ONLY,sizeof(int)*1,NULL,&error);
	if (error < 0) {
		printf("Creat memObject1 failed!\n");
		return 0;
	}

	cl_mem memObject4 = clCreateBuffer(context, CL_MEM_READ_ONLY,sizeof(int)*1, NULL, &error);
	if (error < 0) {
		printf("Creat memObject1 failed!\n");
		return 0;
	}

	cl_mem memObject5 = clCreateBuffer(context, CL_MEM_WRITE_ONLY,sizeof(double) * (size-(int)(2*floor(size2/2)))*(size-(int)(2*floor(size2/2))), NULL, &error);
	if (error < 0) {
		printf("Creat memObject3 failed!\n");
		return 0;
	}
	//设置内核参数
	//int width = 1024;
	//int height = 1024;

	error  = clSetKernelArg(tKernel, 0, sizeof(cl_mem), &memObject1);
	error |= clSetKernelArg(tKernel, 1, sizeof(cl_mem), &memObject2);
	error |= clSetKernelArg(tKernel, 2, sizeof(cl_mem), &memObject3);
	error |= clSetKernelArg(tKernel, 3, sizeof(cl_mem), &memObject4);
	error |= clSetKernelArg(tKernel, 4, sizeof(cl_mem), &memObject5);
	// error |= clSetKernelArg(tKernel, 2, sizeof(unsigned int), (int*)&bmp_header[18]);
	// error |= clSetKernelArg(tKernel, 3, sizeof(unsigned int), (int*)&bmp_header[22]);
	if (error != CL_SUCCESS) {
		printf("Error setting kernel arguments!\n");
		return 0;
	}
/*	
	//初始化参数
	float* input1 = (float *)malloc(sizeof(float)*MIXSIZE);
	float* input2 = (float *)malloc(sizeof(float)*MIXSIZE);
	//float* input3 = (float *)malloc(sizeof(float)*MIXSIZE);
	float* result = (float *)malloc(sizeof(float)*1024*1024);
	float* check = (float *)malloc(sizeof(float)*1024*1024);
	//memset(input3, 0, sizeof(float)*MIXSIZE);
	memset(result, 0, sizeof(float)*1024*1024);
	memset(check, 0, sizeof(float) * 1024*1024);
	//数据读入
	//采用随机数函数产生输入
	
	//input1是
	for (int i = 0; i < 1024; i++) {
		srand(i);
		for (int j = 0; j < 1024; j++) {
			input1[1024 * i + j] = 20 * rand() / (double)(RAND_MAX);
		}
	}
	//input2是
	for (int i = 0; i < 1024; i++) {
		srand(i);
		for (int j = 0; j < 1024; j++) {
			input2[1024 * i + j] = 20 * rand() / (double)(RAND_MAX);
		}
	}
	
	const double start= getCurrentTimestamp();	
	
	for (int i = 0; i < 1024; i++) {
		for (int k = 0; k < 1024; k++) {
			for (int j = 0; j <1024; j++) {
				check[i * 1024 + k] += input1[j + i * 1024] * input2[k + j * 1024];
			}
		}
		
	}

	const double end = getCurrentTimestamp();
	printf("\nTime1: %0.3f ms\n", (end - start) * 1e3);
*/
	cl_int status = 0;
	cl_event evt1[4];
	cl_event evt3;

	const double start_time = getCurrentTimestamp();

	//数据写入缓冲对像
	error = clEnqueueWriteBuffer(queue, memObject1, CL_FALSE, 0, 
		          sizeof(double) * size*size, values1, 0, NULL, &evt1[0]);
	error |= clEnqueueWriteBuffer(queue, memObject2, CL_FALSE, 0, 
		          sizeof(double) * size2*size2, kernel2, 0, NULL, &evt1[1]);
	error |= clEnqueueWriteBuffer(queue, memObject3, CL_FALSE, 0, 
		          sizeof(int), s1, 0, NULL, &evt1[3]);
	error |= clEnqueueWriteBuffer(queue, memObject4, CL_FALSE, 0, 
		          sizeof(int), s2, 0, NULL, &evt1[4]);
	std::cout<<s1[0]<<" "<<s2[0]<<" "<<values1[0]<<" "<<kernel2[0]<<" "<<std::endl;
	if (error != CL_SUCCESS) {
		printf("write data failed!\n");
		return 0;
	}
/*
	error = clEnqueueWriteBuffer(queue, memObject2, CL_FALSE, 0,
		         MIXSIZE * sizeof(float), input2, 0, NULL, &evt1[1]);
	if (error != CL_SUCCESS) {
		printf("write data failed!\n");
		return -1;
	}
*/
	//配置工作项(转置)
	/*
	size_t maxWorkGroupSize = 0;
	clGetDeviceInfo(devices, CL_DEVICE_MAX_WORK_GROUP_SIZE,
		sizeof(maxWorkGroupSize), &maxWorkGroupSize, NULL);
	size_t globalWorkSize = MIXSIZE / 65;
	size_t localWorkSize = maxWorkGroupSize;
	

	
	//执行内核
	error = clEnqueueNDRangeKernel(queue, tKernel, 1, NULL, &globalWorkSize,
		             &localWorkSize, 2, evt1, &evt2);
	if (error != CL_SUCCESS) {
		printf("1 Error queuing kernel for execution!\n");
		printf("%d\n",error);
		return -1;
	}
	*/
	//配置工作项(乘法)
	size_t globalWorkSize[2] ={1024,1024};
	//size_t localThreads[2] = {256,1};
	//执行内核
	error = clEnqueueNDRangeKernel(queue, tKernel, 2, NULL, globalWorkSize,
		NULL, 0, NULL, &evt3);
	if (error != CL_SUCCESS) {
		printf("2 Error queuing kernel for execution!\n");
		printf("%d\n",error);
		return 0;
	}

	//读取内核结果
	 error = clEnqueueReadBuffer(queue, memObject5, CL_FALSE, 0,
		 sizeof(double) * (size-(int)(2*floor(size2/2)))*(size-(int)(2*floor(size2/2))), output, 1, &evt3, NULL);
	 if (error != CL_SUCCESS) {
		 checkError(error, "Error: could not copy data into device");
		 printf("Error reading result buffer!\n");
		 return 0;
	 }


	const double end_time = getCurrentTimestamp();
	printf("\nTime: %0.3f ms\n", (end_time - start_time) * 1e3);

	cl_ulong time_ns = getStartEndTime(evt3);
   	printf("Kernel time (device %d): %0.3f ms\n", 1, double(time_ns) * 1e-6);

  
  	free(output);
	
	printf("successed!\n");

	cleanup();
	
	int x=size-(int)(2*floor(size2/2));
	double **output1=new double*[x];
	for(int i=0;i<x;i++){
		output1[i]=new double[x];
		for(int j=0;j<x;j++){
			output1[i][j]=output[i*x+j];
		}
	}
	return output1;
	
	
}

void cleanup(){
	clReleaseProgram(program);
	clReleaseContext(context);
	clReleaseCommandQueue(queue);
	//clReleaseDevice(devices);
	clReleaseKernel(tKernel);
	clReleaseKernel(mKernel);
}

