# 系统概要设计
![](images/system.png)

**处理攻击**时需要进一步分析流量窗口中采集到的异常数据包，提取其中的某些特征并进行特征匹配，当某种特征的匹配数量超过一定值后，即可判定与该特征匹配的数据包确实为异常的.

**需要提取的特征包括：**

1. 数据包类型
2. 源地址（MAC）
3. 目的地址（MAC,IP,端口号)

