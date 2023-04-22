#include "Board.h"
Board::Board(bool init) {
    this->setVisible(init);
    this->addChild(drawNode, 100);

    cocos2d::Vec2 point[4] = {
        cocos2d::Vec2(-300,200),
        cocos2d::Vec2(300,200),
        cocos2d::Vec2(300,-200),
        cocos2d::Vec2(-300,-200)
    };
    drawNode->drawPolygon(point, 4, cocos2d::Color4F(1, 0, 0, 0.5), 1, cocos2d::Color4F(1, 0, 0, 1));

    staticLabels->setPosition(-100, -20);
    staticLabels->setTextColor(cocos2d::Color4B::WHITE);
    staticLabels->setAlignment(cocos2d::TextHAlignment::RIGHT);
    staticLabels->enableOutline(cocos2d::Color4B::RED, 3);
    this->addChild(staticLabels, 999);

    dynamicLabels->setPosition(132, -2);
    dynamicLabels->setTextColor(cocos2d::Color4B::WHITE);
    dynamicLabels->setAlignment(cocos2d::TextHAlignment::LEFT);
    dynamicLabels->enableOutline(cocos2d::Color4B::RED, 3);
    this->addChild(dynamicLabels, 999);

    scheduleUpdate();
}
void Board::update(float dt) {
    GetRate = 1 - ((float)DepartedTotal / (float)VisitorTotal);
    auto Tmp_Label = cocos2d::StringUtils::format(
        "%d\n%d\n%d\n%d\n%d%%\n%d\n%d\n%d\n%d\n%d\n%d        ",
        TimePassed,
        Counter,
        VisitorTotal,
        DepartedTotal,
        (int)(GetRate * 100),
        AverageWaitingTime[5],
        AverageWaitingTime[0],
        AverageWaitingTime[1],
        AverageWaitingTime[2],
        AverageWaitingTime[3],
        AverageWaitingTime[4]
    );
    dynamicLabels->setString(Tmp_Label.c_str());
}
void Board::AddNumberToAverage(int floor, int waitingTime) {
    if (floor < 5) {
        AverageWaitingTime[floor] = (AverageWaitingTime[floor] * GetPersonNum[floor] + waitingTime) / (GetPersonNum[floor] + 1);
        GetPersonNum[floor]++;
    }
    else {
        AverageWaitingTime[floor] = (AverageWaitingTime[floor] * VisitorTotal + waitingTime) / (VisitorTotal + 1);
    }
}

