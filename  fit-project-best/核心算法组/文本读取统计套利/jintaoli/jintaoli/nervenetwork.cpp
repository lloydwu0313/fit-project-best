#include"fit.h"

//取得预测值，由威哥来补充这个函数
 double nervenet(double*g,int n){//n是给的数据个数，就是前几个，g是数据数组
	 srand((unsigned)(time(0)));
	 double tep=((rand()%50)*0.01+1)*getavg(g,n);//得到一个随机的神经网络值，只是我用来预测用的，威哥要重新写
	// cout<<"计算的神经网络预测值为："<<tep<<endl;
	 return tep;
 }