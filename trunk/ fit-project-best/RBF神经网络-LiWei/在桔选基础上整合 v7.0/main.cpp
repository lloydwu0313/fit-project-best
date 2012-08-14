// testTraderApi.cpp : 定义控制台应用程序的入口点。
//

#include <iostream>
#include"fit.h"
#include <mysql_connection.h>  
#include <mysql_driver.h>  
#include "stdafx.h"
#include ".\ThostTraderApi\ThostFtdcMdApi.h"
#include ".\ThostTraderApi\ThostFtdcTraderApi.h"
#include "MdSpi.h"
#include "TraderSpi.h"

#include <time.h>

using namespace sql;  
using namespace std;

//以下为全局变量
   PriceList priceList1;//品种1的价格数组
   PriceList priceList2;//品种2的价格数组
   PriceList priceListGap;//价差数组
   int beginPoint;//开始套利的点；

   int numOfData;//得到的行情数据的数量，为1则得到了第一个，为2则得到了第二个，为3则得到了两个，为0 则都没有得到
   CThostFtdcMdApi* pUserApi;
   // UserApi对象：TraderApi

  CThostFtdcTraderApi* pUserTraderApi; 
	 mysql::MySQL_Driver *driver;  
     Connection *con;  
     Statement *state; 
	 ResultSet *result;

	 double m;//算出来的价差均值；
	double d;//d是算出来的标准差；
	int beginhour;//开始的小时
	int beginmin;//开始的分
	int beginsec;//开始的秒
	bool dirty=true;//控制beginhour,beginmin,beginsec的初始化的。
 //以上为全局变量。

// 行情配置参数
char FRONT_ADDR[] = "tcp://asp-sim2-md1.financial-trading-platform.com:26213";		// 前置地址
TThostFtdcBrokerIDType	BROKER_ID = "2030";				// 经纪公司代码
TThostFtdcInvestorIDType INVESTOR_ID = "00092";			// 投资者代码
TThostFtdcPasswordType  PASSWORD = "888888";			// 用户密码

// 交易配置参数
char  FRONT_ADDR_Trader[] = "tcp://asp-sim2-front1.financial-trading-platform.com:26205";		// 前置地址
TThostFtdcInstrumentIDType INSTRUMENT_ID = pin1;	// 合约代码
TThostFtdcDirectionType	DIRECTION = THOST_FTDC_D_Sell;	// 买卖方向
TThostFtdcPriceType	LIMIT_PRICE = 5885;				// 价格
TThostFtdcVolumeType VOLUMETOTALORIGINAL = 1;			//数量


char *ppInstrumentID[] = {pin1,pin2};			// 行情订阅列表
int iInstrumentID = 2;									// 行情订阅数量
// 请求编号
int iRequestID = 0; 
void getdata(){
		pUserApi = CThostFtdcMdApi::CreateFtdcMdApi();			// 创建UserApi
	CThostFtdcMdSpi* pUserSpi = new CMdSpi();
	pUserApi->RegisterSpi(pUserSpi);						// 注册事件类
	try{
		pUserApi->RegisterFront(FRONT_ADDR);					// connect
		pUserApi->Init();
		pUserApi->Join();
		//	pUserApi->Release();
	}
	catch(char * msg)
	{
		cout<<"error connect！"<<msg<<endl;
	}
}
//这个是用来发出交易指令的函数，其中instr是合约名，dire为THOST_FTDC_D_Buy即为买，为THOST_FTDC_D_Sell为卖，pric为买卖的价格，我们以现在价格，以尽快脱手，tranum为交易笔数。
 void makeTraderDecision(TThostFtdcInstrumentIDType instr,TThostFtdcDirectionType direct,TThostFtdcPriceType pric,TThostFtdcVolumeType tranum)
{
	strcpy(INSTRUMENT_ID ,instr);	// 合约代码
	DIRECTION = direct;	// 买卖方向
	LIMIT_PRICE = pric;				// 价格
	VOLUMETOTALORIGINAL = tranum;			//数量
	pUserTraderApi->Init();
	pUserTraderApi->Join();
	//	pUserApi->Release();
}
 void makeTraderDecisionIni(){     //做交易决策的初始化操作
	 // 初始化UserApi
	pUserTraderApi = CThostFtdcTraderApi::CreateFtdcTraderApi();		// 创建UserApi
	CTraderSpi* pUserSpi = new CTraderSpi();
	pUserTraderApi->RegisterSpi((CThostFtdcTraderSpi*)pUserSpi);		// 注册事件类
	pUserTraderApi->SubscribePublicTopic(TERT_RESTART);					// 注册公有流
	pUserTraderApi->SubscribePrivateTopic(TERT_RESTART);				// 注册私有流
	pUserTraderApi->RegisterFront(FRONT_ADDR_Trader);							// connect
 }
 // 每当要交易就调用这个函数，其中instr是合约名，dire为THOST_FTDC_D_Buy即为买，为THOST_FTDC_D_Sell为卖，pric为买卖的价格，我们以现在价格，以尽快脱手，tranum为交易笔数。
 void maketotaltrade(TThostFtdcInstrumentIDType instr,TThostFtdcDirectionType direct,TThostFtdcPriceType pric,TThostFtdcVolumeType tranum){
	strcpy(INSTRUMENT_ID ,instr);	// 合约代码
	DIRECTION = direct;	// 买卖方向
	LIMIT_PRICE = pric;				// 价格
	VOLUMETOTALORIGINAL = tranum;			//数量
	 pUserTraderApi = CThostFtdcTraderApi::CreateFtdcTraderApi();			// 创建UserApi
	CTraderSpi* pUserSpi = new CTraderSpi();
	pUserTraderApi->RegisterSpi((CThostFtdcTraderSpi*)pUserSpi);			// 注册事件类
	pUserTraderApi->SubscribePublicTopic(TERT_RESTART);					// 注册公有流
	pUserTraderApi->SubscribePrivateTopic(TERT_RESTART);					// 注册私有流
	pUserTraderApi->RegisterFront(FRONT_ADDR_Trader);							// connect
	pUserTraderApi->Init();
	pUserTraderApi->Join();
 }
	//	pUserApi->Release();
void main(void)
{
	// 初始化驱动  
	sqlini();//初始化连接数据库


	clock_t t_start = clock();
	PriceList gap_list;
	gap_list.getDataFromDb_all();
	/*
	for(int i = 0;i < MaxNumberOfRecord;i++)
	{
		cout<<priceListGap.priceArray[i]<<" ";
	}*/
	cout<<"Start!!"<<endl;
	gapforrbf_write(priceListGap);
	fortest_write(priceListGap);

	clock_t t_end = clock();

	for(int i = 0;i < 200;i++)
	{
		cout<<priceListGap.priceArray[i]<<" ";
	
	}

	cout<<nervenet(priceListGap,20,3)<<endl;
	clock_t t_end2 = clock();
	cout<<"End!!"<<"Time : "<<(t_end2 - t_start)<<" "<<(t_end - t_start)<<endl;
	system("pause");


	/*
	//priceListGap.readmdfromdb();//从数据库中读取昨天的最后一个小时的平均值和标准差
	// 初始化UserApi
	 
	getdata();
//	makeTraderDecisionIni();
	const char* nowDate=pUserApi->GetTradingDay();
	sqlend(nowDate);
	*/
}

