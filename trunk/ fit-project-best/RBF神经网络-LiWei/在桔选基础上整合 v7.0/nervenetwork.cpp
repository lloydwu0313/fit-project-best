#include "fit.h"

#include <mysql_connection.h>
#include <mysql_driver.h>
#include <statement.h>


#include <iostream>
#include <fstream>
#include<string>
#include<cstdio>
#include<cstring>
#include<cmath>
#include<vector>  
#include<ctime>
#include<algorithm>
//**********************************for rbf head file************************************start
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <math.h>
#include <time.h>
//**********************************for rbf head file************************************end
using namespace std;
using namespace sql;

#define MaxNumberOfRecord 200  //一次性获取的最大的记录数目
struct node
{
	double x,y,z,w;
}point[MaxNumberOfRecord];
#define eps 1e-15
#define H 7
const double inf = 1000000000;
node pre[10],cur[10];//存放代表点
int n = 200;
double minDistance[7]; //存放扩展常数

double priceArrayGap[MaxNumberOfRecord];//价差数组

//**********************************for rbf defination************************************start！
static char *p,**endpoint;
#define VAL(p) strtod(p,endpoint)
#define N_BUF (100+1)   //输入维数
#define O_BUF 3         //输出维数
#define C_BUF 8         //隐藏层位数+1
#define STUDY_SPEED 0.3 //学习速率
#define ALPHA  0.2      //冲动系数
#define EP     0.02     //误差阀值
#define EP2    0.1      //误差阀值2
#define TEST_SETS 8
#define EACH_LINE_OFFSET  40

static int N[3]={N_BUF,C_BUF,O_BUF};  //各层的节点数
#define max_layer   3//网络层数
#define max_node  (100+1) //层中最大的节点数
static double u[max_layer][max_node];  //某一层某一节点的输出
static double w[max_layer][max_node][max_node];//权值
static double temp_w_1[max_layer][max_node][max_node];
static double temp_w_2[max_layer][max_node][max_node];
static double delta_w[max_layer][max_node][max_node];

static double wucha = 1; //误差阀值
double gaosi_yinzi[C_BUF];  //高思 核函数 形状因子
static int * theita[C_BUF];  //对于每一权向量的聚类集合

float input[N_BUF];   //输入向量
int output[O_BUF];    //输出向量
int count_num=0;          //测试正确样本数量
int k_k=-1;
   FILE *fp;
   FILE *fpwucha;
double e[max_layer][max_node];  //误差函数对u的导数
double g[max_layer][max_node][max_node]; //误差函数对w的导数，即梯度
int file_time = 0;
int rbf_test_time = 0;
int bp_test_time = 0;

//**********************************for rbf function's defination************************************end！
//**************  mysql_start

//************** mysql_end


inline double dis(node a,node b)
{
	return (a.x-b.x)*(a.x-b.x)+(a.y-b.y)*(a.y-b.y)+(a.z-b.z)*(a.z-b.z)+(a.w-b.w)*(a.w-b.w);
}
int vis[MaxNumberOfRecord];//每个点所属类
int pc[MaxNumberOfRecord];

