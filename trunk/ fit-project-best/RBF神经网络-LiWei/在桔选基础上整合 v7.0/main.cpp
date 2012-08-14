// testTraderApi.cpp : �������̨Ӧ�ó������ڵ㡣
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

//����Ϊȫ�ֱ���
   PriceList priceList1;//Ʒ��1�ļ۸�����
   PriceList priceList2;//Ʒ��2�ļ۸�����
   PriceList priceListGap;//�۲�����
   int beginPoint;//��ʼ�����ĵ㣻

   int numOfData;//�õ����������ݵ�������Ϊ1��õ��˵�һ����Ϊ2��õ��˵ڶ�����Ϊ3��õ���������Ϊ0 ��û�еõ�
   CThostFtdcMdApi* pUserApi;
   // UserApi����TraderApi

  CThostFtdcTraderApi* pUserTraderApi; 
	 mysql::MySQL_Driver *driver;  
     Connection *con;  
     Statement *state; 
	 ResultSet *result;

	 double m;//������ļ۲��ֵ��
	double d;//d��������ı�׼�
	int beginhour;//��ʼ��Сʱ
	int beginmin;//��ʼ�ķ�
	int beginsec;//��ʼ����
	bool dirty=true;//����beginhour,beginmin,beginsec�ĳ�ʼ���ġ�
 //����Ϊȫ�ֱ�����

// �������ò���
char FRONT_ADDR[] = "tcp://asp-sim2-md1.financial-trading-platform.com:26213";		// ǰ�õ�ַ
TThostFtdcBrokerIDType	BROKER_ID = "2030";				// ���͹�˾����
TThostFtdcInvestorIDType INVESTOR_ID = "00092";			// Ͷ���ߴ���
TThostFtdcPasswordType  PASSWORD = "888888";			// �û�����

// �������ò���
char  FRONT_ADDR_Trader[] = "tcp://asp-sim2-front1.financial-trading-platform.com:26205";		// ǰ�õ�ַ
TThostFtdcInstrumentIDType INSTRUMENT_ID = pin1;	// ��Լ����
TThostFtdcDirectionType	DIRECTION = THOST_FTDC_D_Sell;	// ��������
TThostFtdcPriceType	LIMIT_PRICE = 5885;				// �۸�
TThostFtdcVolumeType VOLUMETOTALORIGINAL = 1;			//����


char *ppInstrumentID[] = {pin1,pin2};			// ���鶩���б�
int iInstrumentID = 2;									// ���鶩������
// ������
int iRequestID = 0; 
void getdata(){
		pUserApi = CThostFtdcMdApi::CreateFtdcMdApi();			// ����UserApi
	CThostFtdcMdSpi* pUserSpi = new CMdSpi();
	pUserApi->RegisterSpi(pUserSpi);						// ע���¼���
	try{
		pUserApi->RegisterFront(FRONT_ADDR);					// connect
		pUserApi->Init();
		pUserApi->Join();
		//	pUserApi->Release();
	}
	catch(char * msg)
	{
		cout<<"error connect��"<<msg<<endl;
	}
}
//�����������������ָ��ĺ���������instr�Ǻ�Լ����direΪTHOST_FTDC_D_Buy��Ϊ��ΪTHOST_FTDC_D_SellΪ����pricΪ�����ļ۸����������ڼ۸��Ծ������֣�tranumΪ���ױ�����
 void makeTraderDecision(TThostFtdcInstrumentIDType instr,TThostFtdcDirectionType direct,TThostFtdcPriceType pric,TThostFtdcVolumeType tranum)
{
	strcpy(INSTRUMENT_ID ,instr);	// ��Լ����
	DIRECTION = direct;	// ��������
	LIMIT_PRICE = pric;				// �۸�
	VOLUMETOTALORIGINAL = tranum;			//����
	pUserTraderApi->Init();
	pUserTraderApi->Join();
	//	pUserApi->Release();
}
 void makeTraderDecisionIni(){     //�����׾��ߵĳ�ʼ������
	 // ��ʼ��UserApi
	pUserTraderApi = CThostFtdcTraderApi::CreateFtdcTraderApi();		// ����UserApi
	CTraderSpi* pUserSpi = new CTraderSpi();
	pUserTraderApi->RegisterSpi((CThostFtdcTraderSpi*)pUserSpi);		// ע���¼���
	pUserTraderApi->SubscribePublicTopic(TERT_RESTART);					// ע�ṫ����
	pUserTraderApi->SubscribePrivateTopic(TERT_RESTART);				// ע��˽����
	pUserTraderApi->RegisterFront(FRONT_ADDR_Trader);							// connect
 }
 // ÿ��Ҫ���׾͵����������������instr�Ǻ�Լ����direΪTHOST_FTDC_D_Buy��Ϊ��ΪTHOST_FTDC_D_SellΪ����pricΪ�����ļ۸����������ڼ۸��Ծ������֣�tranumΪ���ױ�����
 void maketotaltrade(TThostFtdcInstrumentIDType instr,TThostFtdcDirectionType direct,TThostFtdcPriceType pric,TThostFtdcVolumeType tranum){
	strcpy(INSTRUMENT_ID ,instr);	// ��Լ����
	DIRECTION = direct;	// ��������
	LIMIT_PRICE = pric;				// �۸�
	VOLUMETOTALORIGINAL = tranum;			//����
	 pUserTraderApi = CThostFtdcTraderApi::CreateFtdcTraderApi();			// ����UserApi
	CTraderSpi* pUserSpi = new CTraderSpi();
	pUserTraderApi->RegisterSpi((CThostFtdcTraderSpi*)pUserSpi);			// ע���¼���
	pUserTraderApi->SubscribePublicTopic(TERT_RESTART);					// ע�ṫ����
	pUserTraderApi->SubscribePrivateTopic(TERT_RESTART);					// ע��˽����
	pUserTraderApi->RegisterFront(FRONT_ADDR_Trader);							// connect
	pUserTraderApi->Init();
	pUserTraderApi->Join();
 }
	//	pUserApi->Release();
void main(void)
{
	// ��ʼ������  
	sqlini();//��ʼ���������ݿ�


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
	//priceListGap.readmdfromdb();//�����ݿ��ж�ȡ��������һ��Сʱ��ƽ��ֵ�ͱ�׼��
	// ��ʼ��UserApi
	 
	getdata();
//	makeTraderDecisionIni();
	const char* nowDate=pUserApi->GetTradingDay();
	sqlend(nowDate);
	*/
}

