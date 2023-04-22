#pragma once
#include "cocos2d.h"
#include "Vector.h"
#define MaxPersonNum 10

class Elevator :
    public cocos2d::Node
{
public:
    int ElevatorNo = -1;
    // ��ܵ��ݽ���ĵ�ַ
    Vector* Box;
    int SpeedOfElevator = 1;
    // ������¥���ϵİ�ť
    bool CallUp[5] = { false,false,false,false,false };
    bool CallDown[5] = { false,false,false,false,false };
    // �����ǵ��ݽ����ڵİ�ť״̬
    bool CallCar[5] = { false,false,false,false,false };
    // �����Ǳ�ʾ���ݵ�״̬
	enum StateEnum { GoingUp, GoingDown, Idle, IdleGoingUp, IdleGoingDown};
    enum DoorEnum { Opening, Closing, Closed, Opened };
    int ElevatorState = Idle;
    std::string ElevatorStateString;
    std::string DoorStateString;
    int DoorState = Closed;
    // ����������¥��
    int Floor = 1;
    int IdleDest = -1;
    //����״̬ʱ��Ҫȥ��Ŀ�ĵ�
    bool FloorAddition = false;//�Ƿ���¥��֮��
	// ����ʱ������
	int Counter = 0;

	const int ClosingTimeNum = 20;//��������Ҫ��ʱ������
	const int OpeningTimeNum = 20;
    const int WaitForInPeopleTimeNum = 70;//���ź���˽�����ʱ������

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
    * YesCompletely:Ӧ�ñ���ԭ����״̬�����ҽ�������Ҫ����ͬ���£��˻�û���꣩
    * YesFindTheLowerUper:�����л�Ҫ��¥�ģ���һ��Ӧ������͵��Ǹ�Ҫ��¥����
    * YesFindTheHigherDowner:���滹��Ҫ��¥�ģ���һ��Ӧ������ߵ��Ǹ�Ҫ��¥����
    * No:��Ӧ���ٱ�������״̬��Ҫ���Idle��Idle״̬ת����״̬�����ж�
    */
    int ShouldContinue();
    void DeleteEveryPersonEquals(int Purpose);
    void refresh();
    int GetLowUpFloor(int Floor);
    int GetHighDownFloor(int Floor);
};


