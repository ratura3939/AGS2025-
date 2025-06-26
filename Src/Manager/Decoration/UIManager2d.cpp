#include"../../Utility/Utility.h"
#include "UIManager2d.h"

UIManager2d* UIManager2d::instance_ = nullptr;

void UIManager2d::CreateInstance(void)
{
	if (instance_ == nullptr) {
		instance_ = new UIManager2d();
	}
}

UIManager2d& UIManager2d::GetInstance(void)
{
	return *instance_;
}

void UIManager2d::Add(const std::string& _name, const int _imgHndl, const UI_DIRECTION_2D _type)
{
	//‰æ‘œ‚ÌŠi”[
	images_.emplace(_name, _imgHndl);

	//Šî‘bî•ñ‰Šú‰»
	UIInfo info = {};
	info.pos = Utility::VECTOR_INIT;
	info.scl = 1.0f;
	info.deg = 0.0f;
	info.alpha = ALPHA_MAX;

	//Šî‘bî•ñ’Ç‰Á
	infoes_.emplace(_name, info);

	//‰‰oˆ—‚Ì’Ç‰Á
	PushUIDirection(_name, _type);
	
}

void UIManager2d::PushUIDirection(const std::string& _name, UI_DIRECTION_2D _type)
{
	//‰‰oî•ñ‰Šú‰»
	DirectionInfo direcInfo = {};
	direcInfo.type = _type;
	direcInfo.pow = 0.0f;
	direcInfo.acc = 0.0f;
	direcInfo.max = 0.0f;
	direcInfo.min = 0.0f;

	//‰‰oî•ñ‚Ì’Ç‰Á
	direcInfoes_[_name].push_back(direcInfo);

	//XV‚Ì’Ç‰Á
	UI_DIRECTION_GROUP group = GetDirectionGroup(_type);
	if (group == UI_DIRECTION_GROUP::MOVE) {
		updates_[_name].push_back(&UIManager2d::Move);
	}
	if (group == UI_DIRECTION_GROUP::ZOOM) {
		updates_[_name].push_back(&UIManager2d::Zoom);
	}
	if (group == UI_DIRECTION_GROUP::ROTATION) {
		updates_[_name].push_back(&UIManager2d::Rotation);
	}
	if (group == UI_DIRECTION_GROUP::GRADUALLY) {
		updates_[_name].push_back(&UIManager2d::AlphaAcc);
	}
}

void UIManager2d::PopUIDirection(const std::string& _name)
{
	updates_[_name].pop_back();
	direcInfoes_[_name].pop_back();
}

void UIManager2d::SetUIInfo(const std::string& _name, const VECTOR _pos, const float _scale, const float _deg, const float _alpha)
{
	infoes_[_name].pos = _pos;
	infoes_[_name].scl = _scale;
	infoes_[_name].deg = _deg;
	infoes_[_name].alpha = _alpha;
}

void UIManager2d::SetUIDirectionPram(const std::string& _name, const UI_DIRECTION_GROUP _group, const float _acc, const float _max, const float _min)
{
	for (auto& info:direcInfoes_[_name]) {
		
		auto group = GetDirectionGroup(info.type);
		//Ý’è‚·‚éƒpƒ‰ƒ[ƒ^‚ÌˆÓ–¡‚ªˆá‚Á‚½‚ç
		if (group != _group)continue;
		info.acc = _acc;
		info.max = _max;
		info.min = _min;


		//‰Á‘¬‚ª•‰‚Ì•ûŒü‚Ì•¨‚½‚¿‚Íæ‚ÉÝ’è‚µ‚Ä‚¨‚­
		auto direcType = info.type;
		if (direcType == UI_DIRECTION_2D::MOVE_UP ||
			direcType == UI_DIRECTION_2D::MOVE_LEFT ||
			direcType == UI_DIRECTION_2D::ROT_RIGHT ||
			direcType == UI_DIRECTION_2D::ZOOM_OUT ||
			direcType == UI_DIRECTION_2D::GRAD_DISAP) {
			info.acc *= -1.0f;
		}
	}

	
}

