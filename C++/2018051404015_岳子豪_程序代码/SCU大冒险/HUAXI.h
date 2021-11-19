#ifndef Huaxi_H
#define Huaxi_H

#include"STUCD.h"
#include"SCU.h"

#define border 150

class Huaxi :public STUCD
{
protected:
	int is_speedup;  //是否加速
	int scu_distance;  //与用户（川大）距离
	void distance(SCU &scu);//计算与用户精灵的距离
	void overboundary();  //越界处理

public:
	Huaxi(int x, int y, int speedx, int speedy, int width, int heigth, WINSIZE winsize, string iconimage, int rank, int life =0);
	Huaxi(Huaxi &huaxi);
	void move(SCU &scu);
	void move();
	void move(int key);
	virtual ~Huaxi();
};

#endif