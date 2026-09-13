#pragma once
#include "ScenceGameBegin.h"
#include "ScenceGamePlay.h"
#include "GameUI.h"
#include "Player.h"

class CApplication
{
public:
    int Init();
    int Update();
    int Render();
    int Destroy();
private:
    enum class Scene { Begin, Play };
    Scene m_scene = Scene::Begin;
    ScenceGameBegin m_sceneBegin;
    ScenceGamePlay m_scenePlay;
    GameUI m_ui;
    Player m_player;
};
