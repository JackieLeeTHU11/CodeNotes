## performance comparison of different object-detection models


![Alt text](./detection-tend.png?raw=true "Object detection models")

主流的目标检测框架依旧是两大类，一类是region proposal + refinement，检测分两步进行，以Faster R-CNN, R-FCN 等为代表；另一类是single stage的，一步到位回归出物体的位置，以SSD, YOLO 等为代表。 

Google 在16年下半年出了一篇paper [Speed/accuracy trade-offs for modern convolutional object detectors] (https://arxiv.org/abs/1611.10012) ，详细比较了Faster R-CNN、R-FCN和SSD的各个性能指标，还是很值得一读的。
