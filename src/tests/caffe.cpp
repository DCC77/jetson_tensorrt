#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <cuda_runtime_api.h>
#include <fstream>
#include <iostream>
#include <string>
#include <sys/stat.h>
#include <unordered_map>
#include <cassert>
#include <vector>

#include "NvInfer.h"

#include "CaffeRTEngine.h"

#define NUM_SAMPLES 10
#define BATCH_SIZE 1

using namespace nvinfer1;
using namespace std;

int main(int argc, char** argv) {

	CaffeRTEngine engine = CaffeRTEngine();
	engine.addInput("data", DimsCHW(3, 224, 224), sizeof(float));

	Dims outputDims; outputDims.nbDims = 1; outputDims.d[0] = 1000;
	engine.addOutput("prob", outputDims, sizeof(float));

	if(!engine.loadCache(string("caffe.tensorcache"), BATCH_SIZE)){
		assert(engine.loadModel(string("googlenet.prototxt"), string("bvlc_googlenet.caffemodel"), (size_t) BATCH_SIZE));
		engine.saveCache(string("caffe.tensorcache"));
	}

	std::cout << engine.engineSummary() << std::endl;

	/* Allocate memory for predictions */
	vector<vector<void*>> batch(BATCH_SIZE);
	for (int b=0; b < BATCH_SIZE; b++) {

		//Inputs
		batch[b].push_back(new unsigned char[3 * 256 * 256 * 4]);
	}

	for (;;) {
		int totalMs = 0;

		for (int i = 0; i < NUM_SAMPLES; i++) {
			auto t_start = std::chrono::high_resolution_clock::now();

			vector<vector<void*>> batchOutputs = engine.predict(batch);
			for (int b = 0; b < batchOutputs.size(); b++)
				delete ((unsigned char*)batchOutputs[b][0]);

			auto t_end = std::chrono::high_resolution_clock::now();
			auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(t_end - t_start).count();

			totalMs += ms;

		}

		totalMs /= NUM_SAMPLES;
		std::cout << "Average over " << NUM_SAMPLES << " runs is " << totalMs
				<< " ms." << std::endl;

	}
}

