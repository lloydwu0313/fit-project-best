#include"fit.h"
extern double m;//算出来的价差均值；
extern 	double d;//d是算出来的标准差；
	extern    PriceList priceList1;//品种1的价格数组
  extern  PriceList priceList2;//品种2的价格数组
 extern   PriceList priceListGap;//价差数组
//以下是交易参数的外部声明
 extern TThostFtdcInstrumentIDType INSTRUMENT_ID;	// 合约代码
 extern TThostFtdcDirectionType	DIRECTION ;	// 买卖方向
 extern TThostFtdcPriceType	LIMIT_PRICE ;				// 价格
 extern TThostFtdcVolumeType VOLUMETOTALORIGINAL;			//数量

//多单策略下的加仓，买入某种期货，p是品种id，n是买卖的手数
  void duojiacang( char* p1, char* p2,double price1,double price2,int n){
	  cout<<"多单下建仓  "<<n<<"  笔"<<endl;
	maketotaltrade(p1,THOST_FTDC_D_Buy,price1,n);
	maketotaltrade(p2,THOST_FTDC_D_Sell,price2,n);
 }
//多单策略下的减仓，卖出某种期货，p是品种id，n是买卖的手数
  void duopingcang( char* p1, char* p2,double price1,double price2,int n){
	  cout<<"空单下平仓  "<<n<<"  笔"<<endl;
	maketotaltrade(p1,THOST_FTDC_D_Sell,price1,n);
	maketotaltrade(p2,THOST_FTDC_D_Buy,price2,n);
 }
 //空单策略下的加仓，买入某种期货，p是品种id，n是买卖的手数
  void kongjiacang( char* p1, char* p2,double price1,double price2,int n){
	  cout<<"空单下建仓  "<<n<<"  笔"<<endl;
	maketotaltrade(p1,THOST_FTDC_D_Sell,price1,n);
	maketotaltrade(p2,THOST_FTDC_D_Buy,price2,n);
 }
 //空单策略下的平仓，买入某种期货，p是品种id，n是买卖的手数
  void kongpingcang( char* p1, char* p2,double price1,double price2,int n){
	 cout<<"空单下平仓  "<<n<<"  笔"<<endl;
	 //instr是合约名，dire为THOST_FTDC_D_Buy即为买，为THOST_FTDC_D_Sell为卖，pric为买卖的价格，我们以现在价格，以尽快脱手，tranum为交易笔数。
	maketotaltrade(p1,THOST_FTDC_D_Buy,price1,n);
	maketotaltrade(p2,THOST_FTDC_D_Sell,price2,n);
	}

  /*
double nervenet(PriceList list,int n,double m){//n是给的数据个数，就是前几个，g是数据数组,m是此刻的均值
	 srand((unsigned)(time(0)));
	 double tep=((rand()%50)*0.01+1)*m;//得到一个随机的神经网络值，只是我用来预测用的，威哥要重新写
	// cout<<"计算的神经网络预测值为："<<tep<<endl;
	 return tep;
 }
 */

  extern int beginPoint;//第一次开始交易的时间点
 //交易决策函数，统计套利的核心思想
