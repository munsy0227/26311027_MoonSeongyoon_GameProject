#include "ScenceGamePlay.h"
#include <cstdio>

namespace
{
struct Line
{
    const char *speaker;
    const char *text;
    bool showGhost;
};

const Line Lines[] = {
    {"서준", "사흘째 같은 시간, 같은 장소.\n그냥 지나치기에는 너무 정확하네.", false},
    {"서준", "이번에도 이름만 부르면 돌아간다.\n무슨 일인지 확인만 하고 나오는 거야.", false},
    {"???", "......서준.", false},
    {"서준", "내 이름까지 아는 건 확실하고.", false},
    {"???", "들리면...... 이쪽으로 와 줘.", false},
    {"서준", "목소리는 움직이지 않아.\n적어도 나를 따라붙는 쪽은 아니네.", false},
    {"서준", "거기 있었구나.", true},
    {"서연", "너...... 지금 나를 보고 말한 거야?", true},
    {"서연", "정말 내가 보이는구나.\n사흘 동안 불렀는데 아무도 한 번도 멈추지 않았어.", true},
    {"서준", "보이는 것만으로 여기까지 들어온 건 아니야.\n왜 내 이름을 알고 있었는지부터 말해 줘.", true},
    {"서연", "첫날, 담장 앞에서 전화받을 때 네가 이름을 말했어.\n그때 내 쪽을 돌아보길래... 혹시 들리는 걸까 했어.", true},
    {"서준", "내가 귀신을 볼 수 있다는 걸 알고 부른 건 아니네.", true},
    {"서연", "응. 그냥...... 네가 나를 지나치지 않을 것 같았어.", true},
    {"서준", "그 기대만으로 위험한 폐교에 사람을 불렀다면,\n부탁 정도는 제대로 설명해야겠지.", true},
    {"서연", "미안해. 내가 기억하는 게 많지 않아.\n이름은 서연이고, 교실로 돌아가야 한다는 것만 확실해.", true},
    {"서준", "왜 돌아가야 하는데?", true},
    {"서연", "누군가한테 사과해야 해.\n내가 그 애를 두고 먼저 도망친 것 같아.", true},
    {"서준", "도망쳤는데 넌 왜 아직 여기 있어?", true},
    {"서연", "그걸 모르겠어. 나는 교실 안에 남아 있었던 것 같기도 해.", true},
    {"서준", "둘 다 사실일 수는 없겠네. 적어도 네가 기억한 사건과\n네가 내린 결론 중 하나는 틀렸어.", true},
    {"서연", "내가 누군가를 문 쪽으로 밀었어.\n그 장면만 생각하면 내가 그 애를 버린 것 같아.", true},
    {"서준", "밀었다는 행동만 기억나고,\n왜 밀었는지는 기억 안 난다는 거지?", true},
    {"서연", "응. 그래서 더 무서워. 이유를 잊은 건지,\n이유 같은 건 애초에 없었던 건지 모르겠어.", true},
    {"서준", "좋아. 그럼 기억나는 사실과 네 추측을 따로 보자.\n내가 찾은 증거랑 맞는지도 확인하고.", true},
    {"서연", "그래 줘. 내가 정말 잘못했다면 도망치지 않을게.\n대신 내가 잘못 기억하고 있다면 그것도 알려 줘.", true},
    {"서연", "복도 보관함에 사진이 있었어. 그리고 교실에서는......\n누군가 손을 잡았던 감각이 남아 있어.", true}};
const int LineCount = sizeof(Lines) / sizeof(Lines[0]);
const RECT Next{1460, 1260, 1830, 1360};
const RECT Back{1500, 35, 1860, 135};
const RECT ChoiceA{200, 710, 1720, 820};
const RECT ChoiceB{200, 850, 1720, 960};
const RECT Dialog{90, 1020, 1830, 1370};
} // namespace

int ScenceGamePlay::Init()
{
    m_background = g2_TextureLoad("resource/background/bg_corridor.png");
    m_character = g2_TextureLoad("resource/character/ghost_seoyeon.png", 0);
    Reset();
    return m_background < 0 || m_character < 0 ? -1 : 0;
}

void ScenceGamePlay::Reset()
{
    m_stage = Stage::Dialogue;
    m_line = 0;
    m_choice = 0;
}

