#include"fit.h"

//多单策略下的加仓，买入某种期货，p是品种id，n是买卖的手数
 inline void duojiacang(const char*p1,const char* p2,int n){
	  cout<<"多单下建仓  "<<n<<"  笔"<<endl;
	  jia(p1,n);
		jian(p2,n);
 }
//多单策略下的减仓，卖出某种期货，p是品种id，n是买卖的手数
 inline void duopingcang(const char*p1,const char* p2,int n){
	  cout<<"空单下平仓  "<<n<<"  笔"<<endl;
	  jia(p1,n);//把之前买入的全部卖出
	   jian(p2,n);//把之前卖出的全部买入
 }
 //空单策略下的加仓，买入某种期货，p是品种id，n是买卖的手数
 inline void kongjiacang(const char*p1,const char* p2,int n){
	  cout<<"空单下建仓  "<<n<<"  笔"<<endl;
	  jian(p1,n);//把之前买入的全部卖出
	   jia(p2,n);//把之前卖出的全部买入
 }

 //空单策略下的平仓，买入某种期货，p是品种id，n是买卖的手数
 inline void kongpingcang(const char*p1,const char* p2,int n){
	 cout<<"空单下平仓  "<<n<<"  笔"<<endl;
	 jia(p1,n);//把之前买入的全部卖出
	 jian(p2,n);//把之前卖出的全部买入
	}

 //交易决策函数，统计套利的核心思想
