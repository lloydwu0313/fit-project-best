#include "MdSpi.h"
#include <iostream>
using namespace std;
#include "fit.h"
#pragma warning(disable : 4996)
 

//��fit.h���ù�����ȫ�ֱ�����
 extern  PriceList priceList1;//Ʒ��1�ļ۸�����
   extern  PriceList priceList2;//Ʒ��2�ļ۸�����
   extern  PriceList priceListGap;//�۲�����

 extern int numOfData;//�õ����������ݵ�������Ϊ1��õ��˵�һ����Ϊ2��õ��˵ڶ�����Ϊ3��õ���������Ϊ0 ��û�еõ�

// USER_API����

 extern CThostFtdcMdApi* pUserApi;
// ���ò���
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

// ������
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
	///�û���¼����
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
	cerr << "--->>> �����û���¼����: " << ((iResult == 0) ? "�ɹ�" : "ʧ��") << endl;
}

void CMdSpi::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin,
		CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	cerr << "--->>> " << __FUNCTION__ << endl;
	if (bIsLast && !IsErrorRspInfo(pRspInfo))
	{
		///��ȡ��ǰ������
		cerr << "--->>> ��ȡ��ǰ������ = " << pUserApi->GetTradingDay() << endl;
		// ����������
		SubscribeMarketData();	
	}
}

void CMdSpi::SubscribeMarketData()
{
	int iResult = pUserApi->SubscribeMarketData(ppInstrumentID, iInstrumentID);
	cerr << "--->>> �������鶩������: " << ((iResult == 0) ? "�ɹ�" : "ʧ��") << endl;
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





//���������ÿ�ζ�ȡ�������ݾͻ���õĺ�������������������һЩ�����Ĳ���
void CMdSpi::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData)
{
	//cerr << __FUNCTION__ << endl;
	if(dirty){//�����ʼ��ֵ��û���޸ġ�
	TimeStructToHMS(pDepthMarketData->UpdateTime,beginhour,beginmin,beginsec);
	cout<<"beginhour:"<<beginhour<<"         beginmin: "<<beginmin<<"    beginsec"<<beginsec<<endl;
	dirty=false;
	}
	cout<<"InstrumentID: "<<pDepthMarketData->InstrumentID<<";UpdateTime: "<<pDepthMarketData->UpdateTime<<" ;TradingDay:"<<pDepthMarketData->TradingDay<<";LasPrice: "<<pDepthMarketData->LastPrice<<endl;
	if(strcmp(pin1,pDepthMarketData->InstrumentID)){
		cout<<"��һ���������ݴﵽ��"<<endl;
		//readPrice(pDepthMarketData->LastPrice,priceList1);//������һ��������
		priceList1.add(pDepthMarketData->LastPrice,pDepthMarketData->UpdateTime);
		priceList1.storeToDb(pDepthMarketData->UpdateTime,pDepthMarketData->LastPrice,pDepthMarketData->TradingDay,pDepthMarketData->InstrumentID);
		if(numOfData==2)numOfData=3;
		if(numOfData==0)numOfData=1;
		//if(numOfData==1)firstNumOfData1++;
	}
	else{
		if(strcmp(pin2,pDepthMarketData->InstrumentID)){ 
		//readPrice(pDepthMarketData->LastPrice,priceList2);//�����ڶ���������
		cout<<"�ڶ����������ݴﵽ��"<<endl;
		priceList2.add(pDepthMarketData->LastPrice,pDepthMarketData->UpdateTime);
		priceList2.storeToDb(pDepthMarketData->UpdateTime,pDepthMarketData->LastPrice,pDepthMarketData->TradingDay,pDepthMarketData->InstrumentID);
		if(numOfData==1)numOfData=3;
		if(numOfData==0)numOfData=2;
		//if(numOfData==2)firstNumOfData2++;
	}
		else cout<<"��ȡ��������Ȳ���pin1��Ҳ����pin2"<<"��ʱ��pDepthMarketData->InstrumentIDΪ��"<<pDepthMarketData->InstrumentID<<endl;
		}

	//	cout<<"numOfData==3,���Խ����ˣ����ڼ���۲�"<<endl;
		double m=priceList1.getNowPriceGap();
		double n =priceList2.getNowPriceGap();
        double gap=m-n; 
		priceListGap.add(gap,pDepthMarketData->UpdateTime);//��Ӽ۲�����
		//priceListGap.storeToGapDb(pin1,pin2,gap,pDepthMarketData->UpdateTime,pDepthMarketData->TradingDay,4);
		numOfData=0;
		//firstNumOfData2--;
		//firstNumOfData1--;
		trade(priceListGap,pDepthMarketData->AveragePrice,pDepthMarketData->UpdateTime,gap,pDepthMarketData->TradingDay);//�������׾���
	
}





bool CMdSpi::IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo)
{
	// ���ErrorID != 0, ˵���յ��˴������Ӧ
	bool bResult = ((pRspInfo) && (pRspInfo->ErrorID != 0));
	if (bResult)
		cerr << "--->>> ErrorID=" << pRspInfo->ErrorID << ", ErrorMsg=" << pRspInfo->ErrorMsg << endl;
	return bResult;
}