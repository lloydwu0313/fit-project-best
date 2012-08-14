#include"fit.h"

//从文本中读取数据到数组中，priceTxt是要读取的文件的对应的char值，price是要传入的数组
void read(const char*priceTxt,double *priceArray ,int& numOfRecord){
	string stringArray[MaxNumberOfRecord];
	ifstream ftxt;
		ftxt.open(priceTxt);	  
		int i=0;
	     while(!ftxt.eof())   
	     { 
			 getline(ftxt,stringArray[i++]);
         }
		 //如果i等于NumberOfRecorder-1说明读到了最大可以容纳的数据的条数，再读就溢出了
		 if(i>MaxNumberOfRecord){      //当i=NumberOfRecorder才说明达到了最大可以容纳的数据的条数，这个NumberOfRecorder设的太小了
			 cout<<"这个NumberOfRecorder设的太小了"<<endl;
			 exit(1);
		 }
		 numOfRecord=i;//真实的记录条数为i(因为i是从零算起的，一直加到i-1)，这里是将真实数据条数传出去
		 ftxt.close();		
		 --i;
		for(;i>=0;i--){//我改了，还是从头到尾吧，就是
		 priceArray[i]=atof(stringArray[i].c_str());//将得到的string转化为double值并写进数组
		}		
}

//计算出价差序列，计算出价差序列
void getGap( double*priceArrayGap,int& numOfRecordGap){
	int numOfRecordOne;//第一个文件的真实记录数目
	int numOfRecordTwo;//第二个文件的真实记录数目

	const char *txtOne="qihuo.txt";//文本文件,存放第一份价格值
	const char* txtTwo="xianhuo.txt";//文本文件,存放第二份价格值
	//const char* txtGap="xianhuo.txt";//文本文件,存放价差           想了想还是不要这个了，我们直接在内存里操作，不需要写出文件保存价差值

	double priceArrayOne[MaxNumberOfRecord];//第一个价格数组
	double priceArrayTwo[MaxNumberOfRecord];//第二个价格数组

	read(txtOne,priceArrayOne,numOfRecordOne);//将第一个文本price里的数据读到txtOne数组中
	read(txtTwo,priceArrayTwo,numOfRecordTwo);//将第一个文本里的数据读到txtOne数组中
	//自此，读取两个文本得到的数据结束，本别读到了priceArrayOne和priceArrayTwo两个数组中

	/*一下三个if都是为了把numOfRecordGap取得(numOfRecordOne和numOfRecordTwo)中更小的一个并将三个都赋值为相等，
	更长的一个将自动舍弃数组后面的值(也就是时间上更早的历史数据)*/
		 if(numOfRecordOne==numOfRecordTwo) 
			 numOfRecordGap=numOfRecordOne;
		 if(numOfRecordOne>numOfRecordTwo){
			 numOfRecordGap=numOfRecordTwo;
		     numOfRecordOne=numOfRecordTwo;
		 }
	     if(numOfRecordOne<numOfRecordTwo){
			 numOfRecordGap=numOfRecordOne;
		     numOfRecordTwo=numOfRecordOne;
		 }
	 
	
		 for(int i=0;i<numOfRecordGap;i++){
		 priceArrayGap[i]=priceArrayOne[i]-priceArrayTwo[i];//	计算价差
		 }
	//这个 priceArrayGap数组就是我们要的价差数组了，那么要拿去做什么拿去做什么吧，加油，伙伴们
	//	 for(int i=0;i<numOfRecordGap;i++)cout<<priceArrayGap[i]<<endl;  //输出所有的价差序列
}

double getavg(double *g,int n){//n是给的数据个数，就是前几个，g是数据数组
	 double tmp=0;
	 for(int i=0;i<n;i++ )
		 tmp+=g[i];
	// cout<<"均值为："<<tmp/((double)n)<<endl;
	 return tmp/((double)n);
}
 double getstd(double*g ,int n){
	 double tep=0;
	 double a=getavg(g,n);
	 for(int i=0;i<n;i++)
	 tep+=(a-g[i])*(a-g[i]);
	//  cout<<"标准差为："<<(sqrt(tep/((double)n)))<<endl;
	 return sqrt(tep/((double)n));

 }