#include "HelloWorldScene.h"


USING_NS_CC;

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

bool HelloWorld::init()
{
    title->setPosition(1024, 1100);
    title->setTextColor(cocos2d::Color4B::WHITE);
    this->addChild(title);

    AssessmentPatternLabel->setPosition(500, 750);
    AssessmentPatternLabel->setTextColor(cocos2d::Color4B::WHITE);
    AssessmentPatternLabel->setAlignment(cocos2d::TextHAlignment::LEFT);
    this->addChild(AssessmentPatternLabel);

    NextArrivalLabel->setPosition(500, 600);
    NextArrivalLabel->setTextColor(cocos2d::Color4B::WHITE);
    NextArrivalLabel->setAlignment(cocos2d::TextHAlignment::LEFT);
    this->addChild(NextArrivalLabel);

    TolerateTimeLabel->setPosition(500, 450);
    TolerateTimeLabel->setTextColor(cocos2d::Color4B::WHITE);
    TolerateTimeLabel->setAlignment(cocos2d::TextHAlignment::LEFT);
    this->addChild(TolerateTimeLabel);

    WaitingTimeLabel->setPosition(500, 300);
    WaitingTimeLabel->setTextColor(cocos2d::Color4B::WHITE);
    WaitingTimeLabel->setAlignment(cocos2d::TextHAlignment::LEFT);
    this->addChild(WaitingTimeLabel);

    SpeedOfElevatorLabel->setPosition(500, 150);
    SpeedOfElevatorLabel->setTextColor(cocos2d::Color4B::WHITE);
    SpeedOfElevatorLabel->setAlignment(cocos2d::TextHAlignment::LEFT);
    this->addChild(SpeedOfElevatorLabel);

    Btn5->setPosition(1467, 735.5);
    this->addChild(Btn5);
    Btn1->setPosition(1557.5, 588.5);
    this->addChild(Btn1);
    Btn2->setPosition(1557.5, 441.5);
    this->addChild(Btn2);
    Btn3->setPosition(1340, 287.5);
    this->addChild(Btn3);
    Btn4->setPosition(1367, 142.5);
    this->addChild(Btn4);


    AssessmentPattern_Num->setPosition(964, 750);
    AssessmentPattern_Num->setTextColor(cocos2d::Color4B::WHITE);
    this->addChild(AssessmentPattern_Num);

    NextArrival_Num->setPosition(964, 600);
    NextArrival_Num->setTextColor(cocos2d::Color4B::WHITE);
    this->addChild(NextArrival_Num);

    TolerateTime_Num->setPosition(964, 450);
    TolerateTime_Num->setTextColor(cocos2d::Color4B::WHITE);
    this->addChild(TolerateTime_Num);

    WaitingTime_Num->setPosition(964, 300);
    WaitingTime_Num->setTextColor(cocos2d::Color4B::WHITE);
    this->addChild(WaitingTime_Num);

    SpeedOfElevator_Num->setPosition(964, 150);
    SpeedOfElevator_Num->setTextColor(cocos2d::Color4B::WHITE);
    this->addChild(SpeedOfElevator_Num);

    auto TouchListener = cocos2d::EventListenerTouchOneByOne::create();
    TouchListener->onTouchBegan = CC_CALLBACK_2(HelloWorld::Callback1, this);
    TouchListener->onTouchEnded = CC_CALLBACK_2(HelloWorld::Callback2, this);

    _eventDispatcher->addEventListenerWithSceneGraphPriority(TouchListener, this);

    auto CreateBtn = MenuItemImage::create("CreateTheWorld_StartBtn.png", "CreateTheWorld_StartBtn.png", CC_CALLBACK_1(HelloWorld::CreateTheWorld, this));
    CreateBtn->setPosition(Vec2(1024, 900));
    CreateBtn1 = Menu::create(CreateBtn, NULL);
    CreateBtn1->setPosition(Vec2::ZERO);
    this->addChild(CreateBtn1);

    return true;
}

