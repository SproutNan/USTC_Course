#include "TheWorld.h"
#include <cmath>

TheWorld::TheWorld(int NextArrivalL, int NextArrivalR, int TolerateTimeL, int TolerateTimeR, int WaitingTime, int SpeedOfElevator) {
	this->NextArrivalL = NextArrivalL;
	this->NextArrivalR = NextArrivalR;
	this->TolerateTimeL = TolerateTimeL;
	this->TolerateTimeR = TolerateTimeR;
	this->WaitingTime = WaitingTime;
	this->SpeedOfElevator = SpeedOfElevator;

	for (int i = 0; i < ELEVATOR_NUM; i++) {
		elev[i] = new Elevator(i, this->SpeedOfElevator);
	}
	for (int i = 0; i < FLOOR_NUM; i++) {
		UpLine[i] = new Vector(Vector::WLU);
		DownLine[i] = new Vector(Vector::WLD);
	}

	Boundry = cocos2d::Sprite::create("Elevator_Surface.png");
	Boundry->setPosition(0, 0);
	this->addChild(Boundry, 3);

	for (int i = 0; i < ELEVATOR_NUM; i++) {
		elev[i]->setPosition(-761 + 295.2 * i, FloorY[1]);
		this->addChild(elev[i], 2);
	}
	for (int i = 0; i < ELEVATOR_NUM; i++) {
		ElevLabel[i] = cocos2d::Label::createWithTTF("  ", "fonts/Marker Felt.ttf", 20);
		ElevLabel[i]->setPosition(-830 + 800 * i, 300);
		ElevLabel[i]->setTextColor(cocos2d::Color4B::WHITE);
		ElevLabel[i]->enableOutline(cocos2d::Color4B::BLUE, 3);
		this->addChild(ElevLabel[i]);
	}

	// �ַ�һ������¼�
	auto TouchListener = cocos2d::EventListenerTouchOneByOne::create();
	TouchListener->onTouchBegan = CC_CALLBACK_2(TheWorld::UPDATE_Before, this);
	TouchListener->onTouchEnded = CC_CALLBACK_2(TheWorld::UPDATE, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(TouchListener, this);

	this->addChild(board, 999);

	scheduleUpdate();

}
bool TheWorld::UPDATE_Before(cocos2d::Touch* touch, cocos2d::Event* event) {
	return true;
}
void TheWorld::UPDATE(cocos2d::Touch* touch, cocos2d::Event* event) {
	if (board->isVisible() == false) {
		board->setVisible(true);
	}
	else {
		board->setVisible(false);
	}
}
int TheWorld::CheckUpingElevators(int Floor) {
	//����Ƿ���ͣ��Floor¥�����ϵ��ſ��ĵ���
	for (int i = 0; i < ELEVATOR_NUM; i++) {
		if (elev[i]->Floor == Floor && elev[i]->FloorAddition == false) {
			if (elev[i]->ElevatorState == Elevator::GoingUp && elev[i]->DoorState == Elevator::Opened) {
				if (elev[i]->Box->PersonNums < MaxPersonNum) {
					return i;//�п�λ�òŷ���
				}
			}
		}
	}
	return -1;
}
int TheWorld::CheckDownElevators(int Floor) {
	for (int i = 0; i < ELEVATOR_NUM; i++) {
		if (elev[i]->Floor == Floor && elev[i]->FloorAddition == false) {
			if (elev[i]->ElevatorState == Elevator::GoingDown && elev[i]->DoorState == Elevator::Opened) {
				if (elev[i]->Box->PersonNums < MaxPersonNum) {
					return i;//�п�λ�òŷ���
				}
			}
		}
	}
	return -1;
}
void TheWorld::PersonOperate(Person* p) {
	if (p) {
		//���º��е��ݰ�ť, ���������
		if (p->tag == 0) {
			p->TolerateTimeNums--;
			if (p->ToUp) {
				//Ҫ��¥����ÿһ�ܵ��ݵ��ϰ���
				for (int i = 0; i < ELEVATOR_NUM; i++) {
					if (!elev[i]->CallUp[p->FloorNow]) {
						elev[i]->CallUp[p->FloorNow] = true;
						cocos2d::log(
							"[%.1f]Person[%s][%p] from %d floor press %d# elevator's %d button!", 
							Time, p->Name.c_str(), p, 
							p->FloorNow, i, p->ToUp
						);
					}
				}
			}
			else {
				//Ҫ��¥����ÿһ�ܵ��ݵ��°���
				for (int i = 0; i < ELEVATOR_NUM; i++) {
					if (!elev[i]->CallDown[p->FloorNow]) {
						elev[i]->CallDown[p->FloorNow] = true;
						cocos2d::log(
							"[%.1f]Person[%s][%p] from %d floor press %d# elevator's %d button!",
							Time, p->Name.c_str(), p,
							p->FloorNow, i, p->ToUp
						);
					}
				}
			}
		}
		//���˵Ȳ���ȥ�ˣ����ˣ�������������ɾ������
		if (p->tag == 0 && p->TolerateTimeNums < 0) {
			cocos2d::log(
				"[%.1f]Person[%s][%p] from %d floor departed from the line.", 
				Time, p->Name.c_str(), p, p->FloorNow
			);
			board->DepartedTotal++;
			board->AddNumberToAverage(5, p->TolerateHistory);
			if (p->ToUp == 1) {
				UpLine[p->FloorNow]->DeleteFromLineCleanUp(p);
			}
			else {
				DownLine[p->FloorNow]->DeleteFromLineCleanUp(p);
			}
		}
	}
}
int Rand(int l, int r) {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> distrib(l, r);
	return distrib(gen);
}

void TheWorld::update(float dt) {
	Time += Interval;
	board->TimePassed++;
	// ʱ����ǰŲ��
	Counter -= Interval;
	board->Counter = (int)(Counter * 10);
	// ������һ���������ļ�ʱ������

	if (Counter < 0) {
		int RandFloor = Rand(0, 4);
		auto NewPerson = new Person(RandFloor, TolerateTimeL, TolerateTimeR, AssessmentPattern);
		NewPerson->setPosition(Rand(-331, 769), -480 + RandFloor * 240);
		this->addChild(NewPerson, 4);
		if (NewPerson->ToUp == 1) {
			UpLine[RandFloor]->PushPerson(NewPerson);
		}
		else {
			DownLine[RandFloor]->PushPerson(NewPerson);
		}
		int NextCounter = Rand(NextArrivalL, NextArrivalR);
		Counter += (float)NextCounter;
		board->VisitorTotal++;
	}

	//������ȴ������е���������
	for (int i = 0; i < FLOOR_NUM; i++) {
		auto trav_inLine = UpLine[i]->FirstPerson;
		for (int j = 0; trav_inLine != nullptr; trav_inLine = trav_inLine->NextPerson) {
			PersonOperate(trav_inLine);
		}
		trav_inLine = DownLine[i]->FirstPerson;
		for (int j = 0; trav_inLine != nullptr; trav_inLine = trav_inLine->NextPerson) {
			PersonOperate(trav_inLine);
		}
	}

	//��ÿ�������ڲ�����������
	for (int i = 0; i < ELEVATOR_NUM; i++) {
		auto trav_inLine = elev[i]->Box->FirstPerson;
		for (int j = 0; trav_inLine != nullptr; trav_inLine = trav_inLine->NextPerson) {
			PersonOperate(trav_inLine);
		}
	}

	//��ÿ������������
	for (int j = 0; j < ELEVATOR_NUM; j++) {
		ElevatorOperate(elev[j]);
	}
}

void TheWorld::DisableCallAllElevators(int Floor, int Up) {
	//�ڶ�������Ϊ1��ʾ�ϣ�Ϊ0��ʾ��
	for (int i = 0; i < ELEVATOR_NUM; i++) {
		if (Up) {
			this->elev[i]->CallUp[Floor] = false;
		}
		else {
			this->elev[i]->CallDown[Floor] = false;
		}
	}
}
void TheWorld::CheckAddition(Elevator* e) {
	//����Ƿ���¥��֮��
	bool tag1 = false;
	for (int i = 0; i < FLOOR_NUM; i++) {
		//cocos2d::log("Check:%d", (int)(e->getPosition().y));
		if ((int)(e->getPosition().y) == FloorY[i]) {
			e->FloorAddition = false;//����¥��֮��
			e->Floor = i;
			tag1 = true;
			break;
		}
	}
	if (tag1 == false) {
		//������涼����¥��֮�䣬��
		e->FloorAddition = true;
	}
}
void TheWorld::ElevatorOperate(Elevator* e) {
	CheckAddition(e);//���µ���¥��״̬

	/*���ҽ����ĸ������������ʱ���״̬���ó�Idle:
	* 1.����¥���м䣬Ҳ����ͣ����ĳһ��
	* 2.���ǹ��ŵ�
	* 3.��ǰ����Idle״̬
	* 4.��Ӧ���ٱ�����ǰ��״̬
	*/
	if (e->FloorAddition == false &&
		e->DoorState == Elevator::Closed &&
		e->ElevatorState != Elevator::Idle &&
		e->Box->PersonNums == 0) 
	{
		e->ElevatorState = Elevator::Idle;
	}
	//���ݴ�ʱ����¥��֮�䣬ͣ��ĳһ���ˡ�
	if (e->FloorAddition == false) {
		//�����ǰ�ǹ��ŵģ�Ҫ��һ��Ӧ��Ӧ�ÿ���
		if (e->DoorState == Elevator::Closed) {
			e->refresh();
			//������ϣ�ֻ����Ҫ�����¥�µĻ������¥��Ҫ�ϵ�����ȥ�Ĳ���Ҫ����
			if (e->ElevatorState == Elevator::GoingUp) {
				//��Ա
				if (e->Box->PersonNums == MaxPersonNum) {
					if (e->CallCar[e->Floor]) {
						e->DoorState = Elevator::Opened;
						cocos2d::log(
							"[%.1f]Elevator[%d] stop at floor[%d] and opened its door!",
							Time, e->ElevatorNo, e->Floor
						);
						e->Counter = e->WaitForInPeopleTimeNum;
						e->CallCar[e->Floor] = false;
						e->DeleteEveryPersonEquals(e->Floor);
					}
					cocos2d::log(
						"[%.1f]Elevator[%d] is full!",
						Time, e->ElevatorNo
					);
					//�������һ��¥����û��ɣ��ͼ��������ƶ�һ���
					if (e->ShouldContinue() == Elevator::YesCompletely) {
						e->MoveOneFloor(SpeedOfElevator);
					}
				}
				//����Ա
				else {
					//����Ҫ��������¥��������Ҫ��������¥
					if (e->CallCar[e->Floor] || e->CallUp[e->Floor]) {
						e->DoorState = Elevator::Opened;
						cocos2d::log(
							"[%.1f]Elevator[%d] stop at floor[%d] and opened its door!",
							Time, e->ElevatorNo, e->Floor
						);
						e->Counter = e->WaitForInPeopleTimeNum;
						//���������Ҫ����һ���ȥ�����а�ťϨ�𣬰�Ҫ����һ����¥����ȫ������ȥ
						if (e->CallCar[e->Floor]) {
							e->CallCar[e->Floor] = false;
							e->DeleteEveryPersonEquals(e->Floor);
						}
						//�ر����е��ݵĺ�����¥��ť
						if (e->CallUp[e->Floor]) {
							DisableCallAllElevators(e->Floor, 1);
						}
					}
					//û��Ҫ����
					else {
						//�������һ��¥����û��ɣ��ͼ��������ƶ�һ���
						if (e->ShouldContinue() == Elevator::YesCompletely) {
							e->MoveOneFloor(SpeedOfElevator);
						}
					}
				}
			}
			//������£�ֻ����Ҫ�����¥�µĻ������¥��Ҫ�ϵ�����ȥ�Ĳ���Ҫ����
			else if (e->ElevatorState == Elevator::GoingDown) {
				//��Ա
				if (e->Box->PersonNums == MaxPersonNum) {
					if (e->CallCar[e->Floor]) {
						e->DoorState = Elevator::Opened;
						e->Counter = e->WaitForInPeopleTimeNum;
						e->CallCar[e->Floor] = false;
						e->DeleteEveryPersonEquals(e->Floor);
						cocos2d::log(
							"[%.1f]Elevator[%d] stop at floor[%d] and opened its door!",
							Time, e->ElevatorNo, e->Floor
						);
					}
					//�������һ��¥����û��ɣ��ͼ��������ƶ�һ���
					if (e->ShouldContinue() == Elevator::YesCompletely) {
						e->MoveOneFloor(-1 * SpeedOfElevator);
					}
				}
				//����Ա
				else {
					if (e->CallCar[e->Floor] || e->CallDown[e->Floor]) {
						e->DoorState = Elevator::Opened;
						e->Counter = e->WaitForInPeopleTimeNum;
						//���������Ҫ����һ���ȥ�����а�ťϨ�𣬰�Ҫ����һ����¥����ȫ������ȥ
						if (e->CallCar[e->Floor]) {
							e->CallCar[e->Floor] = false;
							e->DeleteEveryPersonEquals(e->Floor);
							cocos2d::log(
								"[%.1f]Elevator[%d] stop at floor[%d] and opened its door!",
								Time, e->ElevatorNo, e->Floor
							);
						}
						//�ر����е��ݵĺ�����¥��ť
						if (e->CallDown[e->Floor]) {
							DisableCallAllElevators(e->Floor, 0);
						}
					}
					//û��Ҫ����
					else {
						//�������һ��¥����û��ɣ��ͼ��������ƶ�һ���
						if (e->ShouldContinue() == Elevator::YesCompletely) {
							e->MoveOneFloor(-1 * SpeedOfElevator);
						}
					}
				}
			}
			//�����ǰ����״̬��Idle�������һ������Ҫ���£���Ҫ���Ŵ�
			else if (e->ElevatorState == Elevator::Idle) {
				//��ǰ¥���������£�����ȡ��¥��
				if (e->CallUp[e->Floor] == true) {
					e->DoorState = Elevator::Opened;
					e->Counter = e->WaitForInPeopleTimeNum;
					e->ElevatorState = Elevator::GoingUp;
					DisableCallAllElevators(e->Floor, 1);
					cocos2d::log(
						"[%.1f]Elevator[%d] opened its door and start going up!",
						Time, e->ElevatorNo
					);
				}
				else if (e->CallDown[e->Floor] == true) {
					e->DoorState = Elevator::Opened;
					e->Counter = e->WaitForInPeopleTimeNum;
					e->ElevatorState = Elevator::GoingDown;
					cocos2d::log(
						"[%.1f]Elevator[%d] opened its door and start going down!",
						Time, e->ElevatorNo
					);
					DisableCallAllElevators(e->Floor, 0);
				}
				//��ǰ¥��û��Ҫ���µ�
				else {
					//��ʾ��ǰ¥��û��Ҫ���µģ�������¥����û��Ҫ���µ�
					if (e->ElevatorNo == 0) {
						//�����̨�����������˺���
						if (e->CheckOutside(1, 0) || e->CheckOutside(1, 1)) {
							if (elev[1]->ElevatorState == Elevator::GoingDown) {
								e->ElevatorState = Elevator::GoingUp;
								e->MoveOneFloor(SpeedOfElevator);

							}
							if (elev[1]->ElevatorState == Elevator::GoingUp &&
								elev[1]->Floor > e->GetLowUpFloor(e->Floor)) {
								e->ElevatorState = Elevator::GoingUp;
								e->MoveOneFloor(SpeedOfElevator);

							}
							if (elev[1]->ElevatorState == Elevator::Idle) {
								int floor = e->GetLowUpFloor(e->Floor);
								if (floor < elev[1]->Floor) {
									e->ElevatorState = Elevator::GoingUp;
									e->MoveOneFloor(SpeedOfElevator);
								}
								if (floor == elev[1]->Floor) {
									//������
								}
								if (floor > elev[1]->Floor) {
									if (elev[1]->Floor < e->Floor) {
										e->ElevatorState = Elevator::GoingUp;
										e->MoveOneFloor(SpeedOfElevator);
									}
									else {
										elev[1]->ElevatorState = Elevator::GoingUp;
										elev[1]->MoveOneFloor(SpeedOfElevator);
									}
								}
							}
						}
						//�����̨�����������˺���
						else if (e->CheckOutside(0, 0) || e->CheckOutside(0, 1)) {
							if (elev[1]->ElevatorState == Elevator::GoingUp) {
								e->ElevatorState = Elevator::GoingDown;
								e->MoveOneFloor(-1 * SpeedOfElevator);
								
							}
							if (elev[1]->ElevatorState == Elevator::GoingDown &&
								elev[1]->Floor < e->GetHighDownFloor(e->Floor)) {
								e->ElevatorState = Elevator::GoingDown;
								e->MoveOneFloor(-1 * SpeedOfElevator);
								
							}
							if (elev[1]->ElevatorState == Elevator::Idle) {
								int floor = e->GetHighDownFloor(e->Floor);
								cocos2d::log("%d", floor);
								if (floor > elev[1]->Floor) {
									e->ElevatorState = Elevator::GoingDown;
									e->MoveOneFloor(-1 * SpeedOfElevator);
									
								}
								if (floor == elev[1]->Floor) {
									//������
								}
								if (floor < elev[1]->Floor) {
									if (elev[1]->Floor > e->Floor) {
										e->ElevatorState = Elevator::GoingDown;
										e->MoveOneFloor(-1 * SpeedOfElevator);
									}
									else {
										elev[1]->ElevatorState = Elevator::GoingDown;
										elev[1]->MoveOneFloor(-1 * SpeedOfElevator);
									}
								}
							}
						}
					}
					if (e->ElevatorNo == 1) {
						//������Ҫ��һ̨����ȥ����ӣ����ǵ�һ̨����ȥ����
						if (e->CheckOutside(1, 0) || e->CheckOutside(1, 1)) {
							if (elev[0]->ElevatorState == Elevator::GoingDown && elev[0]->Floor< e->GetLowUpFloor(e->Floor)) {
								e->ElevatorState = Elevator::GoingUp;
								e->MoveOneFloor(SpeedOfElevator);
								
							}
							if (elev[0]->ElevatorState == Elevator::GoingUp && elev[0]->Floor > e->GetLowUpFloor(e->Floor)) {
								e->ElevatorState = Elevator::GoingUp;
								e->MoveOneFloor(SpeedOfElevator);
							}
							//�������������
						}
						else if (e->CheckOutside(0, 0) || e->CheckOutside(0, 1)) {
							if (elev[0]->ElevatorState == Elevator::GoingUp && elev[0]->Floor > e->GetHighDownFloor(e->Floor)) {
								e->ElevatorState = Elevator::GoingDown;
								e->MoveOneFloor(-1 * SpeedOfElevator);
							}
							if (elev[0]->ElevatorState == Elevator::GoingDown && elev[0]->Floor < e->GetHighDownFloor(e->Floor)) {
								e->ElevatorState = Elevator::GoingDown;
								e->MoveOneFloor(-1 * SpeedOfElevator);
							}
							//�������������
						}
					}
				}
			}
		}
		if (e->DoorState == Elevator::Opened) {
			//������
			//�Ŵ򿪵�ʱ���Ѿ���Closed״̬ʱ�����˼�ʱ�������Ե��Ŵ򿪵�ʱ��ÿִ��һ���������Counter--
			e->Counter--;
			//�Ƿ�Ӧ�ù��ŵ������ټ��
			
			//�����ʱ��������ִ����������
			if (e->ElevatorState == Elevator::GoingUp) {
				//���ܳ�����
				if (e->Box->PersonNums < MaxPersonNum) {
					//������¥��������
					auto person = UpLine[e->Floor]->GetFirstPerson();
					if (person) {
						e->Counter = e->WaitForInPeopleTimeNum;
						//������½��������Լ�ʱ������
						person->tag = 1;
						person->ElevNo = e->ElevatorNo;
						e->Box->PushPerson(person);
						board->AddNumberToAverage(person->FloorNow, person->TolerateHistory - person->TolerateTimeNums);
						board->AddNumberToAverage(5, person->TolerateHistory - person->TolerateTimeNums);
						//������˷ŵ���������
						e->CallCar[person->FloorPurpose] = true;
						//����Ҫ���������Լ�Ҫȥ�ĵط�
						DisableCallAllElevators(e->Floor, 1);
						//ִֻ��һ�Σ�������滹��Ҫ��ȥ���ˣ������ٰ����ݰ�ť�ģ����ݸ��������������ʱ��
					}
				}
				//�����ˣ������Ѽ�ʱ��ȡ����׼������
				else {
					//e->Counter = 0;
					//[MARK]��һ�仰���ܵ���һЩ�����������⣬��Ϊ��������˵����һ���ط�֮�������
					//Ա����ĳһ��¥��һЩ�ˣ��ָպ���һЩ�ˣ���������µ������Ͽ���
				}
				if (e->Counter <= 0) {
					//�ù�����
					e->Counter = 0;
					e->DoorState = Elevator::Closed;
					cocos2d::log(
						"[%.1f]Elevator[%d] closed its door!",
						Time, e->ElevatorNo
					);
				}
			}
			else if(e->ElevatorState == Elevator::GoingDown){
				//���ܳ�����
				if (e->Box->PersonNums < MaxPersonNum) {
					//������¥��������
					auto person = DownLine[e->Floor]->GetFirstPerson();
					if (person) {
						e->Counter = e->WaitForInPeopleTimeNum;
						//������½��������Լ�ʱ������
						person->tag = 1;
						person->ElevNo = e->ElevatorNo;
						e->Box->PushPerson(person);
						board->AddNumberToAverage(person->FloorNow, person->TolerateHistory - person->TolerateTimeNums);
						board->AddNumberToAverage(5, person->TolerateHistory - person->TolerateTimeNums);
						//������˷ŵ���������
						e->CallCar[person->FloorPurpose] = true;
						//����Ҫ���������Լ�Ҫȥ�ĵط�
						DisableCallAllElevators(e->Floor, -1);
						//ִֻ��һ�Σ�������滹��Ҫ��ȥ���ˣ������ٰ����ݰ�ť�ģ����ݸ��������������ʱ��
					}
				}
				//�����ˣ������Ѽ�ʱ��ȡ����׼������
				else {
					//e->Counter = 0;
				}
				if (e->Counter <= 0) {
					//�ù�����
					e->Counter = 0;
					e->DoorState = Elevator::Closed;
					cocos2d::log(
						"[%.1f]Elevator[%d] closed its door!",
						Time, e->ElevatorNo
					);
				}
			}
		}
	}
	if (e->FloorAddition == true) {
		if (e->ElevatorState == Elevator::GoingUp) {
			e->setPosition(e->getPosition().x, e->getPosition().y + SpeedOfElevator);
		}
		if (e->ElevatorState == Elevator::GoingDown) {
			e->setPosition(e->getPosition().x, e->getPosition().y - SpeedOfElevator);
		}
	}
}