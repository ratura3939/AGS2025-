#pragma once
#include<string>

class CharacterUIController
{
public:
	CharacterUIController(void);
	~CharacterUIController(void);

	virtual void Init(const std::string& _master) = 0;
	virtual void Update(void) = 0;
	virtual void Release(void) = 0;

protected:
	/// <summary>
	/// HP‚Ì•`‰æ
	/// </summary>
	virtual void DrawHp(void) = 0;
};

