# MyPacketInProcessor
一个处理`Packet-In`的基本框架

## 在IntelliJ IDEA中完成开发
新建一个`Java Class`
```
net.floodlightcontroller.mypacketinprocessor.MyPacketInProcessor
```
注册模块该模块的步骤参考`MacTracker`.
**注意:**`MacTracker`和`MyPacketInProcessor`存在冲突，需在配置文件中删除`MacTracker`的配置信息.