#pragma once
#include<DxLib.h>]
#include<string>
#include<unordered_map>

class AttackBase
{
public:
	//UŒ‚”­¶Ò‚Ìw‰c
	enum class ATTACK_MASTER {
		PLAYER,
		ENEMY
	};

	//UŒ‚‚Ìí—Ş
	enum class ATTCK_TYPE {
		SWORD,	//Œ•(‹ßÚ)
		BOW,	//‹|(‰“‹——£)
	};

	//UŒ‚ŠÖ˜Aî•ñ
	struct Attack {
		VECTOR pos_;		//”­¶ˆÊ’uî•ñ
		float atkPow_;		//UŒ‚—Í

		float totalMotion_;	//ƒ‚[ƒVƒ‡ƒ“‘ŠÔ
		float startAttack_;	//UŒ‚”»’è”­¶ŠÔ
		float endAttack_;	//UŒ‚”»’èÁ–ÅŠÔ
		float conter_;		//ƒJƒEƒ“ƒ^[
	};

	AttackBase(void);
	~AttackBase(void);


	void Update(void);

private:
	std::unordered_map<std::string, Attack>attacks_;
};

