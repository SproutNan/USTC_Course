#include "Elevator.h"

int BRand(int l, int r) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(l, r);
    return distrib(gen);
}
Elevator::Elevator(int i, int SpeedOfElevator) {
    this->SpeedOfElevator = SpeedOfElevator;
    Box = new Vector(Vector::BOX);
	ElevSprite = cocos2d::Sprite::create("Elevator_Inside.png");
	ElevSprite->setPosition(0, 0);
	this->addChild(ElevSprite);

	ElevStatusLabel = cocos2d::Label::createWithTTF("   ", "fonts/Marker Felt.ttf", 20);
	ElevStatusLabel->setPosition(0,0);
	ElevStatusLabel->setTextColor(cocos2d::Color4B::WHITE);
	ElevStatusLabel->enableOutline(cocos2d::Color4B::RED, 3);
    this->addChild(ElevStatusLabel, 999);

    ElevatorNo = i;

    for (int i = 0; i < MaxPersonNum; i++) {
        PersonInTheBox[i] = cocos2d::Sprite::create("Person_Idle.png");
        PersonInTheBox[i]->setScale(0.7f);
        PersonInTheBox[i]->setPosition(BRand(-40, 40), -10);
        PersonInTheBox[i]->setVisible(false);
        this->addChild(PersonInTheBox[i]);
    }

    Box->BoxElevNo = ElevatorNo;

	scheduleUpdate();
}

void Elevator::update(float dt) {
    switch (ElevatorState) {
    case GoingUp:
        ElevatorStateString = "GoingUp";
        break;
    case GoingDown:
        ElevatorStateString = "GoingDown";
        break;
    case Idle:
        ElevatorStateString = "Idle";
        break;
    }
    switch (DoorState) {
    case Opened:
        DoorStateString = "Opened";
        break;
    case Closed:
        DoorStateString = "Closed";
        break;
    default:
        DoorStateString = "default";
        break;
    }

	auto Tmp_Label = cocos2d::StringUtils::format(
        "Nums:%d\nCounter:%d\nDoor:%s\nState:%s\nShouldContinue:%s\nAddition:%s", 
        Box->PersonNums, 
        Counter, 
        DoorStateString.c_str(), 
        ElevatorStateString.c_str(),
        (ShouldContinue()!=No)?"Yes":"No",
        (FloorAddition==true)?"Yes":"No"
    );
	ElevStatusLabel->setString(Tmp_Label.c_str());

    if (Box->ShouldRefresh == true) {
        refresh();
        Box->ShouldRefresh = false;
    }
}
void Elevator::MoveOneFloor(int UpDown) {
    //UpDown = 1��ʾ���ϣ�UpDown = -1��ʾ����
    auto posNow = this->getPosition();
    this->FloorAddition = true;//��ʼ�ƶ�
    this->DoorState = Closed;//��������ٹ�һ��
    this->setPosition(posNow.x, posNow.y + UpDown);
}
bool Elevator::CheckOutside(int UpDown, int WantTo) {
    //���UpDown�����Ƿ�����ҪWantTo������ˣ���(1,0)��ʾ����Ϸ��Ƿ�����Ҫ��¥����
    if (UpDown == 1) {
        //������Ϸ���
        if (WantTo == 1) {
            //�Ƿ�����Ҫ���ϵ���
            for (int j = Floor + 1; j < 5; j++) {
                if (CallUp[j] == true) {
                    return true;
                }
            }
            return false;
        }
        else {
            //�Ƿ�����Ҫ���µ���
            for (int j = Floor + 1; j < 5; j++) {
                if (CallDown[j] == true) {
                    return true;
                }
            }
            return false;
        }
    }
    else {
        //������·���
        if (WantTo == 1) {
            //�Ƿ�����Ҫ���ϵ���
            for (int j = 0; j < Floor; j++) {
                if (CallUp[j] == true) {
                    return true;
                }
            }
            return false;
        }
        else {
            //�Ƿ�����Ҫ���µ���
            for (int j = 0; j < Floor; j++) {
                if (CallDown[j] == true) {
                    return true;
                }
            }
            return false;
        }
    }
}
int Elevator::ShouldContinue() {
    //Ӧ�ü����������ڵ�״̬��
    //ע�⣬�����������������еĳ�����һ����
    if (ElevatorState == GoingUp) {
        if (Box->PersonNums > 0) {
            //�ǿ�
            return YesCompletely;
        }
        else {
            //�գ���������л�Ҫ��¥�ģ������������Ҫ��¥��֮�󣬻��ܵ����ߵĵط���������������Ϊ����
            if (CheckOutside(1, 1)) {
                return YesFindTheLowerUper;
            }
            else {
                return No;
            }
        }
    }
    else if (ElevatorState == GoingDown) {
        if (Box->PersonNums > 0) {
            //�ǿ�
            return YesCompletely;
        }
        else {
            //�գ���������л�Ҫ��¥�ģ������������Ҫ��¥��֮�󣬻��ܵ����͵ĵط���������������Ϊ����
            if (CheckOutside(0, 0)) {
                return YesFindTheHigherDowner;
            }
            else {
                return No;
            }
        }
    }
    else {
        //Idle��
        if (CheckOutside(0, 0) ||
            CheckOutside(0, 1) ||
            CheckOutside(1, 0) ||
            CheckOutside(1, 1) ||
            CallUp[Floor] ||
            CallDown[Floor])
        {
            //���˽е���
            return No;
        }
        else {
            return YesCompletely;
        }
    }
}
void Elevator::DeleteEveryPersonEquals(int Purpose) {
    auto trav = Box->FirstPerson;//һ�����ǿ�ָ�룬��Ϊִ����һ���������Ϊ�����ڵ����ﰴ��Ҫ����һ������
    for (; trav != nullptr; ) {
        if (trav->FloorPurpose == Purpose) {
            auto trav2 = Box->GetPrevPerson(trav);
            if (trav2 == nullptr) {
                Box->FirstPerson = trav->NextPerson;
                trav = trav->NextPerson;
            }
            else {
                trav2->NextPerson = trav->NextPerson;
                trav = trav->NextPerson;
            }
            Box->PersonNums--;
        }
        else {
            trav = trav->NextPerson;
        }
    }
}
void Elevator::refresh() {
    for (int i = 0; i < Box->PersonNums; i++) {
        PersonInTheBox[i]->setVisible(true);
    }
    for (int i = Box->PersonNums; i < MaxPersonNum; i++) {
        PersonInTheBox[i]->setVisible(false);
    }
}
int Elevator::GetLowUpFloor(int Floor) {
    for (int i = Floor + 1; i < 5; i++) {
        if (CallUp[i] || CallDown[i]) {
            return i;
        }
    }
    return 5;
}
int Elevator::GetHighDownFloor(int Floor) {
    for (int i = Floor - 1; i >= 0; i--) {
        if (CallUp[i] || CallDown[i]) {
            return i;
        }
    }
    return -1;
}