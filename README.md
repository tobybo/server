# server
linux/c++/epoll/lua/mongo

开始重构之前的gameserver:

1.配置文件导入: over
  
    a.采用lua文件配置
  
    b.准备基本代码目录，导入lua5.1的源码, 因为读lua文件配置用lua虚拟机比较方便
  
	c.读取参数: 配置文件目录

	d.导入lunar的源码, 调用lunar封装过的luaapi来获取脚本数据更方便

2.守护进程模式: over
    
    a.信号处理接口：关服信号要具体化 SIGINI-reload SIGUSR1-stop SIGUSR2-breakwhile

	b.一个控制静态变量构造和析构的类

3.日志系统: over
    
    a.定时定帧: 每秒更新一次日志时间串 格式为: "[2020-06-08 11:49:50][00000001] 具体日志信息" 日期+帧数+具体信息 
		
		1.一个线程来控制全局定时变量 gMsec 定到整毫秒 gTime 定到整秒.
    
    b.分文件写入: 一小时一个文件，一个日志级别一个文件
    
    c.维护一个日志buff空闲list 和 一个日志buff待写入文件list
    
		1.定义 CMlist<T> 类，封装 std::list 的几个主要方法;
		
		2.自动加锁解锁，自动检查条件变量，自动唤醒线程
		
		3.定义 CLock，构造函数加锁，析构函数解锁
    
    d.定义一个线程抽象基类：子类实现run方法，调用父类的start(用友元函数void* thread_start(void*)作为线程执行函数的入口,将子类对象转化成void*作为参数)，友元函数内部再调用run方法来执行实际逻辑。
    
4.内存池: over

	a.模板类，内存单元为单个对象

	b.由于分配大小固定，所以采用freelist算法即可

	c.超出预分配大小自由分配，记录分配情况

	d.定义重载new，delete的宏，方便使用内存池的类重载new，delete运算符

