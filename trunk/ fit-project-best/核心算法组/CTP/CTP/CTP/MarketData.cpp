//��ȡ�������ݵĶ�������
#include ".\ThostTraderApi\ThostFtdcMdApi.h"
#include "MdSpi.h"
#include <iostream>
using namespace std;

// UserApi����
//CThostFtdcMdApi* pUserApi;

// ���ò���
//char FRONT_ADDR[] = "tcp://asp-sim2-md1.financial-trading-platform.com:26213";		// ǰ�õ�ַ
//TThostFtdcBrokerIDType	BROKER_ID = "2030";				// ���͹�˾����
//TThostFtdcInvestorIDType INVESTOR_ID = "00092";			// Ͷ���ߴ���
//TThostFtdcPasswordType  PASSWORD = "888888";			// �û�����
//char *ppInstrumentID[] = {"ag1212","au1212"};			// ���鶩���б�
//int iInstrumentID = 2;									// ���鶩������

	
// ������
//int iRequestID = 0; 


//void main(void)
//{
//	// ��ʼ��UserApi
//	pUserApi = CThostFtdcMdApi::CreateFtdcMdApi();			// ����UserApi
//	CThostFtdcMdSpi* pUserSpi = new CMdSpi();
//	((CMdSpi*)pUserSpi)->set_BROKER_ID(BROKER_ID);
//	((CMdSpi*)pUserSpi)->set_INVESTOR_ID(INVESTOR_ID);
//	((CMdSpi*)pUserSpi)->set_PASSWORD(PASSWORD);
//	((CMdSpi*)pUserSpi)->set_ppInstrumentID(ppInstrumentID,iInstrumentID);
//	pUserApi->RegisterSpi(pUserSpi);						// ע���¼���
//	try{
//		pUserApi->RegisterFront(FRONT_ADDR);					// connect
//		pUserApi->Init();
//		pUserApi->Join();
//		//	pUserApi->Release();
//	}
//	catch(char * msg)
//	{
//		cout<<"error connect��"<<endl;
//	}
//
//}