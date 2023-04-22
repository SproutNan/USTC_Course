#include "Vector.h"

Vector::Vector(int VectorTag) {
	FirstPerson = nullptr;
	PersonNums = 0;
	this->VectorTag = VectorTag;
}
Person* Vector::GetLastPerson() {
	if (FirstPerson) {
		auto trav = FirstPerson;
		while (trav->NextPerson != nullptr) {
			trav = trav->NextPerson;
		}
		return trav;
	}
	else {
		return nullptr;
	}
}
Person* Vector::GetPrevPerson(Person* p) {
	//返回p的前一个人的地址
	auto trav = FirstPerson;
	if (p == trav) {
		return nullptr;
		//表示p是第一个人，删除时要特别注意
	}
	while (1) {
		if (trav->NextPerson == p) {
			return trav;
		}
		trav = trav->NextPerson;
		//因为用到此函数的时候p一定在队列中，所以不用对尾部做判断
	}
}

int FloorY[5] = { -450,-210,30,270,510 };

void Vector::PushPerson(Person* p) {
	
	if (p->tag == 1) {
		auto getin = cocos2d::MoveTo::create(0.5, cocos2d::Vec2(-761 + 295.2 * p->ElevNo, FloorY[p->FloorNow]));
		auto delay = cocos2d::DelayTime::create(0.2);
		auto hide = cocos2d::Hide::create();
		auto call = cocos2d::CallFunc::create([=]() {
			ShouldRefresh = true;
			});
		auto seq = cocos2d::Sequence::create(getin, delay, hide, call, nullptr);

		p->runAction(seq);

	}

	// 如果是距离优先，优先接送距离大的
	if (p->AssessmentPattern == 1) {
		auto trav = FirstPerson;
		if (FirstPerson == nullptr) {
			FirstPerson = p;
			p->NextPerson = nullptr;
			PersonNums++;
			return;
		}
		else if (trav->Assessment < p->Assessment) {
			p->NextPerson = trav;
			FirstPerson = p;
			PersonNums++;
			return;
		}
		else {
			while (trav->NextPerson) {
				//trav不是最后一个元素
				if (p->Assessment <= trav->Assessment && p->Assessment >= trav->NextPerson->Assessment) {
					p->NextPerson = trav->NextPerson;
					trav->NextPerson = p;
					PersonNums++;
					return;
				}
				trav = trav->NextPerson;
			}
			trav->NextPerson = p;
			p->NextPerson = nullptr;
			PersonNums++;
			return;
		}
	}
	// 如果是时间优先
	else if (p->AssessmentPattern == 2) {
		// 如果队列是空
		if (FirstPerson == nullptr) {
			// 让头人为现在这个人
			FirstPerson = p;
		}
		// 如果队列不是空
		else {
			// 尾人的下一个人为现在这个人
			GetLastPerson()->NextPerson = p;
		}
		PersonNums++;
		p->NextPerson = nullptr;
	}
}
void Vector::DeleteFromLine(Person* p) {
	//使p所指的人从队列中出去
	auto prev = GetPrevPerson(p);
	if (prev) {
		prev->NextPerson = p->NextPerson;
	}
	else {
		FirstPerson = p->NextPerson;
	}
	PersonNums--;
	//队列人数减少
	p->NextPerson = nullptr;
}
void Vector::DeleteFromLineCleanUp(Person* p) {
	DeleteFromLine(p);
	p->removeFromParentAndCleanup(true);
}
Person* Vector::GetFirstPerson() {
	auto ans = FirstPerson;
	if (FirstPerson) {
		DeleteFromLine(FirstPerson);
	}
	return ans;
}