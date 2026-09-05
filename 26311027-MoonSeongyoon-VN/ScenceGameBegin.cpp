#include "ScenceGameBegin.h"
#include <glc2d.h>

int ScenceGameBegin::Init()
{
    m_txGameStart = g2_TextureLoad("Texture/GameStart.png");
    m_txLena = g2_TextureLoad("Texture/lena.png");
    m_txMario = g2_TextureLoad("resource/character/mario.png");
    return 0;
}

int ScenceGameBegin::Destroy()
{
    g2_TextureRelease(m_txGameStart);
    g2_TextureRelease(m_txLena);
    g2_TextureRelease(m_txMario);
    return 0;
}

int ScenceGameBegin::Update()
{
    return 0;
}

int ScenceGameBegin::Render()
{
    VEC2 pos1(0, 0);
    VEC2 pos2(0, 200);
    VEC2 pos3(300, 200);

    g2_Draw2D(m_txGameStart, {}, &pos1);
    g2_Draw2D(m_txLena, {}, &pos2);
    g2_Draw2D(m_txMario, {}, &pos3);
    return 0;
}
