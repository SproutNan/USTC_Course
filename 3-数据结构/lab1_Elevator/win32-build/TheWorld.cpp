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

	// 分发一个点击事件
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
	//检查是否有停在Floor楼的向上的门开的电梯
	for (int i = 0; i < ELEVATOR_NUM; i++) {
		if (elev[i]->Floor == Floor && elev[i]->FloorAddition == false) {
			if (elev[i]->ElevatorState == Elevator::GoingUp && elev[i]->DoorState == Elevator::Opened) {
				if (elev[i]->Box->PersonNums < MaxPersonNum) {
					return i;//有空位置才返回
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
					return i;//有空位置才返回
				}
			}
		}
	}
	return -1;
}
void TheWorld::PersonOperate(Person* p) {
	if (p) {
		//按下呼叫电梯按钮, 电梯外的人
		if (p->tag == 0) {
			p->TolerateTimeNums--;
			if (p->ToUp) {
				//要上楼，把每一架电梯的上按了
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
				//要下楼，把每一架电梯的下按了
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
		//这人等不下去了，跑了，从所在容器中删除此人
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
	// 时钟向前挪动
	Counter -= Interval;
	board->Counter = (int)(Counter * 10);
	// 计算下一个人来到的计时器减少

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

	//对外面等待队列中的人做操作
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

	//对每个电梯内部的人做操作
	for (int i = 0; i < ELEVATOR_NUM; i++) {
		auto trav_inLine = elev[i]->Box->FirstPerson;
		for (int j = 0; trav_inLine != nullptr; trav_inLine = trav_inLine->NextPerson) {
			PersonOperate(trav_inLine);
		}
	}

	//对每部电梯做操作
	for (int j = 0; j < ELEVATOR_NUM; j++) {
		ElevatorOperate(elev[j]);
	}
}

void TheWorld::DisableCallAllElevators(int Floor, int Up) {
	//第二个参数为1表示上，为0表示下
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
	//检查是否在楼层之间
	bool tag1 = false;
	for (int i = 0; i < FLOOR_NUM; i++) {
		//cocos2d::log("Check:%d", (int)(e->getPosition().y));
		if ((int)(e->getPosition().y) == FloorY[i]) {
			e->FloorAddition = false;//不在楼层之间
			e->Floor = i;
			tag1 = true;
			break;
		}
	}
	if (tag1 == false) {
		//如果上面都不在楼层之间，则
		e->FloorAddition = true;
	}
}
void TheWorld::ElevatorOperate(Elevator* e) {
	CheckAddition(e);//更新电梯楼层状态

	/*当且仅当四个条件都满足的时候把状态设置成Idle:
	* 1.不在楼层中间，也就是停靠在某一层
	* 2.门是关着的
	* 3.当前不是Idle状态
	* 4.不应该再保持先前的状态
	*/
	if (e->FloorAddition == false &&
		e->DoorState == Elevator::Closed &&
		e->ElevatorState != Elevator::Idle &&
		e->Box->PersonNums == 0) 
	{
		e->ElevatorState = Elevator::Idle;
	}
	//电梯此时不在楼层之间，停在某一层了。
	if (e->FloorAddition == false) {
		//如果当前是关门的，要看一看应不应该开门
		if (e->DoorState == Elevator::Closed) {
			e->refresh();
			//如果是上，只有有要在这层楼下的或者这层楼有要上电梯上去的才需要开门
			if (e->ElevatorState == Elevator::GoingUp) {
				//满员
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
					//如果在这一层楼任务还没完成，就继续向上移动一点点
					if (e->ShouldContinue() == Elevator::YesCompletely) {
						e->MoveOneFloor(SpeedOfElevator);
					}
				}
				//不满员
				else {
					//有人要在这里下楼或者有人要在这里上楼
					if (e->CallCar[e->Floor] || e->CallUp[e->Floor]) {
						e->DoorState = Elevator::Opened;
						cocos2d::log(
							"[%.1f]Elevator[%d] stop at floor[%d] and opened its door!",
							Time, e->ElevatorNo, e->Floor
						);
						e->Counter = e->WaitForInPeopleTimeNum;
						//如果是有人要在这一层出去，呼叫按钮熄灭，把要在这一层下楼的人全部干下去
						if (e->CallCar[e->Floor]) {
							e->CallCar[e->Floor] = false;
							e->DeleteEveryPersonEquals(e->Floor);
						}
						//关闭所有电梯的呼叫上楼按钮
						if (e->CallUp[e->Floor]) {
							DisableCallAllElevators(e->Floor, 1);
						}
					}
					//没人要开门
					else {
						//如果在这一层楼任务还没完成，就继续向上移动一点点
						if (e->ShouldContinue() == Elevator::YesCompletely) {
							e->MoveOneFloor(SpeedOfElevator);
						}
					}
				}
			}
			//如果是下，只有有要在这层楼下的或者这层楼有要上电梯下去的才需要开门
			else if (e->ElevatorState == Elevator::GoingDown) {
				//满员
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
					//如果在这一层楼任务还没完成，就继续向下移动一点点
					if (e->ShouldContinue() == Elevator::YesCompletely) {
						e->MoveOneFloor(-1 * SpeedOfElevator);
					}
				}
				//不满员
				else {
					if (e->CallCar[e->Floor] || e->CallDown[e->Floor]) {
						e->DoorState = Elevator::Opened;
						e->Counter = e->WaitForInPeopleTimeNum;
						//如果是有人要在这一层出去，呼叫按钮熄灭，把要在这一层下楼的人全部干下去
						if (e->CallCar[e->Floor]) {
							e->CallCar[e->Floor] = false;
							e->DeleteEveryPersonEquals(e->Floor);
							cocos2d::log(
								"[%.1f]Elevator[%d] stop at floor[%d] and opened its door!",
								Time, e->ElevatorNo, e->Floor
							);
						}
						//关闭所有电梯的呼叫下楼按钮
						if (e->CallDown[e->Floor]) {
							DisableCallAllElevators(e->Floor, 0);
						}
					}
					//没人要开门
					else {
						//如果在这一层楼任务还没完成，就继续向下移动一点点
						if (e->ShouldContinue() == Elevator::YesCompletely) {
							e->MoveOneFloor(-1 * SpeedOfElevator);
						}
					}
				}
			}
			//如果当前电梯状态是Idle，如果这一层有人要上下，就要把门打开
			else if (e->ElevatorState == Elevator::Idle) {
				//当前楼层有无上下，优先取上楼的
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
				//当前楼层没有要上下的
				else {
					//表示当前楼层没有要上下的，看上面楼层有没有要上下的
					if (e->ElevatorNo == 0) {
						//如果这台电梯上面有人呼叫
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
									//不操作
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
						//如果这台电梯下面有人呼叫
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
									//不操作
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
						//可能需要这一台电梯去上面接，但是第一台电梯去了吗？
						if (e->CheckOutside(1, 0) || e->CheckOutside(1, 1)) {
							if (elev[0]->ElevatorState == Elevator::GoingDown && elev[0]->Floor< e->GetLowUpFloor(e->Floor)) {
								e->ElevatorState = Elevator::GoingUp;
								e->MoveOneFloor(SpeedOfElevator);
								
							}
							if (elev[0]->ElevatorState == Elevator::GoingUp && elev[0]->Floor > e->GetLowUpFloor(e->Floor)) {
								e->ElevatorState = Elevator::GoingUp;
								e->MoveOneFloor(SpeedOfElevator);
							}
							//其他情况不操作
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
							//其他情况不操作
						}
					}
				}
			}
		}
		if (e->DoorState == Elevator::Opened) {
			//开了门
			//门打开的时候已经在Closed状态时设置了计时器，所以当门打开的时候每执行一次无论如何Counter--
			e->Counter--;
			//是否应该关门到下面再检测
			
			//如果此时电梯正在执行上行任务
			if (e->ElevatorState == Elevator::GoingUp) {
				//还能乘坐人
				if (e->Box->PersonNums < MaxPersonNum) {
					//如果这层楼有人想上
					auto person = UpLine[e->Floor]->GetFirstPerson();
					if (person) {
						e->Counter = e->WaitForInPeopleTimeNum;
						//这个人新进来，所以计时器重置
						person->tag = 1;
						person->ElevNo = e->ElevatorNo;
						e->Box->PushPerson(person);
						board->AddNumberToAverage(person->FloorNow, person->TolerateHistory - person->TolerateTimeNums);
						board->AddNumberToAverage(5, person->TolerateHistory - person->TolerateTimeNums);
						//把这个人放到电梯里面
						e->CallCar[person->FloorPurpose] = true;
						//这人要按电梯中自己要去的地方
						DisableCallAllElevators(e->Floor, 1);
						//只执行一次，如果外面还有要上去的人，他会再按电梯按钮的，电梯给了这种情况缓冲时间
					}
				}
				//人满了，立即把计时器取消，准备关门
				else {
					//e->Counter = 0;
					//[MARK]这一句话可能导致一些看起来的问题，因为按道理来说到了一个地方之后，如果满
					//员，在某一层楼放一些人，又刚好上一些人，结果将导致电梯马上开走
				}
				if (e->Counter <= 0) {
					//该关门了
					e->Counter = 0;
					e->DoorState = Elevator::Closed;
					cocos2d::log(
						"[%.1f]Elevator[%d] closed its door!",
						Time, e->ElevatorNo
					);
				}
			}
			else if(e->ElevatorState == Elevator::GoingDown){
				//还能乘坐人
				if (e->Box->PersonNums < MaxPersonNum) {
					//如果这层楼有人想下
					auto person = DownLine[e->Floor]->GetFirstPerson();
					if (person) {
						e->Counter = e->WaitForInPeopleTimeNum;
						//这个人新进来，所以计时器重置
						person->tag = 1;
						person->ElevNo = e->ElevatorNo;
						e->Box->PushPerson(person);
						board->AddNumberToAverage(person->FloorNow, person->TolerateHistory - person->TolerateTimeNums);
						board->AddNumberToAverage(5, person->TolerateHistory - person->TolerateTimeNums);
						//把这个人放到电梯里面
						e->CallCar[person->FloorPurpose] = true;
						//这人要按电梯中自己要去的地方
						DisableCallAllElevators(e->Floor, -1);
						//只执行一次，如果外面还有要下去的人，他会再按电梯按钮的，电梯给了这种情况缓冲时间
					}
				}
				//人满了，立即把计时器取消，准备关门
				else {
					//e->Counter = 0;
				}
				if (e->Counter <= 0) {
					//该关门了
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