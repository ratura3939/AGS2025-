#include<cassert>
#include"../../Object/Attack/Arrow.h"
#include"../../Object/Attack/AttackBase.h"
#include"../../Object/Character/CharacterBase.h"
#include"../Decoration/SoundManager.h"

#include "AttackManager.h"


void AttackManager::AddAttack(const std::string _name, const ATTACK_TYPE& _type, const bool _friendFire,
	const float _total, const int _modelId, const float _start, const float _end)
{
	//‚·‚Å‚É—v‘f‚ª‚ ‚é‚Æ‚«
	if (attackInfoes_.contains(_name)) {
		//ƒGƒ‰[–h~
		assert("‚·‚Å‚É“o˜^‚µ‚Ä‚¢‚é‚à‚Ì‚ğÄ“o˜^‚µ‚æ‚¤‚Æ‚µ‚Ä‚¢‚Ü‚·");
		return;
	}


	//İ’è‚³‚ê‚½î•ñ‚©‚çUŒ‚‚ğ¶¬
	AttackInfo info = {};
	info.master = ATTACK_MASTER::NONE;
	info.type = _type;

	info.isFriendFire = _friendFire;

	info.totalMotion = _total;
	info.startAttack = _start;
	info.endAttack = _end;
	info.counter = 0.0f;
	info.isHit = false;

	//UŒ‚î•ñ‚ğ’Ç‰Á
	attackInfoes_.emplace(_name, info);
}

void AttackManager::Attack(std::string _name, const float _pow, const VECTOR& _pos, const Quaternion& _qua, const ATTACK_MASTER _master, const float _scale, const std::string _sndName, const int _arrowModel)
{
	//‚»‚à‚»‚àg—p‚µ‚½‚¢UŒ‚‚ª“o˜^‚³‚ê‚Ä‚¢‚È‚¢‚Æ‚«
	if (!attackInfoes_.contains(_name)) {
		//ƒGƒ‰[–h~
		assert("“o˜^‚³‚ê‚Ä‚¢‚È‚¢UŒ‚‚ğ”­¶‚³‚¹‚æ‚¤‚Æ‚µ‚Ä‚¢‚Ü‚·");
		return;
	}

	//‚·‚Å‚ÉUŒ‚’†‚Ì‚È‚©‚É—v‘f‚ª‚ ‚é‚Æ‚«
	if (activeAttacks_.contains(_name)) {
		//‚»‚à‚»‚à‚ ‚é‚Ì‚Åˆ—‚µ‚È‚¢
		return;
	}

	//î•ñ‚Ì’Ç‰Á
	attackInfoes_[_name].scale = _scale;
	attackInfoes_[_name].master = _master;

	//UŒ‚”»’è‚Ì¶¬
	//Œ•‚Ìê‡
	if (attackInfoes_[_name].type == ATTACK_TYPE::SWORD) {
		activeAttacks_.emplace(_name, std::make_shared<AttackBase>(_pos, _pow));
	}
	//‹|‚Ìê‡
	else if (attackInfoes_[_name].type == ATTACK_TYPE::BOW) {
		//”O‚Ì‚½‚ß‚Ì—\–hô
		if (_arrowModel == -1) {
			assert("‹|‚Ìƒ‚ƒfƒ‹‚ªİ’è‚³‚ê‚Ä‚¢‚Ü‚¹‚ñ");
		}
		//‹|–î‚Ìì¬
		//arrows_.push_back(std::make_unique<Arrow>(attackInfoes_[_name].master, _arrowModel, _pos, _pow, _qua));
	}

	//‰½‚©Ä¶‚·‚é•¨‚ª‚ ‚éê‡
	if (_sndName != "") {
		//Œø‰Ê‰¹‚ÌÄ¶
		SoundManager::GetInstance().Play(_sndName);
	}
	
}

bool AttackManager::Update(void)
{
	//íœ€–Ú‹L‰¯—p
	std::vector<std::string>deleteIndex;

	//UŒ‚XVˆ—iƒAƒNƒeƒBƒu‚É‚È‚Á‚Ä‚¢‚éUŒ‚•ªj
	for (auto& atk : activeAttacks_) {
		AttackInfo& info = attackInfoes_[atk.first];
		//ƒJƒEƒ“ƒ^[‚ªãŒÀ‚æ‚èã‚¾‚Á‚½‚ç
		if (info.counter >= info.totalMotion) {
			//I—¹
			info.counter = 0;
			info.isHit = false;
			//íœ€–Ú‚É’Ç‰Á
			deleteIndex.push_back(atk.first);
			continue;
		}
		//ƒJƒEƒ“ƒ^[‚ÌXV
		info.counter++;
	}

	//íœ
	for (auto& idx : deleteIndex) {
		activeAttacks_.erase(idx);
	}


	//‹|–î‚ÌXV
	//for (auto& arrow : arrows_) {
	//	arrow->Update();
	//	//‹|–î‚ªÁ–Å‚µ‚½‚ç
	//	//”z—ñ‚©‚çíœ‚·‚é
	//}

	return true;
}

std::vector<AttackManager::AttackCollision> AttackManager::GetActiveAttacks(void)
{
	std::vector<AttackCollision>retVector;
	for (auto& atk : activeAttacks_) {
		AttackCollision ret = { attackInfoes_[atk.first],atk.second };
		retVector.push_back(ret);
	}

	return retVector;
}

const float AttackManager::GetTotalTime(const std::string _name) const
{
	//—v‘f‚ª‚È‚¢‚Æ‚«
	if (!attackInfoes_.contains(_name)) {
		return -1.0f;
	}
	return attackInfoes_.at(_name).totalMotion;
}

void AttackManager::DrawDebug(void)
{
	int color = 0xff00ff;
	for (auto& atk : activeAttacks_) {

		auto& info = attackInfoes_[atk.first];
		if (info.master == ATTACK_MASTER::ENEMY) {
			if (info.counter < info.startAttack) {
				color = 0x00ff00;
			}
			else if (info.counter >= info.endAttack) {
				color = 0x0000ff;
			}
			else color = 0xff00ff;
		}
		//ƒfƒoƒbƒO—p‚Ì‹…‘Ì‚ğ•`‰æ
		DrawSphere3D(atk.second->GetPos(), static_cast<int>(info.scale), 8, color, color, false);
	}
	
}
