// CTP.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include ".\ThostTraderApi\ThostFtdcMdApi.h"
#include ".\ThostTraderApi\ThostFtdcTraderApi.h"
#include "MdSpi.h"
#include "TraderSpi.h"
#include <iostream>
using namespace std;

// UserApi����TraderApi
CThostFtdcTraderApi* pUserTraderApi;

//UserApi����MdApi
CThostFtdcMdApi* pUserApi;

// �������ò���
TThostFtdcBrokerIDType	BROKER_ID = "2030";				// ���͹�˾����
TThostFtdcInvestorIDType INVESTOR_ID = "0000000624";	// Ͷ���ߴ���
TThostFtdcPasswordType  PASSWORD = "asdfgh";			// �û�����

// �������ò���
char  FRONT_ADDR_Trader[] = "tcp://asp-sim2-front1.financial-trading-platform.com:26205";		// ǰ�õ�ַ
TThostFtdcInstrumentIDType INSTRUMENT_ID = "ag1212";	// ��Լ����
TThostFtdcDirectionType	DIRECTION = THOST_FTDC_D_Sell;	// ��������
TThostFtdcPriceType	LIMIT_PRICE = 5885;				// �۸�
TThostFtdcVolumeType VOLUMETOTALORIGINAL = 1;			//����

//�������ò���
char FRONT_ADDR[] = "tcp://asp-sim2-md1.financial-trading-platform.com:26213";		// ǰ�õ�ַ
char *ppInstrumentID[] = {"ag1212","au1212"};	
int iInstrumentID = 2;
//TThostFtdcInvestorIDType MD_INVESTOR_ID = "00092";			// Ͷ���ߴ���/"00069"
//TThostFtdcPasswordType  MD_PASSWORD = "888888";			// �û�����

// ������
int iRequestID = 0;

//��ȡ��������
void getMarketData()
{
	// ��ʼ��UserApi
	pUserApi = CThostFtdcMdApi::CreateFtdcMdApi();			// ����UserApi
	CThostFtdcMdSpi* pUserSpi = new CMdSpi();
	((CMdSpi*)pUserSpi)->set_BROKER_ID(BROKER_ID);
	((CMdSpi*)pUserSpi)->set_INVESTOR_ID(INVESTOR_ID);
	((CMdSpi*)pUserSpi)->set_PASSWORD(PASSWORD);
	((CMdSpi*)pUserSpi)->set_ppInstrumentID(ppInstrumentID,iInstrumentID);
	pUserApi->RegisterSpi(pUserSpi);						// ע���¼���
	try{
		pUserApi->RegisterFront(FRONT_ADDR);					// connect
		pUserApi->Init();
		pUserApi->Join();
		//	pUserApi->Release();
	}
	catch(char * msg)
	{
		cout<<"error connect��"<<endl;
	}

}

//���н��׾���
void makeTraderDecision()
{
	// ��ʼ��UserApi
	pUserTraderApi = CThostFtdcTraderApi::CreateFtdcTraderApi();			// ����UserApi
	CTraderSpi* pUserSpi = new CTraderSpi();
	pUserTraderApi->RegisterSpi((CThostFtdcTraderSpi*)pUserSpi);			// ע���¼���
	pUserTraderApi->SubscribePublicTopic(TERT_RESTART);					// ע�ṫ����
	pUserTraderApi->SubscribePrivateTopic(TERT_RESTART);					// ע��˽����
	pUserTraderApi->RegisterFront(FRONT_ADDR_Trader);							// connect
	pUserTraderApi->Init();
	pUserTraderApi->Join();
	//	pUserApi->Release();
}

void main(void)
{
	//  ����
	getMarketData();
	//makeTraderDecision();
}

