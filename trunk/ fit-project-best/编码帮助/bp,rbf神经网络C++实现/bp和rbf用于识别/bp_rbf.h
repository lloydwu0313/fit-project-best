#include<iostream.h>
#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>
#include<math.h>
#include<string>
#include<time.h>
static char* p,**endpoint;
#define VAL(p) strtod(p,endpoint)
#define N_BUF (100+1)//����ά��  +1
#define O_BUF 3 //���ά��
#define C_BUF 9//���ز�ά��  +1
#define STUDY_SPEED 0.3 //ѧϰ����
#define ALPHA    0.2 //�嶯ϵ��
#define EP       0.02 //�����ֵ
#define EP2      0.1
#define TEST_SETS   8
#define  EACH_LINE_OFFSET   40
static int N[3]={N_BUF,C_BUF,O_BUF};//����Ľڵ���
#define max_layer   3 //�������
#define max_node   (100+1) //�������Ľڵ���
static double u[max_layer][max_node];//ĳһ��ĳһ�ڵ�����
static double w[max_layer][max_node][max_node];//Ȩֵ
static double temp_w_1[max_layer][max_node][max_node];//w(k-1)
static double temp_w_2[max_layer][max_node][max_node];//w(k)
static double delta_w[max_layer][max_node][max_node];//�嶯����
static double wucha=1;//���
double gaosi_yinzi[C_BUF];//��˹�˺�����״����
static	int * theita[C_BUF];//����ÿһȨ�����ľ��༯��
float input[N_BUF];//��������
int  output[O_BUF];//�������
int count=0;//������ȷ��������
int k_k=-1;
	FILE *fp;//�ļ�
	FILE *	fpwucha;
double e[max_layer][max_node];//������u�ĵ���
double g[max_layer][max_node][max_node];//������w�ĵ��������ݶ�
int file_time=0;
int rbf_test_time=0;
int bp_test_time=0;