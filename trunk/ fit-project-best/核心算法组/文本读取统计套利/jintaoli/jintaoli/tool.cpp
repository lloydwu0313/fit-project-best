#include"fit.h"

//���ı��ж�ȡ���ݵ������У�priceTxt��Ҫ��ȡ���ļ��Ķ�Ӧ��charֵ��price��Ҫ���������
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
		for(;i>=0;i--){//�Ҹ��ˣ����Ǵ�ͷ��β�ɣ�����
		 priceArray[i]=atof(stringArray[i].c_str());//���õ���stringת��Ϊdoubleֵ��д������
		}		
}

//������۲����У�������۲�����
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
	//��� priceArrayGap�����������Ҫ�ļ۲������ˣ���ôҪ��ȥ��ʲô��ȥ��ʲô�ɣ����ͣ������
	//	 for(int i=0;i<numOfRecordGap;i++)cout<<priceArrayGap[i]<<endl;  //������еļ۲�����
}

double getavg(double *g,int n){//n�Ǹ������ݸ���������ǰ������g����������
	 double tmp=0;
	 for(int i=0;i<n;i++ )
		 tmp+=g[i];
	// cout<<"��ֵΪ��"<<tmp/((double)n)<<endl;
	 return tmp/((double)n);
}
 double getstd(double*g ,int n){
	 double tep=0;
	 double a=getavg(g,n);
	 for(int i=0;i<n;i++)
	 tep+=(a-g[i])*(a-g[i]);
	//  cout<<"��׼��Ϊ��"<<(sqrt(tep/((double)n)))<<endl;
	 return sqrt(tep/((double)n));

 }