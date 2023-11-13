#include "Framework.h"

UIManager::UIManager()
{
}

UIManager::~UIManager()
{
}

bool UIManager::IsMouseOver()
{
    for (Quad* ui : totalUI)
    {
        if (ui->CollisionChack(Mouse::Get()->GetPosition()))
            return true;
    }

    return false;
}
