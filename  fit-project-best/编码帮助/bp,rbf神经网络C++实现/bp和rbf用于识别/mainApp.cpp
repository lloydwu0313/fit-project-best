#include"bp_rbf.h"
 double randnumber()//-0.1~0.1��������� 
{
	return (sin(rand())/10) ;
}
//��ĳ���ڵ�����ֵ
double f(int node,int layer){
	double sum=0;
for (int node2=0;node2<N[layer-1];node2++)
{
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
	 u[layer][node]=f(node,layer);
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
	 
	for(i=0;i<O_BUF;i++)
       fscanf(fp,"%d",output+i);
//	cout<<input[2]<<endl<<output[2]<<endl;
	//////////////////////���˶���һ�����������ݣ�input,output��;
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
//////////////RBF�Ķ���///////////////////////////////////////////////////////////////////////
/////////////////////�ļ���λ////////////////////////////////
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
 ////�������/////
void min_juli(int *set_number)//*j���������������ڵľ���
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
//////////////////�������////////////
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
//////////////k��ֵ�㷨//////////////////////
void k_means()
{
	size_t size;
//	double  _min_change;
//	double  _min_max_change=0.0;
	double   min_change=10.0;//�������ε����ı仯
	int set_number=0;//�������
for (int i0=0;i0<max_layer;i0++)
		for (int i1=0;i1<max_node;i1++)
			for (int i2=0;i2<max_node;i2++)
		
				w[i0][i1][i2]=randnumber();//��ʼ��Ȩֵ
			
 
	 for(int ii=1;ii<C_BUF;ii++)
		theita[ii]=NULL;//ָ���ʼ��
  		
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
		int   set_member_number=0;//ĳһ�������������������
while (min_change>EP2)//��ѭ��
{  
	set_member_number=0;

	while (!feof(fp))
	{   
		//fseek(fp,(long)(set_member_number*EACH_LINE_OFFSET),0);
	    for(int i=1;i<N_BUF;i++)
		fscanf(fp,"%f",input+i);//����һ����������
		while (!feof(fp)&&(fgetc(fp)!='\n'));
	  min_juli(&set_number); //����һ��������С�ľ������
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
             /////��������//////////

  	for(int i=1;i<C_BUF;i++)
	{ 
		  cout<<*(theita[i])<<"theita[]i   "<<i<<endl;
		if(theita[i])
		{
       size=_msize(theita[i]);
	 			int m=size/4;//�þ�����Ԫ�ظ���	
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
    if(min_change>EP2)//����û�н�����������˴ξ��༯��
			{
	
  for(int i_free=0;i_free<C_BUF;i_free++)
	  free(theita[i_free]);
  for(int ii=0;ii<C_BUF;ii++)
		theita[ii]=NULL;//ָ���ʼ��
	}
 
}//////////��ѭ������////////////


	/////////����˺�������״����////////////////////
	for(int ss=0;ss<C_BUF;ss++)//��˹���ӳ�ʼ��
		gaosi_yinzi[ss]=0.0;
	
   for(int j=1;j<C_BUF;j++)//��ʼ����
   {   
	  
	   if(theita[j]) 
	   {
	        size=_msize(theita[j]);
			int m=size/4;//�þ�����Ԫ�ظ���	
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
/////////////////////////�������ز����/////////////
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
////////////////////////////ѵ���ڶ���Ȩֵ/////////////////////////
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
////////////////////////���������//////////////////////////
int main()
{  
	
////////////////////////��Ȩֵ��ʼ��Ϊ-0.1~0.1֮��������
//	FILE * fpw=NULL;
//	if (!(fpw=fopen("Ȩֵ.txt","r")))
	{
	for (int i0=0;i0<max_layer;i0++)
		for (int i1=0;i1<max_node;i1++)
			for (int i2=0;i2<max_node;i2++)
				w[i0][i1][i2]=randnumber();
	}
/*else /////////////��ȡȨֵ
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
 	fpwucha=fopen("bp���\\���仯����.txt","w");
 if (!fpwucha){
	 cout<<"cannot open file"<<endl;
	 exit(0);
 }
			////////////////////////////
			//��ѵ���ļ�
		fp=fopen("study.txt","r");
	if (fp==NULL){
				 cout<<"cannot open file"<<endl;
	 exit(0);
	}
printf("����ѵ��,���Ե�.......");
	//��ʼʱ��
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
	//����ʱ��
      clock_t t2=clock();
	if (t2==clock_t(-1)){
		cerr<<"sorry ,no clock\n";
		exit(1);
	}  
	cout<<"\r"<<"        ѵ������!            "<<endl;
	cout<<"ѵ��ʱ��Ϊ"<<"     "<<double(t2-t1)/CLOCKS_PER_SEC<<"seconds"<<"(���ʱ������˶�д�ļ�ʱ�䣬���Ծ�ȷ)"<<endl;
    cout<<"ѵ������Ϊ   "<<bp_test_time<<endl;
   	fclose(fp);
    fclose(fpwucha);
/////////////��������
		test();
		cout<<"ʶ�����Ϊ"<<count<<endl<<"ʶ����Ϊ"<<(count/TEST_SETS)*100<<"%"<<"������Ϊ"<<wucha<<endl;

		FILE *fp2;//����Ȩֵ
		fp2=fopen("bp���\\Ȩֵ.txt","w");
		if (fp2==NULL)
		{
			exit(0);
		}
	for (int layer=1;layer<=2;layer++)
	    	for (int j=0;j<N[layer];j++)
		    	for(int i=0;i<N[layer-1];i++)
					fprintf(fp2,"w[%d][%d][%d]=%f\n",layer,j,i,w[layer][j][i]);
				fclose(fp2);
////////////////////////////////////////rbfѵ��////////////////////
				//////////�����һ���ַ�����Ҫ��ʱ��///////
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
			fpwucha=fopen("rbf���\\���.txt","w");
			//	��ʼʱ��
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
              //����ʱ��
          clock_t    t22=clock();
			  	if (t22==clock_t(-1)){
		cerr<<"sorry ,no clock\n";
		exit(1);
	}  
				fclose(fpwucha);
//	cout<<"\r"<<"        ѵ������!            "<<endl;
	cout<<"ѵ��ʱ��Ϊ"<<"     "<<double(t22-t11)/CLOCKS_PER_SEC-file_time*file_time_each<<"seconds"<<"(���ʱ������˶�д�ļ�ʱ�䣬���Ծ�ȷ)"<<endl;
	  cout<<"ѵ������Ϊ   "<<rbf_test_time<<endl;
				fclose(fp);
               rbf_test();
			   cout<<count<<"   "<<endl;
			   	FILE *fp2_rbf;//����Ȩֵ
		fp2_rbf=fopen("rbf���\\Ȩֵ.txt","w");
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