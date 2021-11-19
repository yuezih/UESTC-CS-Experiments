#include"Huaxi.h"

STUCD::STUCD(int x, int y, int speedx, int speedy, int width, int heigth, WINSIZE winsize, string iconimage,
	 int rank, int life):Base(x,y,speedx,speedy,width,heigth,winsize,iconimage)
{
	this->rank = rank;
	this->life = life;
}
STUCD::STUCD(STUCD &stucds)
:Base(stucds.x,stucds.y,stucds.speedx,stucds.speedy,stucds.width,stucds.heigth,stucds.Winsize,stucds.iconimage)
{
	this->rank = stucds.rank;
	this->life = stucds.life;
}
int STUCD::getrank()
{
	return this->rank;
}
bool STUCD::islife()
{
	return this->life;
}
void STUCD::setlife(int life)
{
	this->life = life;
}
void STUCD::move()
{
	if (this->life == 0)
		return;
	if (x < 0 || x + width > Winsize.getwidth())
	{
		speedx *= -1;
	}
	if (y < 0 || y + heigth > Winsize.getheight())
	{
		speedy *= -1;
	}
	x = x + speedx;
	y = y + speedy;
}
void STUCD::move(int key)
{}
STUCD::~STUCD()
{}