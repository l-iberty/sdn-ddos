# 实验环境搭建
OS: Ubuntu 14.04.5 LTS x86_x64

## [一] Floodlight
### Download, Update and Build
```
$ git clone http://github.com/floodlight/floodlight.git
$ cd floodlight
$ git pull origin master
$ git submodule init
$ git submodule update
$ ant
```
### Running Floodlight in the Terminal
```
$ java -jar target/floodlight.jar
```

### 登录Floodlight的web管理界面
在浏览器中输入URL`localhost:8080/ui/index.html`

![](images/floodlight-web.png)

### Developing Floodlight in Eclipse
```
$ ant eclipse
```

This creates several files: `Floodlight.launch`, `Floodlight_junit.launch`, `.classpath`, and `.project`. 

- `Import existing projects`
- `Select root directory`选择floodlight文件夹
- 在`Project Explorer`中右键`floodlight`->`Run as`->`Run Configurations`, `Name`输入`FloodlightLaunch`，`Project`输入`floodlight`,`Main class`输入`net.floodlightcontroller.core.Main`, `Apply`并`Run`，floodlight就运行起来了，打开浏览器可以正常访问管理界面.

### Developing Floodlight in IntelliJ IDEA
- 下载并更新源码后**不要**用`ant`进行build, Idea直接从floodlight文件夹导入源码，不要使用外部框架（Eclipse,Maven,Gradle）.
- 右键单击`floodlight/src/main/resources`->`Mark Directory as`->`Resources root`.
- 进行如下配置后, `Run->Run 'Floodlight'`, floodlight就运行起来了，打开浏览器可以正常访问管理界面.

![](images/idea-setting.png)

### 配置信息
缺省配置信息在`floodlight/src/main/resources/floodlightdefault.properties`，如`http`端口`8080`, `openflow`端口`6653`

## [二]Mininet
使用VirtualBox创建一个虚拟机，网络配置选择桥接，如下：

![](images/vm-setting-network.png)

确保虚拟机和宿主机互相ping通

在虚拟机中下载`mininet`源码进行安装：

```
$ git clone https://github.com/mininet/mininet.git
$ cd mininet/util
$ ./install.sh
```

测试安装是否正确：

```
$ sudo mn --test pingall # 登录mininet,测试ping连通性
```

得到如下结果则说明安装成功:

![](images/mininet-test.png)

## [三]mininet连接floodlight控制器
在宿主机中打开`floodlight`，确保能登录web管理界面. 在虚拟机中启动mininet:

```
sudo mn --controller=remote,ip=10.168.1.144,port=6653
```

- `10.168.1.144`是宿主机的ip
- `6653`是宿主机上的openflow监听端口

mininet与floodlight成功连接后，在floodlight的管理界面可以看到新增的hosts:

![](images/floodlight-hosts.png)

此时在宿主机打开Wireshark，可以抓到虚拟机和宿主机的OpenFlow包:

![](images/wireshark.png)