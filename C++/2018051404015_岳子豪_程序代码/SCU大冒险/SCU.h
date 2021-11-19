#ifndef SCU_H
#define SCU_H

#define chancemax 5

#include"Base.h"
using namespace win;

class SCU :public Base
{
protected:
	int isbreak; //是否停滞标志
	int get_rank;//当前排名
	int chance;
public:
	SCU(int x, int y, int speedx, int speedy, int width, int heigth, WINSIZE winsize, string iconimage, int chance=5);
	SCU(SCU &scu);
	void setxy(int x, int y);//设置位置
	int getrank();
	void setrank(int addrank);//排名改变（增量为负）
	int getchance();
	void addchance();//增加机会（生命值）
	void reducechance();//减少机会（生命值）
	void setspeed(double multiple,bool mul_or_div);//速度提升/降低multiple倍
	int getbreak();//是否停滞
	void setbreak(int isbreak = 1);//设置停滞
	void move();
	void move(int key);
	virtual ~SCU();
};

#endif