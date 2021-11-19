#ifndef STUCD_H
#define STUCD_H

#include"Base.h"
using namespace win;

class STUCD :public Base
{
protected:
	int rank; //增益
	int life; //存活标志
public:
	STUCD(int x, int y, int speedx, int speedy, int width, int heigth, WINSIZE winsize, string iconimage,
	 int rank, int life = 0);
	STUCD(STUCD &stucd);
	int getrank();
	bool islife();//判断是否存活
	void setlife(int life);
	void move();
	void move(int key);
	virtual ~STUCD();
};

#endif