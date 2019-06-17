#ifndef __FRAMEWORK_CNN_FULLY_CONNECTED_LAYER_H__
#define __FRAMEWORK_CNN_FULLY_CONNECTED_LAYER_H__

#include "../../include/cnn/Layer.h"

namespace lzw{
		namespace cnn{
	class FullyConnectedLayer : public Layer{
		private:
		
		ConvertionKernel *generateConvertionKernel(Vector *v, KernelGenerateMode kernelMode);
        FullyConnectedLayer *afterFullyConnectedLayer;

		public:
			FullyConnectedLayer();
			FullyConnectedLayer(int width, int height, int batchSize, std::vector<double **> values);

			~FullyConnectedLayer();

			void convolution(bool isFirstEpouch,Vector *v, KernelGenerateMode kernelMode);
			FullyConnectedLayer *getAfterFullyConnectedLayer(){return this->afterFullyConnectedLayer;};
			void print();
	};
		};
};

#endif // ! __FRAMEWORK_CNN_FULLY_CONNECTED_LAYER_H__