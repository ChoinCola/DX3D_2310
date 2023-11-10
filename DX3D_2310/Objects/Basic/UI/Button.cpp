#include "Framework.h"

Button::Button(wstring textureFile) : Quad(textureFile)
{
}

Button::Button(Float2 size) : Quad(size)
{
}

void Button::Update()
{
    if (IsActive()) return;

    if (CollisionChack(Mouse::Get()->GetPosition()))
    {
        if (Mouse::Get()->Down(0))
            isDownCheck = true;

        if (Mouse::Get()->Press(0))
            state = DOWN;
        else
            state = OVER;

        if (isDownCheck && Mouse::Get()->Up(0))
        {
            if (event)
                event();
            isDownCheck = false;
        }
    }
    else
    {
        state = NONE;

        if (Mouse::Get()->Up(0))
            isDownCheck = false;
    }

    switch (state)
    {
    case Button::NONE:
        material->GetBuffer()->diffuse = NONE_COLOR;
        break;
    case Button::DOWN:
        material->GetBuffer()->diffuse = DOWN_COLOR;
        break;
    case Button::OVER:
        material->GetBuffer()->diffuse = OVER_COLOR;
        break;
    }

    UpdateWorld();
}