//***********************************   for 中心值矩阵    *******************************************Start!
//求出中心值矩阵
void solve()
{
	int i,j,tot=0;
	memset(vis,0,sizeof(vis)); memset(pc,0,sizeof(pc));
	while(tot<7)
	{
		srand(time(0));
		int x;
		x=rand()%n;
		while(pc[x])
			x=rand()%n;
		pc[x]=1;
		pre[tot]=point[x];
		
		vis[x]=tot;
		tot++;
	}                                       //初始化，随机得到七个不同的聚类点                                       

	while(1)
	{
		for(i=0;i<n;i++)                     //得到vis数组，即存放对应的聚类编号
		{
			double Min=inf; int id;
			for(j=0;j<7;j++)
			{
				double tmp=dis(pre[j],point[i]);
				if(Min-tmp>0)
				{
					Min=tmp;
					id=j;
				}
			}
			vis[i]=id;
	//		cout<<vis[i]<<" ";
		}
		
		for(j=0;j<7;)                        //得到新的聚类点数组 cur   
		{
			int cnt=0; 
			node tmp; 
			tmp.x=tmp.y=tmp.z=tmp.w=0;

			for(i=0;i<n;i++)
			{
				
				if(vis[i]==j) 
				{
					
					tmp.x+=point[i].x;  //价差
					tmp.y+=point[i].y;  //前5天平均
					tmp.z+=point[i].z;  //前10天平均 
					tmp.w+=point[i].w;  //前20天平均
					cnt++;

		//			cout<<point[j].x;
					
				}
				
			}
			cur[j].x=tmp.x*1.0/cnt;
			cur[j].y=tmp.y*1.0/cnt;
			cur[j].z=tmp.z*1.0/cnt;
			cur[j].w=tmp.w*1.0/cnt;
	//		if(tmp.w==0)
	//		cout<<point[j].x;
		    j++;

			
		}
		int key;
		for(key=0;key<7;key++)
		{
			if(dis(pre[key],cur[key])<eps)    //比较新老聚类点的差值
			{  
				continue;
			}
			else break;
		}
		
		if(key>=7) break;               //i为7时跳出整个循环，运算结束，否则不断循环
		else {
		    
			for(int num = 0; num<7;++num) {     //把初始聚类中心更新
		   
		          pre[num] = cur[num];
		   }
		   
		}
	}
}

//求出均值
double getAvg(float *myArray,int num,int index) {
    float sum = 0;
	if(index+num >= 200){
	      return myArray[index];
	}

	for(int i = 0;i<num;++i) {
	     sum = myArray[i+index]+sum;
	  }
	  return sum/num;
}

//求出最小距离
void getMinDistance(node* n) {
	double in = 1000000;
	double min = 0 ;
	for(int i = 0;i<7;++i) {
		in = 1000000;
        for(int j =0;j<7;++j) {
			
			if(i!=j) {
				double tmp = dis(cur[i],cur[j]);
		     if(tmp<in)
				 in = tmp;

			}

		}
		minDistance[i] =  in;
		//cout<<in;
	}
}

//印出所有中心值，测试时使用
void printall(node* n) {

    for(int i =0;i<7;++i)
		cout<<n[i].w<<" "<<n[i].x<<" "<<n[i].y<<" "<<n[i].z<<endl;

}

//***********************************   for 中心值矩阵    *******************************************End!

//***********************************   辅助函数，I/O操作（已无效）   ******************************Start!
//priceTxt是要读取的文件的对应的char值，price是要传入的数组
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
		for(int k=0;i>=0;i--){//从尾到头，把stringArray中的string转化为priceArray中的double,而且顺序恰恰相反，离当前值更近的值反而标更小
		 priceArray[k]=atof(stringArray[i].c_str());//将得到的string转化为double值并写进数组
		 k++;
		}		
}
//这个是的价差的函数
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
	//这个 priceArrayGap数组就是我们要的价差数组了，那么要拿去做什么酒拿去做什么吧，加油，伙伴们
		 for(int i=0;i<numOfRecordGap;i++)
			 if(priceArrayGap[i]==0)cout<<"价差："<<priceArrayGap[i]<<endl;
}
//***********************************   辅助函数，I/O操作（已无效）   ******************************End!


//**********************************for rbf function's definition************************************start!

double randnumber()//-0.1~0.1的随机函数 
{
	double b = rand();
	return (sin(b)/ 10) ;
}

//求某个节点的输出值
double f(int node,int layer){
	double sum=0;
for (int node2=0;node2<N[layer-1];node2++)
{
	//sum=sum+w[layer-1][node][node2]*u[layer-1][node2];
	sum=sum+w[layer][node][node2]*u[layer-1][node2];
}
sum=1.0/(1+exp(0-sum));
return sum;
}

//求误差函数对u的导数
double gf(int layer,int node){
	double sum=0;
for (int m=0;m<N[layer+1];m++)
sum=sum+e[layer+1][m]*u[layer+1][m]*(1-u[layer+1][m])*w[layer+1][m][node];
return sum;
}


//求前馈
void feed_forward()//前馈输出值
{
for (int layer=1;layer<=2;layer++)
 for(int node=0;node<N[layer];node++)
	 u[layer][node]=f(node,layer);    //node = 0~8,layer=1~2
}


