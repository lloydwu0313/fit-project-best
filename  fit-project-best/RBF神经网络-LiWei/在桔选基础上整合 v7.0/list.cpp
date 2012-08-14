#include"fit.h"
#include <mysql_connection.h>  
#include <mysql_driver.h>  
#include <statement.h>

using namespace sql; 
	extern    	int beginhour;//开始的小时
	extern    	int beginmin;//开始的分
	extern    	int beginsec;//开始的秒
	extern    PriceList priceList1;//品种1的价格数组
  extern  PriceList priceList2;//品种2的价格数组
 extern   PriceList priceListGap;//价差数组
extern double m;//算出来的价差均值；
extern	double d;//d是算出来的标准差；
extern mysql::MySQL_Driver *driver;  
   extern  Connection *con;  
    extern Statement *state;  
    extern ResultSet *result; 
      char text[60]={0};
PriceList::PriceList(){
		fence=0;
		for(int i=0;i<MaxNumberOfRecord;i++)
		priceArray[i]=INIVALUE;
	}
int PriceList::getFence(){
		return this->fence;
	}
void PriceList::add(double price,char* time){
	int hour,min,sec;
	int newdex;
	TimeStructToHMS(time, hour, min,sec);
	newdex=fence+getnumofmisecond( hour, min,sec);//新的位置
	for(int i=fence;i<newdex;i++)
		priceArray[i]=priceArray[fence];//以之前的值填充数据缺失，因为ctp的api是有了更新才给吃数据
	fence=newdex;//更新fence
		priceArray[fence]=price;
     }
void PriceList::addforfirsttime(double price){
	priceArray[fence++]=price;
}
void PriceList:: storeToDb(TThostFtdcTimeType traTime,double price,TThostFtdcDateType traDate,TThostFtdcInstrumentIDType id){
	   
        sprintf(text,"insert into tradehisdata values(\"%s\",%f,%s,\"%s\");",traTime,price,traDate,id);
		cout<<text<<endl;	
	try{
		state->execute(text);
	}
	catch(std::runtime_error& e){
		cout<<"已经有这条记录了，数据库访问出错"<<endl;
	}
}

void PriceList:: storeToGapDb(TThostFtdcInstrumentIDType id1,TThostFtdcInstrumentIDType id2,double price,TThostFtdcTimeType traTime,TThostFtdcDateType traDate,int tradestate){

        sprintf(text,"insert into gaphisdata values(\"%s\",\"%s\",%f,\"%s\",%s,%d);",id1,id2,price,traTime,traDate,tradestate);
		cout<<text<<endl;
		//state->execute(text);
	try{
		state->execute(text);
	}
	catch(std::runtime_error& e){
		cout<<"已经有这条记录了，数据库访问出错"<<endl;
	}
}
void PriceList:: getDataFromDb(){
	double pri;
	string tim;
	char *tm;
	  result = state->executeQuery("select pricegap,gaptime from gaphisdata where gapTime between \"14:00:00\"and \"15:00:00\"and gapDate = 20120807");  	
	 
	  
    // 输出查询  
	  int i=0;
    while(result->next()&&i<MaxNumberOfRecord)  
    { 
		 pri=result->getDouble("pricegap");
		 tim=result->getString("gaptime");
		 tm=(char*)(tim.c_str());
        priceListGap.add(pri,tm);  //存入数组内存
    }
}

void PriceList:: getDataFromDb_all(){
	double pri;
	string tim;
	char *tm;
	//  result = state->executeQuery("select pricegap,gaptime from gaphisdata where pin1 = 'ag1209' and pin2 = 'ag1212' and gaptime > '11:17:00'");  	
	 result = state->executeQuery("select pricegap,gaptime from gaphisdata where pin1 = 'ag1209' and pin2 = 'ag1212' and gaptime > '11:17:00'");  	
	  
    // 输出查询  
	  int i=0;
    while(result->next()&&i<MaxNumberOfRecord)  
    { 
		 pri=result->getDouble("pricegap");
		 tim=result->getString("gaptime");
		 tm=(char*)(tim.c_str());
		 priceListGap.priceArray[i] = pri;
		 i++;
    }
}


