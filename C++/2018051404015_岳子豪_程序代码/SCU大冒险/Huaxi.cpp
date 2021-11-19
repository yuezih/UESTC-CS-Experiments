#include"Huaxi.h"
#include<math.h>

Huaxi::Huaxi(int x, int y, int speedx, int speedy, int width, int heigth, WINSIZE winsize, string iconimage,
	 int rank, int life)
	 :STUCD(x,y,speedx,speedy,width,heigth,winsize,iconimage,rank,life)
	{
		this->scu_distance = Winsize.getheight();
		this->is_speedup = 0;
	}
Huaxi::Huaxi(Huaxi &huaxi)
:STUCD(huaxi.x,huaxi.y,huaxi.speedx,huaxi.speedy,huaxi.width,huaxi.heigth,huaxi.Winsize,huaxi.iconimage,huaxi.rank,huaxi.life)
{
	this->scu_distance = huaxi.scu_distance;
	this->is_speedup = huaxi.is_speedup;
}
void Huaxi::distance(SCU &scu)
{
	double dx, dy;
	x += speedx;
	y += speedy;
	dx = pow(x + width / 2.0 - (scu.getx() + scu.getwidth() / 2.0), 2);
	dy = pow(y + heigth / 2.0 - (scu.gety() + scu.getheight() / 2.0), 2);
	this->scu_distance = sqrt(dx + dy);
}
void Huaxi::move(SCU &scu)
{
	distance(scu);
	//与用户精灵距离小于设定值时加速躲避
	if (this->scu_distance <= border && is_speedup == 0)
	{
		if (x < scu.getx())
		{
			if (speedx > 0)
				speedx *= -1.5;
			else
				speedx *= 1.5;
		}
		if (x >= scu.getx())
		{
			if (speedx < 0)
				speedx *= -1.5;
			else
				speedx *= 1.5;
		}
		if (y < scu.gety())
		{
			if (speedy > 0)
				speedy *= -1.5;
			else
				speedy *= 1.5;
		}
		if (y >= scu.gety())
		{
			if (speedy < 0)
				speedy *= -1.5;
			else
				speedy *= 1.5;
		}
		is_speedup = 1;
	}
	//距离大于设定值时，恢复原速度
	if (this->scu_distance > border && is_speedup == 1)
	{
		speedx /= 1.5;
		speedy /= 1.5;
		is_speedup = 0;
	}
	overboundary();
}
void Huaxi::overboundary()
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
void Huaxi::move()
{}
void Huaxi::move(int key)
{}
Huaxi::~Huaxi()
{}