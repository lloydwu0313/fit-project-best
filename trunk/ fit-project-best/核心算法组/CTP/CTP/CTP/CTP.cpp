// CTP.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include ".\ThostTraderApi\ThostFtdcMdApi.h"
#include ".\ThostTraderApi\ThostFtdcTraderApi.h"
#include "MdSpi.h"
#include "TraderSpi.h"
#include <iostream>
using namespace std;

// UserApi对象：TraderApi
CThostFtdcTraderApi* pUserTraderApi;

//UserApi对象：MdApi
CThostFtdcMdApi* pUserApi;

// 公共配置参数
TThostFtdcBrokerIDType	BROKER_ID = "2030";				// 经纪公司代码
TThostFtdcInvestorIDType INVESTOR_ID = "0000000624";	// 投资者代码
TThostFtdcPasswordType  PASSWORD = "asdfgh";			// 用户密码

// 交易配置参数
char  FRONT_ADDR_Trader[] = "tcp://asp-sim2-front1.financial-trading-platform.com:26205";		// 前置地址
TThostFtdcInstrumentIDType INSTRUMENT_ID = "ag1212";	// 合约代码
TThostFtdcDirectionType	DIRECTION = THOST_FTDC_D_Sell;	// 买卖方向
TThostFtdcPriceType	LIMIT_PRICE = 5885;				// 价格
TThostFtdcVolumeType VOLUMETOTALORIGINAL = 1;			//数量

//行情配置参数
char FRONT_ADDR[] = "tcp://asp-sim2-md1.financial-trading-platform.com:26213";		// 前置地址
char *ppInstrumentID[] = {"ag1212","au1212"};	
int iInstrumentID = 2;
//TThostFtdcInvestorIDType MD_INVESTOR_ID = "00092";			// 投资者代码/"00069"
//TThostFtdcPasswordType  MD_PASSWORD = "888888";			// 用户密码

// 请求编号
int iRequestID = 0;

//获取行情数据
void getMarketData()
{
	// 初始化UserApi
	pUserApi = CThostFtdcMdApi::CreateFtdcMdApi();			// 创建UserApi
	CThostFtdcMdSpi* pUserSpi = new CMdSpi();
	((CMdSpi*)pUserSpi)->set_BROKER_ID(BROKER_ID);
	((CMdSpi*)pUserSpi)->set_INVESTOR_ID(INVESTOR_ID);
	((CMdSpi*)pUserSpi)->set_PASSWORD(PASSWORD);
	((CMdSpi*)pUserSpi)->set_ppInstrumentID(ppInstrumentID,iInstrumentID);
	pUserApi->RegisterSpi(pUserSpi);						// 注册事件类
	try{
		pUserApi->RegisterFront(FRONT_ADDR);					// connect
		pUserApi->Init();
		pUserApi->Join();
		//	pUserApi->Release();
	}
	catch(char * msg)
	{
		cout<<"error connect！"<<endl;
	}

}

//进行交易决策
void makeTraderDecision()
{
	// 初始化UserApi
	pUserTraderApi = CThostFtdcTraderApi::CreateFtdcTraderApi();			// 创建UserApi
	CTraderSpi* pUserSpi = new CTraderSpi();
	pUserTraderApi->RegisterSpi((CThostFtdcTraderSpi*)pUserSpi);			// 注册事件类
	pUserTraderApi->SubscribePublicTopic(TERT_RESTART);					// 注册公有流
	pUserTraderApi->SubscribePrivateTopic(TERT_RESTART);					// 注册私有流
	pUserTraderApi->RegisterFront(FRONT_ADDR_Trader);							// connect
	pUserTraderApi->Init();
	pUserTraderApi->Join();
	//	pUserApi->Release();
}

void main(void)
{
	//  测试
	getMarketData();
	//makeTraderDecision();
}

