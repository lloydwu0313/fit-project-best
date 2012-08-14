//获取行情数据的独立测试
#include ".\ThostTraderApi\ThostFtdcMdApi.h"
#include "MdSpi.h"
#include <iostream>
using namespace std;

// UserApi对象
//CThostFtdcMdApi* pUserApi;

// 配置参数
//char FRONT_ADDR[] = "tcp://asp-sim2-md1.financial-trading-platform.com:26213";		// 前置地址
//TThostFtdcBrokerIDType	BROKER_ID = "2030";				// 经纪公司代码
//TThostFtdcInvestorIDType INVESTOR_ID = "00092";			// 投资者代码
//TThostFtdcPasswordType  PASSWORD = "888888";			// 用户密码
//char *ppInstrumentID[] = {"ag1212","au1212"};			// 行情订阅列表
//int iInstrumentID = 2;									// 行情订阅数量

	
// 请求编号
//int iRequestID = 0; 


//void main(void)
//{
//	// 初始化UserApi
//	pUserApi = CThostFtdcMdApi::CreateFtdcMdApi();			// 创建UserApi
//	CThostFtdcMdSpi* pUserSpi = new CMdSpi();
//	((CMdSpi*)pUserSpi)->set_BROKER_ID(BROKER_ID);
//	((CMdSpi*)pUserSpi)->set_INVESTOR_ID(INVESTOR_ID);
//	((CMdSpi*)pUserSpi)->set_PASSWORD(PASSWORD);
//	((CMdSpi*)pUserSpi)->set_ppInstrumentID(ppInstrumentID,iInstrumentID);
//	pUserApi->RegisterSpi(pUserSpi);						// 注册事件类
//	try{
//		pUserApi->RegisterFront(FRONT_ADDR);					// connect
//		pUserApi->Init();
//		pUserApi->Join();
//		//	pUserApi->Release();
//	}
//	catch(char * msg)
//	{
//		cout<<"error connect！"<<endl;
//	}
//
//}