//求梯度
void compute_gradient()
{
for (int layer=2;layer>0;layer--)
{
  for(int node=0;node<N[layer];node++)
  {
	  if (layer==2) e[2][node]=u[2][node]-output[node];
	  else e[layer][node]=gf(layer,node);
  }

  for (int j=0;j<N[layer];j++)
		  for(int i=0;i<N[layer-1];i++)
			  {
                g[layer][j][i]=e[layer][j]*u[layer][j]*(1.0-u[layer][j])*u[layer-1][i];  
		     	  
		  }
}
}


//修改权值
void update_weights()
{
		for (int layer=1;layer<=2;layer++)
	    	for (int j=0;j<N[layer];j++)
		    	for(int i=0;i<N[layer-1];i++)
				{
					temp_w_1[layer][j][i]=w[layer][j][i];
					w[layer][j][i]=w[layer][j][i]-STUDY_SPEED*g[layer][j][i]+ALPHA*delta_w[layer][j][i];
					temp_w_2[layer][j][i]=w[layer][j][i];
					delta_w[layer][j][i]=temp_w_2[layer][j][i]-temp_w_1[layer][j][i];
				}
		
}
double erro()//误差
{
	double sum=0;
	
 sum=pow(fabs(u[2][0]-output[0]),2)+pow(fabs(u[2][1]-output[1]),2)+pow(fabs(u[2][2]-output[2]),2);
			
return sum;
}


//////////初始化，装载样例
void initial(FILE *fp)
{

 if(feof(fp)) 
 {
	 rewind(fp);
  k_k=(++k_k)%7;
  if(!k_k)  fprintf(fpwucha,"\n");
fprintf(fpwucha,"%16f",wucha);
	 wucha=0;
 }
 input[0]=1.0;
	for(int i=1;i<N_BUF;i++)
		fscanf(fp,"%f",input+i);
	 
	for(int i=0;i<O_BUF;i++)
       fscanf(fp,"%d",output+i);
//	cout<<input[2]<<endl<<output[2]<<endl;
	//////////////////////至此读出一个样本的数据（input,output）;
	for (int m=0;m<N_BUF;m++)
    u[0][m]=input[m];
				

}
void test()
{
	FILE *fpt=NULL;
	fpt=fopen("forrbftest.txt","r");
	for (int i=0;i<=7;i++)
	{
		initial(fpt);
		feed_forward();
		u[2][0]=floor(u[2][0]+0.5);
		u[2][1]=floor(u[2][1]+0.5);
		u[2][2]=floor(u[2][2]+0.5);
//		u[2][0] = 43;
//		u[2][1] = 44;
//		u[2][2] = 45;
//	cout<<u[2][0]<<"   "<<u[2][1]<<"   "<<u[2][2]<<endl;		layer	1	int
		if(!((output[0]==u[2][0])&&(output[1]==u[2][1])&&(output[2]==u[2][2])))
			;
		else 
		{
			count_num++;
//	cout<<output[0]<<output[1]<<output[2]<<endl;
		}
	}
	fclose(fpt);
}


//文件定位
void  fseeking(FILE* fp,int row_no)
{  
	rewind(fp);
	if (!row_no) return;
l:while((!feof(fp))&&(fgetc(fp)!='\n'))
  {
	  file_time++;
  };
    //if(feof(fp)) return;
  row_no--;
  if (row_no) goto l; 
  else	return;
}       


//////////////////向量相加////////////
void add_vector(double sum_x[N_BUF],float input[N_BUF])   //向量想家
{
	for (int i=1;i<N_BUF;i++)
		sum_x[i]+=input[i];
}
void sub_vector(double sum_x[N_BUF],float input[N_BUF])  //向量相减
{
	for (int i=1;i<N_BUF;i++)
		sum_x[i]-=input[i];
}
void sub_vector(double sum_x[C_BUF],double input[C_BUF],int pp)
{   
	int ppp=0;
	if (pp==N_BUF)  ppp=1;

	for (int i=ppp;i<pp;i++)
		sum_x[i]-=input[i];
	return;
}
double vector_width(double *sum_x)  //向量宽度
{
	
 double sum=0.0;
 for (int i=1;i<N_BUF;i++)
	 sum+=sum_x[i]*sum_x[i];
  return sum;
}

void k_set(node *tmp)
{
	for(int i;i<(C_BUF - 1);i++)
		gaosi_yinzi[i] = tmp[i].x;
}


