#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "TheWorld.h"

class HelloWorld : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    CREATE_FUNC(HelloWorld);

    void CreateTheWorld(cocos2d::Ref* pSender);

    virtual void update(float dt);

    cocos2d::Label* terminal;
    TheWorld* world = nullptr;

    cocos2d::Sprite* Door[2][5];
    cocos2d::Sprite* CallUpBtn[2][5];
    cocos2d::Sprite* CallDownBtn[2][5];

    int NextArrival[2] = { 30,40 };//下一个人到来的左范围和右范围
    int TolerateTime[2] = { 3000,6000 };//人的等待时间的左范围和右范围
    int SpeedOfElevator = 3;//电梯运行速度，只能选1，2，3，4，5
    int WaitingTime = 20;//电梯在每一层的开门时间

    cocos2d::Label* title = cocos2d::Label::createWithTTF("Welcome to the Elevator World!", "fonts/sf-pro-display_semibold.ttf", 90);
    cocos2d::Label* NextArrivalLabel = cocos2d::Label::createWithTTF("Next Arrival Time Range", "fonts/sf-pro-display_semibold.ttf", 60);
    cocos2d::Label* TolerateTimeLabel = cocos2d::Label::createWithTTF("Tolerate Time Range", "fonts/sf-pro-display_semibold.ttf", 60);
    cocos2d::Label* SpeedOfElevatorLabel = cocos2d::Label::createWithTTF("Speed of Elevator", "fonts/sf-pro-display_semibold.ttf", 60);
    cocos2d::Label* WaitingTimeLabel = cocos2d::Label::createWithTTF("Waiting Time", "fonts/sf-pro-display_semibold.ttf", 60);
    cocos2d::Label* AssessmentPatternLabel = cocos2d::Label::createWithTTF("Assessment Pattern", "fonts/sf-pro-display_semibold.ttf", 60);

    cocos2d::Label* NextArrival_Num = 
        cocos2d::Label::createWithTTF("30 40", "fonts/sf-pro-display_semibold.ttf", 60);
    cocos2d::Label* TolerateTime_Num =
        cocos2d::Label::createWithTTF("3000 6000", "fonts/sf-pro-display_semibold.ttf", 60);
    cocos2d::Label* SpeedOfElevator_Num =
        cocos2d::Label::createWithTTF("3", "fonts/sf-pro-display_semibold.ttf", 60);
    cocos2d::Label* WaitingTime_Num =
        cocos2d::Label::createWithTTF("20", "fonts/sf-pro-display_semibold.ttf", 60);
    cocos2d::Label* AssessmentPattern_Num =
        cocos2d::Label::createWithTTF("2", "fonts/sf-pro-display_semibold.ttf", 60);

    cocos2d::Sprite* Btn1 = cocos2d::Sprite::create("CreateTheWorld_ValueSetBtn.png");
    cocos2d::Sprite* Btn2 = cocos2d::Sprite::create("CreateTheWorld_ValueSetBtn.png");
    cocos2d::Sprite* Btn3 = cocos2d::Sprite::create("CreateTheWorld_ValueSetBtn_Single.png");
    cocos2d::Sprite* Btn4 = cocos2d::Sprite::create("CreateTheWorld_ValueSetBtn_Number.png");
    cocos2d::Sprite* Btn5 = cocos2d::Sprite::create("CreateTheWorld_Assessment.png");

    bool Callback1(cocos2d::Touch* touch, cocos2d::Event* event);
    void Callback2(cocos2d::Touch* touch, cocos2d::Event* event);

    enum AssessmentPatternEnum { DistanceMatters = 1, TolerateMatters, BothMatters };
    int AssessmentPattern = TolerateMatters;

    cocos2d::Menu* CreateBtn1;
};

#endif // __HELLOWORLD_SCENE_H__
