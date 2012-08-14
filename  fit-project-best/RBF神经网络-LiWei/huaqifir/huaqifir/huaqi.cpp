#include <iostream>
#include <fstream>
#include<string>
#include<cstdio>
#include<cstring>
#include<cmath>
#include<vector>  
#include<ctime>
#include<algorithm>

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <math.h>
#include <time.h>

using namespace std;


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

//**********************************for rbf defination************************************start
static char *p,**endpoint;
#define VAL(p) strtod(p,endpoint)
#define N_BUF (100+1)   //输入维数
#define O_BUF 3         //输出维数
#define C_BUF 9         //隐藏层位数+1
#define STUDY_SPEED 0.3 //学习速率
#define ALPHA  0.2      //冲动系数
#define EP     0.02     //误差阀值
#define EP2    0.1 
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



//**********************************for rbf defination************************************end




inline double dis(node a,node b)
{
	return (a.x-b.x)*(a.x-b.x)+(a.y-b.y)*(a.y-b.y)+(a.z-b.z)*(a.z-b.z)+(a.w-b.w)*(a.w-b.w);
}
int vis[MaxNumberOfRecord];//每个点所属类
int pc[MaxNumberOfRecord];

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
			//cout<<vis[i]<<" ";
		}
		
		for(j=0;j<7;j++)                        //得到新的聚类点数组 cur   
		{
			int cnt=0; 
			node tmp; 
			tmp.x=tmp.y=tmp.z=tmp.w=0;

			for(i=0;i<n;i++)
			{
				
				if(vis[i]==j) 
				{
					
					tmp.x+=point[i].x;
					tmp.y+=point[i].y;
					tmp.z+=point[i].z;
					tmp.w+=point[i].w;
					cnt++;
					
				}
				
			}
			cur[j].x=tmp.x*1.0/cnt;
			cur[j].y=tmp.y*1.0/cnt;
			cur[j].z=tmp.z*1.0/cnt;
			cur[j].w=tmp.w*1.0/cnt;
			if(tmp.w==0)
			cout<<j;
		

			
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


double getAvg(double *myArray,int num,int index) {
    double sum = 0;
	if(index+num >= 200){
	      return myArray[index];
	}

	for(int i = 0;i<num;++i) {
	     sum = myArray[i+index]+sum;
	  }
	  return sum/num;
}


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

void printall(node* n) {

    for(int i =0;i<7;++i)
		cout<<n[i].w<<" "<<n[i].x<<" "<<n[i].y<<" "<<n[i].z<<endl;

}

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


//**********************************for rbf function************************************start

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
	 u[layer][node]=f(node,layer);
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
	cout<<u[2][0]<<"   "<<u[2][1]<<"   "<<u[2][2]<<endl;
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






//////////////RBF的东东///////////////////////////////////////////////////////////////////////
/////////////////////文件定位////////////////////////////////
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
 ////径向距离/////
void min_juli(int *set_number)//*j返回输入向量所在的聚类
{   
	
//	size_t size;
		int jj=0;
  double  sum_max=1000.0;
  double  sum=0.0;
  for(jj=1;jj<C_BUF;jj++)
  {
     for (int k=1;k<N_BUF;k++)
       sum+=pow(fabs(input[k]-w[1][jj][k]),2);
	 if(sum<sum_max)  
  {
	  *set_number=jj;
        sum_max=sum;
				}
	 sum=0;
    
  }
    return;
}
//////////////////向量相加////////////
void add_vector(double sum_x[N_BUF],float input[N_BUF])
{
	for (int i=1;i<N_BUF;i++)
		sum_x[i]+=input[i];
}
void sub_vector(double sum_x[N_BUF],float input[N_BUF])
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
double vector_width(double *sum_x)
{
	
 double sum=0.0;
 for (int i=1;i<N_BUF;i++)
	 sum+=sum_x[i]*sum_x[i];
  return sum;
}
//////////////k均值算法//////////////////////
void k_means()
{
	size_t size;
//	double  _min_change;
//	double  _min_max_change=0.0;
	double   min_change=10.0;//连续两次迭代的变化
	int set_number=0;//聚类序号
for (int i0=0;i0<max_layer;i0++)
		for (int i1=0;i1<max_node;i1++)
			for (int i2=0;i2<max_node;i2++)
		
				w[i0][i1][i2]=randnumber();//初始化权值
			
 
	 for(int ii=1;ii<C_BUF;ii++)
		theita[ii]=NULL;//指针初始化
  		
	fp=fopen("gapforrbf.txt","r");
	if (fp==NULL){
				 cout<<"cannot open file"<<endl;      
	             exit(0);
	}
	int _i=0;

	while (!feof(fp))
	{
		for(int _j=1;_j<N_BUF;_j++)
		{
	    fscanf(fp,"%f",input+_j);
		w[1][_i][_j]=input[_j];
		}
	
			while (!feof(fp)&&(fgetc(fp)!='\n'));
     _i++;
	}
	rewind(fp);
		int   set_member_number=0;//某一个聚类中输入向量序号
while (min_change>EP2)//主循环
{  
	set_member_number=0;

	while (!feof(fp))
	{   
		//fseek(fp,(long)(set_member_number*EACH_LINE_OFFSET),0);
	    for(int i=1;i<N_BUF;i++)
		fscanf(fp,"%f",input+i);//读出一个输入向量
		while (!feof(fp)&&(fgetc(fp)!='\n'));
	  min_juli(&set_number); //返回一个距离最小的聚类序号
			if(!theita[set_number])
		{
			theita[set_number]=(int*)malloc(sizeof(int));
		   *(theita[set_number])=set_member_number;
		}
			else{
		size=_msize(theita[set_number]);
	 theita[set_number]=(int*)realloc(theita[set_number],sizeof(int)+size);
	 *(theita[set_number]+size/4)=set_member_number;
		}	  
	 set_member_number++;
	    
	}
             /////计算中心//////////

  	for(int i=1;i<C_BUF;i++)
	{ 
	//	  cout<<*(theita[i])<<"theita[]i   "<<i<<endl;
		if(theita[i])
		{
       size=_msize(theita[i]);
	 			int m=size/4;//该聚类中元素个数	
			double sum_x[N_BUF];
			for(int s=0;s<N_BUF;s++)
				sum_x[s]=0.0;
	
				for (int mm=0;mm<m;mm++)
		{	
					
			fseeking(fp,(*(theita[i]+mm)));
			for (int k1=1;k1<N_BUF;k1++)
			fscanf(fp,"%f",input+k1);
		while((!feof(fp))&&(fgetc(fp)!='\n'));

		    add_vector(sum_x,input);				
    		}
		for(int k=1;k<N_BUF;k++)
		{  
			temp_w_1[1][i][k]=w[1][i][k];
			w[1][i][k]=sum_x[k]/m;
	   		 
		}
		   sub_vector(temp_w_1[1][i],w[1][i],N_BUF);
		 	min_change=vector_width(temp_w_1[1][i]);
	
		}
		else 
		{
				for(int k=1;k<N_BUF;k++)
					w[1][i][k]=0;
		}
	}
		rewind(fp);
    if(min_change>EP2)//迭代没有结束，则放弃此次聚类集合
			{
	
  for(int i_free=0;i_free<C_BUF;i_free++)
	  free(theita[i_free]);
  for(int ii=0;ii<C_BUF;ii++)
		theita[ii]=NULL;//指针初始化
	}
 
}//////////主循环结束////////////


	/////////计算核函数的形状参数////////////////////
	for(int ss=0;ss<C_BUF;ss++)//高斯因子初始化
		gaosi_yinzi[ss]=0.0;
	
   for(int j=1;j<C_BUF;j++)//开始计算
   {   
	  
	   if(theita[j]) 
	   {
	        size=_msize(theita[j]);
			int m=size/4;//该聚类中元素个数	
			double sum_x[N_BUF];
			for(int s=0;s<N_BUF;s++)
				sum_x[s]=w[1][j][s];				
			for (int mm=0;mm<m;mm++)
				{	
			     fseeking(fp,*(theita[j]+mm));
			     for (int kk=1;kk<N_BUF;kk++)
		       	 fscanf(fp,"%f",input+kk);
				while((!feof(fp))&&(fgetc(fp)!='\n'));
		         sub_vector(sum_x,input);
			    gaosi_yinzi[j]+=vector_width(sum_x);
				}
   	  gaosi_yinzi[j]/=m;
	}
   }
  
	fclose(fp);	

}
/////////////////////////计算隐藏层输出/////////////
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
////////////////////////////训练第二层权值/////////////////////////
void lms()
{
	double ee[C_BUF];
	double e_e;
	count_num=0;
for (int j=0;j<O_BUF;j++)
{  
	u[2][j]=0.0;
   for(int jj=0;jj<C_BUF;jj++)
    u[2][j]=u[2][j]+w[2][j][jj]*u[1][jj];
     u[2][j]=1.0/(1+exp(0-u[2][j]));
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
	cout<<u[2][0]<<"   "<<u[2][1]<<"   "<<u[2][2]<<endl;
    if(!((output[0]==u[2][0])&&(output[1]==u[2][1])&&(output[2]==u[2][2])))
		;
	else 
	{
		count_num++;
	}

}
 fclose(fp_test);
}


double rbf_result()
{
	////////////////////////////////////////rbf训练////////////////////
				//////////计算读一行字符所需要的时间///////
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

			//	开始时钟
          clock_t   t11=clock();

				k_means();

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
//	cout<<"\r"<<"        训练结束!            "<<endl;
/*
	cout<<"训练时间为"<<"     "<<double(t22-t11)/CLOCKS_PER_SEC-file_time*file_time_each<<"seconds"<<"(这个时间包括了读写文件时间，可以精确)"<<endl;
	  cout<<"训练轮数为   "<<rbf_test_time<<endl;
				fclose(fp);
               rbf_test();
			   cout<<count_num<<"   "<<endl;
			   	FILE *fp2_rbf;//保存权值
		fp2_rbf=fopen("rbf结果\\权值.txt","w");
		if (fp2_rbf==NULL)
		{
			exit(0);
		}

	for (int layer=1;layer<=2;layer++)   		///打印出第1层、第2层的权值
	    	for (int j=0;j<N[layer];j++)
		    	for(int i=0;i<N[layer-1];i++)
					fprintf(fp2_rbf,"w[%d][%d][%d]=%f\n",layer,j,i,w[layer][j][i]);
				fclose(fp2_rbf);

				
cout<<"u"<<endl;
	for(int j=0;j<max_layer;j++)
	{
		for (int i=0;i<max_node;i++)
		{

			cout<<"u["<<j<<"]["<<i<<"]"<<u[j][i]<<endl;
		}
	}

cout<<"w"<<endl;

/*
cout<<"f"<<endl;
		for(int j=0;j<max_layer;j++)
	{
		for (int i=0;i<max_node;i++)
		{

			cout<<"u["<<j<<"]["<<i<<"]"<<f(i,j)<<endl;
		}
	}
*/
   return u[2][2];
}

//**********************************for rbf function************************************start

void gap_write(ofstream ut,int start,int end)  //写入规则
{
	for(;start < end;++start)
	{
		ut<<priceArrayGap[start]<<" ";
	}
	ut<<priceArrayGap[end-1]<<" "<<priceArrayGap[end]<<" "<<priceArrayGap[end+1]<<"/n";
}

void gapforrbf_write()   //写入文件并分成多个组
{
	ofstream fout("gapforrbf.txt");
	int length = sizeof(priceArrayGap)/sizeof(double); //即，length = 200
	int start = 0,end = 100;
	int j = 0;
	for(int i = 0;i< 24 ;i++)
	{
		for(;j<end;j++)
		{
			fout<<priceArrayGap[j]<<" ";
		}
		fout<<priceArrayGap[end-1]<<" "<<priceArrayGap[end]<<" "<<priceArrayGap[end+1]<<"\n";
		start ++ ;
		j=start;
		end = start + 100;
	}
	fout.close();
}

void fortest_write()
{
	ofstream fout("forrbftest.txt");
	int length = sizeof(priceArrayGap)/sizeof(double); //即，length = 200
	int start = 0,end = 100;
	int j = 0;
	for(int i = 0;i< 8 ;i++)
	{
		for(;j<end;j++)
		{
			fout<<priceArrayGap[j]<<" ";
		}
		fout<<priceArrayGap[end-1]<<" "<<priceArrayGap[end]<<" "<<priceArrayGap[end+1]<<"\n";
		start ++ ;
		j=start;
		end = start + 100;
	}
	fout.close();
}



int main()
{

	int numOfRecordGap;//最终的价差数组的真实记录数目，gap是差的意思
	getGap(priceArrayGap,numOfRecordGap);
	//cout<<numOfRecordGap;
	//这个 priceArrayGap数组就是我们要的价差数组了，大家要拿去做什么就拿去做什么吧，加油，伙伴们!!!!
	fortest_write(); //test练习数据
	gapforrbf_write();//study学习数据
	for(int i = 0;i<200;++i) {
	
		point[i].w = priceArrayGap[i];
		point[i].x = getAvg(priceArrayGap,5,i);
		point[i].y = getAvg(priceArrayGap,10,i);
		point[i].z = getAvg(priceArrayGap,20,i);
		
	}
	
	solve();
	
	printall(cur);
	
	getMinDistance(cur);
	cout<<endl;
	for(int i = 0 ;i<7;++i) {
	
	   cout<<minDistance[i]<<"  ";

	}

	cout<<"after NN: the value is "<<rbf_result()<<endl;

	system("pause");
	
}