bool HelloWorld::Callback1(cocos2d::Touch* touch, cocos2d::Event* event) {
    return true;
}
void HelloWorld::Callback2(cocos2d::Touch* touch, cocos2d::Event* event) {
    auto location = touch->getLocation();
    //第一行第一个按钮
    if (location.x >= 1141 && location.x <= 1329 && location.y >= 553 && location.y <= 636) {
        if (NextArrival[0] < NextArrival[1] - 1)NextArrival[0]++;
    }
    //第一行第二个按钮
    if (location.x >= 1355 && location.x <= 1546 && location.y >= 553 && location.y <= 636) {
        if (NextArrival[0] > 5)NextArrival[0]--;
    }
    //第一行第三个按钮
    if (location.x >= 1572 && location.x <= 1760 && location.y >= 553 && location.y <= 636) {
        NextArrival[1]++;
    }
    //第一行第四个按钮
    if (location.x >= 1788 && location.x <= 1979 && location.y >= 553 && location.y <= 636) {
        if (NextArrival[1] > NextArrival[0] + 1)NextArrival[1]--;
    }
    auto Tmp_Label = cocos2d::StringUtils::format("%d %d", NextArrival[0], NextArrival[1]);
    NextArrival_Num->setString(Tmp_Label.c_str());

    //第二行第一个按钮
    if (location.x >= 1141 && location.x <= 1329 && location.y >= 408 && location.y <= 490) {
        if (TolerateTime[0] < TolerateTime[1] - 1000)TolerateTime[0] += 1000;
    }
    //第二行第二个按钮
    if (location.x >= 1355 && location.x <= 1546 && location.y >= 408 && location.y <= 490) {
        if (TolerateTime[0] > 5)TolerateTime[0] -= 10;
    }
    //第二行第三个按钮
    if (location.x >= 1572 && location.x <= 1760 && location.y >= 408 && location.y <= 490) {
        TolerateTime[1] += 10;
    }
    //第二行第四个按钮
    if (location.x >= 1788 && location.x <= 1979 && location.y >= 408 && location.y <= 490) {
        if (TolerateTime[1] > TolerateTime[0] + 1000)TolerateTime[1] -= 1000;
    }
    Tmp_Label = cocos2d::StringUtils::format("%d %d", TolerateTime[0], TolerateTime[1]);
    TolerateTime_Num->setString(Tmp_Label.c_str());
    //第三行第一个按钮
    if (location.x >= 1141 && location.x <= 1329 && location.y >= 254 && location.y <= 335) {
        WaitingTime++;
    }
    //第三行第二个按钮
    if (location.x >= 1355 && location.x <= 1546 && location.y >= 254 && location.y <= 335) {
        if (WaitingTime > 5)WaitingTime--;
    }
    Tmp_Label = cocos2d::StringUtils::format("%d", WaitingTime);
    WaitingTime_Num->setString(Tmp_Label.c_str());
    if (location.x >= 1139 && location.x <= 1220 && location.y >= 107 && location.y <= 191) {
        SpeedOfElevator = 1;
    }
    if (location.x >= 1237 && location.x <= 1317 && location.y >= 107 && location.y <= 191) {
        SpeedOfElevator = 2;
    }
    if (location.x >= 1330 && location.x <= 1411 && location.y >= 107 && location.y <= 191) {
        SpeedOfElevator = 3;
    }
    if (location.x >= 1423 && location.x <= 1505 && location.y >= 107 && location.y <= 191) {
        SpeedOfElevator = 4;
    }
    if (location.x >= 1517 && location.x <= 1598 && location.y >= 107 && location.y <= 191) {
        SpeedOfElevator = 5;
    }
    Tmp_Label = cocos2d::StringUtils::format("%d", SpeedOfElevator);
    SpeedOfElevator_Num->setString(Tmp_Label.c_str());
    if (location.x >= 1139 && location.x <= 1342 && location.y >= 700 && location.y <= 783) {
        AssessmentPattern = 1;
    }
    if (location.x >= 1368 && location.x <= 1571 && location.y >= 700 && location.y <= 783) {
        AssessmentPattern = 2;
    }
    Tmp_Label = cocos2d::StringUtils::format("%d", AssessmentPattern);
    AssessmentPattern_Num->setString(Tmp_Label.c_str());
}
void HelloWorld::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

}
void HelloWorld::CreateTheWorld(Ref* pSender) {
    CreateBtn1->removeFromParentAndCleanup(true);
    world = new TheWorld(NextArrival[0], NextArrival[1], TolerateTime[0], TolerateTime[1], WaitingTime, SpeedOfElevator);
    world->setPosition(1024, 600);
    this->addChild(world, 1);

    auto Background = cocos2d::Sprite::create("Elevator_Background.png");
    Background->setPosition(1024, 600);
    this->addChild(Background, 0);

    for (int i = 0; i < ELEVATOR_NUM; i++) {
        for (int j = 0; j < FLOOR_NUM; j++) {
            Door[i][j] = cocos2d::Sprite::create("Elevator_Door.png");
            Door[i][j]->setPosition(263 + 295.2 * i, 599 + world->FloorY[j]);
            Door[i][j]->setScale(0);
            //先不设置门了，有点鬼畜
            this->addChild(Door[i][j], 999);
        }
    }
    for (int j = 0; j < ELEVATOR_NUM; j++) {
        for (int i = 0; i < FLOOR_NUM; i++) {
            CallUpBtn[j][i] = cocos2d::Sprite::create("Button_HighLight.png");
            CallUpBtn[j][i]->setPosition(2000, 2000);
            this->addChild(CallUpBtn[j][i], 999);
        }
    }

    for (int j = 0; j < ELEVATOR_NUM; j++) {
        for (int i = 0; i < FLOOR_NUM; i++) {
            CallDownBtn[j][i] = cocos2d::Sprite::create("Button_HighLight2.png");
            CallDownBtn[j][i]->setPosition(2000, 2000);
            this->addChild(CallDownBtn[j][i], 999);
        }
    }

    scheduleUpdate();
}


void HelloWorld::update(float dt) {
    if (world) {
        if (world->AssessmentPattern < 1) {
            world->AssessmentPattern = AssessmentPattern;
        }
    }
    //设置向上的按钮
    for (int j = 0; j < 2; j++) {
        for (int i = 0; i < 5; i++) {
            if (world->elev[j]->CallUp[i]) {
                CallUpBtn[j][i]->setPosition(165 + 295 * j, 185 + 240 * i);
            }
            else {
                CallUpBtn[j][i]->setPosition(2000, 2000);
            }
            if (world->elev[j]->CallDown[i]) {
                CallDownBtn[j][i]->setPosition(165 + 295 * j, 135 + 240 * i);
            }
            else {
                CallDownBtn[j][i]->setPosition(2000, 2000);
            }
        }
    }
    //设置门是否打开
    for (int j = 0; j < 2; j++) {
        for (int i = 0; i < 5; i++) {
            if (abs((int)(world->elev[j]->getPosition().y) - world->FloorY[world->elev[j]->Floor]) < 3 && world->elev[j]->DoorStateString == "Opened") {
                Door[j][world->elev[j]->Floor]->setVisible(false);
            }
            else {
                Door[j][world->elev[j]->Floor]->setVisible(true);
            }
        }
    }
}