//计算隐藏层输出
void compute_u1()
{    
   double x[N_BUF];
	
	for(int j=1;j<C_BUF;j++)
	{    
		for (int xx=1;xx<N_BUF;xx++)
		x[xx]=input[xx];
       sub_vector(x,w[1][j],N_BUF);

		if (gaosi_yinzi[j])
	   u[1][j]=exp((0.0-vector_width(x))/(2.0*gaosi_yinzi[j]));
	  else u[1][j]=0;
	}
	u[1][0]=1;
	return;
}

//训练第二层权值
void lms()
{
	double ee[C_BUF];
	double e_e;
	count_num=0;
for (int j=0;j<O_BUF;j++)
{  
//	u[2][j]=0.0;
   for(int jj=0;jj<C_BUF;jj++)
    u[2][j]=u[2][j]+w[2][j][jj]*u[1][jj];
 //    u[2][j]=1.0/(1+exp(0-u[2][j]));
   e_e=u[2][j]-output[j];
    for (int jjj=0;jjj<C_BUF;jjj++)
	{
		 temp_w_1[2][j][jjj]=w[2][j][jjj];
	   ee[jjj]=e_e*u[1][jjj]*STUDY_SPEED;
	   w[2][j][jjj]=w[2][j][jjj]-ee[jjj]+ALPHA*delta_w[2][j][jjj];
	   temp_w_2[2][j][jjj]=w[2][j][jjj];
	   delta_w[2][j][jjj]=temp_w_2[2][j][jjj]-temp_w_1[2][j][jjj];
	   }
	  // ee[jjj]=e_e*u[1][jjj]*STUDY_SPEED;
  //sub_vector(w[2][j],ee,C_BUF);
}
}
//rbf训练
void rbf_test()
{
FILE * fp_test=NULL;
fp_test=fopen("forrbftest.txt","r");
if(!fp_test)
{
cout<<"cannot open file"<<endl;
	 exit(0);
}
while (!feof(fp_test))
{
 for(int i=1;i<N_BUF;i++)
	 fscanf(fp_test,"%f",input+i);
  for(int j=0;j<O_BUF;j++)
	 fscanf(fp_test,"%d",output+j);
   compute_u1();
  for(int j_j=0;j_j<O_BUF;j_j++)
 {
    u[2][j_j]=0.0;
    for(int jj=0;jj<C_BUF;jj++)
    u[2][j_j]=u[2][j_j]+w[2][j_j][jj]*u[1][jj];
   u[2][j_j]=1.0/(1+exp(0-u[2][j_j]));
 }
         u[2][0]=floor(u[2][0]+0.5);
    u[2][1]=floor(u[2][1]+0.5);
    u[2][2]=floor(u[2][2]+0.5);
//	cout<<u[2][0]<<"   "<<u[2][1]<<"   "<<u[2][2]<<endl;
    if(!((output[0]==u[2][0])&&(output[1]==u[2][1])&&(output[2]==u[2][2])))
		;
	else 
	{
		count_num++;
	}

}
 fclose(fp_test);
}

