#include "ScenceGamePlay.h"
#include <cstdio>
#include <cstring>

namespace
{
const RECT Next{1460, 1260, 1830, 1360};
const RECT Back{1500, 35, 1860, 135};
const RECT Choices[2]{{200, 730, 1720, 840}, {200, 870, 1720, 980}};
const RECT Commands[4]{{90, 850, 510, 970}, {530, 850, 950, 970},
    {970, 850, 1390, 970}, {1410, 850, 1830, 970}};
const RECT Dialog{90, 1020, 1830, 1370};
}

int ScenceGamePlay::Init()
{
    m_background = g2_TextureLoad("resource/background/bg_corridor.png");
    m_classroom = g2_TextureLoad("resource/background/bg_classroom.png");
    m_character = g2_TextureLoad("resource/character/ghost_seoyeon.png", 0);
    m_shadow = g2_TextureLoad("resource/character/ghost_shadow.png", 0);
    m_attackSound = g2_SoundLoad("resource/sound/se_attack.mp3");
    m_healSound = g2_SoundLoad("resource/sound/se_heal.mp3");
    Reset();
    return m_background < 0 || m_classroom < 0 || m_character < 0 || m_shadow < 0 || m_attackSound < 0 || m_healSound < 0 ? -1 : 0;
}

void ScenceGamePlay::Reset()
{
    m_story.Reset();
    m_battle.Reset();
    m_choiceFocus = 0;
}

int ScenceGamePlay::Destroy()
{
    for (int* sound : {&m_attackSound, &m_healSound})
    {
        if (*sound >= 0)
        {
            g2_SoundStop(*sound);
            g2_SoundRelease(*sound);
        }
        *sound = -1;
    }
    for (int* texture : {&m_background, &m_classroom, &m_character, &m_shadow})
    {
        if (*texture >= 0)
            g2_TextureRelease(*texture);
        *texture = -1;
    }
    return 0;
}

bool ScenceGamePlay::Update(const GameUI& ui, Player& player)
{
    if (ui.Pressed(VK_ESCAPE) || ui.Clicked(Back))
    {
        ui.PlaySelect();
        return true;
    }
    if (m_battle.IsActive())
    {
        if (m_battle.GetPhase() == BattleProgress::Phase::Choosing)
        {
            if (ui.Pressed(VK_UP))
                m_choiceFocus = (m_choiceFocus + 3) % 4;
            else if (ui.Pressed(VK_DOWN))
                m_choiceFocus = (m_choiceFocus + 1) % 4;
            int selection = -1;
            for (int i = 0; i < 4; ++i)
                if (ui.Pressed('1' + i) || ui.Clicked(Commands[i]))
                {
                    selection = i;
                    break;
                }
            if (selection < 0 && ui.Pressed(VK_RETURN))
                selection = m_choiceFocus;
            if (selection >= 0 && m_battle.Choose(static_cast<BattleProgress::Command>(selection), player))
            {
                m_choiceFocus = selection;
                ui.PlaySelect();
                PlayBattleSound();
            }
        }
        else if (ui.Advance() || ui.Clicked(Next) || ui.Clicked(Dialog))
        {
            ui.PlaySelect();
            if (m_battle.IsFinished())
                return true;
            m_battle.Advance(player);
            PlayBattleSound();
        }
        return false;
    }
    if (m_story.IsChoosing())
    {
        if (ui.Pressed(VK_UP) || ui.Pressed(VK_DOWN))
            m_choiceFocus = 1 - m_choiceFocus;
        int selection = -1;
        if (ui.Pressed('1') || ui.Clicked(Choices[0]))
            selection = 0;
        else if (ui.Pressed('2') || ui.Clicked(Choices[1]))
            selection = 1;
        else if (ui.Pressed(VK_RETURN))
            selection = m_choiceFocus;
        if (selection >= 0 && m_story.Choose(selection, player))
        {
            ui.PlaySelect();
            m_choiceFocus = 0;
        }
        return false;
    }
    if (ui.Advance() || ui.Clicked(Next) || ui.Clicked(Dialog))
    {
        ui.PlaySelect();
        m_story.Advance(player);
        if (m_story.IsComplete())
            m_battle.Start(m_story.ClueCount());
        m_choiceFocus = 0;
    }
    return false;
}