void trade(PriceList list,TThostFtdcPriceType avg,TThostFtdcTimeType traTime,double pricegap,TThostFtdcDateType traDate){
	//n是获得的数据的组数，g是价差数组
	double totalEarn=0;//赚到的钱
	double singleEarn=0;//一次套利赚到的钱
	
	// char pin1[]=pin1;//贵金属品种1
	// char pin2[]=pin2;//贵金属品种2
	int num=0;//品种1和2的买入或者卖出的笔数
	unsigned int state=0;//状态，为1表示正在做多交易，为2表示正在做空交易，为0表示目前没有套利机会，手中也没有持有期货合约
	int t=list.getFence();//表示时间的数组中的下标。g[0]表示的是当前的价差

	
	double k1=3.0;//m+k1*d是不考虑预测值时候的空单界限3<k1<=6
	double k2=2.5;//m+k2*d是考了预测值时候的空单界限2<k2<=3
	double k3=1;//m+k3*d是空单平仓线0<k3<=2
	double k4=1;//m-k4*d是多单平仓线0<k4<=2
	double k5=2.5;//m-k5*d是考虑预测值的多单界限2<k5<=3
	double k6=3.0;//m-k6*d是不考虑预测值的多单界限3<k6<=6
	double fm=1;//价差预测值，由神经网络给出

	double nowPri;//当前的价差，从list里面读取
	

		m=avg;//当日均价
		if(priceListGap.getFence()>300)//当数据量小于300的时候就用昨天的标准差
		d=list.getPreStd(300,m);//取得标准差


	   fm=nervenet(list,20,m);//神经网络取得预测值
	   m=(999*m+pricegap)/1000;//更新平均值
	   d=list.getPreStd(MaxNumberOfRecord,m);
	   nowPri=list.getNowPriceGap();

	 //  cout<<"当t="<<t<<"时神经网络预测值为："<<fm<<endl;
	   cout<<"经过switch之前此时的state为："<<state<<endl;
	   priceListGap.storeToGapDb(pin1,pin2,pricegap,traTime,traDate,state);//输入数据库
	switch (state){
	case 0:{
		num=0;//state=0 的时候一定都是没有做交易
	//以下是空单策略
		if(nowPri>m+k1*d)
	{
		
		kongjiacang(pin1,pin2,priceList1.getNowPriceGap(),priceList2.getNowPriceGap(),1);
		num++;
		state=2;
		beginPoint=t;//开始套利了，这是进入套利 的开始点
			cout<<"开始做空套做多利了，这是进入套利的开始点"<<endl;
	}
		if(nowPri<m-k6*d){
			duojiacang(pin1,pin2,priceList1.getNowPriceGap(),priceList2.getNowPriceGap(),1);
		num++;
		state=1;
		beginPoint=t;//开始套利了，这是进入套利 的开始点
		cout<<"开始做多套利了，这是进入套利 的开始点"<<endl;
	}//Ø	如果gt<=m-k6σ，不考虑预测值直接建仓（一手）

		break;
    }
       cout<<"经过switch之后此时的state为："<<state<<endl;
	case 2:{
		//Ø	如果gt>=m+k1σ，不考虑预测值直接建仓（一手）,进入空单交易状态
	                     //Ø	如果m+k2σ<=gt<m+k1σ，且fmt+1>=m+k1σ，此时价差偏离正常值，但预测值显示价差进一步扩大的可能性比较大，此时不进行任何操作 
	if(m+k2*d<nowPri && nowPri<m+k1*d){
		kongjiacang(pin1,pin2,priceList1.getNowPriceGap(),priceList2.getNowPriceGap(),1);
		num++;
	}//Ø	如果m+ k2σ<=gt<m+k1σ，且m+k3σ<=fmt+1<m+k1σ，此时价差偏离正常值，但预测值显示价差未出现明显的回归趋势和进一步扩大的可能，此时建仓（一手）
	if(m+k2*d<=nowPri && nowPri<m+k1*d&&fm<m+k3*d){
		kongjiacang(pin1,pin2,priceList1.getNowPriceGap(),priceList2.getNowPriceGap(),1);
		num+=2;
	}//如果m+ k2σ<=gt<m+k1σ，且fmt+1<m+k3σ，此时价差偏离正常值，但预测值显示价差出现回归的可能性比较大，此时可以大胆地建仓（两手）
	if(nowPri<=m+k3*d){
		kongpingcang(pin1,pin2,priceList1.getNowPriceGap(),priceList2.getNowPriceGap(),2);
		for(int k=beginPoint;k<t;k++)singleEarn+=(list.priceArray[k]-nowPri);
		cout<<"此次套利是从第"<<beginPoint<<"到第"<<t<<"，共赚钱："<<singleEarn<<endl;
		totalEarn+=singleEarn;
		singleEarn=0;//要清零，这个变量还要下次利用呢
		num=0;
	state=0;
	}//Ø	如果gt+1<= m+k3σ，则平仓
	break;
  }

	case 1:{

	//以下的多单策略

                         	//if(m-k6*d<g[0]&&g[0]<m-k5*d&&fm<m-k6*d)如果m-k6σ<gt<=m-k5σ，且fmt+1<=m-k6σ，此时价差偏离正常值，但预测值显示价差进一步缩小的可能性比较大，此时不进行任何操作 
	if(m-k6*d<nowPri && nowPri<m-k5*d&&m-k6*d<fm&&fm<=m-k4*d){
		duojiacang(pin1,pin2,priceList1.getNowPriceGap(),priceList2.getNowPriceGap(),1);
		num++;
	}//Ø	如果m-k6σ<gt<=m-k5σ，且m-k6σ<fmt+1<=m-k4σ，此时价差偏离正常值，但预测值显示价差未出现明显的回归趋势和进一步缩小的可能，此时建仓（一手）
	if(m-k6*d<nowPri&&nowPri<=m-k5*d&&fm>m-k4*d){
		duojiacang(pin1,pin2,priceList1.getNowPriceGap(),priceList2.getNowPriceGap(),1);
		num+=2;
	}//Ø	如果m-k6σ<gt<=m-k5σ，且fmt+1>m-k4σ，此时价差偏离正常值，但预测值显示价差出现回归的可能性比较大，此时可以大胆地建仓（两手）
	if(nowPri>=m-k4*d){
		duopingcang(pin1,pin2,priceList1.getNowPriceGap(),priceList2.getNowPriceGap(),2); 
		for(int k=beginPoint;k<t;k++)singleEarn+=(nowPri-list.priceArray[k]);
		cout<<"此次套利是从第"<<beginPoint<<"到第"<<t<<"，共赚钱："<<singleEarn<<endl;
		totalEarn+=singleEarn;
		singleEarn=0;//要清零，这个变量还要下次利用呢
		num=0;
		state=0;
	}//如果gt+1>= m-k4σ，则平仓
	break;
     }//case 1结束
	}//case 语句结束

	cout<<"赚到的钱为:  "<<totalEarn<<endl;
}//trade函数结束