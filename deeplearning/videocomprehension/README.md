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

1). Two-Stream Convolutional Networks for Action Recognition in Videos

Two Stream方法最初在这篇文章中被提出，基本原理为对视频序列中每两帧计算密集光流，得到密集光流的序列（即temporal信息）。然后对于视频图像（spatial）和密集光流（temporal）分别训练CNN模型，两个分支的网络分别对动作的类别进行判断，最后直接对两个网络的class score进行fusion（包括直接平均和svm两种方法），得到最终的分类结果。注意，对与两个分支使用了相同的2D CNN网络结构，其网络结构见下图。

实验效果：UCF101-88.0%，HMDB51-59.4% 
	
2). Convolutional Two-Stream Network Fusion for Video Action Recognition

这篇论文的主要工作为在two stream network的基础上，利用CNN网络进行了spatial以及temporal的融合，从而进一步提高了效果。此外，该文章还将基础的spatial和temporal网络都换成了VGG-16 network。

实验效果：UCF101-92.5%，HMDB51-65.4% 
	
3). Temporal Segment Networks: Towards Good Practices for Deep Action Recognition

这篇文章是港中文Limin Wang大神的工作，他在这方面做了很多很棒的工作，可以followt他的主页：http://wanglimin.github.io/ 。

这篇文章提出的TSN网络也算是spaital+temporal fusion，结构图见下图。这篇文章对如何进一步提高two stream方法进行了详尽的讨论，主要包括几个方面（完整内容请看原文）： 

输入数据的类型：除去two stream原本的RGB image和 optical flow field这两种输入外，这篇文章中还尝试了RGB difference及 warped optical flow field两种输入。最终结果是 RGB+optical flow+warped optical flow的组合效果最好。

网络结构：尝试了GoogLeNet,VGGNet-16及BN-Inception三种网络结构，其中BN-Inception的效果最好。

训练策略：包括 跨模态预训练，正则化，数据增强等。

实验效果：UCF101-94.2%，HMDB51-69.4% 

4). Beyond Short Snippets: Deep Networks for Video Classification
	
这篇文章主要是用LSTM来做two-stream network的temporal融合。效果一般

实验效果：UCF101-88.6% 
		
##### C3D Network

1). Learning spatiotemporal features with 3d convolutional networks

