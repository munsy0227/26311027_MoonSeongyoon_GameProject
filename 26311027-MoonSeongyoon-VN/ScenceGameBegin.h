#pragma once

class ScenceGameBegin
{
public:
    int Init();
    int Update();
    int Render();
    int Destroy();

protected:
    // game textures
    int m_txGameStart = -1;
    int m_txLena = -1;
    int m_txMario = -1;
};
