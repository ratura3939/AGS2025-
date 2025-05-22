#pragma once
#include<DxLib.h>
#include<string>
#include<vector>
#include<memory>
#include<unordered_map>

class Arrow;

class AttackBase
{
public:
	//UŒ‚”­¶Ò‚Ìw‰c
	enum class ATTACK_MASTER {
		PLAYER,
		ENEMY
	};

	//UŒ‚‚Ìí—Ş
	enum class ATTACK_TYPE {
		SWORD,	//Œ•(‹ßÚ)
		BOW,	//‹|(‰“‹——£)
	};

	//UŒ‚ŠÖ˜Aî•ñ
	struct Attack {
		ATTACK_MASTER master;
		VECTOR pos;		//”­¶ˆÊ’uî•ñ
		float atkPow;		//UŒ‚—Í

		float totalMotion;	//ƒ‚[ƒVƒ‡ƒ“‘ŠÔ
		float startAttack;	//UŒ‚”»’è”­¶ŠÔ
		float endAttack;	//UŒ‚”»’èÁ–ÅŠÔ
		float conter;		//ƒJƒEƒ“ƒ^[
		bool isFriendFire;	//FFƒAƒŠ‚©‚Ç‚¤‚©
	};

	AttackBase(void);
	~AttackBase(void);

	/// <summary>
	/// UŒ‚‚Ì“o˜^
	/// </summary>
	/// <param name="_name">“o˜^–¼</param>
	/// <param name="_master">UŒ‚Ò‚Ìw‰c</param>
	/// <param name="_friendFire">FF‚ ‚è‚©‚Ç‚¤‚©</param>
	/// <param name="_type">í—Ş</param>
	/// <param name="_pos">”­¶ˆÊ’u</param>
	/// <param name="_pow">UŒ‚—Í</param>
	/// <param name="_total">‘Ä¶ŠÔ</param>
	/// <param name="_modelId">ƒ‚ƒfƒ‹î•ñ(‹|ŒÀ’è)</param>
	/// <param name="_start">”»’èŠJn</param>
	/// <param name="_end">”»’èI—¹</param>
	void AddAttack(const std::string _name, const ATTACK_MASTER& _master, const ATTACK_TYPE& _type, const bool _friendFire, const VECTOR _pos, const float _pow, const float _total, const int _modelId = -1, const float _start = 0.0f, const float _end = 0.0f);

	void OccurAttack(const std::string _name);

	void Update(void);

private:
	std::unordered_map<std::string, Attack>attacks_;
	std::vector<Attack>activeAttacks_;
	std::vector<std::unique_ptr<Arrow>>arrows_;
};

