#include "Framework.h"
#include "UIRenderMaster.h"

UIRenderMaster::UIRenderMaster()
{
	
}

UIRenderMaster::~UIRenderMaster()
{
}

void UIRenderMaster::InputUI2D(string UI)
{
	if(UImap.count(UI) < 1)
		UImap[UI] = new Quad();
}

void UIRenderMaster::InputUI2D(string UI, wstring Route)
{
	if (UImap.count(UI) < 1)
		UImap[UI] = new Quad(Route);
}

Quad* UIRenderMaster::GetUI2D(string UI)
{
	if (UImap.count(UI) >= 1)
		return UImap[UI];

	InputUI2D(UI);
	return UImap[UI];
}

void UIRenderMaster::DeleteUI2D(string UI)
{
	if(UImap.count(UI) >= 1)
		UImap.erase(UI);
}

/// /////////////////////////////////

void UIRenderMaster::InputUI3D(string UI)
{
	if (_3DUImap.count(UI) < 1)
		_3DUImap[UI] = new Quad();
}

void UIRenderMaster::InputUI3D(string UI, wstring Route)
{
	if (_3DUImap.count(UI) < 1)
		_3DUImap[UI] = new Quad(Route);
}

Quad* UIRenderMaster::GetUI3D(string UI)
{
	if (_3DUImap.count(UI) >= 1)
		return _3DUImap[UI];

	InputUI3D(UI);
	return _3DUImap[UI];
}

void UIRenderMaster::DeleteUI3D(string UI)
{
	if (_3DUImap.count(UI) >= 1)
		_3DUImap.erase(UI);
}

void UIRenderMaster::Set2DUIScaleAtSort
(string UI, UINT sort, float x, float y, Vector3 DefaltPos ,Vector3 DefaltScale)
{
	// 0 = 謝難
	// 1 = 辦難
	// 2 = 醞懈
	Quad* ui = UImap[UI];
	Float2 size = ui->GetDaflatSize();

	ui->SetLocalScale(DefaltScale - Vector3(1-x, 1-y, 0));

	switch (sort)
	{
	case 0:
		ui->SetLocalPosition(DefaltPos - Vector3(size.x * (1 - x), size.y * (1 - y), 0) * 0.5);
		break;
	case 1:
		ui->SetLocalPosition(DefaltPos + Vector3(size.x * (1 - x), size.y * (1 - y), 0) * 0.5);
		break;
	case 2:
		ui->SetLocalPosition(DefaltPos);
		break;
	}
}

void UIRenderMaster::Set3DUIScaleAtSort
(string UI, UINT sort, float x, float y, Vector3 DefaltPos, Vector3 DefaltScale)
{
	// 0 = 謝難
	// 1 = 辦難
	// 2 = 醞懈
	Quad* ui = _3DUImap[UI];
	Float2 size = ui->GetDaflatSize();
	Vector3 depot = ((1 - x) * DefaltScale.x, (1 - y)* DefaltScale.y, 0);
	ui->SetLocalScale(DefaltScale - Vector3((1 - x) * DefaltScale.x, (1 - y) * DefaltScale.y, 0));

	switch (sort)
	{
	case 0:
		ui->SetLocalPosition(DefaltPos - depot * 0.5);
		break;
	case 1:
		ui->SetLocalPosition(DefaltPos + depot * 0.5);
		break; 
	case 2:  
		ui->SetLocalPosition(DefaltPos);
		break;
	}
}


void UIRenderMaster::Update()
{
	Update2D();
	Update3D();
}

void UIRenderMaster::Update2D()
{
	if (UImap.empty()) return;
	UImapIter = UImap.begin();

	if (UImapIter == UImap.end()) {
		UImapIter = UImap.begin();
		return;
	}

	for (; UImapIter != UImap.end(); UImapIter++)
		UImapIter->second->UpdateWorld();
}

void UIRenderMaster::Update3D()
{
	if (_3DUImap.empty()) return;
	_3DUImapIter = _3DUImap.begin();

	if (_3DUImapIter == _3DUImap.end()) {
		_3DUImapIter = _3DUImap.begin();
		return;
	}

	for (; _3DUImapIter != _3DUImap.end(); _3DUImapIter++) {
		Vector3 PtoMro = _3DUImapIter->second->GetGlobalPosition() - CAM->GetGlobalPosition();
		float rotate = Vector3::Dot(_3DUImapIter->second->GetRight(), PtoMro);

		if (rotate < 0) rotate = -1;
		else if (rotate > 0) rotate = 1;
		else rotate = 0;

		_3DUImapIter->second->UpdateWorld();

		if(rotate != 0)
			_3DUImapIter->second->Rotate(Vector3(0, rotate * DELTA, 0));
	}
}

void UIRenderMaster::Render()
{
	if (_3DUImap.empty()) return;
	_3DUImapIter = _3DUImap.begin();

	if (_3DUImapIter == _3DUImap.end()) {
		_3DUImapIter = _3DUImap.begin();
		return;
	}

	for (; _3DUImapIter != _3DUImap.end(); _3DUImapIter++)
		_3DUImapIter->second->Render();
}

void UIRenderMaster::PostRender()
{

	if (UImap.empty()) return;
	UImapIter = UImap.begin();

	if (UImapIter == UImap.end()) {
		UImapIter = UImap.begin();
		return;
	}

	for (; UImapIter != UImap.end(); UImapIter++)
		UImapIter->second->Render();
}

void UIRenderMaster::GUIRender()
{
	if (UImap.empty()) return;
	UImapIter = UImap.begin();

	if (UImapIter == UImap.end()) {
		UImapIter = UImap.begin();
		return;
	}

	for (; UImapIter != UImap.end(); UImapIter++)
		UImapIter->second->GUIRender();
}
