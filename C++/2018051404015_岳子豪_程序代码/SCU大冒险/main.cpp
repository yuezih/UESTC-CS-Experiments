// 本游戏设定纯属娱乐，系兄弟院校之间的调侃，绝无吹捧或抹黑之意。

// 游戏背景：四川大学是四川省排名第100的学校，但随着其不断发展，排名也在不断进步。
// 在本游戏中，如果四川大学的排名提升到第1，游戏结束，四川大学成为最好的学校；
// 如果四川大学在发展的过程中总是遇到强劲对手电子科技大学，就会受到挫败，次数一定时，游戏将结束，四川大学还有很长的路要走。

// STUCD：成都科技大学 四川大学合并1个成都科技大学可以前进3名
// Huaxi：华西医科大学 四川大学合并1个华西医科大学可以前进5名
// UESTC：电子科技大学 四川大学遭遇电子科技大学将会失去一次机会（生命值）
// SCU：四川大学 由用户控制
// Prop道具：
// 1.川大123周年校庆：如果机会有损，可以增加一次机会；
// 2.nature:电子科大封面Nature论文，将减慢川大前进的步伐（角色减速）；
// 3.science:川大发表science论文，角色加速；
// 4.cdxy：电子科技大学成都学院，将使川大停滞一段时间。

#include"acllib.h"
#include<time.h>
#include"Base.h"
#include"SCU.h"
#include"STUCD.h"
#include"Huaxi.h"
#include"UESTC.h"
#include"Prop.h"

//编号
#define chance 0
#define speedup 1
#define speeddown 2  
#define breakdown 3    
//初始化各类对象个数

//游戏结束标志变量
int is_over = 0;		

int stucd_mark = 0;		//生成成科个数
int huaxi_mark = 0;		//生成华西个数
int uestc_mark = 0;		//是否生成成电
int prop_mark = 0;		//是否生成道具

void Initspirit();							//初始化各个类
void KeyboardEvent(int key, int event);		//输入处理
void TimerEvent(int ID);					//游戏进度与定时
void Paint();								//展示与绘制
void Collision_stucd_or_huaxi();			//排名变化类碰撞：合并成科/华西
void Collision_uestc_or_prop();				//其它效果类碰撞：遭遇电子科大/吃到道具
void PropTrigger(int prop);					//道具效果
void Gameover();

const int stucdnum = 20;  
const int huaxinum = 10; 
//成科尺寸
const int stucdwidth = 50;
const int stucdheight = 50;
//华西尺寸
const int huaxiwidth = 80;
const int huaxiheight = 80;
//川大尺寸
const int scuwidth = 100;
const int scuheight = 100;
//成电尺寸
const int uestcwidth = 120;
const int uestcheight = 120;
//道具尺寸
const int propwidth = 75;
const int propheight = 100;

WINSIZE winsize(800, 600);//窗口大小

Prop *prop[4] = {NULL};
STUCD *stucds[stucdnum] = {NULL};
Huaxi *huaxi[huaxinum] = {NULL};
UESTC *uestc = NULL;
SCU *scu = NULL;


int Setup()
{
	srand(time(NULL));
	//初始化窗口
	initWindow("Spirit_Game", DEFAULT, DEFAULT, winsize.getwidth(), winsize.getheight());
	Initspirit();
	registerKeyboardEvent(KeyboardEvent);
	registerTimerEvent(TimerEvent);
		
	startTimer(0, 1000);//每1s生成一个学校（精灵）
	startTimer(1, 100); //每0.1s刷新，形成移动效果
	return 0;
}

