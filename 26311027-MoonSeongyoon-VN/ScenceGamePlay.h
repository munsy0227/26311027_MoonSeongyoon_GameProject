#pragma once
#include "GameUI.h"
#include "Player.h"
#include "StoryProgress.h"

class ScenceGamePlay
{
  public:
    int Init();
    void Reset();
    bool Update(const GameUI &ui, Player &player);
    void Render(const GameUI &ui, const Player &player);
    int Destroy();
    const StoryProgress& GetProgress() const { return m_story; }

  private:
    StoryProgress m_story;
    int m_choiceFocus = 0;
    int m_background = -1;
    int m_classroom = -1;
    int m_character = -1;
    int m_shadow = -1;
};
