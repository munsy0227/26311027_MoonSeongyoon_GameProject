// include the 2d game header file
#include <glc2d.h>
#include <stdio.h>


// link the 2d game library
#if defined(_DEBUG)
#if defined(_M_X64) // 64-bit 아키텍처
#pragma comment(lib, "glc2d_x64_debug.lib")
#elif defined(_M_IX86) // 32-bit 아키텍처
#pragma comment(lib, "glc2d_win32_debug.lib")
#endif
#else
#if defined(_M_X64)
#pragma comment(lib, "glc2d_x64_release.lib")
#elif defined(_M_IX86)
#pragma comment(lib, "glc2d_win32_release.lib")
#endif
#endif


int		nTx;					// 이미지 인덱스
int		nTx2;


int Render()
{
	VEC2 pos1(0, 0);
	VEC2 pos2(0, 200);


	g2_Draw2D(nTx, {}, &pos1);
	g2_Draw2D(nTx2, {}, &pos2);

	return 0;
}


int main()
{
	//엔진을 초기화 한다.
	g2_InitSdk();
	printf("그림 올리기.......................\n\n");

	//배경색을 바꾼다.
	g2_SetClearColor(0xFF336699);

	// 화면에 출력하기 위해서 함수를 연결한다.
	g2_SetRender(Render);

	// window 생성.
	g2_CreateWin(100, 100, 800, 600, "My First Game Window");


	// 그림을 프로그램에 로딩
	nTx = g2_TextureLoad("Texture/GameStart.png");
	nTx2 = g2_TextureLoad("Texture/lena.png");

	// 실행
	g2_Run();


	// 텍스처 해제
	g2_TextureRelease(nTx);
	g2_TextureRelease(nTx2);

	// 윈도우 해제
	g2_DestroyWin();

	return 0;
}