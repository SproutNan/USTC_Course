#pragma once
#include "Person.h"

class Vector {
public:
    // 电梯轿厢和等候队列共用区
    Person* FirstPerson = nullptr;    // 容器中第一个人的指针
    int PersonNums = 0;               // 容器中人数
    enum VectorTagEnum { WLU, WLD, BOX };
    int VectorTag = WLU;              // 容器性质，WaitingLineUp, WaitingLineDown, ElevatorBox

    // 轿厢特有的成员
    int BoxElevNo = -1;               // 轿厢编号
    bool ShouldRefresh = false;       // 是否应该根据PersonNums刷新图形化

    // 成员函数部分
    void PushPerson(Person* p);              // 将p所指的人放到此容器末尾
    void DeleteFromLine(Person* p);          // 将p所指的人从此容器中删除，但是不释放内存
    void DeleteFromLineCleanUp(Person* p);   // 将p所指的人从此容器中删除，并且从程序中卸载
    Person* GetPrevPerson(Person* p);        // 获得p所指的人的前一个人的指针，没有返回nullptr
    Person* GetLastPerson();                 // 获得容器内最后一个人的指针
    Person* GetFirstPerson();                // 获得容器内优先级最高的人

    // 构造函数
    Vector(int VectorTag);
};



