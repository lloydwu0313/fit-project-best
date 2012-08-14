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

#define MaxNumberOfRecord 200  //һ���Ի�ȡ�����ļ�¼��Ŀ
struct node
{
	double x,y,z,w;
}point[MaxNumberOfRecord];
#define eps 1e-15
#define H 7
const double inf = 1000000000;
node pre[10],cur[10];//��Ŵ����
int n = 200;
double minDistance[7]; //�����չ����

double priceArrayGap[MaxNumberOfRecord];//�۲�����

//**********************************for rbf defination************************************start��
static char *p,**endpoint;
#define VAL(p) strtod(p,endpoint)
#define N_BUF (100+1)   //����ά��
#define O_BUF 3         //���ά��
#define C_BUF 8         //���ز�λ��+1
#define STUDY_SPEED 0.3 //ѧϰ����
#define ALPHA  0.2      //�嶯ϵ��
#define EP     0.02     //��ֵ
#define EP2    0.1      //��ֵ2
#define TEST_SETS 8
#define EACH_LINE_OFFSET  40

static int N[3]={N_BUF,C_BUF,O_BUF};  //����Ľڵ���
#define max_layer   3//�������
#define max_node  (100+1) //�������Ľڵ���
static double u[max_layer][max_node];  //ĳһ��ĳһ�ڵ�����
static double w[max_layer][max_node][max_node];//Ȩֵ
static double temp_w_1[max_layer][max_node][max_node];
static double temp_w_2[max_layer][max_node][max_node];
static double delta_w[max_layer][max_node][max_node];

static double wucha = 1; //��ֵ
double gaosi_yinzi[C_BUF];  //��˼ �˺��� ��״����
static int * theita[C_BUF];  //����ÿһȨ�����ľ��༯��

float input[N_BUF];   //��������
int output[O_BUF];    //�������
int count_num=0;          //������ȷ��������
int k_k=-1;
   FILE *fp;
   FILE *fpwucha;
double e[max_layer][max_node];  //������u�ĵ���
double g[max_layer][max_node][max_node]; //������w�ĵ��������ݶ�
int file_time = 0;
int rbf_test_time = 0;
int bp_test_time = 0;

//**********************************for rbf function's defination************************************end��
//**************  mysql_start

//************** mysql_end


inline double dis(node a,node b)
{
	return (a.x-b.x)*(a.x-b.x)+(a.y-b.y)*(a.y-b.y)+(a.z-b.z)*(a.z-b.z)+(a.w-b.w)*(a.w-b.w);
}
int vis[MaxNumberOfRecord];//ÿ����������
int pc[MaxNumberOfRecord];

