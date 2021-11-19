#include"UESTC.h"
#include<math.h>

UESTC::UESTC(int x, int y, int speedx, int speedy, int width, int heigth, WINSIZE winsize, string iconimage)
	 :Base(x,y,speedx,speedy,width,heigth,winsize,iconimage)
	{
		this->life = 0;
		scu_distance = Winsize.getheight();
		is_speedup = 0;
	}
UESTC::UESTC(UESTC &uestc)
	:Base(uestc.x,uestc.y,uestc.speedx,uestc.speedy,uestc.width,uestc.heigth,uestc.Winsize,uestc.iconimage)
{
	this->life = uestc.life;
	this->scu_distance = uestc.scu_distance;
	this->is_speedup = uestc.is_speedup;
}
int UESTC::islife()
{
	return this->life;
}
void UESTC::setlife(int life)
{
	this->life = life;
}
void UESTC::distance(SCU &scu)
{
	double dx, dy;
	dx = pow(x + width / 2.0 - (scu.getx() + scu.getwidth() / 2.0), 2);
	dy = pow(y + heigth / 2.0 - (scu.gety() + scu.getheight() / 2.0), 2);
	this->scu_distance = sqrt(dx + dy);
	
}
void UESTC::move(SCU &scu)
{
	x += speedx;
	y += speedy;
	distance(scu);
	//远距离快速追捕
	if (this->scu_distance <= BORDER && is_speedup == 0)
	{
		if (x < scu.getx()){
			if (speedx < 0)	speedx *= -1.5;
			else speedx *= 1.5;
		}
		else {
			if (speedx > 0)	speedx *= -1.5;
			else speedx *= 1.5;
		}
		if (y<scu.gety()){
			if (speedy < 0) speedy *= -1.5;
			else speedy *= 1.5;
		}
		else {
			if (speedy > 0)	speedy *= -1.5;
			else speedy *= 1.5;
		}
		is_speedup = 1;
	}
	//近距离慢速追捕
	if (scu_distance > BORDER && is_speedup == 1)
	{
		speedx /= 1.5;
		speedy /= 1.5;
		is_speedup = 0;
	}
	overboundary();
}
void UESTC::overboundary()
{
	if (x + width < 0 && speedx < 0)
	{ 
		x = Winsize.getwidth();
		y = Winsize.getheight() - heigth - y;
	}
	if (x > Winsize.getwidth() && speedx > 0)
	{
		x = -width;
		y = Winsize.getheight() - heigth - y;
	}
	if (y + heigth < 0 && speedy < 0)
	{
		y = Winsize.getheight();
		x = Winsize.getwidth() - width - x;
	}

	if (y > Winsize.getheight() && speedy > 0)
	{
		y = -heigth;
		x = Winsize.getwidth() - width - x;
	}
}
void UESTC::move()
{}
void UESTC::move(int key)
{}
UESTC::~UESTC()
{}
