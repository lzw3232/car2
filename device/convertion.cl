__kernel void ConvertionDetector(__global double* input1,
					__global double* input2,
					__global int* size,
					__global int* size2,
					__global double* output){
        for(int row=0;row<size[0]-size2[0]+1; row++){
            for(int col=0; col<size[0]-size2[0]+1;col++){
                double sum=0;
                for(int i=0; i<size2[0];i++){
                    for(int j=0;j<size2[0];j++){
                        sum+=input1[(row+i)*size[0]+col+j]*input2[i*size2[0]+j];
                    }
                }
		output[row*size[0]+col]=sum;
            }
        }
}