C3D是facebook的一个工作，采用3D卷积和3D Pooling构建了网络。论文笔记见[C3D论文笔记](http://blog.csdn.net/wzmsltw/article/details/61192243) 。通过3D卷积，C3D可以直接处理视频（或者说是视频帧的volume）

实验效果：UCF101-85.2% 可以看出其在UCF101上的效果距离two stream方法还有不小差距。我认为这主要是网络结构造成的，C3D中的网络结构为自己设计的简单结构，如下图所示。

速度：C3D的最大优势在于其速度，在文章中其速度为314fps。而实际上这是基于两年前的显卡了。用Nvidia 1080显卡可以达到600fps以上。所以C3D的效率是要远远高于其他方法的，个人认为这使得C3D有着很好的应用前景。

作者在其 [项目主页](http://vlg.cs.dartmouth.edu/c3d/) 放出了新版本的Res-C3D网络的caffe模型，但论文还没放出，估计是ICCV2017的投稿文章。新版本的模型大小是之前的一半，速度比C3D快了很多，效果也比之前提高了几个百分点（UCF上）。非常期待看到这个论文，等放出后也会好好写博客讨论一下的。
	
##### 其他方法

1). A Key Volume Mining Deep Framework for Action Recognition

本文主要做的是key volume的自动识别。通常都是将一整段动作视频进行学习，而事实上这段视频中有一些帧与动作的关系并不大。因此进行关键帧的学习，再在关键帧上进行CNN模型的建立有助于提高模型效果。本文达到了93%的正确率吗，为目前最高。

实验效果：UCF101-93.1%，HMDB51-63.3%

[Deep Temporal Linear Encoding Networks]

本文主要提出了“Temporal Linear Encoding Layer” 时序线性编码层，主要对视频中不同位置的特征进行融合编码。至于特征提取则可以使用各种方法，文中实验了two stream以及C3D两种网络来提取特征。

实验效果：UCF101-95.6%，HMDB51-71.1% （特征用two stream提取）。应该是目前为止看到效果最好的方法了（CVPR2017里可能会有更好的效果）
	
### 小结

可以看出，这几年action recognition领域发展的非常快，有各种各样的方法被提出。但要注意，action recognition一般是对预先分割过的短视频进行分类，而真实环境中的视频一般都是没有预先切分过的，而且会包含大量无关信息。所以我认为这个领域的研究很像对Image Classification的研究，比较基础，可以为相关领域的研究提供有力的工具。


##  Temporal Action Detection(时序行为检测), [转载](http://blog.csdn.net/wzmsltw/article/details/70849132)

### 任务目的

给定一段未分割的长视频，算法需要检测视频中的行为片段（action instance），包括其开始时间、结束时间以及类别。一段视频中可能包含一个或多个行为片段。

### 任务特点与难点

1). action recognition与temporal action detection之间的关系同 image classfication与 object detection之间的关系非常像。基于image classification问题，发展出了许多强大的网络模型（比如ResNet,VGGNet等），这些模型在object detection的方法中起到了很大的作用。同样，action recognition的相关模型（如2stream，C3D, iDT等)也被广泛的用在temporal action detection的方法中。
2). 由于temporal action detection和object detection之间存在一定的相似性，所以很多temporal action detection方法都采用了与一些object detection方法相似的框架（最常见的就是参考R-CNN系列方法）。具体的会在后面的论文介绍中讲到。
3). 时序行为检测的难点1：在目标检测中，物体目标的边界通常都是非常明确的，所以可以标注出较为明确的边界框。但时序行为的边界很多时候并不是很明确，什么时候一个行为算开始，什么时候行为算结束常常无法给出一个准确的边界（指精确的第几帧）。
4). 时序行为检测的难点2：只使用静态图像的信息，而不结合时序信息在行为识别中是可以的（虽然结合时序信息效果会更好）。但在时序行为检测中，是无法只使用静态图像信息的。必须结合时序的信息，比如使用RNN读入每帧图像上用CNN提取的特征，或是用时序卷积等。
5). 时序行为检测的难点3：时序行为片段的时间跨度变化可能非常大。比如在ActivityNet中，最短的行为片段大概1s左右，最长的行为片段则超过了200s。巨大的时长跨度，也使得检测时序动作非常难。

### 任务关键点

1). 高质量的时序片段（行为的时序边界）：很多方法都是使用Proposal + classification的框架。对于这类方法，重要的是较高的proposal质量（即在保证平均召回率的情况下，尽可能减少proposal的数量）。此外，对于所有方法，获取准确的时序行为边界都是非常重要的。

2). 准确的分类（行为的类别）：即能准确的得到时序行为片段的类别信息。这里通常都会使用行为识别中的一些方法与模型。

### 数据库

1). THUMOS 2014：该数据集即为THUMOS Challenge 2014，地址为 THUMOS14。该数据集包括行为识别和时序行为检测两个任务。它的训练集为UCF101数据集，包括101类动作，共计13320段分割好的视频片段。THUMOS2014的验证集和测试集则分别包括1010和1574个未分割过的视频。在时序行为检测任务中，只有20类动作的未分割视频是有时序行为片段标注的，包括200个验证集视频（包含3007个行为片段）和213个测试集视频（包含3358个行为片段）。这些经过标注的未分割视频可以被用于训练和测试时序行为检测模型。实际上之后还有THUMOS Challenge 2015,包括更多的动作类别和视频数，但由于上面可以比较的方法不是很多，所以目前看到的文章基本上还是在THUMOS14上进行实验。

