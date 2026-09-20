#pragma once
#include "Player.h"
#include <string>

// Combat rules are independent of graphics, sound and input.
class BattleProgress
{
  public:
    enum class Command { Attack, Defend, Heal, Clue };
    enum class Phase { Inactive, Choosing, PlayerResult, EnemyResult, Retry, Won, Lost };
    enum class Sound { None, Attack, Heal };

    void Reset();
    void Start(int clues);
    bool CanUse(Command command, const Player& player) const;
    bool Choose(Command command, Player& player);
    bool Advance(Player& player);
    Phase GetPhase() const { return m_phase; }
    Sound GetSound() const { return m_sound; }
    int EnemyHP() const { return m_enemyHP; }
    int ExpectedDamage() const { return m_strong ? 25 : 15; }
    int Potions() const { return m_potions; }
    int Turn() const { return m_turn; }
    bool ClueUsed() const { return m_clueUsed; }
    bool IsActive() const { return m_phase != Phase::Inactive; }
    bool IsFinished() const { return m_phase == Phase::Won || m_phase == Phase::Lost; }
    const char* Message() const { return m_message.c_str(); }

  private:
    void ResetAttempt();
    Phase m_phase = Phase::Inactive;
    Sound m_sound = Sound::None;
    int m_enemyHP = 100;
    int m_potions = 1;
    int m_clues = 0;
    int m_turn = 1;
    bool m_strong = false;
    bool m_defending = false;
    bool m_clueUsed = false;
    std::string m_message;
};
