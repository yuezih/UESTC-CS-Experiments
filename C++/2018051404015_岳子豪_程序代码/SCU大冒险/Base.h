#ifndef BASE_H
#define BASE_H

#include"acllib.h"
#include<string>
using namespace std;

namespace win
{
class WINSIZE
{
	int winwidth;
	int winheight;
public:
	WINSIZE(int winwidth, int winheight);
	WINSIZE(WINSIZE &winsize);
	int getwidth();
	int getheight();
	void setwidth(int width);
	void setheight(int height);
	void setsize(int width, int height);
	virtual ~WINSIZE();
};
}

using namespace win;
class Base
{
protected:
	int x;
	int y;
	int speedx;
	int speedy;
	int width;
	int heigth;
	WINSIZE Winsize;
	string iconimage;
	ACL_Image img;

public:
	Base(int x, int y, int speedx, int speedy, int width, int heigth, WINSIZE winsize, string iconimage);
	Base(Base &base);
	void virtual move() = 0;
	void virtual move(int key) = 0;
	bool collision(Base *base);
	int getx() const;
	int gety() const;
	int getwidth() const;
	int getheight() const;
	void paint();
	virtual ~Base();
};


#endif