#include "ScenceGamePlay.h"
#include <cstdio>
#include <cstring>

namespace
{
const RECT Next{1460, 1260, 1830, 1360};
const RECT Back{1500, 35, 1860, 135};
const RECT Choices[2]{{200, 730, 1720, 840}, {200, 870, 1720, 980}};
const RECT Dialog{90, 1020, 1830, 1370};
}

int ScenceGamePlay::Init()
{
    m_background = g2_TextureLoad("resource/background/bg_corridor.png");
    m_classroom = g2_TextureLoad("resource/background/bg_classroom.png");
    m_character = g2_TextureLoad("resource/character/ghost_seoyeon.png", 0);
    m_shadow = g2_TextureLoad("resource/character/ghost_shadow.png", 0);
    Reset();
    return m_background < 0 || m_classroom < 0 || m_character < 0 || m_shadow < 0 ? -1 : 0;
}

void ScenceGamePlay::Reset()
{
    m_story.Reset();
    m_choiceFocus = 0;
}

int ScenceGamePlay::Destroy()
{
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
        if (m_story.IsComplete())
            return true;
        m_story.Advance(player);
        m_choiceFocus = 0;
    }
    return false;
}

void ScenceGamePlay::Render(const GameUI& ui, const Player& player)
{
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
    ui.Text(m_story.IsComplete() ? "다음 장면: 마지막 전투" : line.speaker,
        {140, 1060, 1790, 1130}, 1, 0xFFD4BB86);
    ui.Text(m_story.IsComplete() ? "그림자와 맞서기 위한 조사를 마쳤다.\n이후 이야기는 준비 중입니다." : line.text,
        {140, 1145, 1790, 1265});
    if (m_story.IsChoosing())
        ui.Text("위 / 아래 + Enter  |  숫자 1 / 2  |  선택지 클릭", {140, 1300, 1770, 1360}, 0, 0xFFABBDCD);
    else
    {
        ui.Text("Enter / Space / 대화창 클릭", {140, 1290, 1260, 1350}, 0, 0xFFABBDCD);
        ui.Button(m_story.IsComplete() ? "타이틀로" : "다음  >", Next);
    }
}
