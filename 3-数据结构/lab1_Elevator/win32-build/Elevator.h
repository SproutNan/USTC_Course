#pragma once
#include "cocos2d.h"
#include "Vector.h"
#define MaxPersonNum 10

class Elevator :
    public cocos2d::Node
{
public:
    int ElevatorNo = -1;
    // 这架电梯轿厢的地址
    Vector* Box;
    int SpeedOfElevator = 1;
    // 下面是楼层上的按钮
    bool CallUp[5] = { false,false,false,false,false };
    bool CallDown[5] = { false,false,false,false,false };
    // 下面是电梯轿厢内的按钮状态
    bool CallCar[5] = { false,false,false,false,false };
    // 下面是表示电梯的状态
	enum StateEnum { GoingUp, GoingDown, Idle, IdleGoingUp, IdleGoingDown};
    enum DoorEnum { Opening, Closing, Closed, Opened };
    int ElevatorState = Idle;
    std::string ElevatorStateString;
    std::string DoorStateString;
    int DoorState = Closed;
    // 电梯所处的楼层
    int Floor = 1;
    int IdleDest = -1;
    //空闲状态时，要去的目的地
    bool FloorAddition = false;//是否在楼层之间
	// 倒计时计数器
	int Counter = 0;

	const int ClosingTimeNum = 20;//开关门需要的时间间隔数
	const int OpeningTimeNum = 20;
    const int WaitForInPeopleTimeNum = 70;//开门后等人进来的时间间隔数

	cocos2d::Sprite* ElevSprite;
    cocos2d::Sprite* PersonInTheBox[MaxPersonNum];
	cocos2d::Label* ElevStatusLabel;

    Elevator(int i, int SpeedOfElevator);
    virtual void update(float dt);
    void MoveOneFloor(int UpDown);
    bool CheckOutside(int UpDown, int WantTo);
    enum ContinueEnum { 
        YesCompletely, 
        YesFindTheLowerUper,
        YesFindTheHigherDowner,
        No 
    };
    /*
    * YesCompletely:应该保持原来的状态，并且接下来还要做相同的事（人还没送完）
    * YesFindTheLowerUper:上面有还要上楼的，下一步应该找最低的那个要上楼的人
    * YesFindTheHigherDowner:上面还有要下楼的，下一步应该找最高的那个要下楼的人
    * No:不应该再保持现有状态，要变成Idle，Idle状态转其他状态另行判断
    */
    int ShouldContinue();
    void DeleteEveryPersonEquals(int Purpose);
    void refresh();
    int GetLowUpFloor(int Floor);
    int GetHighDownFloor(int Floor);
};


