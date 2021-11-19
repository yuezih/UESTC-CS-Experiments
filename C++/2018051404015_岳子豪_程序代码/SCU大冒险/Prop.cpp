#include"Prop.h"

Prop::Prop(int x, int y, int speedx, int speedy, int width, int heigth, WINSIZE winsize, string iconimage)
:Base(x,y,speedx,speedy,width,heigth,winsize,iconimage)
{
	isexist = 0;
}
Prop::Prop(Prop &prop)
:Base(prop.x,prop.y,prop.speedx,prop.speedy,prop.width,prop.heigth,prop.Winsize,prop.iconimage)
{
	this->isexist = prop.isexist;
}
int Prop::getexist()
{
	return isexist;
}
void Prop::setexist(int exist)
{
	this->isexist = exist;
	if (exist == 1)
	{
		this->x = rand() % (Winsize.getwidth() - width);
		this->y = 0 - heigth;
	}
}
void Prop::move()
{
	if (this->isexist == 0)
		return;
	this->y += speedy;
	this->x += speedx;
	if (this->x < 0)
	{
		this->x = 0;
		speedx *= -1;
	}
	if (this->x > Winsize.getwidth() - width)
	{
		this->x = Winsize.getwidth() - width;
		speedx *= -1;
	}
	if (this->y > Winsize.getheight())
	{
		this->isexist = 0;
	}
}
void Prop::move(int key)
{}
Prop::~Prop()
{}