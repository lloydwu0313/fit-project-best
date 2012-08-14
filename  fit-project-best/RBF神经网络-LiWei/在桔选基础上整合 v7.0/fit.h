#pragma once
#include <iostream>
#include <fstream>
#include<string>
#include<ctime>
#include<cstdlib>
#include <math.h>
#include <iostream>
#include"ThostTraderApi/ThostFtdcUserApiStruct.h"
#include <iostream>
#include <statement.h>
#define INIVALUE 99999.9//价格数组的初始值
#define MaxNumberOfRecord 3600  //一次性获取的最大的记录数目
#define pin1 "ag1212"//品种1的代号
#define pin2 "ag1209"//品种2的代号

using namespace std;


class PriceList{
//private :
	
	int fence;//fence-1之前的数据都是写入的有效的数据，fence本身是无效数据
	//fence=0;
public:	
	float priceArray[MaxNumberOfRecord];
	PriceList();
	int getFence();
	void add(double price,char* time);
	void storeToDb(TThostFtdcTimeType traTime,double price,TThostFtdcDateType traDate,TThostFtdcInstrumentIDType id);
	void storeToGapDb(TThostFtdcInstrumentIDType id1,TThostFtdcInstrumentIDType id2,double price,TThostFtdcTimeType traTime,TThostFtdcDateType traDate,int tradestate);
	double getPreAvg(int preNum);//preNum是需要的前几个数据(包括自身)的平均值
	double getPreStd(int preNum,double avg);
	double getNowPriceGap();
	void getDataFromDb();
	void getDataFromDb_all();
	void readmdfromdb();
	void addforfirsttime(double price);
};
//本来是想把注册和发送交易请求分开，但是目前还不可以，所以makeTraderDecision和，makeTraderDecisionIni只能有maketotaltrade来替代了
 void makeTraderDecision(TThostFtdcInstrumentIDType instr,TThostFtdcDirectionType direct,TThostFtdcPriceType pric,TThostFtdcVolumeType tranum);
  void makeTraderDecisionIni();
  void getdata();
  void maketotaltrade(TThostFtdcInstrumentIDType instr,TThostFtdcDirectionType direct,TThostFtdcPriceType pric,TThostFtdcVolumeType tranum);



//trade.cpp  
 inline void duojiacang( char* p1, char* p2,double price1,double price2,int n);//多单策略下的加仓，买入某种期货，p是品种id，n是买卖的手数
 inline void duopingcang( char* p1, char* p2,double price1,double price2,int n);//多单策略下的减仓，卖出某种期货，p是品种id，n是买卖的手数
 inline void kongjiacang( char* p1, char* p2,double price1,double price2,int n);//空加仓，买入某种期货，p是品种id，n是买卖的手数
 inline void kongpingcang( char* p1, char* p2,double price1,double price2,int n); //空单单策略下的策略下的平仓，买入某种期货，p是品种id，n是买卖的手数
 void trade(PriceList list,TThostFtdcPriceType avg,TThostFtdcTimeType traTime,double pricegap,TThostFtdcDateType traDate);//交易决策函数，统计套利的核心思想
 
 //nervenetwork.cpp
 //取得预测值，由威哥来补充这个函数
double nervenet(PriceList list,int n,double m);
void readgap(PriceList list);
void gapforrbf_write(PriceList list);
void fortest_write(PriceList list);
double rbf_result(PriceList list);

void TimeStructToHMS(const char* time,int& hour,int& minute,int& second);

 //CTP.cpp
 //void getMarketData();//获得行情
// void makeTraderDecision();//交易决策
void sqlini();
void sqlend(const char*);
int getnumofmisecond(const int h1,const int m1 ,const int s1);