void trade(double*g,int numOfRecord){
	//n是获得的数据的组数，g是价差数组
	double totalEarn=0;//赚到的钱
	double singleEarn=0;//一次套利赚到的钱
	int beginPoint ;//开始赚钱的点，也就是进入做多或者做空的点
	const char *p1="au1212";//贵金属品种1
	const char *p2="ag1212";//贵金属品种2
	int num=0;//品种1和2的买入或者卖出的笔数
	unsigned int state=0;//状态，为1表示正在做多交易，为2表示正在做空交易，为0表示目前没有套利机会，手中也没有持有期货合约
    int t=20;//表示时间的数组中的下标。g[0]表示的是当前的价差

	double m;//算出来的价差均值；
	double d;//d是算出来的标准差；
	double k1=3.0;//m+k1*d是不考虑预测值时候的空单界限3<k1<=6
	double k2=2.5;//m+k2*d是考了预测值时候的空单界限2<k2<=3
	double k3=1;//m+k3*d是空单平仓线0<k3<=2
	double k4=1;//m-k4*d是多单平仓线0<k4<=2
	double k5=2.5;//m-k5*d是考虑预测值的多单界限2<k5<=3
	double k6=3.0;//m-k6*d是不考虑预测值的多单界限3<k6<=6
	double fm=1;//价差预测值，由神经网络给出

	for(;t<numOfRecord;t++){   //把所有数据都轮试一遍,从20开始是因为钱20个数据要用来求均值和标准差，不能用来做交易决策
		cout<<"现在是第"<<t<<"个数据，此时价差为："<<g[t]<<endl;
		//计算出均值和标准差
		m=getavg(g,t);//取得均值
		cout<<"当t="<<t<<"时均值为："<<m<<endl;
	   d=getstd(g,t);//取得标准差
	   cout<<"当t="<<t<<"时标准差为："<<d<<endl;
	   fm=nervenet(g,t);//神经网络取得预测值
	   cout<<"当t="<<t<<"时神经网络预测值为："<<fm<<endl;
	   cout<<"经过switch之前此时的state为："<<state<<endl;
	switch (state){
	case 0:{
		num=0;//state=0 的时候一定都是没有做交易
	//以下是空单策略
	if(g[t]>m+k1*d)
	{
		kongjiacang(p1,p2,1);
		num++;
		state=2;
		beginPoint=t;//开始套利了，这是进入套利 的开始点
			cout<<"开始做空套做多利了，这是进入套利 的开始点"<<endl;
	}
		if(g[t]<m-k6*d){
		duojiacang(p1,p2,1);
		num++;
		state=1;
		beginPoint=t;//开始套利了，这是进入套利 的开始点
		cout<<"开始做多套利了，这是进入套利 的开始点"<<endl;
	}//	如果gt<=m-k6σ，不考虑预测值直接建仓（一手）

		break;
    }
       cout<<"经过switch之后此时的state为："<<state<<endl;
	case 2:{
		//	如果gt>=m+k1σ，不考虑预测值直接建仓（一手）,进入空单交易状态
	                     //	如果m+k2σ<=gt<m+k1σ，且fmt+1>=m+k1σ，此时价差偏离正常值，但预测值显示价差进一步扩大的可能性比较大，此时不进行任何操作 
	if(m+k2*d<g[t]&&g[t]<m+k1*d){
		kongjiacang(p1,p2,1);
		num++;
	}//	如果m+ k2σ<=gt<m+k1σ，且m+k3σ<=fmt+1<m+k1σ，此时价差偏离正常值，但预测值显示价差未出现明显的回归趋势和进一步扩大的可能，此时建仓（一手）
	if(m+k2*d<=g[t]&&g[t]<m+k1*d&&fm<m+k3*d){
		kongjiacang(p1,p2,2);
		num+=2;
	}//如果m+ k2σ<=gt<m+k1σ，且fmt+1<m+k3σ，此时价差偏离正常值，但预测值显示价差出现回归的可能性比较大，此时可以大胆地建仓（两手）
	if(g[t]<=m+k3*d){
		kongpingcang(p1,p2,num);
		for(int k=beginPoint;k<t;k++)singleEarn+=(g[k]-g[t]);
		cout<<"此次套利是从第"<<beginPoint<<"到第"<<t<<"，共赚钱："<<singleEarn<<endl;
		totalEarn+=singleEarn;
		singleEarn=0;//要清零，这个变量还要下次利用呢
		num=0;
	state=0;
	}//	如果gt+1<= m+k3σ，则平仓
	break;
  }

	case 1:{

	//以下的多单策略

                         	//if(m-k6*d<g[0]&&g[0]<m-k5*d&&fm<m-k6*d)如果m-k6σ<gt<=m-k5σ，且fmt+1<=m-k6σ，此时价差偏离正常值，但预测值显示价差进一步缩小的可能性比较大，此时不进行任何操作 
	if(m-k6*d<g[t]&&g[t]<m-k5*d&&m-k6*d<fm&&fm<=m-k4*d){
		duojiacang(p1,p2,1);
		num++;
	}//	如果m-k6σ<gt<=m-k5σ，且m-k6σ<fmt+1<=m-k4σ，此时价差偏离正常值，但预测值显示价差未出现明显的回归趋势和进一步缩小的可能，此时建仓（一手）
	if(m-k6*d<g[t]&&g[t]<=m-k5*d&&fm>m-k4*d){
		duojiacang(p1,p2,2);
		num+=2;
	}//	如果m-k6σ<gt<=m-k5σ，且fmt+1>m-k4σ，此时价差偏离正常值，但预测值显示价差出现回归的可能性比较大，此时可以大胆地建仓（两手）
	if(g[t]>=m-k4*d){
		duopingcang(p1,p2,num); 
			for(int k=beginPoint;k<t;k++)singleEarn+=(g[t]-g[k]);
		cout<<"此次套利是从第"<<beginPoint<<"到第"<<t<<"，共赚钱："<<singleEarn<<endl;
		totalEarn+=singleEarn;
		singleEarn=0;//要清零，这个变量还要下次利用呢
		num=0;
		state=0;
	}//如果gt+1>= m-k4σ，则平仓
	break;
     }//case 1结束
	}//case 语句结束
}//for循环结束
	cout<<"赚到的钱为:  "<<totalEarn<<endl;
}//trade函数结束