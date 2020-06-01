# server
linux/c++/epoll/lua/mongo
开始重构之前的gameserver:
1.配置文件导入:
  a.采用lua文件配置
  b.准备基本代码目录，导入lua5.1的源码, 因为读lua文件配置用lua虚拟机比较方便
