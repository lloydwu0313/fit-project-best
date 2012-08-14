#include <iostream>
#include <fstream>
#include<string>
#include<ctime>
#include<cstdlib>
#include <math.h>
#define MaxNumberOfRecord 300  //一次性获取的最大的记录数目
using namespace std;

//tool.cpp
//工具包，用来读取文件和计算均值标准差
void read(const char*priceTxt,double *priceArray ,int& numOfRecord);//从文本中读取数据到数组中，priceTxt是要读取的文件的对应的char值，price是要传入的数组
void getGap( double*priceArrayGap,int& numOfRecordGap);//计算出价差序列，计算出价差序列
 double getavg(double *g,int n);//求均值，n是给的数据个数，就是前几个，g是数据数组
 double getstd(double*g ,int n);//求标准差，n是给的数据个数，就是前几个，g是数据数组

//trade.cpp  
 inline void duojiacang(const char*p1,const char* p2,int n);//多单策略下的加仓，买入某种期货，p是品种id，n是买卖的手数
 inline void duopingcang(const char*p1,const char* p2,int n);//多单策略下的减仓，卖出某种期货，p是品种id，n是买卖的手数
 inline void kongjiacang(const char*p1,const char* p2,int n);//空加仓，买入某种期货，p是品种id，n是买卖的手数
 inline void kongpingcang(const char*p1,const char* p2,int n); //空单单策略下的策略下的平仓，买入某种期货，p是品种id，n是买卖的手数
 void trade(double*g,int n);//交易决策函数，统计套利的核心思想

 //nervenetwork.cpp
 //取得预测值，由威哥来补充这个函数
 double nervenet(double*g,int n);//n是给的数据个数，就是前几个，g是数据数组

 //tradeinterface.cpp  
 //对一个品种的操作。这些面两个函数由选哥来补充
 void jia(const char*p,int n);//买入一个品种p的期货合约n手
 void jian(const char*p,int n);//卖出一个品种p的期货合约n手
