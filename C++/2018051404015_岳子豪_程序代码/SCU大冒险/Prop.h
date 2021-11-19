#ifndef PROP_H
#define PROP_H

#include"Base.h"
using namespace win;

class Prop :public Base
{
protected:
	int isexist;//是否存在

public:
	Prop(int x, int y, int speedx, int speedy, int width, int heigth, WINSIZE winsize, string iconimage);
	Prop(Prop &prop);
	int getexist();
	void setexist(int exist = 0);
	void move();
	void move(int key);
	virtual ~Prop();
};

#endif