#include "stdafx.h"
#include "UI.h"

UI::UI()
{
}

UI::UI(POINT pt)
{
	ui_PT = pt;
}

UI::~UI()
{
}

POINT UI::GetUiPT() const
{
	return ui_PT;
}

SIZE UI::GetUiSize() const
{
	return ui_Size;
}

RECT UI::GetUiRC() const
{
	/// <summary>
	/// ui렌더 범위를 내놓는 함수
	/// </summary>
	/// <returns></returns>
	return RECT{ ui_PT.x - ui_Size.cx / 2, ui_PT.y - ui_Size.cy / 2,
		ui_PT.x + ui_Size.cx / 2, ui_PT.y + ui_Size.cy / 2 };
}

SIZE UI::GetUiClickSize() const
{
	return ui_ClickSize;
}

RECT UI::GetUiClickRC() const
{
	return RECT{ ui_ClickPT.x - ui_ClickSize.cx / 2, ui_ClickPT.y - ui_ClickSize.cy / 2,
		ui_ClickPT.x + ui_ClickSize.cx / 2, ui_ClickPT.y + ui_ClickSize.cy / 2 };
}

void UI::SetUiPT(POINT pt)
{
	ui_PT = pt;
}

void UI::SetUiSize(SIZE size)
{
	ui_Size = size;
}

void UI::SetUiClickPT(POINT pt)
{
	ui_ClickPT = pt;
}

void UI::SetUiClickSize(SIZE size)
{
	ui_ClickSize = size;
}




void UI::Update(float deltaTime)
{
}

void UI::Render(HDC destDC)
{
	/// <summary>
	/// ui렌더 함수
	/// </summary>
	/// <param name="destDC"></param>
	if (ui_PT.x - ui_Size.cx / 2 > cameraPos.x + clientSize.cx / 2) {
		return;
	}
	else if (ui_PT.x + ui_Size.cx / 2 < cameraPos.x - clientSize.cx / 2) {
		return;
	}

	if (ui_PT.y - ui_Size.cy / 2 > cameraPos.y + clientSize.cy / 2) {
		return;
	}
	else if (ui_PT.y + ui_Size.cy / 2 < cameraPos.y - clientSize.cy / 2) {
		return;
	}

	/* 
	ui 렌더 부분
	RenderHitBox(destDC, GetUiRC());

	bool rev = m_lookLR == MD_RIGHT ? true : false;
	m_animation->Render(destDC, m_monsterSize, GetRenderRC(),
		m_curClip->GetImgRC(), rev, m_colorKey);
	*/
}

bool UI::Save(FILE* fp)
{
	return false;
}

bool UI::Load(FILE* fp)
{
	return false;
}
