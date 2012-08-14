//main.cpp
#include"fit.h"

int main()
{
	double priceArrayGap[MaxNumberOfRecord];//价差数组
	int numOfRecordGap;//最终的价差数组的真实记录数目，gap是差的意思
	getGap(priceArrayGap,numOfRecordGap);
	//这个 priceArrayGap数组就是我们要的价差数组了，//就凭着这个价差数组，就能做出交易决策了.大家要拿去做什么就拿去做什么吧，加油，伙伴们!!!!
	trade(priceArrayGap,numOfRecordGap); 
}