void ScenceGamePlay::Render(const GameUI& ui, const Player& player)
{
    if (m_battle.IsActive())
    {
        RenderBattle(ui, player);
        return;
    }
    const auto& section = m_story.Section();
    const auto& line = m_story.Line();
    const bool classroom = section.chapter >= 5;
    ui.Image(classroom ? m_classroom : m_background, 0, 0, 1920, 1440);
    if (line.visual == StoryVisual::Both)
    {
        ui.Image(m_character, 180, 195, 700, 1080,
            std::strcmp(line.speaker, "그림자") == 0 ? 0xFFAAAAAA : 0xFFFFFFFF);
        ui.Image(m_shadow, 1070, 165, 700, 1080,
            std::strcmp(line.speaker, "서연") == 0 ? 0xFFAAAAAA : 0xFFFFFFFF);
    }
    else if (line.visual == StoryVisual::Seoyeon)
        ui.Image(m_character, 980, 135, 778, 1200);

    ui.Panel({30, 35, 1460, 215}, 0xE8101923);
    char status[256];
    sprintf_s(status, "%s  |  %s", classroom ? "폐교 교실" : "폐교 복도", section.title);
    ui.Text(status, {60, 55, 1430, 105}, 0, 0xFFD4BB86);
    sprintf_s(status, "서준  HP %d  /  LIFE %d  /  점수 %d  /  핵심 단서 %d / 2",
        player.GetHP(), player.GetLife(), player.GetScore(), m_story.ClueCount());
    ui.Text(status, {60, 110, 1430, 160}, 0);
    sprintf_s(status, "열쇠: %s   |   사진: %s   |   사고 기록: %s",
        m_story.HasKey() ? "보유" : "없음", m_story.HasPhoto() ? "보유" : "없음",
        m_story.HasRecord() ? "보유" : "없음");
    ui.Text(status, {60, 165, 1430, 210}, 0, 0xFFABBDCD);
    ui.Button("타이틀로 [Esc]", Back);

    if (m_story.IsChoosing())
    {
        for (int i = 0; i < 2; ++i)
        {
            const auto& rect = Choices[i];
            if (i == m_choiceFocus)
                ui.Panel({rect.left - 5, rect.top - 5, rect.right + 5, rect.bottom + 5}, 0xFFD4BB86);
            char label[256];
            sprintf_s(label, "%d. %s", i + 1, section.options[i]);
            ui.Button(label, rect);
        }
    }

    ui.Panel(Dialog, 0xF2101923);
    ui.Panel({90, 1020, 1830, 1025}, 0xFFD4BB86);
    ui.Text(line.speaker,
        {140, 1060, 1790, 1130}, 1, 0xFFD4BB86);
    ui.Text(line.text,
        {140, 1145, 1790, 1265});
    if (m_story.IsChoosing())
        ui.Text("위 / 아래 + Enter  |  숫자 1 / 2  |  선택지 클릭", {140, 1300, 1770, 1360}, 0, 0xFFABBDCD);
    else
    {
        ui.Text("Enter / Space / 대화창 클릭", {140, 1290, 1260, 1350}, 0, 0xFFABBDCD);
        ui.Button("다음  >", Next);
    }
}

void ScenceGamePlay::PlayBattleSound() const
{
    const auto sound = m_battle.GetSound();
    int handle = sound == BattleProgress::Sound::Attack ? m_attackSound :
        sound == BattleProgress::Sound::Heal ? m_healSound : -1;
    if (handle >= 0)
    {
        g2_SoundStop(handle);
        g2_SoundPlay(handle);
    }
}

