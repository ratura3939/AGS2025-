#include<cassert>
#include<map>
#include"../../Object/Character/CharacterBase.h"
#include"../Decoration/SoundManager.h"

#include "AttackManager.h"

void AttackManager::AddAttackCollider(const std::string& _name, std::weak_ptr<Collider> _col, const bool _friendFire, const float _totalTime, const float _start, const float _end)
{
	//Šù‚É—v‘f‚ª‚ ‚é‚Æ‚«
	if(attackColliders_.contains(_name)){
		//ƒGƒ‰[–hŽ~
		assert("‚·‚Å‚É“o˜^‚µ‚Ä‚¢‚é‚à‚Ì‚ðÄ“o˜^‚µ‚æ‚¤‚Æ‚µ‚Ä‚¢‚Ü‚·");
		return;
	}

	//UŒ‚—pƒRƒ‰ƒCƒ_[î•ñ‚ÌŠi”[
	AttackInfo addInfo;
	addInfo.collider = _col;
	addInfo.totalTime = _totalTime;
	addInfo.startTime = _start;
	addInfo.endTime = _end;
	if(_end==0.0f){
		addInfo.endTime = _totalTime;
	}
	addInfo.counter = 0.0f;
	addInfo.isUsed = false;
	addInfo.isAllert = false;

	attackColliders_.emplace(_name, addInfo);

	UpdateAttack_f addUpdate = &AttackManager::UpdatePreAttack;
	updateAtk_.emplace(_name, addUpdate);

	//FFÝ’è
	//ƒvƒŒƒCƒ„[–”‚Í“G‚Ì‚Ç‚¿‚ç‚©‚É“–‚½‚ç‚È‚¢‚æ‚¤‚É‚·‚é
	if (_friendFire) {
		for (auto& colTags : _col.lock()->GetTags()) {
			if(colTags==Collider::COL_TAG::PLAYER){
				_col.lock()->AddNoHitTag(Collider::COL_TAG::PLAYER);
				break;
			}
			else if (colTags == Collider::COL_TAG::ENEMY) {
				_col.lock()->AddNoHitTag(Collider::COL_TAG::ENEMY);
				break;
			}
		}
	}
}

void AttackManager::Attack(const std::string& _name, const std::string& _sndName)
{
	//‚»‚à‚»‚àŽg—p‚µ‚½‚¢UŒ‚‚ª“o˜^‚³‚ê‚Ä‚¢‚È‚¢‚Æ‚«
	if(!attackColliders_.contains(_name)){
		//ƒGƒ‰[–hŽ~
		assert("“o˜^‚³‚ê‚Ä‚¢‚È‚¢UŒ‚‚ð”­¶‚³‚¹‚æ‚¤‚Æ‚µ‚Ä‚¢‚Ü‚·");
		return;
	}

	if (attackColliders_[_name].isUsed) {
		//ƒGƒ‰[–hŽ~
		assert("‚·‚Å‚ÉŽg—p’†‚ÌUŒ‚‚ðÄ“x”­¶‚³‚¹‚æ‚¤‚Æ‚µ‚Ä‚¢‚Ü‚·");
		return;
	}

	//UŒ‚”­¶
	attackColliders_[_name].collider.lock()->SetUseThis(true);
	//UŒ‚€”õ‚Ìƒ^ƒOÝ’è
	attackColliders_[_name].collider.lock()->AddTag(Collider::COL_TAG::PREATTACK);
	attackColliders_[_name].isUsed = true;

	//XVˆ—‚ÌÝ’è
	updateAtk_[_name] = &AttackManager::UpdatePreAttack;

	//‰½‚©Ä¶‚·‚é•¨‚ª‚ ‚éê‡
	if (_sndName != "") {
		//Œø‰Ê‰¹‚ÌÄ¶
		SoundManager::GetInstance().Play(_sndName);
	}
}

void AttackManager::DeleteCollider(const std::string& _name)
{
	attackColliders_.erase(_name);
}

bool AttackManager::Update(void)
{
	//UŒ‚XVˆ—iUŒ‚”­¶ŽÒ”•ªj
	//master=first:UŒ‚”­¶ŽÒ–¼ second:UŒ‚ƒf[ƒ^
	for (auto& atk : attackColliders_) {
		//ƒJƒEƒ“ƒ^‚Ì‘‰Á
		atk.second.counter++;
		auto atkCol = atk.second.collider.lock();

		if (atk.second.isUsed) {
			//ƒAƒNƒeƒBƒu‚ÈUŒ‚‚ÌXV
			(this->*updateAtk_[atk.first])(atk.first, atk.second);
		}
	}
	return true;
}

const float AttackManager::GetTotalTime(const std::string& _name) const
{
	//—v‘f‚ª‚È‚¢‚Æ‚«
	if (!attackColliders_.contains(_name)) {
		return -1.0f;
	}
	return attackColliders_.at(_name).totalTime;
}

void AttackManager::UseAllert(const std::string& _name)
{
	attackColliders_[_name].isAllert = true;
}

const bool AttackManager::IsAllert(const std::string& _name) const
{
	return attackColliders_.at(_name).isAllert;
}

void AttackManager::UseAttackCollision(const std::string& _name)
{
	attackColliders_[_name].collider.lock()->SetUseThis(false);
	attackColliders_[_name].isUsed = false;
}

void AttackManager::DrawDebug(void)
{
	//int color = 0xff00ff;
	//for (auto& master : activeAttacks_) {
	//	for (auto& atkData : master.second) {
	//		auto& info = attackInfoes_[atkData.first];
	//		if (info.group == ATTACK_MASTER::ENEMY) {
	//			if (info.counter < info.startAttack) {
	//				color = 0x00ff00;
	//			}
	//			else if (info.counter >= info.endAttack) {
	//				color = 0x0000ff;
	//			}
	//			else color = 0xff00ff;
	//		}
	//		//ƒfƒoƒbƒO—p‚Ì‹…‘Ì‚ð•`‰æ
	//		DrawSphere3D(atkData.second.attack.pos, info.scale, 8, color, color, false);
	//	}
	//}
	
}

void AttackManager::UpdatePreAttack(const std::string& _name, AttackInfo& _info)
{
	if (_info.counter >= _info.startTime) {
		//UŒ‚ŠJŽnˆ—
		auto atkCol = _info.collider.lock();
		//€”õó‘Ô¨UŒ‚ó‘Ô‚Ö
		atkCol->DeleteTag(Collider::COL_TAG::PREATTACK);
		atkCol->AddTag(Collider::COL_TAG::ATTACK);
		updateAtk_[_name] = &AttackManager::UpdateAttack;
	}
}

void AttackManager::UpdateAttack(const std::string& _name, AttackInfo& _info)
{
	if (_info.counter >= _info.endTime) {
		//UŒ‚I—¹ˆ—
		auto atkCol = _info.collider.lock();
		atkCol->SetUseThis(false);
		atkCol->DeleteTag(Collider::COL_TAG::ATTACK);
		_info.counter = 0.0f;
		_info.isUsed = false;
	}
}
