#include"bp_rbf.h"
 double randnumber()//-0.1~0.1的随机函数 
{
	return (sin(rand())/10) ;
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
	 
	for(i=0;i<O_BUF;i++)
       fscanf(fp,"%d",output+i);
//	cout<<input[2]<<endl<<output[2]<<endl;
	//////////////////////至此读出一个样本的数据（input,output）;
	for (int m=0;m<N_BUF;m++)
    u[0][m]=input[m];
				

}
void test()
{

FILE *fpt=NULL;
fpt=fopen("test.txt","r");
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
		count++;
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
  		
	fp=fopen("study.txt","r");
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
		  cout<<*(theita[i])<<"theita[]i   "<<i<<endl;
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
	count=0;
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
fp_test=fopen("test.txt","r");
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
		count++;
	}

}
 fclose(fp_test);
}
////////////////////////主函数入口//////////////////////////
int main()
{  
	
////////////////////////对权值初始化为-0.1~0.1之间的随机数
//	FILE * fpw=NULL;
//	if (!(fpw=fopen("权值.txt","r")))
	{
	for (int i0=0;i0<max_layer;i0++)
		for (int i1=0;i1<max_node;i1++)
			for (int i2=0;i2<max_node;i2++)
				w[i0][i1][i2]=randnumber();
	}
/*else /////////////读取权值
	{
	for ( int i0=0;i0<max_layer;i0++)
		for ( int i1=0;i1<max_node;i1++)
			for ( int i2=0;i2<max_node;i2++)
			{
		    fseek(fpw,11L,1);
			fscanf(fpw,"%f",&w[i0][i1][i2]);
			}
	
	fclose(fpw);
}*/
 	fpwucha=fopen("bp结果\\误差变化序列.txt","w");
 if (!fpwucha){
	 cout<<"cannot open file"<<endl;
	 exit(0);
 }
			////////////////////////////
			//打开训练文件
		fp=fopen("study.txt","r");
	if (fp==NULL){
				 cout<<"cannot open file"<<endl;
	 exit(0);
	}
printf("正在训练,请稍等.......");
	//开始时钟
	clock_t t1=clock();
	if (t1==clock_t(-1)){
		cerr<<"sorry ,no clock\n";
		exit(1);
	}
   while (wucha>EP){
		initial(fp);
		feed_forward();
	    compute_gradient();
        update_weights();
		wucha+=erro();
		bp_test_time++;
   }
	//结束时钟
      clock_t t2=clock();
	if (t2==clock_t(-1)){
		cerr<<"sorry ,no clock\n";
		exit(1);
	}  
	cout<<"\r"<<"        训练结束!            "<<endl;
	cout<<"训练时间为"<<"     "<<double(t2-t1)/CLOCKS_PER_SEC<<"seconds"<<"(这个时间包括了读写文件时间，可以精确)"<<endl;
    cout<<"训练轮数为   "<<bp_test_time<<endl;
   	fclose(fp);
    fclose(fpwucha);
/////////////测试样例
		test();
		cout<<"识别个数为"<<count<<endl<<"识别率为"<<(count/TEST_SETS)*100<<"%"<<"最后误差为"<<wucha<<endl;

		FILE *fp2;//保存权值
		fp2=fopen("bp结果\\权值.txt","w");
		if (fp2==NULL)
		{
			exit(0);
		}
	for (int layer=1;layer<=2;layer++)
	    	for (int j=0;j<N[layer];j++)
		    	for(int i=0;i<N[layer-1];i++)
					fprintf(fp2,"w[%d][%d][%d]=%f\n",layer,j,i,w[layer][j][i]);
				fclose(fp2);
////////////////////////////////////////rbf训练////////////////////
				//////////计算读一行字符所需要的时间///////
			FILE * fp_test=fopen("study.txt","r");
				if (fp_test==NULL){
				 cout<<"cannot open file"<<endl;      
	             exit(0);
				}
				clock_t t_test1=clock();
				while(!(feof(fp_test))&&(fgetc(fp_test)!='\n'));
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
			   fp=fopen("study.txt","r");
            	if (fp==NULL){
				 cout<<"cannot open file"<<endl;      
	             exit(0);
				}
				while(wucha>EP)
				{   
					if (feof(fp)) rewind(fp);
                   for(int i=1;i<N_BUF;i++)
	             	fscanf(fp,"%f",input+i);
	               	for(i=0;i<O_BUF;i++)
                   fscanf(fp,"%d",output+i);
				 	compute_u1();
					lms();
                	wucha=0.0;
					wucha=erro();
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
	cout<<"训练时间为"<<"     "<<double(t22-t11)/CLOCKS_PER_SEC-file_time*file_time_each<<"seconds"<<"(这个时间包括了读写文件时间，可以精确)"<<endl;
	  cout<<"训练轮数为   "<<rbf_test_time<<endl;
				fclose(fp);
               rbf_test();
			   cout<<count<<"   "<<endl;
			   	FILE *fp2_rbf;//保存权值
		fp2_rbf=fopen("rbf结果\\权值.txt","w");
		if (fp2_rbf==NULL)
		{
			exit(0);
		}
		
	for ( layer=1;layer<=2;layer++)
	    	for (int j=0;j<N[layer];j++)
		    	for(int i=0;i<N[layer-1];i++)
					fprintf(fp2_rbf,"w[%d][%d][%d]=%f\n",layer,j,i,w[layer][j][i]);
				fclose(fp2_rbf);
  	
		return 0;
}