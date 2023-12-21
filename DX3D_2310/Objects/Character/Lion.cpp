#include "Framework.h"

Lion::Lion()
{
	SetTag("Lion_Collider");
	Load();
	BoxLoad();

	model = new ModelAnimator("Lion");
	model->ReadClip("Idle");
	model->ReadClip("Run");
	model->CreateTexture();
	model->SetParent(this);
	model->Load();

	model->PlayClip(curState);
}

Lion::~Lion()
{
	delete model;
}

void Lion::Update()
{
	Control();
	Move();
	Rotate();

	UpdateWorld();
	model->Update();
}

void Lion::Render()
{
	Collider::Render();
	model->Render();
}

void Lion::GUIRender()
{
	BoxCollider::GUIRender();
	model->GUIRender();
}

void Lion::Control()
{
	if (KEY->Down(VK_LBUTTON))
	{
		destPos = terrain->Picking();

		int start = aStar->FindCloseNode(GetGlobalPosition());
		int end = aStar->FindCloseNode(destPos);

		aStar->GetPath(start, end, path);

		//path.insert(path.begin(), destPos);
	}
}

void Lion::Move()
{
	if (path.empty())
	{
		SetState(IDLE);
		return;
	}

	SetState(RUN);

	Vector3 dest = path.back();

	Vector3 direction = dest - GetGlobalPosition();
	direction.y = 0.0f;

	if (direction.Length() < 1.0f)
		path.pop_back();

	velocity = direction.GetNormalized();

	Translate(velocity * moveSpeed * DELTA);
	localPosition = terrain->GetOnGrondPosition(localPosition);
}

void Lion::Rotate()
{
	if (curState == IDLE) return;

	Vector3 forward = GetForward();
	Vector3 cross = Vector3::Cross(forward, velocity);

	if (cross.y < 0)
		Transform::Rotate(Vector3::Down() * rotSpeed * DELTA);
	else if (cross.y > 0)
		Transform::Rotate(Vector3::Up() * rotSpeed * DELTA);
}

void Lion::SetState(State state)
{
	if (curState == state) return;

	curState = state;
	model->PlayClip(state);
}
