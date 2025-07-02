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

void UIManager2d::Add(const std::string& _name, const int _imgHndl, VECTOR _size, const UI_DIRECTION_2D _type, const UI_DRAW_DIMENSION _dimension)
{
	//óvëfÇ™Ç†ÇÈÇ∆Ç´
	if (direcInfoes_.contains(_name)) {
		//ç°Ç†ÇÈï™âÒÇ∑
		for (auto& direc : direcInfoes_[_name]) {
			if (GetDirectionGroup(direc.type) == GetDirectionGroup(_type)) {
				return;
			}
		}
	}

	//âÊëúÇÃäiî[
	images_.emplace(_name, _imgHndl);

	//äÓëbèÓïÒèâä˙âª
	UIInfo info = {};
	info.dimension = _dimension;
	info.pos = Utility::VECTOR_INIT;
	info.size = _size;
	info.scl = 1.0f;
	info.deg = 0.0f;
	info.alpha = ALPHA_MAX;

	//äÓëbèÓïÒí«â¡
	infoes_.emplace(_name, info);

	//ââèoèàóùÇÃí«â¡
	PushUIDirection(_name, _type);
	
}

void UIManager2d::PushUIDirection(const std::string& _name, UI_DIRECTION_2D _type)
{
	//ââèoèÓïÒèâä˙âª
	DirectionInfo direcInfo = {};
	direcInfo.type = _type;
	direcInfo.pow = 0.0f;
	direcInfo.acc = 0.0f;
	direcInfo.max = 0.0f;
	direcInfo.min = 0.0f;

	//ââèoèÓïÒÇÃí«â¡
	direcInfoes_[_name].push_back(direcInfo);

	//çXêVÇÃí«â¡
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
		//ê›íËÇ∑ÇÈÉpÉâÉÅÅ[É^ÇÃà”ñ°Ç™à·Ç¡ÇΩÇÁ
		if (group != _group)continue;
		info.acc = _acc;
		info.max = _max;
		info.min = _min;

		//â¡ë¨Ç™ïâÇÃï˚å¸ÇÃï®ÇΩÇøÇÕêÊÇ…ê›íËÇµÇƒÇ®Ç≠
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

void UIManager2d::SetPos(const std::string& _name, const VECTOR& _pos)
{
	infoes_[_name].pos = _pos;
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
	//Ç§Ç¡Ç∑ÇÁçïÇ≠Ç∑ÇÈ
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, info.alpha);
	if (info.dimension == UI_DRAW_DIMENSION::DIMENSION_2) {
		DrawRotaGraph(info.pos.x, info.pos.y, info.scl, info.deg / 180.0f, images_[_name], true);
	}
	else {
		DrawBillboard3D(info.pos, info.size.x / 2.0f, info.size.y / 2.0f, info.scl, images_[_name], info.deg / 180.0f, true);
	}
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void UIManager2d::Draw(const std::vector<std::string> _names)
{
	for (auto& name : _names) {
		Draw(name);	//íPëÃï`âÊÇ…ìnÇ∑ÅB
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

const bool UIManager2d::IsFinishDirection(const std::string _name, const UI_DIRECTION_GROUP _group)
{
	//ÉãÅ[ÉvÇ∑ÇÈèÍçáÇÕèàóùÇ™ïKóvÇ»Ç¢ÇΩÇﬂèIóπ

	return false;
}

const bool UIManager2d::IsLoopUpdate(const std::string _name, const UI_DIRECTION_GROUP _group)
{
	//èÓïÒï™âÒÇ∑
	for (auto& direction : direcInfoes_[_name]) {
		//äYìñÇÃçXêVÇÃÇ∆Ç´îªíËÇâ¡Ç¶ÇÈÅB
		if (GetDirectionGroup(direction.type) == _group) {
			//ÉãÅ[Évà»äOÇ»ÇÁtrueÇï‘Ç∑ÅB
			if (direction.type != UI_DIRECTION_2D::UP_DOWN &&
				direction.type != UI_DIRECTION_2D::LEFT_RIGHT &&
				direction.type != UI_DIRECTION_2D::ZOOM_INOUT &&
				direction.type != UI_DIRECTION_2D::ROT_CRADLE &&
				direction.type != UI_DIRECTION_2D::FLASHING)
			{
				return true;
			}
		}
	}
	return false;
}

const UIManager2d::UI_DIRECTION_GROUP UIManager2d::GetDirectionGroup(const std::string _name)
{
	return GetDirectionGroup(direcInfoes_[_name][0].type);
}

const UIManager2d::UI_DIRECTION_GROUP UIManager2d::GetDirectionGroup(const UI_DIRECTION_2D _type)
{
	UI_DIRECTION_GROUP ret = UI_DIRECTION_GROUP::NONE;

	//à⁄ìÆ
	if (_type == UI_DIRECTION_2D::MOVE_UP	||
		_type == UI_DIRECTION_2D::MOVE_DOWN ||
		_type == UI_DIRECTION_2D::MOVE_LEFT ||
		_type == UI_DIRECTION_2D::MOVE_RIGHT||
		_type == UI_DIRECTION_2D::UP_DOWN	||
		_type == UI_DIRECTION_2D::LEFT_RIGHT) {

		ret = UI_DIRECTION_GROUP::MOVE;
	}

	//ägëÂèkè¨
	if (_type == UI_DIRECTION_2D::ZOOM_IN || _type == UI_DIRECTION_2D::ZOOM_OUT || _type == UI_DIRECTION_2D::ZOOM_INOUT) {
		ret = UI_DIRECTION_GROUP::ZOOM;
	}

	//âÒì]
	if (_type == UI_DIRECTION_2D::ROT_LEFT || _type == UI_DIRECTION_2D::ROT_RIGHT || _type == UI_DIRECTION_2D::ROT_CRADLE) {
		ret = UI_DIRECTION_GROUP::ROTATION;
	}

	//ìßñæìx
	if (_type == UI_DIRECTION_2D::GRAD_AP || _type == UI_DIRECTION_2D::GRAD_DISAP || _type == UI_DIRECTION_2D::FLASHING) {
		ret = UI_DIRECTION_GROUP::GRADUALLY;
	}

	return ret;
}

void UIManager2d::Move(const std::string& _name, DirectionInfo& _direcInfo)
{
	VECTOR afterPos = infoes_[_name].pos;
	auto direcType = _direcInfo.type;

	//è„â∫
	if (direcType == UI_DIRECTION_2D::MOVE_UP || 
		direcType == UI_DIRECTION_2D::MOVE_DOWN || 
		direcType == UI_DIRECTION_2D::UP_DOWN) {

		//à⁄ìÆó â¡éZ
		afterPos.y += _direcInfo.acc;
		_direcInfo.pow += _direcInfo.acc;

		//à⁄ìÆè„å¿ÅEâ∫å¿ÇÃê›íË
		if (_direcInfo.pow >= _direcInfo.max ||
			_direcInfo.pow <= _direcInfo.min) {
			
			//è„â∫à⁄ìÆåJÇËï‘ÇµÇÃÇ∆Ç´
			if (direcType == UI_DIRECTION_2D::UP_DOWN) {
				//â¡éZï˚å¸ÇãtÇ…
				_direcInfo.acc *= -1.0f;
			}
			else {
				//ìÆÇ©Ç≥Ç»Ç¢
				return;
			}
			
		}
	}

	//ç∂âE
	if (direcType == UI_DIRECTION_2D::MOVE_LEFT ||
		direcType == UI_DIRECTION_2D::MOVE_RIGHT ||
		direcType == UI_DIRECTION_2D::LEFT_RIGHT) {

		//à⁄ìÆó â¡éZ
		afterPos.x += _direcInfo.acc;

		//à⁄ìÆè„å¿ÅEâ∫å¿ÇÃê›íË
		if (_direcInfo.pow >= _direcInfo.max ||
			_direcInfo.pow <= _direcInfo.min) {

			//è„â∫à⁄ìÆåJÇËï‘ÇµÇÃÇ∆Ç´
			if (direcType == UI_DIRECTION_2D::LEFT_RIGHT) {
				//â¡éZï˚å¸ÇãtÇ…
				_direcInfo.acc *= -1.0f;
			}
			else {
				//ìÆÇ©Ç≥Ç»Ç¢
				return;
			}

		}
	}

	//à⁄ìÆèàóù
	infoes_[_name].pos = afterPos;
}

void UIManager2d::Zoom(const std::string& _name, DirectionInfo& _direcInfo)
{
	float afterScl = infoes_[_name].scl;
	auto direcType = _direcInfo.type;

	afterScl += _direcInfo.acc;
	_direcInfo.pow += _direcInfo.acc;

	//êßå¿
	//à⁄ìÆè„å¿ÅEâ∫å¿ÇÃê›íË
	if (afterScl >= _direcInfo.max ||
		afterScl <= _direcInfo.min) {
		//åJÇËï‘ÇµèàóùÇ»ÇÁÇŒâ¡éZï˚å¸ÇãtÇ…
		if (_direcInfo.type == UI_DIRECTION_2D::ZOOM_INOUT) {
			_direcInfo.acc *= -1.0f;
		}
		else {
			//Ç±ÇÍà»è„ïœçXÇâ¡Ç¶Ç»Ç¢
			return;
		}
	}


	//à⁄ìÆèàóù
	infoes_[_name].scl = afterScl;
}

void UIManager2d::Rotation(const std::string& _name, DirectionInfo& _direcInfo)
{
	float afterDeg = infoes_[_name].deg;
	auto direcType = _direcInfo.type;

	afterDeg += _direcInfo.acc;
	_direcInfo.pow += _direcInfo.acc;

	//êßå¿
	//à⁄ìÆè„å¿ÅEâ∫å¿ÇÃê›íË
	if (_direcInfo.pow >= _direcInfo.max ||
		_direcInfo.pow <= _direcInfo.min) {
		//åJÇËï‘ÇµèàóùÇ»ÇÁÇŒâ¡éZï˚å¸ÇãtÇ…
		if (_direcInfo.type == UI_DIRECTION_2D::ROT_CRADLE) {
			_direcInfo.acc *= -1.0f;
		}
	}


	//à⁄ìÆèàóù
	infoes_[_name].deg = afterDeg;
}

void UIManager2d::AlphaAcc(const std::string& _name, DirectionInfo& _direcInfo)
{
	float afterAlpha = infoes_[_name].alpha;
	auto direcType = _direcInfo.type;

	afterAlpha += _direcInfo.acc;
	_direcInfo.pow += _direcInfo.acc;

	//êßå¿
	//à⁄ìÆè„å¿ÅEâ∫å¿ÇÃê›íË
	if (afterAlpha >= _direcInfo.max ||
		afterAlpha <= _direcInfo.min) {
		//åJÇËï‘ÇµèàóùÇ»ÇÁÇŒâ¡éZï˚å¸ÇãtÇ…
		if (_direcInfo.type == UI_DIRECTION_2D::FLASHING) {
			_direcInfo.acc *= -1.0f;
		}
		else {
			//Ç±ÇÍà»è„ïœçXÇâ¡Ç¶Ç»Ç¢
			return;
		}
	}


	//à⁄ìÆèàóù
	infoes_[_name].alpha = afterAlpha;
}

