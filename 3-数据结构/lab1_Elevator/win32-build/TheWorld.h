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
	float Time = 0.0f;//ʱ��
	float Interval = 0.1f;//ʱ����

	cocos2d::Sprite* Boundry;//���

	float AdventInterval = 0.0f;//��һ���˳��ֵ���һ���˳��ֵ�ʱ����
	float Counter = 3.0f;//��ʱ��

	Elevator* elev[ELEVATOR_NUM];//����
	Vector* UpLine[FLOOR_NUM];//��¥����
	Vector* DownLine[FLOOR_NUM];//��¥����
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
	int AssessmentPattern = 0;//��HelloWorld���Ʊ��
};




