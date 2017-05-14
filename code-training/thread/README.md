## python多线程--线程同步
如果多个线程共同对某个数据修改，则可能出现不可预料的结果，为了保证数据的正确性，需要对多个线程进行同步。使用Thread对象的Lock和Rlock可以实现简单的线程同步，这两个对象都有acquire方法和release方法，对于那些需要每次只允许一个线程操作的数据，可以将其操作放到acquire和release方法之间。如下：
多线程的优势在于可以同时运行多个任务（至少感觉起来是这样）。但是当线程需要共享数据时，可能存在数据不同步的问题。

考虑这样一种情况：一个列表里所有元素都是0，线程"set"从后向前把所有元素改成1，而线程"print"负责从前往后读取列表并打印。

那么，可能线程"set"开始改的时候，线程"print"便来打印列表了，输出就成了一半0一半1，这就是数据的不同步。为了避免这种情况，引入了锁的概念。

锁有两种状态——锁定和未锁定。每当一个线程比如"set"要访问共享数据时，必须先获得锁定；如果已经有别的线程比如"print"获得锁定了，那么就让线程"set"暂停，也就是同步阻塞；等到线程"print"访问完毕，释放锁以后，再让线程"set"继续。

经过这样的处理，打印列表时要么全部输出0，要么全部输出1，不会再出现一半0一半1的尴尬场面。

### threading acquire 和 release来实现数据同步

Python 在thread的基础上还提供了一个高级的线程控制库，就是之前提到过的threading。Python的threading module是在建立在thread module基础之上的一个module，在threading module中，暴露了许多thread module中的属性。在thread module中，python提供了用户级的线程同步工具“Lock”对象。而在threading module中，python又提供了Lock对象的变种: RLock对象。RLock对象内部维护着一个Lock对象，它是一种可重入的对象。对于Lock对象而言，如果一个线程连续两次进行acquire操作，那么由于第一次acquire之后没有release，第二次acquire将挂起线程。这会导致Lock对象永远不会release，使得线程死锁。RLock对象允许一个线程多次对其进行acquire操作，因为在其内部通过一个counter变量维护着线程acquire的次数。而且每一次的acquire操作必须有一个release操作与之对应，在所有的release操作完成之后，别的线程才能申请该RLock对象。

### condition

锁只能提供最基本的同步。假如只在发生某些事件时才访问一个“临界区”，这时需要使用条件变量Condition。
Condition对象是对Lock对象的包装，在创建Condition对象时，其构造函数需要一个Lock对象作为参数，如果没有这个Lock对象参数，Condition将在内部自行创建一个Rlock对象。在Condition对象上，当然也可以调用acquire和release操作，因为内部的Lock对象本身就支持这些操作。但是Condition的价值在于其提供的wait和notify的语义。
条件变量是如何工作的呢？首先一个线程成功获得一个条件变量后，调用此条件变量的wait()方法会导致这个线程释放这个锁，并进入“blocked”状态，直到另一个线程调用同一个条件变量的notify()方法来唤醒那个进入“blocked”状态的线程。如果调用这个条件变量的notifyAll()方法的话就会唤醒所有的在等待的线程。
如果程序或者线程永远处于“blocked”状态的话，就会发生死锁。所以如果使用了锁、条件变量等同步机制的话，一定要注意仔细检查，防止死锁情况的发生。对于可能产生异常的临界区要使用异常处理机制中的finally子句来保证释放锁。等待一个条件变量的线程必须用notify()方法显式的唤醒，否则就永远沉默。保证每一个wait()方法调用都有一个相对应的notify()调用，当然也可以调用notifyAll()方法以防万一。


