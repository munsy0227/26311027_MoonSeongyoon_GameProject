#pragma once
#include "GameUI.h"
#include "Player.h"
class ScenceGamePlay
{
public:
    int Init();
    void Reset();
    bool Update(const GameUI& ui, Player& player);
    void Render(const GameUI& ui, const Player& player);
    int Destroy();
private:
    enum class Stage { Dialogue, Choice, Response, Complete };
    Stage m_stage = Stage::Dialogue;
    int m_line = 0;
    int m_choice = 0;
    int m_background = -1;
    int m_character = -1;
};
