#pragma once
#include <Windows.h>
#include <string>
#include "ScenceGameBegin.h"

class CApplication
{
public:
	int Init();
	int CreateGameWindow();
	int Update();
	int Render();
	int Destory();

protected:
	int InitSdk();

protected:
	// windows
	POINT m_winPos{ 100, 100 };
	SIZE  m_winSize{ 768, 768 };

	std::string m_winName = "My Game Window";
	
	// game texture
	int m_txMario = -1;
};

