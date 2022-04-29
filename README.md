# 仿微信客户端开发记录by dyy

### 依据开发记录对不同版本的说明：

**1.MyWeChat局域网：仅仅是局域网聊天用的xml文件读取账号密码；**
**2.MyWeChat_sql打包：只能使用本地的mysql**
**3.MyWeChat_remotesql打包:使用的服务器端的mysql进行登录验证并且实现了文件传输功能**
**4.MyWeChat_remotesql打包2.0：修复了一个文件传输功能的bug**
**5.MyWeChat_remotesql打包3.0:增加用户注册功能。**

**本项目为3.0版本。**

### 4.15号-页面的开发

主要包含两个页面，一个是登录界面，一个是客户端界面。正好自己会剪辑，images全部是自己用Photoshop制作的，一并贴出。

整体运用Qt进行布局开发，后续会使用mysql。

**登录界面如下：**

![LqwRgS.jpg](https://s1.ax1x.com/2022/04/27/LqwRgS.jpg)

**页面聊天界面如下：**

![LxmpwT.png](https://s1.ax1x.com/2022/04/29/LxmpwT.png)



### 4.17号记录-页面登录功能

运用文件存取用户密码，实现用户登录验证功能，跳转窗口。

**分为用户不存在：**

![Lq094x.jpg](https://s1.ax1x.com/2022/04/27/Lq094x.jpg)

**密码错误：**

![Lq0EKe.jpg](https://s1.ax1x.com/2022/04/27/Lq0EKe.jpg)



### 4.20号起-开发基于udp聊天

1.为了简化聊天参考了比较简单的形式，每个用户聊天窗口既是client又是server,通过广播来发送数据，类似于群聊的系统。定义不同发送的消息类型，来判断用户是否上线离线发送消息等情况，最后设置刷新按钮，更新在线的用户，使用QTableWidget实现类似微信的显示。

2.为了在一台电脑上开启测试，所以这里设置共享地址；其次，使用QTableWidget实现类似微信的显示时，注意查手册，一开始没有设置他的列数，就一直不显示，但调试又有结果，所以记录一个坑。

所有人上线提醒并进行聊天：

![LqDacT.jpg](https://s1.ax1x.com/2022/04/27/LqDacT.jpg)



上线设置上线提醒，并可以手动点击刷新进行更新。

下线提醒，从QTableWidget中删除：

![LqDq8P.jpg](https://s1.ax1x.com/2022/04/27/LqDq8P.jpg)

3.为了实现更舒服的界面，将默认的任务栏去掉，添加图片按钮实现其功能；此时，移动拖拽失效，实现无任务栏的移动拖拽。

4.实现具体的tab进行移动，移动对于键盘Enter和回车键的捕捉，实现优化。



###### 最近有点别的事儿，暂且缓一缓，下一步准备实现：

1.利用tcp实现文件的传输。

2.利用数据库实现账号密码的存储、注册。

3.可以实现一些其他例如添加好友等功能。

#### 

### 4.22记录-wechat数据库验证登录（注册）功能更新：

连接数据库部分：暂定采用mysql，注意驱动问题，需要从官网下载https://downloads.mysql.com/archives/c-c/（libmysql.dll）并复制粘贴到，编译目录下

我的为D:\Qt\5.9\mingw53_32\bin。

同时注意：windeployqt *.exe打包文件后，也必须将libmysql.dll拷贝到打包目录，不然还是无法访问远程的数据库。注册功能类似。考虑到本次设计仅仅是peer to peer的模式，可以使用md5加密保证安全性，但是数据库注册则开放为大家都可以访问，所以依旧具有一定的局限性。还是类似QQ通过设计服务器更为稳妥。

数据库采用MySQL开发：

**sql脚本如下：**

```sql
CREATE DATABASE wechat;

INSERT INTO user VALUES(NULL,"111","111");
INSERT INTO user VALUES(NULL,"222","222");


CREATE TABLE user(
	id INT PRIMARY KEY AUTO_INCREMENT,
	username VARCHAR(50) NOT NULL,
	passwd VARCHAR(50) NOT NULL
)DEFAULT CHARSET UTF8;
```

实现了另一个页面进行用户的注册，并验证注册是否有效：

**用户注册页面：**

![LxmAp9.png](https://s1.ax1x.com/2022/04/29/LxmAp9.png)

**用户注册失败：**

![LxmG6I.png](https://s1.ax1x.com/2022/04/29/LxmG6I.png)

**用户注册成功：**

![LxmdAS.png](https://s1.ax1x.com/2022/04/29/LxmdAS.png)

### 4.25记录-开发文件传输功能：

1.设计了文件传输与接受的ui界面

2.完成了文件传输的功能。分为服务器端和客户端，先发送udp包，对方可以选择接受或者拒绝，进而通过tcp传输。

![Lvb654.png](https://s1.ax1x.com/2022/04/29/Lvb654.png)

![Lvb2G9.png](https://s1.ax1x.com/2022/04/29/Lvb2G9.png)

### 4.29记录-解决了文件传输功能的一个bug：

在测试文件传输时，发现了一个bug，点击传输文件后，对于同一个人只能传输一次，页面不会重置。调试发现是因为，我在一开始就初始化好了客户端，所以需要在点击F(发送)按钮后才进行初始化，即可解决。



#### 同时下一步准备实现一下QQ，目前已经做了一个简单的登录界面，如图：

![Lqs8Wq.jpg](https://s1.ax1x.com/2022/04/27/Lqs8Wq.jpg)

#### 提上未来日程，待更新.......

**参考文献**：Qt5开发实例、C++项目开发全程实录、Qt5开发实战、Qt开发人员手册、部分网络博客

**联系方式**：yyding@std.uestc.edu.cn
