#pragma once
#include "cocos2d.h"

class Board : public cocos2d::Node
{
public:
    cocos2d::DrawNode* drawNode = cocos2d::DrawNode::create();
    Board(bool init);

    cocos2d::Label* staticLabels = cocos2d::Label::createWithTTF("TimePassed:\nNextArrival:\nVisitorTotal:\nDepartedTotal:\nGetRate:\nTotalAverageWaitingTime:\n0FAverageWaitingTime:\n1FAverageWaitingTime:\n2FAverageWaitingTime:\n3FAverageWaitingTime:\n4FAverageWaitingTime:\n", "fonts/Marker Felt.ttf", 28);
    cocos2d::Label* dynamicLabels = cocos2d::Label::createWithTTF("", "fonts/Marker Felt.ttf", 28);
    int TimePassed = 0;
    int Counter = 0;
    int VisitorTotal = 0;
    int DepartedTotal = 0;
    float GetRate = 0;
    int GetPersonNum[5] = {0};
    int AverageWaitingTime[6] = {0};

    void AddNumberToAverage(int floor, int waitingTime);

    virtual void update(float dt);
};

