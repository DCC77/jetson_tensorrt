# ros_jetson_tensorrt
This repository is in heavy active development, but will eventually contain ROS nodes specifically designed to utilize TensorRT on Jetson. It currently houses an abstraction layer which hides the host/device memory management paradigm required by TensorRT which will be implemented into ROS nodes.

# Planned Network Support
- [DIGITS][digits] - ImageNet, DetectNet, SegNet
- Caffe - Generic
- Tensorflow - Generic

# Documentation
- [Doxygen][docs]

[digits]: https://github.com/NVIDIA/DIGITS
[docs]: https://csvance.github.io/ros_jetson_tensorrt/
