#include"fit.h"

//ȡ��Ԥ��ֵ���������������������
 double nervenet(double*g,int n){//n�Ǹ������ݸ���������ǰ������g����������
	 srand((unsigned)(time(0)));
	 double tep=((rand()%50)*0.01+1)*getavg(g,n);//�õ�һ�������������ֵ��ֻ��������Ԥ���õģ�����Ҫ����д
	// cout<<"�����������Ԥ��ֵΪ��"<<tep<<endl;
	 return tep;
 }