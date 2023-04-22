#pragma once
#include <random>
#include "cocos2d.h"
#include <string>
#define ELEVATOR_NUM 2
#define FLOOR_NUM 5

class Person : public cocos2d::Node{
public:
	std::string Name;                  // �˵�����
	cocos2d::Sprite* PersonSprite;     // �˵�ͼ��
	Person* NextPerson = nullptr;      // ��һ����ָ��
	int TolerateTimeNums = -1;         // ����ʱ�������
	int TolerateHistory = -1;          // ����ʱ��
	int FloorNow = -1;                 // ����������¥�㣨������¥�㣩
	int FloorPurpose = -1;             // ϣ��ȥ��¥��
	int tag = 0;                       // ״̬��0��ʾ�ڵ�����Ⱥ�1��ʾ�ڵ����ڳ���
	int ToUp = 1;                      // 0��ʾҪ���£�1��ʾҪ���ϣ��ڹ���ʱȷ��
	int ElevNo = -1;                   // ���ڴ��ڼ��ŵ��ݣ�����Ⱥ�ʱΪ-1
	int AssessmentPattern = 0;         // ���ȳ̶ȷ�ʽ
	int Assessment = 0;                // ���ȳ̶ȣ��ɴ�������AssessmentPatternȷ��

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