2). MEXaction2：MEXaction2数据集中包含两类动作：骑马和斗牛。该数据集由三个部分组成：YouTube视频，UCF101中的骑马视频以及INA视频，数据集地址为MEXaction2 。其中YouTube视频片段和UCF101中的骑马视频是分割好的短视频片段，被用于训练集。而INA视频为多段长的未分割的视频，时长共计77小时，且被分为训练，验证和测试集三部分。训练集中共有1336个行为片段，验证集中有310个行为片段，测试集中有329个行为片断。且MEXaction2数据集的特点是其中的未分割视频长度都非常长，被标注的行为片段仅占视频总长的很低比例

3). ActivityNet: 目前最大的数据库，同样包含分类和检测两个任务。数据集地址为ActivityNet ，这个数据集仅提供视频的youtube链接，而不能直接下载视频，所以还需要用python中的youtube下载工具来自动下载。该数据集包含200个动作类别，20000（训练+验证+测试集）左右的视频，视频时长共计约700小时。由于这个数据集实在太大了，我的实验条件下很难完成对其的实验，所以我之前主要还是在THUMOS14和MEXaction2上进行实验。

### 研究进展

temporal action detection近年的文章很多，这里也只简单介绍比较有代表性的几个工作。此外，此处仅介绍2016年底之前发布的相关工作，CVPR2017上的新工作见[知乎回答](https://www.zhihu.com/question/57523080/answer/158568414) 。为了方便对比，下面的数据库均只介绍其在THUMOS14数据库上的结果。

1). End-to-end learning of action detection from frame glimpses in videos (CVPR2016)

这篇文章是李飞飞实验室的工作。这篇文章使用强化学习的方法训练了一个基于RNN的代理(agent,不太确定应该怎么翻译)。这个agent不断观察视频帧并不断决定接下来要看哪里以及什么时候要生成一个动作预测。与后面很多基于proposal的方法不同，该方法是end-to-end且是直接生成行为预测的。

该方法在THUMOS14上的mAP为17.1%（重叠度阈值取0.5）

2). Temporal Action Localization with Pyramid of Score Distribution Features (CVPR2016)

该方法在特征方面使用了传统的iDT特征。简单来说，该方法基于iDT特征对视频提取了一种分数分布金字塔特征(Pyramid of Score Distribution Feature, PSDF).之后再使用了LSTM网络对PSDF特征序列进行处理，并根据输出的frame-level的行为类别置信度分数处理得到行为片段的预测。

PSDF方法效果还是不错的，表明传统特征其实也还有一战之力。但是iDT特征的提取实在是太耗时/耗储存空间了。(iDT特征的大小要比原始视频的大小都要大很多)。个人认为用iDT特征做行为检测的前景不大。
该方法在THUMOS14上的mAP为18.8%（重叠度阈值取0.5）

3). Temporal action localization in untrimmed videos via multi-stage cnns (CVPR2016)

该方法的整体框架图如下所示。我之前写过这篇文章的论文笔记，见http://blog.csdn.net/wzmsltw/article/details/65437295 ，代码见https://github.com/zhengshou/scnn/ 。该方法首先使用滑窗的方法生成多种尺寸的视频片段(segment)，再使用多阶段的网络（Segment-CNN)来处理。SCNN主要包括三个子网络，均使用了C3D network。第一个是proposal network，用来判断当前输入的视频片段是一个动作的概率；第二个为classification network,该网络用于给视频片段分类，但该网络不用于测试环节，而只是用作初始化localization network；第三个子网络为localization network，该网络的输出形式依旧为类别的概率，但在训练时加入了重叠度相关的损失函数，使得网络能更好的估计一个视频片段的类别和重叠度。最后采用了非极大化抑制（NMS）来去除重叠的片段，完成预测。

该方法实际上采用了类似于R-CNN的思路，后面有不少文章也采用了类似的思想，即先提proposal，再分类。

