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

void UIManager2d::Add(const std::string& _name, const int _imgHndl, const UI_EFFECT_2D _type)
{
	//‰æ‘œ‚ÌŠi”[
	images_.emplace(_name, _imgHndl);

	//î•ñ‰Šú‰»
	UIInfo info = {};
	info.pos = Utility::VECTOR_INIT;
	info.scl = 1.0f;
	info.deg = 0.0f;
	info.alpha = ALPHA_MAX;

	info.speed = 0.0f;
	info.rotPow = 0.0f;
	info.alphaAcc = 0.0f;
	//î•ñ’Ç‰Á
	infoes_.emplace(_name, info);

	//XV‚Ì’Ç‰Á
}

void UIManager2d::AddOption(const std::string& _name, UI_EFFECT_2D _type)
{
}

void UIManager2d::Update(const std::string _name)
{
}

void UIManager2d::Update(const std::vector<std::string> _names)
{
}

void UIManager2d::Draw(const std::string _name)
{
}

void UIManager2d::Draw(const std::vector<std::string> _names)
{
}

void UIManager2d::Move(const std::string _name)
{
}

void UIManager2d::Rotation(const std::string _name)
{
}

void UIManager2d::AlphaAcc(const std::string _name)
{
}

