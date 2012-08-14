#include "MdSpi.h"
#include <iostream>
using namespace std;

#pragma warning(disable : 4996)

// USER_API参数
extern CThostFtdcMdApi* pUserApi;

// 配置参数
//extern char FRONT_ADDR[];		
//extern TThostFtdcBrokerIDType	BROKER_ID;
//extern TThostFtdcInvestorIDType INVESTOR_ID;
//extern TThostFtdcPasswordType	PASSWORD;
//extern char* ppInstrumentID[];	
//extern int iInstrumentID;

// 请求编号
extern int iRequestID;

CMdSpi::CMdSpi()
{
	this->ppInstrumentID = new char*[0];
}

int CMdSpi::～CMdSpi()
{
	delete [](this->ppInstrumentID);
	return 0;
}

void CMdSpi::set_BROKER_ID(TThostFtdcBrokerIDType bid)
{
	int i = 0;
	while(bid[i] != '\0')
	{
		this->BROKER_ID[i] = bid[i];
		i++;
	}
}

void CMdSpi::set_INVESTOR_ID(TThostFtdcInvestorIDType iid)
{
	int i = 0;
	while(iid[i] != '\0')
	{
		this->INVESTOR_ID[i] = iid[i];
		i++;
	}
}


void CMdSpi::set_PASSWORD(TThostFtdcPasswordType psw)
{
	int i = 0;
	while(psw[i] != '\0')
	{
		this->PASSWORD[i] = psw[i];
		i++;
	}
}

void CMdSpi::set_ppInstrumentID(char **ppiid, int nc)
{
	delete [](this->ppInstrumentID);
	this->ppInstrumentID = new char*[nc];
	for(int i = 0; i < nc; i ++)
	{
		this->ppInstrumentID[i] = ppiid[i];
	}
	this->iInstrumentID = nc;
}


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
	strcpy(req.BrokerID, this->BROKER_ID);
	strcpy(req.UserID, this->INVESTOR_ID);
	strcpy(req.Password, this->PASSWORD);
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
	int iResult = pUserApi->SubscribeMarketData(this->ppInstrumentID, this->iInstrumentID);
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

void CMdSpi::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData)
{
	//cerr << __FUNCTION__ << endl;
	cout<<"InstrumentID: "<<pDepthMarketData->InstrumentID<<";UpdateTime: "<<pDepthMarketData->UpdateTime<<";LasPrice: "<<pDepthMarketData->LastPrice<<endl;
}

bool CMdSpi::IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo)
{
	// 如果ErrorID != 0, 说明收到了错误的响应
	bool bResult = ((pRspInfo) && (pRspInfo->ErrorID != 0));
	if (bResult)
		cerr << "--->>> ErrorID=" << pRspInfo->ErrorID << ", ErrorMsg=" << pRspInfo->ErrorMsg << endl;
	return bResult;
}