//***********************************   for ����ֵ����    *******************************************Start!
//�������ֵ����
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
	}                                       //��ʼ��������õ��߸���ͬ�ľ����                                       

	while(1)
	{
		for(i=0;i<n;i++)                     //�õ�vis���飬����Ŷ�Ӧ�ľ�����
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
		
		for(j=0;j<7;)                        //�õ��µľ�������� cur   
		{
			int cnt=0; 
			node tmp; 
			tmp.x=tmp.y=tmp.z=tmp.w=0;

			for(i=0;i<n;i++)
			{
				
				if(vis[i]==j) 
				{
					
					tmp.x+=point[i].x;  //�۲�
					tmp.y+=point[i].y;  //ǰ5��ƽ��
					tmp.z+=point[i].z;  //ǰ10��ƽ�� 
					tmp.w+=point[i].w;  //ǰ20��ƽ��
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
			if(dis(pre[key],cur[key])<eps)    //�Ƚ����Ͼ����Ĳ�ֵ
			{  
				continue;
			}
			else break;
		}
		
		if(key>=7) break;               //iΪ7ʱ��������ѭ����������������򲻶�ѭ��
		else {
		    
			for(int num = 0; num<7;++num) {     //�ѳ�ʼ�������ĸ���
		   
		          pre[num] = cur[num];
		   }
		   
		}
	}
}

//�����ֵ
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

//�����С����
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

//ӡ����������ֵ������ʱʹ��
void printall(node* n) {

    for(int i =0;i<7;++i)
		cout<<n[i].w<<" "<<n[i].x<<" "<<n[i].y<<" "<<n[i].z<<endl;

}

//***********************************   for ����ֵ����    *******************************************End!

//***********************************   ����������I/O����������Ч��   ******************************Start!
//priceTxt��Ҫ��ȡ���ļ��Ķ�Ӧ��charֵ��price��Ҫ���������
void read(const char*priceTxt,double *priceArray ,int& numOfRecord){

	string stringArray[MaxNumberOfRecord];
	ifstream ftxt;
		ftxt.open(priceTxt);	  
		int i=0;
	     while(!ftxt.eof())    
	     { 
			 getline(ftxt,stringArray[i++]);
         }
		 //���i����NumberOfRecorder-1˵�����������������ɵ����ݵ��������ٶ��������
		 if(i>MaxNumberOfRecord){      //��i=NumberOfRecorder��˵���ﵽ�����������ɵ����ݵ����������NumberOfRecorder���̫С��
			 cout<<"���NumberOfRecorder���̫С��"<<endl;
			 exit(1);
		 }
		 numOfRecord=i;//��ʵ�ļ�¼����Ϊi(��Ϊi�Ǵ�������ģ�һֱ�ӵ�i-1)�������ǽ���ʵ������������ȥ
		 ftxt.close();		
		 --i;
		for(int k=0;i>=0;i--){//��β��ͷ����stringArray�е�stringת��ΪpriceArray�е�double,����˳��ǡǡ�෴���뵱ǰֵ������ֵ�������С
		 priceArray[k]=atof(stringArray[i].c_str());//���õ���stringת��Ϊdoubleֵ��д������
		 k++;
		}		
}
//����ǵļ۲�ĺ���
void getGap( double*priceArrayGap,int& numOfRecordGap){
	int numOfRecordOne;//��һ���ļ�����ʵ��¼��Ŀ
	int numOfRecordTwo;//�ڶ����ļ�����ʵ��¼��Ŀ

	const char *txtOne="qihuo.txt";//�ı��ļ�,��ŵ�һ�ݼ۸�ֵ
	const char* txtTwo="xianhuo.txt";//�ı��ļ�,��ŵڶ��ݼ۸�ֵ
	//const char* txtGap="xianhuo.txt";//�ı��ļ�,��ż۲�           �����뻹�ǲ�Ҫ����ˣ�����ֱ�����ڴ������������Ҫд���ļ�����۲�ֵ

	double priceArrayOne[MaxNumberOfRecord];//��һ���۸�����
	double priceArrayTwo[MaxNumberOfRecord];//�ڶ����۸�����

	read(txtOne,priceArrayOne,numOfRecordOne);//����һ���ı�price������ݶ���txtOne������
	read(txtTwo,priceArrayTwo,numOfRecordTwo);//����һ���ı�������ݶ���txtOne������
	//�Դˣ���ȡ�����ı��õ������ݽ��������������priceArrayOne��priceArrayTwo����������

	/*һ������if����Ϊ�˰�numOfRecordGapȡ��(numOfRecordOne��numOfRecordTwo)�и�С��һ��������������ֵΪ��ȣ�
	������һ�����Զ�������������ֵ(Ҳ����ʱ���ϸ������ʷ����)*/
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
		 priceArrayGap[i]=priceArrayOne[i]-priceArrayTwo[i];//	����۲�
		 }
	//��� priceArrayGap�����������Ҫ�ļ۲������ˣ���ôҪ��ȥ��ʲô����ȥ��ʲô�ɣ����ͣ������
		 for(int i=0;i<numOfRecordGap;i++)
			 if(priceArrayGap[i]==0)cout<<"�۲"<<priceArrayGap[i]<<endl;
}
//***********************************   ����������I/O����������Ч��   ******************************End!


//**********************************for rbf function's definition************************************start!

double randnumber()//-0.1~0.1��������� 
{
	double b = rand();
	return (sin(b)/ 10) ;
}

//��ĳ���ڵ�����ֵ
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

//��������u�ĵ���
double gf(int layer,int node){
	double sum=0;
for (int m=0;m<N[layer+1];m++)
sum=sum+e[layer+1][m]*u[layer+1][m]*(1-u[layer+1][m])*w[layer+1][m][node];
return sum;
}


