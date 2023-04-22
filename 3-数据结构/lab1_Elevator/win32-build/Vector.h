#pragma once
#include "Person.h"

class Vector {
public:
    // ���ݽ���͵Ⱥ���й�����
    Person* FirstPerson = nullptr;    // �����е�һ���˵�ָ��
    int PersonNums = 0;               // ����������
    enum VectorTagEnum { WLU, WLD, BOX };
    int VectorTag = WLU;              // �������ʣ�WaitingLineUp, WaitingLineDown, ElevatorBox

    // �������еĳ�Ա
    int BoxElevNo = -1;               // ������
    bool ShouldRefresh = false;       // �Ƿ�Ӧ�ø���PersonNumsˢ��ͼ�λ�

    // ��Ա��������
    void PushPerson(Person* p);              // ��p��ָ���˷ŵ�������ĩβ
    void DeleteFromLine(Person* p);          // ��p��ָ���˴Ӵ�������ɾ�������ǲ��ͷ��ڴ�
    void DeleteFromLineCleanUp(Person* p);   // ��p��ָ���˴Ӵ�������ɾ�������Ҵӳ�����ж��
    Person* GetPrevPerson(Person* p);        // ���p��ָ���˵�ǰһ���˵�ָ�룬û�з���nullptr
    Person* GetLastPerson();                 // ������������һ���˵�ָ��
    Person* GetFirstPerson();                // ������������ȼ���ߵ���

    // ���캯��
    Vector(int VectorTag);
};



