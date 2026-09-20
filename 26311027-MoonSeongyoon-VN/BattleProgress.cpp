#include "BattleProgress.h"
#include <algorithm>
#include <cstdio>

void BattleProgress::Reset()
{
    *this = BattleProgress{};
}

void BattleProgress::ResetAttempt()
{
    m_enemyHP = 100;
    m_potions = 1;
    m_turn = 1;
    m_strong = m_defending = m_clueUsed = false;
    m_sound = Sound::None;
    m_phase = Phase::Choosing;
    m_message = "그림자의 행동 예고를 보고 명령을 선택하세요.";
}

void BattleProgress::Start(int clues)
{
    if (IsActive())
        return;
    m_clues = clues;
    ResetAttempt();
}

bool BattleProgress::CanUse(Command command, const Player& player) const
{
    if (m_phase != Phase::Choosing || player.GetHP() == 0 || player.GetLife() == 0)
        return false;
    switch (command)
    {
    case Command::Attack:
    case Command::Defend: return true;
    case Command::Heal: return m_potions > 0 && player.GetHP() < 100;
    case Command::Clue: return m_clues > 0 && !m_clueUsed;
    default: return false;
    }
}

bool BattleProgress::Choose(Command command, Player& player)
{
    m_sound = Sound::None;
    if (!CanUse(command, player))
        return false;
    char message[256];
    switch (command)
    {
    case Command::Attack:
    case Command::Clue:
    {
        const int damage = (std::min)(m_enemyHP, command == Command::Clue ? 40 : 20);
        m_enemyHP -= damage;
        if (command == Command::Clue)
            m_clueUsed = true;
        sprintf_s(message, "%s 적 HP -%d.", command == Command::Clue ?
            "단서가 그림자의 주장을 무너뜨렸다." : "공격이 적중했다.", damage);
        m_message = message;
        m_sound = Sound::Attack;
        break;
    }
    case Command::Defend:
        m_defending = true;
        m_message = "방어 자세를 잡았다. 이번 적 공격 피해를 10으로 줄인다.";
        break;
    case Command::Heal:
    {
        const int before = player.GetHP();
        player.Heal(30);
        --m_potions;
        sprintf_s(message, "회복 아이템을 사용했다. HP +%d. 남은 회복약 %d개.",
            player.GetHP() - before, m_potions);
        m_message = message;
        m_sound = Sound::Heal;
        break;
    }
    }
    m_phase = Phase::PlayerResult;
    return true;
}

bool BattleProgress::Advance(Player& player)
{
    m_sound = Sound::None;
    if (m_phase == Phase::PlayerResult)
    {
        if (m_enemyHP == 0)
        {
            player.AddScore(20);
            m_phase = Phase::Won;
            m_message = "그림자가 흩어졌다. 전투 승리 +20점.\n4주차 전투 구현 구간을 완료했습니다.";
        }
        else
        {
            const int damage = (std::min)(player.GetHP(), m_defending ? 10 : ExpectedDamage());
            player.TakeDamage(damage);
            char message[256];
            sprintf_s(message, "%s HP -%d.", m_defending ? "방어로 충격을 줄였다." :
                (m_strong ? "강한 충격을 받았다." : "그림자의 공격을 받았다."), damage);
            m_message = message;
            m_defending = false;
            m_phase = Phase::EnemyResult;
            m_sound = Sound::Attack;
        }
        return true;
    }
    if (m_phase == Phase::EnemyResult)
    {
        if (player.GetHP() == 0)
        {
            m_phase = player.GetLife() > 0 ? Phase::Retry : Phase::Lost;
            m_message = player.GetLife() > 0 ?
                "HP가 0이 되었다. Life -1.\n다음을 누르면 HP 100과 전투 시작 상태로 재도전합니다." :
                "HP가 0이 되었다. 마지막 Life를 소진했다.\n조사를 계속할 수 없다. 타이틀에서 새 게임을 시작하세요.";
        }
        else
        {
            m_strong = !m_strong;
            ++m_turn;
            m_phase = Phase::Choosing;
            m_message = "그림자의 행동 예고를 보고 명령을 선택하세요.";
        }
        return true;
    }
    if (m_phase == Phase::Retry)
    {
        player.Retry();
        // Score and clues never change in a failed attempt. Restore the single
        // starting potion; life is deliberately kept at its reduced value.
        ResetAttempt();
        return true;
    }
    return false;
}