//��ǰ��
void feed_forward()//ǰ�����ֵ
{
for (int layer=1;layer<=2;layer++)
 for(int node=0;node<N[layer];node++)
	 u[layer][node]=f(node,layer);    //node = 0~8,layer=1~2
}


//���ݶ�
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


//�޸�Ȩֵ
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
double erro()//���
{
	double sum=0;
	
 sum=pow(fabs(u[2][0]-output[0]),2)+pow(fabs(u[2][1]-output[1]),2)+pow(fabs(u[2][2]-output[2]),2);
			
return sum;
}


//////////��ʼ����װ������
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
	//////////////////////���˶���һ�����������ݣ�input,output��;
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


//�ļ���λ
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


//////////////////�������////////////
void add_vector(double sum_x[N_BUF],float input[N_BUF])   //�������
{
	for (int i=1;i<N_BUF;i++)
		sum_x[i]+=input[i];
}
void sub_vector(double sum_x[N_BUF],float input[N_BUF])  //�������
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
double vector_width(double *sum_x)  //�������
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


//�������ز����
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

//ѵ���ڶ���Ȩֵ
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
//rbfѵ��
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

//�õ�rbf���
double rbf_result(PriceList list)
{
//	clock_t t1  = clock();
	for (int i0=0;i0<max_layer;i0++)
		for (int i1=0;i1<max_node;i1++)
			for (int i2=0;i2<max_node;i2++)
				w[i0][i1][i2]=randnumber();

//rbfѵ��
//�����һ���ַ�����Ҫ��ʱ��
	FILE * fp_test=fopen("gapforrbf.txt","r");
	if (fp_test==NULL){
		cout<<"cannot open file"<<endl;      
		exit(0);
	}
	clock_t t_test1=clock();
	while(!(feof(fp_test))&&(fgetc(fp_test)!='\n'));   //���ļ���ʱ��
	clock_t t_test2=clock();
	if (t_test2==clock_t(-1)){
		cerr<<"sorry ,no clock\n";
		exit(1);
	}  
	double file_time_each=double(t_test2-t_test1)/CLOCKS_PER_SEC/EACH_LINE_OFFSET*100000000 ;
	fclose(fp_test);
	wucha=1;
	k_k=-1;
	fpwucha=fopen("rbf���\\���.txt","w");
	test();      //ѧϰ	
	clock_t   t11=clock(); //	��ʼʱ�� 
	fp=fopen("gapforrbf.txt","r");
	if (fp==NULL){
		cout<<"cannot open file"<<endl;      
		exit(0);
	}
	while(wucha>EP)		//����������ֵʱ��ѭ������
	{   
		if (feof(fp)) rewind(fp);
		for(int i=1;i<N_BUF;i++)
			fscanf(fp,"%f",input+i);
		for(int i=0;i<O_BUF;i++)
			fscanf(fp,"%d",output+i);
		compute_u1();  //�������ز����
		lms();   //ѵ����2��Ȩֵ
		wucha=0.0;
		wucha=erro(); //����µ����
		k_k=(++k_k)%7;
		if(!k_k)  fprintf(fpwucha,"\n");
		fprintf(fpwucha,"%16f",wucha);
		rbf_test_time++;
	}
	//����ʱ��
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

//д�����
void gap_write(ofstream ut,int start,int end)  
{
	for(;start < end;++start)
	{
		ut<<priceArrayGap[start]<<" ";
	}
	ut<<priceArrayGap[end-1]<<" "<<priceArrayGap[end]<<" "<<priceArrayGap[end+1]<<"/n";
}

//д��rbf��Ԥ��ֵ��Ҫ������
void gapforrbf_write(PriceList list)   //д���ļ����ֳɶ����
{
	/*
	ofstream fout("gapforrbf.txt");

	int length = sizeof(priceArrayGap)/sizeof(double); //����length = 200

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

	int length = sizeof(priceArrayGap)/sizeof(double); //����length = 200

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

//д��rbf��ѧϰʱ������
void fortest_write(PriceList list)//д������ļ���������ʼ����ѧϰ
{
	ofstream fout("forrbftest.txt");

	int length = sizeof(priceArrayGap)/sizeof(double); //����length = 200

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

//rbf�������㷨���ֵ
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