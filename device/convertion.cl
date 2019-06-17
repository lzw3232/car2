__kernel void ConvertionDetector(__global double* input1,
					__global double* input2,
					const int size,
					const int size2,
					__global double* output){
        for(int row=0;row<size-size2+1; row++){
            for(int col=0; col<size-size2+1;col++){
                double sum=0;
                for(int i=0; i<size2;i++){
                    for(int j=0;j<size2;j++){
                        sum+=input1[row*size+col]*input2[i*size2+col];
                    }
                }
		output[row*size+col]=sum;
            }
        }
}
