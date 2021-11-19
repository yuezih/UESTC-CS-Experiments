#include"SCU.h"

SCU::SCU(int x, int y, int speedx, int speedy, int width, int heigth, WINSIZE winsize, string iconimage, int chance)
:Base(x,y,speedx,speedy,width,heigth,winsize,iconimage)
{
		get_rank = 100;
		isbreak = 0;
		this->chance = chance;
		if (chance < 0)
			this->chance = 0;	
}
SCU::SCU(SCU &scu)
:Base(scu.x,scu.y,scu.speedx,scu.speedy,scu.width,scu.heigth,scu.Winsize,scu.iconimage)
{
	this->chance = scu.chance;
	this->get_rank = scu.get_rank;
	this->isbreak = scu.isbreak;
}
void SCU::setxy(int x, int y)
{
	this->x = x;
	this->y = y;
}
int SCU::getrank()
{
	return get_rank;
}
void SCU::setrank(int addrank)
{
	if(getrank()+addrank>1) get_rank += addrank;
	else get_rank=1;
}
int SCU::getchance()
{
	return chance;
}

void SCU::addchance()
{
	try
	{
		this->chance++;
		if(this->chance>chancemax)
			throw(chance);
	}
	catch(int h)
	{
		this->chance = chancemax;
	}	
}
void SCU::reducechance()
{
	try
	{
		this->chance--;
		if(this->chance < 0)
			throw(chance);
	}
	catch(int)
	{
		this->chance = 0;
	}
	
}
void SCU::setspeed(double multiple,bool mul_or_div)
{
try
{
	if (multiple <= 0)
		throw(multiple);
	if (mul_or_div == true)
	{
		speedx *= multiple;
		speedy *= multiple;
	}
	if (mul_or_div == false)
	{
		speedx /= multiple;
		speedy /= multiple;
	}
}
catch(double)
{
	multiple = 1.5;
}

}
int SCU::getbreak()
{
	return isbreak;
}
void SCU::setbreak(int isbreak)
{
	this->isbreak = isbreak;
}
void SCU::move(){}
void SCU::move(int key)
{
	switch (key)
	{
	case VK_UP:
		y -= speedy;
		if (y < 0)
			y = 0;
		break;
	case VK_DOWN:
		y += speedy;
		if(y+heigth>Winsize.getheight())
			y = Winsize.getheight() - heigth;
		break;
	case VK_LEFT:
		x -= speedx;
		if(x<0)
			x = 0;
		break;
	case VK_RIGHT:
		x += speedx;
		if(x+width>Winsize.getwidth())
			x = Winsize.getwidth() - width;
		break;
	}
}
SCU::~SCU()
{}