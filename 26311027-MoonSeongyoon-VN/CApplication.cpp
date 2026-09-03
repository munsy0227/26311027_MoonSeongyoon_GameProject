#include "CApplication.h"
#include <glc2d.h>
#include <cstdio>

int CApplication::Init()
{
    printf("Init.......................\n\n");
    g2_InitSdk();
    return 0;
}

int CApplication::CreateGameWindow()
{
    return g2_CreateWin(
        m_winPos.x,
        m_winPos.y,
        m_winSize.cx,
        m_winSize.cy,
        m_winName.c_str()
    );
}

int CApplication::Update()
{
    printf("Update.......................\n\n");
    return 0;
}

int CApplication::Destory()
{
    printf("Init.......................\n\n");
    g2_DestroyWin();
    return 0;
}

int CApplication::Render()
{
    printf("Render.......................\n\n");


    return 0;
}

int CApplication::InitSdk()
{
    printf("InitSdk.......................\n\n");
    return 0;
}
