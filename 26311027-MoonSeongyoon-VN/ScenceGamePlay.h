#pragma once
#include "GameUI.h"
#include "Player.h"
#include "StoryProgress.h"
#include "BattleProgress.h"

class ScenceGamePlay
{
  public:
    int Init();
    void Reset();
    bool Update(const GameUI &ui, Player &player);
    void Render(const GameUI &ui, const Player &player);
    int Destroy();
    const StoryProgress& GetProgress() const { return m_story; }

    bool IsBattleMusic() const { return m_battle.IsActive() && !m_battle.IsFinished(); }

  private:
    void RenderBattle(const GameUI& ui, const Player& player);
    void PlayBattleSound() const;
    BattleProgress m_battle;
    int m_attackSound = -1;
    int m_healSound = -1;
    StoryProgress m_story;
    int m_choiceFocus = 0;
    int m_background = -1;
    int m_classroom = -1;
    int m_character = -1;
    int m_shadow = -1;
};