该方法在THUMOS14上的mAP为19.0%（重叠度阈值取0.5）

4). Efficient Action Detection in Untrimmed Videos via Multi-Task Learning (WACV2016)

这篇文章实际上是将SCNN的多阶段网络放到了一个网络里面去，通过共享前面的卷积层，加快了算法的速度。但从效果上看，基本与SCNN完全相同。感觉算是SCNN的完善版本。

该方法在THUMOS14上的mAP为19.0%（重叠度阈值取0.5）.其余重叠度阈值下的表现也与SCNN基本一致。

### 小结与讨论

这两年时序行为检测领域发展的非常快，相对于目标检测来说，时序行为检测这个方向做的人相对还是比较少，有很大的发（水）展（paper）空间。但一个很大的问题是处理视频数据计算量很大，所以这方面的研究很受硬件条件的影响，羡慕大佬们论文中各种四路titan，四路k40的配置啊。之后会再专门写笔记介绍CVPR2017中该领域的进展情况。


## video captioning (给视频生成文字描述) [转载](http://blog.csdn.net/wzmsltw/article/details/71192385)

### 数据库

1). MSR-VTT dataset: 该数据集为ACM Multimedia 2016 的 Microsoft Research - Video to Text (MSR-VTT) Challenge。地址为 MSR-VTT dataset 。该数据集包含10000个视频片段（video clip），被分为训练，验证和测试集三部分。每个视频片段都被标注了大概20条英文句子。此外，MSR-VTT还提供了每个视频的类别信息（共计20类），这个类别信息算是先验的，在测试集中也是已知的。同时，视频都是包含音频信息的。该数据库共计使用了四种机器翻译的评价指标，分别为：METEOR, BLEU@1-4,ROUGE-L,CIDEr。
2). YouTube2Text dataset(or called MSVD dataset):该数据集同样由Microsoft Research提供，地址为 MSVD dataset 。该数据集包含1970段YouTube视频片段（时长在10-25s之间），每段视频被标注了大概40条英文句子。

### 任务关键点分析

video captioning任务可以理解为视频图像序列到文本序列的seq2seq任务。在近年的方法中，大部分文章都使用了LSTM来构造encoder-decoder结构，即使用lstm encoder来编码视频图像序列的特征，再用lstm decoder解码出文本信息。这样的video captioning模型结构最早在ICCV2015的”Sequence to Sequence – Video to Text”一文中提出，如下图所示。

![Alt text](s2s?raw=true "video captioning")

基于上图中的结构，构造一个encoder-decoder结构的模型主要包括几个关键点：

1). 输入特征：即如何提取视频中的特征信息，在很多篇文章中都使用了多模态的特征。主要包括如下几种： 

	基于视频图像的信息：包括简单的用CNN（VGGNet, ResNet等）提取图像(spatial)特征，用action recognition的模型(如C3D)提取视频动态(spatial+temporal)特征
	
	基于声音的特征：对声音进行编码，包括BOAW（Bag-of-Audio-Words)和FV(Fisher Vector)等
	
	先验特征：比如视频的类别，这种特征能提供很强的先验信息
	
	基于文本的特征：此处基于文本的特征是指先从视频中提取一些文本的描述，再將这些描述作为特征，来进行video captioning。这类特征我看到过两类，一类是先对单帧视频进行image captioning,将image captioning的结果作为video captioning的输入特征，另外一类是做video tagging，将得到的标签作为特征。
	
2). encoder-decoder构造：虽然大部分工作都是用lstm做encoder-decoder，但各个方法的具体配置还是存在着一定的差异。

3).  输出词汇的表达：主要包括两类，一类是使用Word2Vec这种词向量表示，另外就是直接使用词袋表示。

4). 其它部分：比如训练策略，多任务训练之类的。

### 论文介绍

ACM MultiMedia 2016： MSR-VTT Challenge