void Initspirit()
{
	int x, y;
	int speedx, speedy;
	for (int i = 0; i < stucdnum;i++)
	{
		x = rand() % (winsize.getwidth() - stucdwidth);
		y = rand() % (winsize.getheight() - stucdheight);
		speedx = rand() % 6 + 1;
		speedy = rand() % 6 + 1;
		stucds[i] = new STUCD(x, y, speedx, speedy, stucdwidth, stucdheight, winsize, ".\\picture\\STUCD.bmp", -3);
	}
	for (int i = 0; i < huaxinum; i++)
	{
		x = rand() % (winsize.getwidth() - huaxiwidth);
		y = rand() % (winsize.getheight() - huaxiheight);
		huaxi[i] = new Huaxi(x, y, 8, 8, huaxiwidth, huaxiheight, winsize, ".\\picture\\Huaxi.bmp", -5);
	}

	x = rand() % (winsize.getwidth() - uestcwidth);
	y = rand() % (winsize.getheight() - uestcheight);
	uestc = new UESTC(x, y, 8, 8, uestcwidth, uestcheight, winsize, ".\\picture\\UESTC.bmp");

	x = rand() % (winsize.getwidth() - scuwidth);
	y = rand() % (winsize.getheight() - scuheight);
	scu = new SCU(x, y, 10, 10, scuwidth, scuheight, winsize, ".\\picture\\SCU.bmp");

	string str;
	for (int i = 0; i < 4;i++)
	{
		switch (i)
		{
		case chance:
			str = ".\\picture\\celebration.bmp";
			break;
		case speedup:
			str = ".\\picture\\science.jpg";
			break;
		case speeddown:
			str = ".\\picture\\nature.jpg";
			break;
		case breakdown:
			str = ".\\picture\\breakdown.bmp";
			break;
		}
		x = rand() % (winsize.getwidth() - propwidth);
		prop[i] = new Prop(x, 0 - propheight, 9, 9, propwidth, propheight, winsize, str);
	}
}

void Collision_stucd_or_huaxi()
{
	int judge = 0;
	for (int i = 0; i < stucdnum;i++)
	{
		judge = 0;
		if(stucds[i]->islife() != 0)
		{
			judge = stucds[i]->collision(scu);
			if (judge == 1)
			{
				scu->setrank(stucds[i]->getrank());
				stucds[i]->setlife(0);
			}
		}
	}
	for (int i = 0; i < huaxinum;i++)
	{
		judge = 0;
		if(huaxi[i]->islife() != 0)
		{
			judge = huaxi[i]->collision(scu);
			if (judge == 1)
			{
				scu->setrank(huaxi[i]->getrank());
				huaxi[i]->setlife(0);
			}
		}
	}
}

void Collision_uestc_or_prop()
{
	int judge = 0;
	if(uestc->islife() == 1)
	{
		judge = 0;
		judge = scu->collision(uestc);
		if(judge == 1)
		{
			scu->reducechance();
			scu->setxy(rand() % (winsize.getwidth() - scu->getwidth()), rand() % (winsize.getheight() - scu->getheight()));
			if (scu->getchance() == 0) is_over = 1;
		}
	}
	for (int i = 0; i < 4;i++)
	{
		judge = 0;
		if(prop[i]->getexist() != 0)
		{
			judge = prop[i]->collision(scu);
			if(judge == 1)
			{
				PropTrigger(i);
				prop[i]->setexist(0);
			}
		}
	}
}

void KeyboardEvent(int key, int event)
{
	if(event != KEY_DOWN) return;
	//判断用户可移动条件（生命值不为0 && 游戏未结束 && 未被停滞）
	if (scu->getchance() != 0 && is_over == 0 && scu->getbreak() != 1)
	{
		scu->move(key);
		Collision_stucd_or_huaxi();
		Paint();
	}	
}