//得到rbf结果
double rbf_result(PriceList list)
{
//	clock_t t1  = clock();
	for (int i0=0;i0<max_layer;i0++)
		for (int i1=0;i1<max_node;i1++)
			for (int i2=0;i2<max_node;i2++)
				w[i0][i1][i2]=randnumber();

//rbf训练
//计算读一行字符所需要的时间
	FILE * fp_test=fopen("gapforrbf.txt","r");
	if (fp_test==NULL){
		cout<<"cannot open file"<<endl;      
		exit(0);
	}
	clock_t t_test1=clock();
	while(!(feof(fp_test))&&(fgetc(fp_test)!='\n'));   //打开文件的时间
	clock_t t_test2=clock();
	if (t_test2==clock_t(-1)){
		cerr<<"sorry ,no clock\n";
		exit(1);
	}  
	double file_time_each=double(t_test2-t_test1)/CLOCKS_PER_SEC/EACH_LINE_OFFSET*100000000 ;
	fclose(fp_test);
	wucha=1;
	k_k=-1;
	fpwucha=fopen("rbf结果\\误差.txt","w");
	test();      //学习	
	clock_t   t11=clock(); //	开始时钟 
	fp=fopen("gapforrbf.txt","r");
	if (fp==NULL){
		cout<<"cannot open file"<<endl;      
		exit(0);
	}
	while(wucha>EP)		//当误差大于误差阀值时，循环处理
	{   
		if (feof(fp)) rewind(fp);
		for(int i=1;i<N_BUF;i++)
			fscanf(fp,"%f",input+i);
		for(int i=0;i<O_BUF;i++)
			fscanf(fp,"%d",output+i);
		compute_u1();  //计算隐藏层输出
		lms();   //训练第2层权值
		wucha=0.0;
		wucha=erro(); //求出新的误差
		k_k=(++k_k)%7;
		if(!k_k)  fprintf(fpwucha,"\n");
		fprintf(fpwucha,"%16f",wucha);
		rbf_test_time++;
	}
	//结束时钟
	clock_t    t22=clock();
	if (t22==clock_t(-1)){
		cerr<<"sorry ,no clock\n";
		exit(1);
	}  
	fclose(fpwucha);
	rbf_test();
//	clock_t t2  = clock();
//	cout<<"Cost: "<<t2-t1<<endl;
	return *(output+0);
}

//写入规则
void gap_write(ofstream ut,int start,int end)  
{
	for(;start < end;++start)
	{
		ut<<priceArrayGap[start]<<" ";
	}
	ut<<priceArrayGap[end-1]<<" "<<priceArrayGap[end]<<" "<<priceArrayGap[end+1]<<"/n";
}

//写入rbf求预测值需要的数据
void gapforrbf_write(PriceList list)   //写入文件并分成多个组
{
	/*
	ofstream fout("gapforrbf.txt");

	int length = sizeof(priceArrayGap)/sizeof(double); //即，length = 200

	int start = 0,end = 100;
	int j = 0;
	for(int i = 0;i< 24 ;i++)
	{
		for(;j<end;j++)
		{
			fout<<list.priceArray[j]<<" ";
		}
		fout<<list.priceArray[end-1]<<" "<<list.priceArray[end]<<" "<<list.priceArray[end+1]<<"\n";
		start ++ ;
		j=start;
		end = start + 100;
	}
	fout.close();
	*/
	ofstream fout("gapforrbf.txt");

	int length = sizeof(priceArrayGap)/sizeof(double); //即，length = 200

	int start = 80,end = 180;
	int j = 80;
	for(int i = 0;i< 20 ;i++)
	{
		for(;j<end;j++)
		{
			fout<<list.priceArray[j]<<" ";
		}
		fout<<list.priceArray[end-1]<<" "<<list.priceArray[end]<<" "<<list.priceArray[end+1]<<"\n";
		start ++ ;
		j=start;
		end = start + 100;
	}
	fout.close();

}

//写入rbf做学习时的数据
void fortest_write(PriceList list)//写入测试文件，用来初始锻炼学习
{
	ofstream fout("forrbftest.txt");

	int length = sizeof(priceArrayGap)/sizeof(double); //即，length = 200

	int start = 0,end = 100;
	int j = 0;
	for(int i = 0;i< 8 ;i++)
	{
		for(;j<end;j++)
		{
			fout<<list.priceArray[j]<<" ";
		}
		fout<<list.priceArray[end-1]<<" "<<list.priceArray[end]<<" "<<list.priceArray[end+1]<<"\n";
		start ++ ;
		j=start;
		end = start + 100;
	}
	fout.close();
}


//**********************************for rbf function************************************start

void prints()
{
	cout<<"abc"<<endl;

}

//rbf神经网络算法算出值
double nervenet(PriceList list,int n,double m){
	cout<<"***************************"<<endl;
	for(int i = 0;i<200;++i) {
		point[i].x = list.priceArray[i];
		point[i].y = getAvg(list.priceArray,5,i);
		point[i].z = getAvg(list.priceArray,10,i);
		point[i].w = getAvg(list.priceArray,20,i);
	}
	solve();
	printall(cur);
	getMinDistance(cur);
	double result = rbf_result(list);
	return result;
}

void readgap(PriceList list)
{
	int i = 0;
	list.getDataFromDb();
	cout<<list.priceArray[5]<<endl;
}