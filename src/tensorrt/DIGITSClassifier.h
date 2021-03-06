/**
 * @file	DIGITSClassifier.h
 * @author	Carroll Vance
 * @brief	Loads and manages a DIGITS ImageNet graph with TensorRT
 *
 * Copyright (c) 2018 Carroll Vance.
 * Copyright (c) 2017, NVIDIA CORPORATION. All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#ifndef CLASSIFICATIONRTENGINE_H_
#define CLASSIFICATIONRTENGINE_H_

#include <string>
#include <vector>

#include "NvCaffeParser.h"
#include "NvInfer.h"
#include "NvUtils.h"

#include "CaffeRTEngine.h"
#include "NetworkDataTypes.h"

namespace jetson_tensorrt {

/**
 * @brief	Loads and manages a DIGITS ImageNet graph with TensorRT
 */
class DIGITSClassifier : public CaffeRTEngine {
public:
  enum DEFAULT { WIDTH = 224, HEIGHT = 224, DEPTH = 3, CLASSES = 1000 };

  DIGITSClassifier() {}
  /**
   * @brief	Creates a new instance of DIGITSClassifier
   * @param	prototextPath	Path to the .prototext file
   * @param	modelPath	Path to the .caffemodel file
   * @param	cachePath	Path to the .tensorcache file which will be
   * loaded instead of building the network if present
   * @param	nbChannels	Number of channels in the input image. 1 for
   * greyscale, 3 for RGB
   * @param	width	Width of the input image
   * @param	height	Height of the input image
   * @param	nbClasses	Number of classes to predict
   * @param	dataType	The data type used to contstruct the TensorRT
   * network. Use FLOAT unless you know how it will effect your model.
   * @param	maxNetworkSize	Maximum size in bytes of the TensorRT network in
   * device memory
   */
  DIGITSClassifier(std::string prototextPath, std::string modelPath,
                   std::string cachePath = "classification.tensorcache",
                   size_t nbChannels = DEFAULT::DEPTH,
                   size_t width = DEFAULT::WIDTH,
                   size_t height = DEFAULT::HEIGHT,
                   size_t nbClasses = DEFAULT::CLASSES,
                   nvinfer1::DataType dataType = nvinfer1::DataType::kFLOAT,
                   size_t maxNetworkSize = (1 << 30));

  /**
   * @brief	DIGITSClassifier destructor
   */
  virtual ~DIGITSClassifier();

  /**
   * @brief	Classifies a single BGR format image.
   * @param	inputs Graph inputs indexed by [batchIndex][inputIndex]
   * @param	outputs Graph inputs indexed by [batchIndex][inputIndex]
   * @param	threshold	Minimum probability of a class detection for it
   * to be returned as a result
   * @return	vector of Classification objects above the threshold
   *
   */
  std::vector<RTClassification> classify(LocatedExecutionMemory &inputs,
                                         LocatedExecutionMemory &outputs,
                                         float threshold = 0.5);

  static const size_t CHANNELS_GREYSCALE = 1;
  static const size_t CHANNELS_BGR = 3;

  size_t modelWidth;
  size_t modelHeight;
  size_t modelDepth;

  size_t nbClasses;

private:
  static const std::string INPUT_NAME;
  static const std::string OUTPUT_NAME;
};

} // namespace jetson_tensorrt

#endif /* CLASSIFICATIONRTENGINE_H_ */