void ScenceGamePlay::RenderBattle(const GameUI& ui, const Player& player)
{
    ui.Image(m_classroom, 0, 0, 1920, 1440);
    ui.Image(m_character, 180, 195, 700, 1080);
    if (m_battle.GetPhase() != BattleProgress::Phase::Won)
        ui.Image(m_shadow, 1070, 165, 700, 1080);
    ui.Panel({30, 35, 1460, 215}, 0xE8101923);
    ui.Text("폐교 교실 | 마지막 전투", {60, 55, 1430, 105}, 0, 0xFFD4BB86);
    char text[256];
    sprintf_s(text, "서준 HP %d / LIFE %d / 점수 %d / 핵심 단서 %d / 2",
        player.GetHP(), player.GetLife(), player.GetScore(), m_story.ClueCount());
    ui.Text(text, {60, 110, 1430, 160}, 0);
    sprintf_s(text, "회복약 %d개 | 단서 활용: %s", m_battle.Potions(),
        m_battle.ClueUsed() ? "사용 완료" : (m_story.ClueCount() ? "사용 가능" : "단서 없음"));
    ui.Text(text, {60, 165, 1430, 210}, 0, 0xFFABBDCD);
    ui.Button("타이틀로 [Esc]", Back);

    if (!m_battle.IsFinished())
    {
        ui.Panel({1060, 245, 1860, 415}, 0xE8101923);
        sprintf_s(text, "그림자 HP %d / 100 | %d턴", m_battle.EnemyHP(), m_battle.Turn());
        ui.Text(text, {1090, 260, 1830, 310}, 0);
        ui.Panel({1090, 325, 1830, 350}, 0xFF333344);
        ui.Panel({1090, 325, 1090 + m_battle.EnemyHP() * 740 / 100, 350}, 0xFFB45D67);
        if (m_battle.GetPhase() == BattleProgress::Phase::Choosing)
        {
            sprintf_s(text, "적 행동 예고: %s / 예상 피해 %d",
                m_battle.ExpectedDamage() == 25 ? "강한 공격" : "일반 공격", m_battle.ExpectedDamage());
            ui.Text(text, {1090, 365, 1840, 410}, 0, 0xFFD4BB86);
        }
    }
    if (m_battle.GetPhase() == BattleProgress::Phase::Choosing)
    {
        const char* labels[]{"1. 공격 (-20)", "2. 방어 (피해 10)", "3. 회복 (+30)", "4. 단서 (-40)"};
        for (int i = 0; i < 4; ++i)
        {
            const auto& r = Commands[i];
            const bool enabled = m_battle.CanUse(static_cast<BattleProgress::Command>(i), player);
            if (i == m_choiceFocus)
                ui.Panel({r.left - 5, r.top - 5, r.right + 5, r.bottom + 5}, 0xFFD4BB86);
            ui.Panel(r, enabled ? (ui.Hit(r) ? 0xEE456074 : 0xF21A2939) : 0xF222252A);
            ui.Text(labels[i], {r.left + 14, r.top + 18, r.right - 8, r.top + 65}, 0,
                enabled ? 0xFFF2EEE7 : 0xFF858585);
            if (!enabled)
            {
                const char* reason = i == 2 ? (m_battle.Potions() == 0 ? "회복약 없음" : "HP 최대") :
                    (m_battle.ClueUsed() ? "이미 활용함" : "단서 없음");
                ui.Text(reason, {r.left + 14, r.top + 68, r.right - 8, r.bottom - 5}, 0, 0xFF858585);
            }
        }
    }
    ui.Panel(Dialog, 0xF2101923);
    ui.Panel({90, 1020, 1830, 1025}, 0xFFD4BB86);
    ui.Text(m_battle.GetPhase() == BattleProgress::Phase::Won ? "전투 승리" :
        m_battle.GetPhase() == BattleProgress::Phase::Lost ? "전투 실패" : "전투", {140, 1060, 1790, 1130}, 1, 0xFFD4BB86);
    ui.Text(m_battle.Message(), {140, 1145, 1790, 1265});
    if (m_battle.GetPhase() == BattleProgress::Phase::Choosing)
        ui.Text("위 / 아래 + Enter | 숫자 1~4 | 명령 클릭", {140, 1290, 1770, 1350}, 0, 0xFFABBDCD);
    else
    {
        ui.Text("Enter / Space / 대화창 클릭", {140, 1290, 1260, 1350}, 0, 0xFFABBDCD);
        ui.Button(m_battle.IsFinished() ? "타이틀로" : "다음 >", Next);
    }
}
