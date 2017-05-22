## Action recognition

### 目的

给一个视频片段进行分类，类别通常是各类人的动作

### 难点/关键点

1) 强有力的特征：即如何在视频中提取出能更好的描述视频判断的特征。特征越强，模型的效果通常较好。
2) 特征的编码（encode）/融合（fusion）：这一部分包括两个方面，第一个方面是非时序的，在使用多种特征的时候如何编码/融合这些特征以获得更好的效果；另外一个方面是时序上的，由于视频很重要的一个特性就是其时序信息，一些动作看单帧的图像是无法判断的，只能通过时序上的变化判断，所以需要将时序上的特征进行编码或者融合，获得对于视频整体的描述。
3) 算法速度：虽然在发论文刷数据库的时候算法的速度并不是第一位的。但高效的算法更有可能应用到实际场景中去。

### Data

1) UCF101:来源为YouTube视频，共计101类动作，13320段视频。共有5个大类的动作：1)人-物交互；2)肢体运动；3)人-人交互；4)弹奏乐器；5)运动。[数据库主页](http://crcv.ucf.edu/data/UCF101.php)
2) HMDB51:来源为YouTube视频，共计51类动作，约7000段视频。[数据库主页](http://serre-lab.clps.brown.edu/resource/hmdb-a-large-human-motion-database/#Downloads)
	
### 研究进展

#### 传统方法 iDT(improved dense trajectories)

1) iDT方法（13年）是深度学习进入该领域前效果最好，稳定性最好，可靠性最高的方法，不过算法速度很慢。这个方法是该实验室之前工作（Dense Trajectories and Motion Boundary Descriptors for Action Recognition）的改进。此前写的笔记见[iDT论文笔记](http://blog.csdn.net/wzmsltw/article/details/53023363)

2) 基本思路：DT算法的基本思路为利用光流场来获得视频序列中的一些轨迹，再沿着轨迹提取HOF，HOG，MBH，trajectory4种特征，其中HOF基于灰度图计算，另外几个均基于dense optical flow计算。最后利用FV（Fisher Vector）方法对特征进行编码，再基于编码结果训练SVM分类器。而iDT改进的地方在于它利用前后两帧视频之间的光流以及SURF关键点进行匹配，从而消除/减弱相机运动带来的影响，改进后的光流图像被成为warp optical flow

![Alt text](./idt.jpg?raw=true "iDT model")
	
#### 深度学习方法

#####  Two Stream Network及衍生方法

[Two-Stream Convolutional Networks for Action Recognition in Videos]

Two Stream方法最初在这篇文章中被提出，基本原理为对视频序列中每两帧计算密集光流，得到密集光流的序列（即temporal信息）。然后对于视频图像（spatial）和密集光流（temporal）分别训练CNN模型，两个分支的网络分别对动作的类别进行判断，最后直接对两个网络的class score进行fusion（包括直接平均和svm两种方法），得到最终的分类结果。注意，对与两个分支使用了相同的2D CNN网络结构，其网络结构见下图。

实验效果：UCF101-88.0%，HMDB51-59.4% 
	
[Convolutional Two-Stream Network Fusion for Video Action Recognition]

这篇论文的主要工作为在two stream network的基础上，利用CNN网络进行了spatial以及temporal的融合，从而进一步提高了效果。此外，该文章还将基础的spatial和temporal网络都换成了VGG-16 network。

实验效果：UCF101-92.5%，HMDB51-65.4% 
	
[Temporal Segment Networks: Towards Good Practices for Deep Action Recognition]

这篇文章是港中文Limin Wang大神的工作，他在这方面做了很多很棒的工作，可以followt他的主页：http://wanglimin.github.io/ 。

这篇文章提出的TSN网络也算是spaital+temporal fusion，结构图见下图。这篇文章对如何进一步提高two stream方法进行了详尽的讨论，主要包括几个方面（完整内容请看原文）： 

输入数据的类型：除去two stream原本的RGB image和 optical flow field这两种输入外，这篇文章中还尝试了RGB difference及 warped optical flow field两种输入。最终结果是 RGB+optical flow+warped optical flow的组合效果最好。

网络结构：尝试了GoogLeNet,VGGNet-16及BN-Inception三种网络结构，其中BN-Inception的效果最好。

训练策略：包括 跨模态预训练，正则化，数据增强等。

实验效果：UCF101-94.2%，HMDB51-69.4% 

[Beyond Short Snippets: Deep Networks for Video Classification]
	
这篇文章主要是用LSTM来做two-stream network的temporal融合。效果一般

实验效果：UCF101-88.6% 
		
##### C3D Network

[Learning spatiotemporal features with 3d convolutional networks]

C3D是facebook的一个工作，采用3D卷积和3D Pooling构建了网络。论文笔记见[C3D论文笔记](http://blog.csdn.net/wzmsltw/article/details/61192243) 。通过3D卷积，C3D可以直接处理视频（或者说是视频帧的volume）

实验效果：UCF101-85.2% 可以看出其在UCF101上的效果距离two stream方法还有不小差距。我认为这主要是网络结构造成的，C3D中的网络结构为自己设计的简单结构，如下图所示。

速度：C3D的最大优势在于其速度，在文章中其速度为314fps。而实际上这是基于两年前的显卡了。用Nvidia 1080显卡可以达到600fps以上。所以C3D的效率是要远远高于其他方法的，个人认为这使得C3D有着很好的应用前景。

作者在其 [项目主页](http://vlg.cs.dartmouth.edu/c3d/) 放出了新版本的Res-C3D网络的caffe模型，但论文还没放出，估计是ICCV2017的投稿文章。新版本的模型大小是之前的一半，速度比C3D快了很多，效果也比之前提高了几个百分点（UCF上）。非常期待看到这个论文，等放出后也会好好写博客讨论一下的。
	
##### 其他方法

[A Key Volume Mining Deep Framework for Action Recognition]

本文主要做的是key volume的自动识别。通常都是将一整段动作视频进行学习，而事实上这段视频中有一些帧与动作的关系并不大。因此进行关键帧的学习，再在关键帧上进行CNN模型的建立有助于提高模型效果。本文达到了93%的正确率吗，为目前最高。

实验效果：UCF101-93.1%，HMDB51-63.3%

[Deep Temporal Linear Encoding Networks]

本文主要提出了“Temporal Linear Encoding Layer” 时序线性编码层，主要对视频中不同位置的特征进行融合编码。至于特征提取则可以使用各种方法，文中实验了two stream以及C3D两种网络来提取特征。

实验效果：UCF101-95.6%，HMDB51-71.1% （特征用two stream提取）。应该是目前为止看到效果最好的方法了（CVPR2017里可能会有更好的效果）
	
### 小结

可以看出，这几年action recognition领域发展的非常快，有各种各样的方法被提出。但要注意，action recognition一般是对预先分割过的短视频进行分类，而真实环境中的视频一般都是没有预先切分过的，而且会包含大量无关信息。所以我认为这个领域的研究很像对Image Classification的研究，比较基础，可以为相关领域的研究提供有力的工具。


