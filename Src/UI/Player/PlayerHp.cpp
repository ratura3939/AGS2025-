#include"../../Manager/Generic/ResourceManager.h"
#include"../../Manager/Decoration/UIManager2d.h"
#include "PlayerHp.h"

namespace {
	constexpr float HP_DISTANCE = 100.0f;		//HP同士の距離
	const float HP_EX = 0.2f;	//HPの大きさの倍率

	const float BROKEN_HP_ALPHA_ACC = 2.0f;		//ひび割れHPのアルファ値の加速度
	const float BROKEN_HP_ALPHA_MAX = 255.0f;	//ひび割れHPのアルファ値の最大値
	const float BROKEN_HP_ALPHA_MIN = 0.0f;		//ひび割れHPのアルファ値の最小値

	const float BROKEN_HP_FALL_ACC = 1.0f;		//ひび割れHPの落下の加速度
	const float BROKEN_HP_FALL_MAX = 50.0f;		//ひび割れHPの落下の最大値
	const float BROKEN_HP_FALL_MIN = 0.0f;		//ひび割れHPの落下の最小値
}

PlayerHp::PlayerHp(VECTOR& _followPos, const int _hp):UIBase(_followPos)
{
	for (int num = 0; num < _hp; num++) {
		states_.push_back(STATE::NORMAL);
	}
}

PlayerHp::~PlayerHp(void)
{
}

bool PlayerHp::Init(const std::string& _master)
{
	ResourceManager& rsM = ResourceManager::GetInstance();
	UIManager2d& uiM = UIManager2d::GetInstance();
	using UI_DIMENSION = UIManager2d::UI_DRAW_DIMENSION;

	VECTOR pos = drawFollowPos_;

	nomalStr_ = _master + "NomalHp";
	emptyStr_ = _master + "EmptyHp";
	brokenStr_ = _master + "BrokenHp";

	//UI設定
	for (int i = 0; i < states_.size(); i++) {
		std::string nomal = nomalStr_.c_str() + i;
		std::string empty = emptyStr_.c_str() + i;
		std::string broken = brokenStr_.c_str() + i;

		//ノーマルHP
		uiM.Add(nomal, rsM.Load(ResourceManager::SRC::HEART_IMG).handleId_, UIManager2d::UI_DIRECTION_2D::NORMAL, UI_DIMENSION::DIMENSION_2);	//追加
		uiM.SetUIInfo(nomal, pos, HP_EX);	//基礎情報設定

		//空のHP
		uiM.Add(empty, rsM.Load(ResourceManager::SRC::HEART_EMPTY_IMG).handleId_, UIManager2d::UI_DIRECTION_2D::NORMAL, UI_DIMENSION::DIMENSION_2);	//追加
		uiM.SetUIInfo(empty, pos, HP_EX);	//基礎情報設定

		//ひび割れHP(落ちてうっすら消えていく)
		uiM.Add(broken, rsM.Load(ResourceManager::SRC::HEART_BROKEN_IMG).handleId_, UIManager2d::UI_DIRECTION_2D::GRAD_DISAP, UI_DIMENSION::DIMENSION_2);	//追加(透過演出付き)
		uiM.SetUIInfo(broken, pos, HP_EX);	//基礎情報設定
		uiM.SetUIDirectionParam(broken, UIManager2d::UI_DIRECTION_GROUP::GRADUALLY, BROKEN_HP_ALPHA_ACC, BROKEN_HP_ALPHA_MAX, BROKEN_HP_ALPHA_MIN);	//アルファ値のパラメータ設定

		uiM.PushUIDirection(broken, UIManager2d::UI_DIRECTION_2D::MOVE_DOWN);	//落下の演出追加
		uiM.SetUIDirectionParam(broken, UIManager2d::UI_DIRECTION_GROUP::MOVE, BROKEN_HP_FALL_ACC, BROKEN_HP_FALL_MAX, BROKEN_HP_FALL_MIN);	//落下のパラメータ設定

		pos.x += HP_DISTANCE;	//位置調整
	}

	//正常終了
	return true;
}

bool PlayerHp::Update(void)
{
	//ダメージ位状態のHPがあるなら更新をかける
	int cnt = 0;

	for (auto& state : states_) {
		//ひび割れHPの更新
		if (state == STATE::BROKEN) {
			UIManager2d::GetInstance().Update(brokenStr_.c_str() + cnt);	//更新

			if (UIManager2d::GetInstance().IsFinishDirection(brokenStr_.c_str() + cnt, UIManager2d::UI_DIRECTION_GROUP::MOVE)) {
				//落下演出が終了しているなら空のHPに変更
				state = STATE::EMPTY;
			}
			break;
		}
		cnt++;
	}

	////正常終了
	return true;
}

void PlayerHp::Draw(void)
{
	UIManager2d& uiM = UIManager2d::GetInstance();

	VECTOR pos = drawPos_;
	int cnt = 0;

	for (auto& state : states_) {
		
		if (state == STATE::NORMAL) {
			uiM.Draw(nomalStr_.c_str() + cnt);	//HP描画
		}
		else {
			uiM.Draw(emptyStr_.c_str() + cnt);	//空のHP描画

			//ひび割れHPは空のHPの上に描画する
			if (state == STATE::BROKEN) {
				uiM.Draw(brokenStr_.c_str() + cnt);	//ひび割れHP描画
			}
		}
		cnt++;
	}
}

void PlayerHp::SetPos(const VECTOR& _pos)
{
	drawPos_ = _pos;
}

void PlayerHp::Reset(void)
{
}

void PlayerHp::Damage(void)
{
	for (int cnt = static_cast<int>(states_.size()) - 1; cnt >= 0; cnt--) {
		if (states_[cnt] == STATE::NORMAL) {
			states_[cnt] = STATE::BROKEN;
			//HPUIに変更を加えたので終了
			break;
		}
	}
}
