#include<iostream.h>
#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>
#include<math.h>
#include<string>
#include<time.h>
static char* p,**endpoint;
#define VAL(p) strtod(p,endpoint)
#define N_BUF (100+1)//输入维数  +1
#define O_BUF 3 //输出维数
#define C_BUF 9//隐藏层维数  +1
#define STUDY_SPEED 0.3 //学习速率
#define ALPHA    0.2 //冲动系数
#define EP       0.02 //误差阈值
#define EP2      0.1
#define TEST_SETS   8
#define  EACH_LINE_OFFSET   40
static int N[3]={N_BUF,C_BUF,O_BUF};//各层的节点数
#define max_layer   3 //网络层数
#define max_node   (100+1) //层中最大的节点数
static double u[max_layer][max_node];//某一层某一节点的输出
static double w[max_layer][max_node][max_node];//权值
static double temp_w_1[max_layer][max_node][max_node];//w(k-1)
static double temp_w_2[max_layer][max_node][max_node];//w(k)
static double delta_w[max_layer][max_node][max_node];//冲动向量
static double wucha=1;//误差
double gaosi_yinzi[C_BUF];//高斯核函数形状因子
static	int * theita[C_BUF];//对于每一权向量的聚类集合
float input[N_BUF];//输入向量
int  output[O_BUF];//输出向量
int count=0;//测试正确样本数量
int k_k=-1;
	FILE *fp;//文件
	FILE *	fpwucha;
double e[max_layer][max_node];//误差函数对u的导数
double g[max_layer][max_node][max_node];//误差函数对w的导数，即梯度
int file_time=0;
int rbf_test_time=0;
int bp_test_time=0;