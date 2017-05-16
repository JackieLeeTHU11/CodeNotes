### caffemodel2prototxt.py

根据caffemodel反推prototxt

### softmax_loss_layer_classweight_fiveclass.cpp

	原型是"softmax_loss_layer.cpp",在此基础上为每一个class增加weight,可以用于样本不均衡等问题

### softmax_loss_partiallabel_layer.cpp

	原型是"softmax_loss_layer.cpp",在此基础上增加了partiallabel,用于数据只标注了一部分,而另一部分未标注的情况
