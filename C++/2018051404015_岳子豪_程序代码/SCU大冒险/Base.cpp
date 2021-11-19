#include"Base.h"
using namespace win;

WINSIZE::WINSIZE(int winwidth, int winheight)
{
	this->winwidth = winwidth;
	this->winheight = winheight;
}
WINSIZE::WINSIZE(WINSIZE &winsize)
{
	winwidth = winsize.winwidth;
	winheight = winsize.winheight;
}
int WINSIZE::getwidth()
{
	return winwidth;
}
int WINSIZE::getheight()
{
	return winheight;
}
void WINSIZE::setwidth(int width)
{
	winwidth = width;
}
void WINSIZE::setheight(int height)
{
	winheight = height;
}
void WINSIZE::setsize(int width, int height)
{
	winwidth = width;
	winheight = height;
}
WINSIZE::~WINSIZE()
{}


Base::Base(int x, int y, int speedx, int speedy, int width, int heigth, WINSIZE winsize, string iconimage)
:Winsize(winsize)
{
	this->x = x;
	this->y = y;
	this->speedx = speedx;
	this->speedy = speedy;
	this->width = width;
	this->heigth = heigth;
	this->img = img;
	this->iconimage = iconimage;
	loadImage(this->iconimage.c_str(), &img);
}
Base::Base(Base &base):Winsize(base.Winsize)
{
	this->x = base.x;
	this->y = base.y;
	this->speedx = base.speedx;
	this->speedy = base.speedy;
	this->width = base.width;
	this->heigth = base.heigth;
	this->img = base.img;
	this->iconimage = base.iconimage;
	this->img = base.img;
}
bool Base::collision(Base *base)
{
	if (x < base->x && x + width > base->x)
	{
		if (y < base->y && y + heigth > base->y)
			return true;
		if (y > base->y && y < base->y + base->heigth)
			return true;
	}
	if (x > base->x && x < base->x + base->width)
	{
		if (y < base->y && y + heigth > base->y)
			return true;
		if (y > base->y && y < base->y + base->heigth)
			return true;
	}
	return false;
}
int Base::getx() const
{
	return this->x;
}
int Base::gety() const
{
	return this->y;
}
int Base::getwidth() const
{
	return this->width;
}
int Base::getheight() const
{
	return this->heigth;
}
void Base::paint()
{
	beginPaint();
	putImageTransparent(&img, x, y, width, heigth, WHITE);
	endPaint();
}
Base::~Base()
{}