void TimerEvent(int ID)
{
	if(ID == 0)//生成学校精灵
	{
		if (stucd_mark < stucdnum)
		{
			stucds[stucd_mark]->setlife(1);
			stucd_mark++;
		}
		//排名进入前85，开始出现华西
		if (scu->getrank() <=85 && huaxi_mark < huaxinum)
		{
			huaxi[huaxi_mark]->setlife(1);
			if (huaxi_mark == 0)
				startTimer(2, 100);
			huaxi_mark++;
		}
		//排名进入前70，开始出现UESTC和道具
		if (scu->getrank() <=70)
			if(uestc_mark == 0)
			{
				uestc_mark = 1;
				uestc->setlife(1);
				startTimer(3, 150);
			}
			if(prop_mark == 0)
			{
				startTimer(4, 5000);
				startTimer(5, 100);
				prop_mark = 1;
			}
		if (stucd_mark == stucdnum && huaxi_mark == huaxinum && uestc_mark == 1 && prop_mark == 1)
			cancelTimer(0);
	}
	if(ID == 1)//成科移动
	{
		for (int i = 0; i < stucdnum;i++)
		{
			if(stucds[i]->islife() != 0)
			{
				stucds[i]->move();
			}
		}
	}
	if(ID == 2)//华西移动
	{
		for (int i = 0; i < huaxinum; i++)
		{
			if (huaxi[i]->islife() != 0)
			{
				huaxi[i]->move(*scu);
			}
		}
	}
	if(ID == 3)//成电移动
	{
		uestc->move(*scu);
	}
	if(ID == 4)//道具随机生成
	{
		int random = rand() % 4;
		if (prop[random]->getexist() == 0)
		{
			prop[random]->setexist(1);
		}
	}
	if(ID == 5)//道具移动
	{
		for (int i = 0; i < 4; i++)
		{
			if (prop[i]->getexist() != 0)
			{
				prop[i]->move();
			}
		}
	}
	if(ID == 6)//加速道具有效时间
	{
		scu->setspeed(2, 0);
		cancelTimer(6);
	}
	if(ID == 7)//减速道具有效时间
	{
		scu->setspeed(2.5, 1);
		cancelTimer(7);
	}
	if(ID == 8)//停滞时间
	{
		scu->setbreak(0);
		cancelTimer(8);
	}
	Collision_uestc_or_prop();
	Paint();
}

void PropTrigger(int prop)
{
	switch (prop)
	{
	case chance:
		scu->addchance();
		break;
	case speedup:
		scu->setspeed(2, 1);
		startTimer(6, 5000);
		break;
	case speeddown:
		scu->setspeed(2, 0);
		startTimer(7, 3000);
		break;
	case breakdown:
		scu->setbreak();
		startTimer(8, 3000);
		break;
	}
}

void Gameover()
{
	beginPaint();
	setTextSize(50);
	if (scu->getchance() != 0)
	{
		setTextColor(RED);
		paintText(50, 225, "SCU becomes the best university!");
	}
	else
	{
		setTextColor(BLUE);
		paintText(75, 225, "SCU still has a long way to go!");
	}
	endPaint();
	for (int id = 1; id <= 8; id++)
		cancelTimer(id);
	is_over = 1;
}

void Paint()
{
	beginPaint();
	clearDevice();
	endPaint();
	if(scu->getchance())
		scu->paint();
	//出现且未被吃掉时显示
	for (int i = 0; i < stucdnum;i++)
	{
		if(stucds[i]->islife() != 0)
			stucds[i]->paint();
	}
	//华西出现且未被吃掉时显示
	for (int i = 0; i < huaxinum;i++)
	{
		if (huaxi[i]->islife() != 0)
			huaxi[i]->paint();
	}
	//成电出现后显示
	if(uestc->islife())	uestc->paint();
	//道具出现后显示
	for (int i = 0; i < 4; i++)
	{
		if (prop[i]->getexist() != 0)
			prop[i]->paint();
	}
	beginPaint();
	setTextSize(25);
	setTextColor(BLACK);
	paintText(0, 0, "Chance:");//显示生命值
	for (int i = 0; i < scu->getchance(); i++)
	{
		if (scu->getchance() >= 4) setBrushColor(GREEN);//生命值为4、5时绿色
		else if (scu->getchance() >= 2) setBrushColor(YELLOW);//生命值为2、3时黄色
		else setBrushColor(RED);//生命值为1时红色
		roundrect(100+i*23,0,100+23*(i+1),23,5,5); //生命值为4、5时绿色
	}
	char str[15];
	sprintf(str, "rank: %d", scu->getrank());//显示排名
	paintText(0, 30, str);
	endPaint();
	//是否得到足够分数（即rank已成为第1）
	if (scu->getrank() <= 1 || is_over==1){
		Gameover();
	}
}