void UIManager2d::Update(const std::string _name)
{
	int cnt = 0;
	for (auto& update : updates_[_name]) {
		(this->*update)(_name, direcInfoes_[_name][cnt]);
		cnt++;
	}
}

void UIManager2d::Update(const std::vector<std::string> _names)
{
	for (auto& name : _names) {
		int cnt = 0;
		for (auto& update : updates_[name]) {
			(this->*update)(name,direcInfoes_[name][cnt]);
			cnt++;
		}
	}
}

void UIManager2d::Draw(const std::string _name)
{
	auto info = infoes_[_name];
	DrawRotaGraph(info.pos.x, info.pos.y, info.scl, info.deg / 180.0f, images_[_name], true);
}

void UIManager2d::Draw(const std::vector<std::string> _names)
{
	for (auto& name : _names) {
		auto info = infoes_[name];
		DrawRotaGraph(info.pos.x, info.pos.y, info.scl, info.deg / 180.0f, images_[name], true);
	}
}

void UIManager2d::Relese(void)
{
	images_.clear();
	infoes_.clear();
	direcInfoes_.clear();
	updates_.clear();
}

void UIManager2d::Destroy(void)
{
	Relese();
	delete instance_;
}

const UIManager2d::UI_DIRECTION_GROUP UIManager2d::GetDirectionGroup(const std::string _name)
{
	return GetDirectionGroup(direcInfoes_[_name][0].type);
}

const UIManager2d::UI_DIRECTION_GROUP UIManager2d::GetDirectionGroup(const UI_DIRECTION_2D _type)
{
	UI_DIRECTION_GROUP ret = UI_DIRECTION_GROUP::NONE;

	//ˆÚ“®
	if (_type == UI_DIRECTION_2D::MOVE_UP	||
		_type == UI_DIRECTION_2D::MOVE_DOWN ||
		_type == UI_DIRECTION_2D::MOVE_LEFT ||
		_type == UI_DIRECTION_2D::MOVE_RIGHT||
		_type == UI_DIRECTION_2D::UP_DOWN	||
		_type == UI_DIRECTION_2D::LEFT_RIGHT) {

		ret = UI_DIRECTION_GROUP::MOVE;
	}

	//Šg‘åk¬
	if (_type == UI_DIRECTION_2D::ZOOM_IN || _type == UI_DIRECTION_2D::ZOOM_OUT || _type == UI_DIRECTION_2D::ZOOM_INOUT) {
		ret = UI_DIRECTION_GROUP::ZOOM;
	}

	//‰ñ“]
	if (_type == UI_DIRECTION_2D::ROT_LEFT || _type == UI_DIRECTION_2D::ROT_RIGHT || _type == UI_DIRECTION_2D::ROT_CRADLE) {
		ret = UI_DIRECTION_GROUP::ROTATION;
	}

	//“§–¾“x
	if (_type == UI_DIRECTION_2D::GRAD_AP || _type == UI_DIRECTION_2D::GRAD_DISAP || _type == UI_DIRECTION_2D::FLASHING) {
		ret = UI_DIRECTION_GROUP::GRADUALLY;
	}

	return ret;
}