首先介绍一下上述数据库MSR-VTT这个竞赛当时的前5名的方案。Team指竞赛中的队伍名称，rank为竞赛的M1排名，paper为对应的方案描述，可以通过google scholar搜到。

1). Team- Aalto; Rank-1; Paper: Frame- and Segment-Level Features and Candidate Pool Evaluation for Video Caption Generation

这个方法的思路很有趣。它先用多个基于不同特征的video caption方法（均为encoder-decoder结构）对视频生成多段描述。再构造了一个基于CNN的评价网络，如下图所示，输入为video caption方法得到的句子和视频的特征，输出为两者之间的匹配度。这个评价网络实际上是作为多个video caption模型的ensemble方法。

2). Team- v2t_navigator; Rank-2; Paper: Describing Videos using Multi-modal Fusion

方法框架采用标准的encoder-decoder模式。

	特征方面：这篇文章基本用上了所有能用到的多模态特征，包括视频，图像，声音，类别信息等等。

	encoder：为了结合各类特征，该方法使用了单层的无激活函数（应该也可以叫线性激活？）的全连接层作为编码器。

	decoder：单层单向LSTM

	其他：各种用于特征提取的模型都是预先训练好的，encoder和decoder同时训练。
	
3). Team- VideoLAB; Rank-3; Paper: Multimodal Video Description

方法框架也是encoder-decoder模式，与v2t_navigator的方法比较像

	特征方面：使用了视频，图像，声音，类别信息等，基本和v2t_navigator队相同
	
	encoder and decoder： 均为lstm
	
4). Team- ruc-uva; Rank-5; Paper:Early Embedding and Late Reranking for Video Captioning

这篇文章的方法复杂一些，在使用CNN提取了视频特征后，该方法使用了Video tagging方法提取视频的关键词，这些关键词和视频CNN特征一同作为decoder的输入。此外，tagging产生的关键词还在最后用于生成句子的排序。

2017年的几篇新论文

1). Multi-Task Video Captioning with Video and Entailment Generation（ACL2017）

这篇文章的想法很有趣，其方法结构图如下图所示。整个框架共包括三个任务：1)Unsupervised video prediction:无监督的未来帧预测，使用video encoder 和video decoder；2) Entailment Generation: 同义句生成/句子含义生成，也就是给句子生成含义相似的新句子，使用language encoder和 language decoder；3)Video Captioning:也就是视频语义生成，使用了video encoder和language decoder。在这几个任务中，video encoder和laguange decoder的参数是共享的。在训练中，该方法在mini-batch层面对三个任务进行迭代训练（不同任务的训练次数比例由参数确定）。

![Alt text](acl2017.png?raw=true "Multi-Task Video Captioning with Video and Entailment Generation")

2). Weakly Supervised Dense Video Captioning（CVPR2017）

这篇文章主要研究的是dense video captioning问题，dense video captioning是要产生对一段视频所有可能的描述。本文提出了一种弱监督方法，Multi-instance multi-label learning(MIMLL) 。MIMLL直接从视频-句子 数据（这样的数据对于该问题算是弱监督的）中学习每个视频图像区域对应的描述词汇向量。之后将这些词汇描述向量结合起来作为encoder-decoder的输入，实现video captioning。在MSR-VTT数据集中，该方法是目前的state-of-the-art 方法。

![Alt text](cvpr2017.png?raw=true "Weakly Supervised Dense Video Captioning")

### 小结与讨论

从以上的论文介绍可以看出，基于基础的encoder-decoder结构，不同的方法在不同的方面对提高模型效果进行了探索，包括多模态特征，多任务学习等等。该领域现在也处在快速发展阶段，新的方法层出不穷，改进模型的核心思想我认为还是如何去更好的挖掘视频中局部的语义信息，并将其有效的组合成自然语言。同时，我觉得这个方向也是很有前景的一个方向，给视频生成文字描述有很多的应用场景。
