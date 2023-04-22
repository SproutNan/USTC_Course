#pragma once
#include <random>
#include "cocos2d.h"
#include <string>
#define ELEVATOR_NUM 2
#define FLOOR_NUM 5

class Person : public cocos2d::Node{
public:
	std::string Name;                  // 人的名字
	cocos2d::Sprite* PersonSprite;     // 人的图像
	Person* NextPerson = nullptr;      // 下一个人指针
	int TolerateTimeNums = -1;         // 容忍时间计数器
	int TolerateHistory = -1;          // 容忍时间
	int FloorNow = -1;                 // 现在所处的楼层（即出生楼层）
	int FloorPurpose = -1;             // 希望去的楼层
	int tag = 0;                       // 状态：0表示在电梯外等候，1表示在电梯内乘坐
	int ToUp = 1;                      // 0表示要向下，1表示要向上，在构造时确定
	int ElevNo = -1;                   // 现在处于几号电梯，在外等候时为-1
	int AssessmentPattern = 0;         // 优先程度方式
	int Assessment = 0;                // 优先程度，由传进来的AssessmentPattern确定

	Person(int BirthFloor, int TolerateTimeL, int TolerateTimeR, int AssessmentPattern);

	virtual void update(float dt);
	cocos2d::Label* StatusLabel;

	int Rand(int l, int r) {
		std::random_device rd;
		std::mt19937 gen(rd()); 
		std::uniform_int_distribution<> distrib(l, r);
		return distrib(gen);
	}

	char FirstName[10][10] = { "Mary ","Annie ","Smith ","Yole ","Kappa ","Rozzon ","Pascal ","Oscar ","Wilson ","Terry " };
	char LastName[10][10] = { "Huang","Zhang","Jin","Liu","Fang","Wen","Peng","Song","Gu","Chen" };
};