void UIManager2d::Move(const std::string& _name, DirectionInfo& _direcInfo)
{
	VECTOR afterPos = infoes_[_name].pos;
	auto direcType = _direcInfo.type;

	//ã‰º
	if (direcType == UI_DIRECTION_2D::MOVE_UP || 
		direcType == UI_DIRECTION_2D::MOVE_DOWN || 
		direcType == UI_DIRECTION_2D::UP_DOWN) {

		//ˆÚ“®—Ê‰ÁŽZ
		afterPos.y += _direcInfo.acc;
		_direcInfo.pow += _direcInfo.acc;

		//ˆÚ“®ãŒÀE‰ºŒÀ‚ÌÝ’è
		if (_direcInfo.pow >= _direcInfo.max ||
			_direcInfo.pow <= _direcInfo.min) {
			
			//ã‰ºˆÚ“®ŒJ‚è•Ô‚µ‚Ì‚Æ‚«
			if (direcType == UI_DIRECTION_2D::UP_DOWN) {
				//‰ÁŽZ•ûŒü‚ð‹t‚É
				_direcInfo.acc *= -1.0f;
			}
			else {
				//“®‚©‚³‚È‚¢
				afterPos = infoes_[_name].pos;
			}
			
		}
	}

	//¶‰E
	if (direcType == UI_DIRECTION_2D::MOVE_LEFT ||
		direcType == UI_DIRECTION_2D::MOVE_RIGHT ||
		direcType == UI_DIRECTION_2D::LEFT_RIGHT) {

		//ˆÚ“®—Ê‰ÁŽZ
		afterPos.x += _direcInfo.acc;

		//ˆÚ“®ãŒÀE‰ºŒÀ‚ÌÝ’è
		if (_direcInfo.pow >= _direcInfo.max ||
			_direcInfo.pow <= _direcInfo.min) {

			//ã‰ºˆÚ“®ŒJ‚è•Ô‚µ‚Ì‚Æ‚«
			if (direcType == UI_DIRECTION_2D::LEFT_RIGHT) {
				//‰ÁŽZ•ûŒü‚ð‹t‚É
				_direcInfo.acc *= -1.0f;
			}
			else {
				//“®‚©‚³‚È‚¢
				afterPos = infoes_[_name].pos;
			}

		}
	}

	//ˆÚ“®ˆ—
	infoes_[_name].pos = afterPos;
}

void UIManager2d::Zoom(const std::string& _name, DirectionInfo& _direcInfo)
{
	float afterScl = infoes_[_name].scl;
	auto direcType = _direcInfo.type;

	afterScl += _direcInfo.acc;

	//§ŒÀ
	//ˆÚ“®ãŒÀE‰ºŒÀ‚ÌÝ’è
	if (afterScl >= _direcInfo.max ||
		afterScl <= _direcInfo.min) {
		//ŒJ‚è•Ô‚µˆ—‚È‚ç‚Î‰ÁŽZ•ûŒü‚ð‹t‚É
		if (_direcInfo.type == UI_DIRECTION_2D::ZOOM_INOUT) {
			_direcInfo.acc *= -1.0f;
		}
	}


	//ˆÚ“®ˆ—
	infoes_[_name].scl = afterScl;
}

void UIManager2d::Rotation(const std::string& _name, DirectionInfo& _direcInfo)
{
	float afterDeg = infoes_[_name].deg;
	auto direcType = _direcInfo.type;

	afterDeg += _direcInfo.acc;

	//§ŒÀ
	//ˆÚ“®ãŒÀE‰ºŒÀ‚ÌÝ’è
	if (afterDeg >= _direcInfo.max ||
		afterDeg <= _direcInfo.min) {
		//ŒJ‚è•Ô‚µˆ—‚È‚ç‚Î‰ÁŽZ•ûŒü‚ð‹t‚É
		if (_direcInfo.type == UI_DIRECTION_2D::ROT_CRADLE) {
			_direcInfo.acc *= -1.0f;
		}
	}


	//ˆÚ“®ˆ—
	infoes_[_name].deg = afterDeg;
}

void UIManager2d::AlphaAcc(const std::string& _name, DirectionInfo& _direcInfo)
{
	float afterAlpha = infoes_[_name].alpha;
	auto direcType = _direcInfo.type;

	afterAlpha += _direcInfo.acc;

	//§ŒÀ
	//ˆÚ“®ãŒÀE‰ºŒÀ‚ÌÝ’è
	if (afterAlpha >= _direcInfo.max ||
		afterAlpha <= _direcInfo.min) {
		//ŒJ‚è•Ô‚µˆ—‚È‚ç‚Î‰ÁŽZ•ûŒü‚ð‹t‚É
		if (_direcInfo.type == UI_DIRECTION_2D::FLASHING) {
			_direcInfo.acc *= -1.0f;
		}
	}


	//ˆÚ“®ˆ—
	infoes_[_name].alpha = afterAlpha;
}

