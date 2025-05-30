#pragma once
#include<string>
#include<memory>
#include<vector>
#include<unordered_map>
#include<DxLib.h>
#include"../../Common/Quaternion.h"
#include"../../Object/Attack/AttackBase.h"


//class AttackBase;
class Arrow;

class AttackManager
{
public:

	//UŒ‚”­¶Ò‚Ìw‰c
	enum class ATTACK_MASTER {
		NONE,
		PLAYER,
		ENEMY
	};

	//UŒ‚‚Ìí—Ş
	enum class ATTACK_TYPE {
		SWORD,	//Œ•(‹ßÚ)
		BOW,	//‹|(‰“‹——£)
	};

	//UŒ‚ŠÖ˜Aî•ñ
	struct AttackInfo {
		ATTACK_MASTER master;
		ATTACK_TYPE type;
		float scale;		//”»’è‚Ì‘å‚«‚³
		float totalMotion;	//ƒ‚[ƒVƒ‡ƒ“‘ŠÔ
		float startAttack;	//UŒ‚”»’è”­¶ŠÔ
		float endAttack;	//UŒ‚”»’èÁ–ÅŠÔ
		float conter;		//ƒJƒEƒ“ƒ^[
		bool isFriendFire;	//FFƒAƒŠ‚©‚Ç‚¤‚©
		bool isHit;			//‚·‚Å‚É”»’è‚³‚ê‚½‚©‚Ç‚¤‚©
	};

	/// <summary>
	/// “–‚½‚è”»’è‚Ég—p‚·‚éî•ñ‚ğ‚Ü‚Æ‚ß‚½‚à‚Ì
	/// </summary>
	struct AttackCollision {
		AttackInfo info;
		std::weak_ptr<AttackBase> attack;
	};

	/// <summary>
	/// UŒ‚‚Ì“o˜^
	/// </summary>
	/// <param name="_name">“o˜^–¼</param>
	/// <param name="_type">UŒ‚‚Ìí—Ş</param>
	/// <param name="_friendFire">FF‚ ‚è‚©‚Ç‚¤‚©</param>
	/// <param name="_total">‘Ä¶ŠÔ</param>
	/// <param name="_modelId">ƒ‚ƒfƒ‹î•ñ(‹|ŒÀ’è)</param>
	/// <param name="_start">”»’èŠJn</param>
	/// <param name="_end">”»’èI—¹</param>
	void AddAttack(const std::string _name, const ATTACK_TYPE& _type, const bool _friendFire,
		const float _total, const int _modelId = -1, const float _start = 0.0f, const float _end = 0.0f);

	/// <summary>
	/// ”­¶
	/// </summary>
	/// <param name="_name">“o˜^–¼</param>
	/// <param name="_pow">UŒ‚—Í</param>
	/// <param name="_pos">”­¶ˆÊ’u</param>
	/// <param name="_qua">‰ñ“]î•ñ</param>
	/// <param name="_maseter">w‰c</param>
	/// <param name="_scale">‘å‚«‚³</param>
	/// <param name="_arrowModel">–î‚Ìƒ‚ƒfƒ‹(‹|UŒ‚‚Ì‚İ)</param>
	void Attack(std::string _name, const float _pow, const VECTOR& _pos, const Quaternion& _qua, const ATTACK_MASTER _master, const float _scale, const int _arrowModel = -1);

	bool Update(void);

	/// <summary>
	/// ƒAƒNƒeƒBƒu‚ÈUŒ‚‚ğ‚·‚×‚Ä“n‚·
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	std::vector<AttackCollision> GetActiveAttacks(void);

	/// <summary>
	/// ‘ƒ‚[ƒVƒ‡ƒ“ŠÔæ“¾
	/// </summary>
	/// <param name="_name">“o˜^–¼</param>
	/// <returns>ŠÔ</returns>
	const float GetTotalTime(const std::string _name)const;

	void DrawDebug(void);

private:
	std::unordered_map<std::string, AttackInfo>attackInfoes_;
	std::unordered_map<std::string, std::shared_ptr<AttackBase>>activeAttacks_;

	//std::vector<std::unique_ptr<Arrow>>arrows_;
};

