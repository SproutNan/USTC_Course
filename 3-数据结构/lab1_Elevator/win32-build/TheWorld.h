#pragma once
#include "cocos2d.h"
#include "Elevator.h"
#include "Person.h"
#include "Vector.h"
#include "Board.h"
#define ELEVATOR_NUM 2
#define FLOOR_NUM 5

class TheWorld : public cocos2d::Node
{
public:
	float Time = 0.0f;//时钟
	float Interval = 0.1f;//时间间隔

	cocos2d::Sprite* Boundry;//框架

	float AdventInterval = 0.0f;//这一个人出现到下一个人出现的时间间隔
	float Counter = 3.0f;//计时器

	Elevator* elev[ELEVATOR_NUM];//电梯
	Vector* UpLine[FLOOR_NUM];//上楼队列
	Vector* DownLine[FLOOR_NUM];//下楼队列
	cocos2d::Label* ElevLabel[ELEVATOR_NUM];

	int NextArrivalL;
	int NextArrivalR;
	int TolerateTimeL;
	int TolerateTimeR;
	int WaitingTime;
	int SpeedOfElevator;

	TheWorld(int NextArrivalL, int NextArrivalR, int TolerateTimeL, int TolerateTimeR, int WaitingTime, int SpeedOfElevator);


	virtual void update(float dt);
	void PersonOperate(Person* p);
	int CheckUpingElevators(int Floor);
	int CheckDownElevators(int Floor);
	void ElevatorOperate(Elevator* v);
	void DisableCallAllElevators(int Floor, int Up);

	int FloorY[5] = { -450,-210,30,270,510 };

	void CheckAddition(Elevator* e);
	bool UPDATE_Before(cocos2d::Touch* touch, cocos2d::Event* event);
	void UPDATE(cocos2d::Touch* touch, cocos2d::Event* event);

	Board* board = new Board(false);
	int AssessmentPattern = 0;//由HelloWorld控制变更
};




