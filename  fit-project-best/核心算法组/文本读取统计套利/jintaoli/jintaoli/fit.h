#include <iostream>
#include <fstream>
#include<string>
#include<ctime>
#include<cstdlib>
#include <math.h>
#define MaxNumberOfRecord 300  //һ���Ի�ȡ�����ļ�¼��Ŀ
using namespace std;

//tool.cpp
//���߰���������ȡ�ļ��ͼ����ֵ��׼��
void read(const char*priceTxt,double *priceArray ,int& numOfRecord);//���ı��ж�ȡ���ݵ������У�priceTxt��Ҫ��ȡ���ļ��Ķ�Ӧ��charֵ��price��Ҫ���������
void getGap( double*priceArrayGap,int& numOfRecordGap);//������۲����У�������۲�����
 double getavg(double *g,int n);//���ֵ��n�Ǹ������ݸ���������ǰ������g����������
 double getstd(double*g ,int n);//���׼�n�Ǹ������ݸ���������ǰ������g����������

//trade.cpp  
 inline void duojiacang(const char*p1,const char* p2,int n);//�൥�����µļӲ֣�����ĳ���ڻ���p��Ʒ��id��n������������
 inline void duopingcang(const char*p1,const char* p2,int n);//�൥�����µļ��֣�����ĳ���ڻ���p��Ʒ��id��n������������
 inline void kongjiacang(const char*p1,const char* p2,int n);//�ռӲ֣�����ĳ���ڻ���p��Ʒ��id��n������������
 inline void kongpingcang(const char*p1,const char* p2,int n); //�յ��������µĲ����µ�ƽ�֣�����ĳ���ڻ���p��Ʒ��id��n������������
 void trade(double*g,int n);//���׾��ߺ�����ͳ�������ĺ���˼��

 //nervenetwork.cpp
 //ȡ��Ԥ��ֵ���������������������
 double nervenet(double*g,int n);//n�Ǹ������ݸ���������ǰ������g����������

 //tradeinterface.cpp  
 //��һ��Ʒ�ֵĲ�������Щ������������ѡ��������
 void jia(const char*p,int n);//����һ��Ʒ��p���ڻ���Լn��
 void jian(const char*p,int n);//����һ��Ʒ��p���ڻ���Լn��
