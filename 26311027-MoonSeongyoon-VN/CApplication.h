#pragma once
#include <Windows.h>
#include <string>
#include "ScenceGameBegin.h"
#include "Player.h"

class CApplication
{
public:
    int Init();
    int Update();
    int Render();
    int Destroy();

protected:
    int InitSdk();

protected:
    // windows
    POINT m_winPos{ 100, 100 };
    SIZE m_winSize{ 768, 768 };
    std::string m_winName = "My Game Window";

    ScenceGameBegin m_sceneBegin;
    Player m_player;
};
