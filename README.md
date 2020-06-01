# server
linux/c++/epoll/lua/mongo

开始重构之前的gameserver:

1.配置文件导入:
  
    a.采用lua文件配置
  
    b.准备基本代码目录，导入lua5.1的源码, 因为读lua文件配置用lua虚拟机比较方便
  
	c.读取参数: 配置文件目录

	d.导入lunar的源码, 调用lunar封装过的luaapi来获取脚本数据更方便
