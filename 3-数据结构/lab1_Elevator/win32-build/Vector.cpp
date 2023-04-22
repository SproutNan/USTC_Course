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
	//����p��ǰһ���˵ĵ�ַ
	auto trav = FirstPerson;
	if (p == trav) {
		return nullptr;
		//��ʾp�ǵ�һ���ˣ�ɾ��ʱҪ�ر�ע��
	}
	while (1) {
		if (trav->NextPerson == p) {
			return trav;
		}
		trav = trav->NextPerson;
		//��Ϊ�õ��˺�����ʱ��pһ���ڶ����У����Բ��ö�β�����ж�
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

	// ����Ǿ������ȣ����Ƚ��;�����
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
				//trav�������һ��Ԫ��
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
	// �����ʱ������
	else if (p->AssessmentPattern == 2) {
		// ��������ǿ�
		if (FirstPerson == nullptr) {
			// ��ͷ��Ϊ���������
			FirstPerson = p;
		}
		// ������в��ǿ�
		else {
			// β�˵���һ����Ϊ���������
			GetLastPerson()->NextPerson = p;
		}
		PersonNums++;
		p->NextPerson = nullptr;
	}
}
void Vector::DeleteFromLine(Person* p) {
	//ʹp��ָ���˴Ӷ����г�ȥ
	auto prev = GetPrevPerson(p);
	if (prev) {
		prev->NextPerson = p->NextPerson;
	}
	else {
		FirstPerson = p->NextPerson;
	}
	PersonNums--;
	//������������
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