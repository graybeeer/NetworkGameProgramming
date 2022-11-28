#pragma once
#include "stdafx.h"
#include "TextureManager.h"

#include "Animation.h"
#include "AnimationClip.h"
#include "Global.h"
#include "Effect.h"
class UI 
{
public:
	UI(); //그냥 UI 생성자
	UI(POINT pt); //위치를 인수로 할 경우 그 위치에 생성되는 생성자
	~UI(); //소멸자
private:
	POINT ui_PT{ 0,0 }; //ui위치
	SIZE ui_Size{ 100,100 }; //ui크기
	POINT ui_ClickPT{ 0,0 }; //ui터치 인식위치
	SIZE ui_ClickSize{ 100,100 }; //ui 터치 인식 크기
	string ui_tag{ }; //ui 태그
public:
	POINT GetUiPT() const;
	SIZE GetUiSize() const;
	RECT GetUiRC() const;
	SIZE GetUiClickSize() const;
	RECT GetUiClickRC() const;
	void SetUiPT(POINT pt);
	void SetUiSize(SIZE size);
	void SetUiClickPT(POINT pt);
	void SetUiClickSize(SIZE size);

	void Update(float deltaTime); //업데이트 함수, 매프레임 업데이트
	void Render(HDC destDC); //렌더함수

	bool Save(FILE* fp);
	bool Load(FILE* fp);
};