#pragma once
class CharacterUIController
{
public:
	CharacterUIController(void);
	~CharacterUIController(void);

	virtual void Init(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;
	virtual void Release(void) = 0;

protected:
	/// <summary>
	/// HP‚Ì•`‰æ
	/// </summary>
	virtual void DrawHp(void) = 0;
};

