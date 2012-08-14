#include "MdSpi.h"
#include <iostream>
using namespace std;
#include "fit.h"
#pragma warning(disable : 4996)
 

//从fit.h引用过来的全局变量：
 extern  PriceList priceList1;//品种1的价格数组
   extern  PriceList priceList2;//品种2的价格数组
   extern  PriceList priceListGap;//价差数组

 extern int numOfData;//得到的行情数据的数量，为1则得到了第一个，为2则得到了第二个，为3则得到了两个，为0 则都没有得到

// USER_API参数

 extern CThostFtdcMdApi* pUserApi;
// 配置参数
extern char FRONT_ADDR[];		
extern TThostFtdcBrokerIDType	BROKER_ID;
extern TThostFtdcInvestorIDType INVESTOR_ID;
extern TThostFtdcPasswordType	PASSWORD;
extern char* ppInstrumentID[];	
extern int iInstrumentID;
extern int  beginhour;
extern int  beginmin;
extern int beginsec;
extern bool dirty;

// 请求编号
extern int iRequestID;

void CMdSpi::OnRspError(CThostFtdcRspInfoField *pRspInfo,
		int nRequestID, bool bIsLast)
{
	cerr << "--->>> "<< __FUNCTION__ << endl;
	IsErrorRspInfo(pRspInfo);
}

void CMdSpi::OnFrontDisconnected(int nReason)
{
	cerr << "--->>> " << __FUNCTION__ << endl;
	cerr << "--->>> Reason = " << nReason << endl;
}
		
void CMdSpi::OnHeartBeatWarning(int nTimeLapse)
{
	cerr << "--->>> " << __FUNCTION__ << endl;
	cerr << "--->>> nTimerLapse = " << nTimeLapse << endl;
}

void CMdSpi::OnFrontConnected()
{
	cerr << "--->>> " << __FUNCTION__ << endl;
	///用户登录请求
	ReqUserLogin();
}

void CMdSpi::ReqUserLogin()
{
	CThostFtdcReqUserLoginField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, BROKER_ID);
	strcpy(req.UserID, INVESTOR_ID);
	strcpy(req.Password, PASSWORD);
	int iResult = pUserApi->ReqUserLogin(&req, ++iRequestID);
	cerr << "--->>> 发送用户登录请求: " << ((iResult == 0) ? "成功" : "失败") << endl;
}

void CMdSpi::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin,
		CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	cerr << "--->>> " << __FUNCTION__ << endl;
	if (bIsLast && !IsErrorRspInfo(pRspInfo))
	{
		///获取当前交易日
		cerr << "--->>> 获取当前交易日 = " << pUserApi->GetTradingDay() << endl;
		// 请求订阅行情
		SubscribeMarketData();	
	}
}

void CMdSpi::SubscribeMarketData()
{
	int iResult = pUserApi->SubscribeMarketData(ppInstrumentID, iInstrumentID);
	cerr << "--->>> 发送行情订阅请求: " << ((iResult == 0) ? "成功" : "失败") << endl;
}

void CMdSpi::OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	//cerr << __FUNCTION__ << endl;
	cout<<"Test: OnRspSubMarketData"<<endl;
}

void CMdSpi::OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	//cerr << __FUNCTION__ << endl;
	cout<<"Test: OnRspUnSubMarketData"<<endl;
}





//这个函数是每次读取行情数据就会调用的函数，可以在里面多加入一些自主的操作
void CMdSpi::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData)
{
	//cerr << __FUNCTION__ << endl;
	if(dirty){//如果初始的值还没有修改。
	TimeStructToHMS(pDepthMarketData->UpdateTime,beginhour,beginmin,beginsec);
	cout<<"beginhour:"<<beginhour<<"         beginmin: "<<beginmin<<"    beginsec"<<beginsec<<endl;
	dirty=false;
	}
	cout<<"InstrumentID: "<<pDepthMarketData->InstrumentID<<";UpdateTime: "<<pDepthMarketData->UpdateTime<<" ;TradingDay:"<<pDepthMarketData->TradingDay<<";LasPrice: "<<pDepthMarketData->LastPrice<<endl;
	if(strcmp(pin1,pDepthMarketData->InstrumentID)){
		cout<<"第一个行情数据达到！"<<endl;
		//readPrice(pDepthMarketData->LastPrice,priceList1);//读到第一个数组中
		priceList1.add(pDepthMarketData->LastPrice,pDepthMarketData->UpdateTime);
		priceList1.storeToDb(pDepthMarketData->UpdateTime,pDepthMarketData->LastPrice,pDepthMarketData->TradingDay,pDepthMarketData->InstrumentID);
		if(numOfData==2)numOfData=3;
		if(numOfData==0)numOfData=1;
		//if(numOfData==1)firstNumOfData1++;
	}
	else{
		if(strcmp(pin2,pDepthMarketData->InstrumentID)){ 
		//readPrice(pDepthMarketData->LastPrice,priceList2);//读到第二个数组中
		cout<<"第二个行情数据达到！"<<endl;
		priceList2.add(pDepthMarketData->LastPrice,pDepthMarketData->UpdateTime);
		priceList2.storeToDb(pDepthMarketData->UpdateTime,pDepthMarketData->LastPrice,pDepthMarketData->TradingDay,pDepthMarketData->InstrumentID);
		if(numOfData==1)numOfData=3;
		if(numOfData==0)numOfData=2;
		//if(numOfData==2)firstNumOfData2++;
	}
		else cout<<"获取行情出错！既不是pin1，也不是pin2"<<"此时的pDepthMarketData->InstrumentID为："<<pDepthMarketData->InstrumentID<<endl;
		}

	//	cout<<"numOfData==3,可以交易了，现在计算价差"<<endl;
		double m=priceList1.getNowPriceGap();
		double n =priceList2.getNowPriceGap();
        double gap=m-n; 
		priceListGap.add(gap,pDepthMarketData->UpdateTime);//添加价差序列
		//priceListGap.storeToGapDb(pin1,pin2,gap,pDepthMarketData->UpdateTime,pDepthMarketData->TradingDay,4);
		numOfData=0;
		//firstNumOfData2--;
		//firstNumOfData1--;
		trade(priceListGap,pDepthMarketData->AveragePrice,pDepthMarketData->UpdateTime,gap,pDepthMarketData->TradingDay);//做出交易决策
	
}





bool CMdSpi::IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo)
{
	// 如果ErrorID != 0, 说明收到了错误的响应
	bool bResult = ((pRspInfo) && (pRspInfo->ErrorID != 0));
	if (bResult)
		cerr << "--->>> ErrorID=" << pRspInfo->ErrorID << ", ErrorMsg=" << pRspInfo->ErrorMsg << endl;
	return bResult;
}