int ScenceGamePlay::Destroy()
{
    if (m_background >= 0)
        g2_TextureRelease(m_background);
    if (m_character >= 0)
        g2_TextureRelease(m_character);
    m_background = m_character = -1;
    return 0;
}

bool ScenceGamePlay::Update(const GameUI &ui, Player &player)
{
    if (ui.Pressed(VK_ESCAPE) || ui.Clicked(Back))
    {
        ui.PlaySelect();
        return true;
    }
    if (m_stage == Stage::Choice)
    {
        if (ui.Pressed('1') || ui.Clicked(ChoiceA))
            m_choice = 1;
        else if (ui.Pressed('2') || ui.Clicked(ChoiceB))
            m_choice = 2;
        if (m_choice)
        {
            ui.PlaySelect();
            if (m_choice == 1)
                player.AddScore(10);
            m_stage = Stage::Response;
            m_line = 0;
        }
    }
    else if (ui.Advance() || ui.Clicked(Next) || ui.Clicked(Dialog))
    {
        ui.PlaySelect();
        if (m_stage == Stage::Dialogue && ++m_line >= LineCount)
        {
            m_line = LineCount - 1;
            m_stage = Stage::Choice;
        }
        else if (m_stage == Stage::Response && ++m_line >= 2)
            m_stage = Stage::Complete;
        else if (m_stage == Stage::Complete)
            return true;
    }
    return false;
}

void ScenceGamePlay::Render(const GameUI &ui, const Player &player)
{
    ui.Image(m_background, 0, 0, 1920, 1440);
    if (m_stage != Stage::Dialogue || Lines[m_line].showGhost)
        ui.Image(m_character, 980, 135, 778, 1200);
    ui.Panel({30, 35, 1420, 135}, 0xDA101923);
    char status[160];
    sprintf_s(status, "폐교 복도   |   서준  HP %d   LIFE %d   점수 %d", player.GetHP(), player.GetLife(),
              player.GetScore());
    ui.Text(status, {60, 63, 1410, 130}, 0);
    ui.Button("타이틀로 [Esc]", Back);
    const char *speaker = "서연";
    const char *dialogue = "내 기억을 어떻게 받아들일까?";
    if (m_stage == Stage::Dialogue)
    {
        speaker = Lines[m_line].speaker;
        dialogue = Lines[m_line].text;
    }
    else if (m_stage == Stage::Choice)
    {
        speaker = "선택";
        ui.Button("1. 서연의 말을 우선 믿고 조사한다.", ChoiceA);
        ui.Button("2. 증거를 확인하기 전까지 판단을 보류한다.", ChoiceB);
    }
    else if (m_stage == Stage::Response)
    {
        speaker = m_line == 0 ? "서준" : "서연";
        if (m_choice == 1)
            dialogue = m_line == 0
                           ? "그럼 우선 네 말을 믿고 따라가 볼게.\n다만 중간에 이상한 점이 나오면 바로 확인한다."
                           : "고마워. 내가 틀렸더라도 끝까지 같이 확인해 줘.";
        else
            dialogue = m_line == 0 ? "도와주긴 할게. 하지만 판단은 증거를 본 다음이야.\n네 기억부터 의심할 수도 있어."
                                   : "그래. 나도 내 기억이 무조건 맞다고 말할 자신은 없어.";
    }
    else
    {
        speaker = "첫 만남 완료";
        dialogue = "서연과 함께 복도 보관함을 조사하기로 했다.\n이번 구현은 여기까지입니다. 타이틀에서 다시 시작할 수 "
                   "있습니다.";
    }
    ui.Panel(Dialog, 0xEE101923);
    ui.Panel({90, 1020, 1830, 1025}, 0xFFD4BB86);
    ui.Text(speaker, {140, 1060, 1500, 1130}, 1, 0xFFD4BB86);
    ui.Text(dialogue, {140, 1145, 1790, 1270});
    if (m_stage != Stage::Choice)
    {
        ui.Text("Enter / Space / 대화창 클릭", {140, 1290, 1260, 1350}, 0, 0xFFABBDCD);
        ui.Button(m_stage == Stage::Complete ? "타이틀로" : "다음  >", Next);
    }
}
