#ifndef UESTC_H
#define UESTC_H

#include"Base.h"
#include"SCU.h"
using namespace win;

const int BORDER = 300;

class UESTC :public Base
{
protected:
	int life;//是否出现
	double scu_distance;//与用户（川大）距离
	void distance(SCU &scu);//计算距离
	int is_speedup;//追捕速度
	void overboundary(); //越界处理

public:
	UESTC(int x, int y, int speedx, int speedy, int width, int heigth, WINSIZE winsize, string iconimage);
	UESTC(UESTC &uestc);
	int islife();
	void setlife(int life);
	void move(SCU &scu);
	void move();
	void move(int key);
	virtual ~UESTC();
};

#endif