#include "Person.h"

Person::Person(int BirthFloor, int TolerateTimeL, int TolerateTimeR, int AssessmentPattern) {
	Name += FirstName[Rand(0, 9)];
	Name += LastName[Rand(0, 9)];
	NextPerson = nullptr;
	TolerateTimeNums = Rand(TolerateTimeL, TolerateTimeR);
	TolerateHistory = TolerateTimeNums;
	FloorNow = BirthFloor;

	PersonSprite = cocos2d::Sprite::create("Person_Idle.png");
	PersonSprite->setPosition(0,0);
	PersonSprite->setScaleX(-1);
	this->addChild(PersonSprite);

	this->setAnchorPoint(cocos2d::Vec2(0, 0.5));

	//�������Ҫ������ȥ
	while (1) {
		FloorPurpose = Rand(0, 4);
		if (FloorPurpose != FloorNow) {
			break;
		}
	}

	//���������ϻ�������
	ToUp = (FloorPurpose > FloorNow) ? 1 : 0;

	StatusLabel = cocos2d::Label::createWithTTF("   ", "fonts/Marker Felt.ttf", 20);
	StatusLabel->setPosition(0, 100);
	StatusLabel->setTextColor(cocos2d::Color4B::WHITE);
	StatusLabel->enableOutline((ToUp ? cocos2d::Color4B::RED : cocos2d::Color4B::BLUE), 3);
	this->addChild(StatusLabel);

	auto moveTo = cocos2d::MoveBy::create(0.1, cocos2d::Vec2(0, -10));
	PersonSprite->runAction(moveTo);

	this->AssessmentPattern = AssessmentPattern;

	//�������ȼ�
	switch (AssessmentPattern) {
	case 1:
		//��������
		Assessment = (int)abs(FloorNow - FloorPurpose);
		break;
	case 2:
		//����ʱ�����ȣ�ֱ�Ӳ�������漴��
		break;
	default:
		break;
	}

	scheduleUpdate();
}

void Person::update(float dt) {
	auto Tmp_Label = cocos2d::StringUtils::format("%d", FloorPurpose);
	StatusLabel->setString(Tmp_Label.c_str());
}