//double PriceList::getNowPriceGap
double PriceList::getPreAvg(int preNum){//preNum是需要的前几个数据(包括自身)的平均值
		double tmp=0;
		for(int i=(fence-1)%MaxNumberOfRecord;i>(fence-preNum+MaxNumberOfRecord)%MaxNumberOfRecord;i=(i-1)%MaxNumberOfRecord)
			tmp+=priceArray[i];
		return tmp/preNum;
	}
double PriceList:: getPreStd(int preNum,double avg){
		double tep=0;
	 
	 for(int i=(fence-1)%MaxNumberOfRecord;i>(fence-preNum+MaxNumberOfRecord)%MaxNumberOfRecord;i=(i-1)%MaxNumberOfRecord)
		 tep+=(avg-this->priceArray[i])*(avg-this->priceArray[i]);
	//  cout<<"标准差为："<<(sqrt(tep/((double)n)))<<endl;
	 return sqrt(tep/((double)preNum));
	}
double PriceList:: getNowPriceGap(){
		//这个函数不允许在一开始调用，切记，要写入有数据才可以
		return this->priceArray[(fence-1+MaxNumberOfRecord)%MaxNumberOfRecord];
}

void sqlini(){
    driver = sql::mysql::get_mysql_driver_instance(); 

	con=driver->connect("localhost:3306","sa","626618");
	state = con->createStatement();  
    state->execute("use fitdb"); 
	
	
}
void sqlend(const char*tradate){
	
	  sprintf(text,"insert into evdaydata values(%s,%f,%f,\"%s\",\"%s\");",tradate,m,d,pin1,pin2);
		cout<<text<<endl;
		//state->execute(text);
	try{
		state->execute(text);
	}
	catch(std::runtime_error& e){
		cout<<"已经有这条记录了，数据库访问出错"<<endl;
	}
	delete state;
	delete con;
	delete driver;
	delete result;
}
void PriceList:: readmdfromdb(){
	
	sprintf(text,"select avg from evdaydata where comDate =20120808 and pin1=\"&s\" and pin2=\"&s\";",pin1,pin2);
	try{
		 result = state->executeQuery(text);
	}
	catch(std::runtime_error& e){
		cout<<"已经有这条记录了，数据库访问出错"<<endl;
	}
	 	  
    // 输出查询  
	 while(result->next())  
    {  
       m=result->getDouble("avg");
	 }
	 sprintf(text,"select stdd from evdaydata where comDate =20120808 and pin1=\"%s\" and pin2=\"%s\";",pin1,pin2);
	   try{
		 result = state->executeQuery(text);
	}
	catch(std::runtime_error& e){
		cout<<"已经有这条记录了，数据库访问出错"<<endl;
	}
	 while(result->next())  
    {  
   d=result->getDouble("stdd");
	 }
	  
}

void TimeStructToHMS(const char* time,int& hour,int& minute,int& second)
{
char timeBack[9];
memset(timeBack,0,sizeof(timeBack));
strcpy(timeBack,time);
timeBack[2]='\0';
timeBack[5]='\0';
hour=atoi(timeBack);
minute=atoi(timeBack+3);
second=atoi(timeBack+6);
}
int getnumofmisecond(const int h1,const int m1 ,const int s1){
	 int hour=h1;
	 int min=m1;
	 int sec=s1;
	 int tmp;
	if(sec<beginsec){
		if(min==0){min=59;hour--;}
		else min--;	
	}
	tmp=(sec+60-beginsec)%60;//不管为多少都一样
	if (min<beginmin){hour--;}
	tmp+=(min+60-beginmin)%60*60;
	tmp+=(hour-beginhour)*3600;
	return tmp;
}