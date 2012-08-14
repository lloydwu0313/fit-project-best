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
#define INIVALUE 99999.9//�۸�����ĳ�ʼֵ
#define MaxNumberOfRecord 3600  //һ���Ի�ȡ�����ļ�¼��Ŀ
#define pin1 "ag1212"//Ʒ��1�Ĵ���
#define pin2 "ag1209"//Ʒ��2�Ĵ���

using namespace std;


class PriceList{
//private :
	
	int fence;//fence-1֮ǰ�����ݶ���д�����Ч�����ݣ�fence��������Ч����
	//fence=0;
public:	
	float priceArray[MaxNumberOfRecord];
	PriceList();
	int getFence();
	void add(double price,char* time);
	void storeToDb(TThostFtdcTimeType traTime,double price,TThostFtdcDateType traDate,TThostFtdcInstrumentIDType id);
	void storeToGapDb(TThostFtdcInstrumentIDType id1,TThostFtdcInstrumentIDType id2,double price,TThostFtdcTimeType traTime,TThostFtdcDateType traDate,int tradestate);
	double getPreAvg(int preNum);//preNum����Ҫ��ǰ��������(��������)��ƽ��ֵ
	double getPreStd(int preNum,double avg);
	double getNowPriceGap();
	void getDataFromDb();
	void getDataFromDb_all();
	void readmdfromdb();
	void addforfirsttime(double price);
};
//���������ע��ͷ��ͽ�������ֿ�������Ŀǰ�������ԣ�����makeTraderDecision�ͣ�makeTraderDecisionIniֻ����maketotaltrade�������
 void makeTraderDecision(TThostFtdcInstrumentIDType instr,TThostFtdcDirectionType direct,TThostFtdcPriceType pric,TThostFtdcVolumeType tranum);
  void makeTraderDecisionIni();
  void getdata();
  void maketotaltrade(TThostFtdcInstrumentIDType instr,TThostFtdcDirectionType direct,TThostFtdcPriceType pric,TThostFtdcVolumeType tranum);



//trade.cpp  
 inline void duojiacang( char* p1, char* p2,double price1,double price2,int n);//�൥�����µļӲ֣�����ĳ���ڻ���p��Ʒ��id��n������������
 inline void duopingcang( char* p1, char* p2,double price1,double price2,int n);//�൥�����µļ��֣�����ĳ���ڻ���p��Ʒ��id��n������������
 inline void kongjiacang( char* p1, char* p2,double price1,double price2,int n);//�ռӲ֣�����ĳ���ڻ���p��Ʒ��id��n������������
 inline void kongpingcang( char* p1, char* p2,double price1,double price2,int n); //�յ��������µĲ����µ�ƽ�֣�����ĳ���ڻ���p��Ʒ��id��n������������
 void trade(PriceList list,TThostFtdcPriceType avg,TThostFtdcTimeType traTime,double pricegap,TThostFtdcDateType traDate);//���׾��ߺ�����ͳ�������ĺ���˼��
 
 //nervenetwork.cpp
 //ȡ��Ԥ��ֵ���������������������
double nervenet(PriceList list,int n,double m);
void readgap(PriceList list);
void gapforrbf_write(PriceList list);
void fortest_write(PriceList list);
double rbf_result(PriceList list);

void TimeStructToHMS(const char* time,int& hour,int& minute,int& second);

 //CTP.cpp
 //void getMarketData();//�������
// void makeTraderDecision();//���׾���
void sqlini();
void sqlend(const char*);
int getnumofmisecond(const int h1,const int m1 ,const int s1);

