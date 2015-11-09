#include <windows.h>//윈플 과제 2-1
#include <stdlib.h>
#include <time.h>
#include "resource.h"
#pragma comment(lib,"winmm")
#include<mmsystem.h>
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
LPCTSTR lpszClass = TEXT("윈도우 프로그래밍 연습문제 4-1");

typedef struct box
{
	int x, y;
	int check;
};

BOOL CALLBACK MainDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)

{

	HICON  hIcon;  // 아이콘 핸들




	switch (iMsg)

	{

	case WM_INITDIALOG:  // 다이얼로그 초기화 작업에 필요한 메시지에서 처리



		// g_hInst 는 WinMain 함수의 첫번째 인자로 넘어오는 HINSTANCE 핸들을 저장한

		//  전역변수

		// IDI_ICON1 는 리소스 편집기에 등록한 새로운 아이콘의 ID

		hIcon = LoadIcon(g_hInst, MAKEINTRESOURCE(IDI_ICON1));




		// 다이얼로그 상자로 아이콘을 설정하는 메시지를 보낸다.

		// ICON_BIG은 32*32 아이콘을 의미...

		SendMessage(hDlg, WM_SETICON, (WPARAM)ICON_BIG, (LPARAM)hIcon);

		return TRUE;



	}
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	RECT rc = { 0, 0, 1000, 1000 };
	g_hInst = hInstance;
	int width = rc.right - rc.left;
	int height = rc.bottom - rc.top;
	HACCEL hAcc;

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = WndProc;
	WndClass.lpszClassName = lpszClass;
	WndClass.lpszMenuName = lpszClass;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);
	hWnd = CreateWindow(lpszClass, lpszClass, WS_BORDER, 0, 0, width, height,
		NULL, (HMENU)NULL, hInstance, NULL);

	ShowWindow(hWnd, nCmdShow);
	while (GetMessage(&Message, NULL, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return (int)Message.wParam;
}

typedef struct xyc{
	double x;
	double y;
	int check;
};
typedef struct chract{
	double x;
	double y;
	int xpos;
	int ypos;
	int check;
	int bomb;
	int power;
};
typedef struct bombP{
	int x;
	int y;
	int num;
};
typedef struct MM{
	double x;
	double y;
	int xpos;
	int ypos;
	int check;
};
//BOOL InCircle(int x, int y, int mx, int my);

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	static HDC      hdc, mem1DC, mem2DC;//배경화면dc
	static HDC  startDC, exitDC;//시작버튼 종료버튼 dc

	srand((unsigned)time(NULL));
	PAINTSTRUCT ps;
	static HBITMAP START[4], hbit1, hbit3, hbit2, item[3], oldBit1, bbox, bomb, oldBit2, bit, obit, hero, startbutton, startbit, useitem[2], exitbutton, exitbit, stage1, ch[8], after, bbox2,gold,
		dead, end, monsterpicture, stage3, endingP, stage2,ghostP;
	static HBITMAP;
	//bit 는 배경화면 ,oldbit도 배경화면
	static RECT rectview;
	static BOOL drag;
	static int i, j, animation, numberbomb, monstermove[5], a[5], bossbomb, bossheal, musicon, useOFF[2], ghosttime,moveup;
	static bombP bombI[5];
	static int mx, my, speed, movecheck,star[2],time;
	static xyc map[9][13];
	static xyc map3[9][15];
	static xyc map2[11][13];
	static int n, timen, bombtime[5];
	static int start;//사각형 클릭할껏들.
	static chract chracter;
	static box box[2];//게임시작 종료 버튼 박스.
	static MM monster[5];//몬스터
	static int monsterbom[5];

	static MM ghost;
	static int bossmove;
	static HBITMAP boss, bossbit;
	static int deadcheck;




	/*map.check ==  0  : 빈공간
	1 ;
	2 : 상자
	3 : 움직이는 아이템을 가진 상자
	4 : 화력 아이템 상자
	5 : 폭탄 개수  상자
	6~9= 폭탄 설치 애니메이션
	10= 캐릭터 위치
	11~15 = 폭탄 터지는 애니메이션
	11 - 제자리 터지기
	12 = 아래
	13 = 위
	14 = 왼
	15 = 오른
	23,24,25 = 아이템
	30=보스;

	chrachter = 0~3 방향  아래 위 왼 오른
	5 사망
	*/

	switch (iMsg)
	{
	case WM_CREATE:

		GetClientRect(hwnd, &rectview);
	stage1:
		deadcheck = 0;
		moveup = 0;
		drag = FALSE;
		bossbomb = 0;
		exitbutton = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP2));
		//     hbit1 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP4));
		startbutton = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP3));
		hbit2 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP1));
		stage1 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP4));
		ch[0] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP5));
		ch[1] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP6));
		ch[2] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP12));
		ch[3] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP13));
		ch[4] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP14));
		ch[5] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP15));
		ch[6] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP16));
		ch[7] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP17));
		bbox = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP7));
		START[0] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP8));
		START[1] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP9));
		START[2] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP10));
		START[3] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP11));
		bomb = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP18));
		after = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP18));
		hero = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP20));
		item[0] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP21));
		item[1] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP22));
		item[2] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP23));
		dead = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP24));
		end = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP25));
		monsterpicture = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP26));
		bossbit = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP27));
		stage3 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP28));
		endingP = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP29));
		stage2 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP30));
		bbox2 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP31));
		useitem[0] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP32));
		gold = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP33));
		ghostP = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP34));
		//	useitem[1] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP34));
		box[0].x = rectview.right / 2 - 100;
		box[0].y = rectview.bottom / 2;
		box[1].x = rectview.right / 2 - 75;
		box[1].y = rectview.bottom / 2 + 50;
		musicon = 0;
		time = 0;
		for (int y = 0; y < 9; y++){
			for (int x = 0; x < 13; x++){
				map[y][x].y = rectview.bottom*0.052 + 0.0975*rectview.bottom*y;
				map[y][x].x = rectview.right*0.075 + 0.0661*rectview.right*x;
			}
		}


		for (int y = 0; y < 11; y++){
			for (int x = 0; x < 13; x++){
				map2[y][x].y = rectview.bottom*0.047 + 0.0692*rectview.bottom*y;
				map2[y][x].x = rectview.right*0.065 + 0.0670*rectview.right*x;
				map2[y][x].check = 0;

				if ((y % 2 == 1 && x % 2 == 1) || x == 0 && y == 5 || x == 12 && y == 5
					|| (x == 3 && (y == 0 || y == 2 || y == 8 || y == 10))
					|| (x == 9 && (y == 0 || y == 2 || y == 8 || y == 10))){
					map2[y][x].check = 1;
				}
				if (!(y % 2 == 1 && x % 2 == 1) && !(x < 2 && y < 2) && !(x < 2 && y>9) && !(x>10 && y < 2) && !(x>10 && y>9)
					&& !(x>4 && x < 7 && y < 2) && !(x>4 && x < 7 && y > 9) && !(x == 0 && y == 5) && !(x == 12 && y == 5) && !(x == 3 && (y == 0 || y == 2 || y == 8 || y == 10))
					&& !(x == 9 && (y == 0 || y == 2 || y == 8 || y == 10))){
					n = rand() % 100;
					if (n < 60)
						map2[y][x].check = 2;
					if (n < 30)
						map2[y][x].check = 3;
					if (n < 20)
						map2[y][x].check = 4;
					if (n < 10)
						map2[y][x].check = 5;
				}
			}
		}

		for (int y = 0; y < 9; y++){
			for (int x = 0; x < 13; x++){
				map[y][x].check = 0;
				if ((y % 2 == 1 && x % 2 == 1)){
					map[y][x].check = 1;
				}
				if (!(y % 2 == 1 && x % 2 == 1) && !(x < 2 && y < 2) && !(x < 2 && y>6) && !(x>10 && y < 2) && !(x>10 && y>6)){
					n = rand() % 100;
					if (n < 60)
						map[y][x].check = 2;
					if (n < 30)
						map[y][x].check = 3;
					if (n < 20)
						map[y][x].check = 4;
					if (n < 10)
						map[y][x].check = 5;
				}
			}
		}
		for (int y = 0; y < 9; y++){
			for (int x = 0; x < 15; x++){
				map3[y][x].y = 0.0975*rectview.bottom*y;
				map3[y][x].x = 0.0661*rectview.right*x;
			}
		}
		for (int y = 5; y < 8; y++){
			for (int x = 5; x < 9; x++){
				map3[y][x].check = 0;

				if (!(x == 0 && y == 0) && !(x == 10 && y == 0) && !(x == 0 && y == 9) && !(x == 10 && y == 9))//0,0   10,0   0,9  10,9  빈공간만들기  확률 나머지 상자
				{
					n = rand() % 100;
					if (n < 60)
						map3[y][x].check = 2;
					if (n < 40)
						map3[y][x].check = 3;
					if (n < 20)
						map3[y][x].check = 4;
				
				}
			}
		}


		monster[0].xpos = 12;
		monster[0].ypos = 0;
		monster[0].check = 0;
		monster[1].xpos = 0;
		monster[1].ypos = 8;
		monster[1].check = 0;
		monster[2].xpos = 12;
		monster[2].ypos = 8;
		monster[2].check = 0;

		ghost.xpos = rand ()% 6+3;
		ghost.ypos = rand() % 6+3;
		ghost.x = map2[ghost.ypos][ghost.xpos].x;
		ghost.y = map2[ghost.ypos][ghost.xpos].y;

		a[0] = 0;
		a[1] = 0;
		a[2] = 0;

		
		animation = 0;
		timen = 0;
		movecheck = 0;
		chracter.xpos = 0;
		for (int i = 0; i < 5; i++)		bombtime[i] = 0;
		chracter.ypos = 0;
		chracter.bomb = 1;
		numberbomb = 1;
		chracter.power = 1;
		chracter.check = 0;
		speed = 100;
		chracter.x = map[chracter.ypos][chracter.xpos].x;
		chracter.y = map[chracter.ypos][chracter.xpos].y;

		map[0][0].check = 10;

		start = 0;
		for (int y = 0; y < 3; y++){
			for (int x = 5; x < 10; x++){
				map3[y][x].check = 30;
			}
		}
		

		star[0] = 1;
		star[1] = 1;
		useOFF[0] = 1;
		useOFF[1] = 1;
		for (int a = 0; a < 5; a++){
		
			monster[a].x = map2[monster[a].ypos][monster[a].xpos].x;
			monster[a].y = map2[monster[a].ypos][monster[a].xpos].y;
			monstermove[a] = 0;
		}
		ghosttime = 0;
		SetTimer(hwnd, 5, 500, NULL);
	
		
		PlaySound(MAKEINTRESOURCE(IDR_WAVE5), g_hInst, SND_RESOURCE | SND_ASYNC | SND_LOOP);
		break;
	case WM_LBUTTONDOWN:
		mx = LOWORD(lParam);
		my = HIWORD(lParam);
		drag = TRUE;
		if (start == 1){
			if (mx >= box[1].x&&mx <= box[1].x + 132 && my >= box[1].y&& my <= box[1].y + 23)//게임종료
			{
				PostQuitMessage(0);
				break;
			}
			else if (mx >= box[0].x&&mx <= box[0].x + 183 && my >= box[0].y  && my <= box[0].y + 32)//게임시작
			{
				start = 2;
				PlaySound(NULL, 0, 0);
				for (int a = 0; a < 3; a++){
					map[monster[a].ypos][monster[a].xpos].check = 31;
					monster[a].x = map[monster[a].ypos][monster[a].xpos].x;
					monster[a].y = map[monster[a].ypos][monster[a].xpos].y;
					monstermove[a] = 0;
				}
				SetTimer(hwnd, 8, 500, NULL);

			}
		}
		if (start == 10){
			if (mx >= box[1].x+300&&mx <= box[1].x + 132+300 && my >= box[1].y+300&& my <= box[1].y + 23+300)//게임종료
			{
				PostQuitMessage(0);
				break;
			}
		}
		if (start == 5){
			if (mx >= box[1].x + 300 && mx <= box[1].x + 132 + 300 && my >= box[1].y + 300 && my <= box[1].y + 23 + 300)//게임종료
			{
				PostQuitMessage(0);
				break;
			}
		}

		InvalidateRect(hwnd, NULL, TRUE);
		break;


	case WM_LBUTTONUP:
		drag = FALSE;


		break;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		GetClientRect(hwnd, &rectview);
		//dc부분======
		for (int y = 0; y < 9; y++){
			for (int x = 0; x < 13; x++){
				map[y][x].y = rectview.bottom*0.052 + 0.0975*rectview.bottom*y;
				map[y][x].x = rectview.right*0.075 + 0.0661*rectview.right*x;
			}
		}
		if (hbit1 == NULL) // hBit1을 hdc와 호환되게 만들어준다.
			hbit1 = CreateCompatibleBitmap(hdc, 9999, 2000);


		// hdc에서 mem1dc를 호환되도록 만들어준다.
		mem1DC = CreateCompatibleDC(hdc);
		// mem1dc에서 mem2dc를 호환이 되도록 만들어준다.

		mem2DC = CreateCompatibleDC(mem1DC);
		// mem2dc의 비트맵을 mem1dc에 옮기고, mem1dc를 hdc로 옮기려고 함

		oldBit1 = (HBITMAP)SelectObject(mem1DC, hbit1); // mem1dc에는 hBit1
		if (start == 0){
			oldBit2 = (HBITMAP)SelectObject(mem2DC, START[animation]); // mem2dc에는 hBit2
			if (animation == 0)
				StretchBlt(mem1DC, 0, 0, rectview.right, rectview.bottom, mem2DC, 0, 0, 403, 404, SRCCOPY);//게임시작화면
			if (animation == 1)
				StretchBlt(mem1DC, 0, 0, rectview.right, rectview.bottom, mem2DC, 0, 0, 547, 398, SRCCOPY);//게임시작화면
			if (animation == 2)
				StretchBlt(mem1DC, 0, 0, rectview.right, rectview.bottom, mem2DC, 0, 0, 545, 393, SRCCOPY);//게임시작화면
			if (animation == 3)
				StretchBlt(mem1DC, 0, 0, rectview.right, rectview.bottom, mem2DC, 0, 0, 548, 393, SRCCOPY);//게임시작화면

		}
		if (start == 1){
			oldBit2 = (HBITMAP)SelectObject(mem2DC, hbit2); // mem2dc에는 hBit2
			//화면에뜰부분========
			StretchBlt(mem1DC, rectview.left, rectview.top, rectview.right, rectview.bottom, mem2DC, 0, 0, 567, 486, SRCCOPY);//게임시작화면

			oldBit2 = (HBITMAP)SelectObject(mem2DC, startbutton); // mem2dc에는 hBit2
			StretchBlt(mem1DC, rectview.right / 2 - 100, rectview.bottom / 2, 183, 32, mem2DC, 0, 0, 183, 32, SRCCOPY);//게임시작버튼

			oldBit2 = (HBITMAP)SelectObject(mem2DC, exitbutton); // mem2dc에는 hBit2
			StretchBlt(mem1DC, rectview.right / 2 - 75, rectview.bottom / 2 + 50, 132, 25, mem2DC, 0, 0, 132, 25, SRCCOPY);//게임종료버튼

			//Rectangle(hdc, box[0].x, box[0].y, box[1].x, box[1].y);//게임스타트 사각형
		}
		if (start == 2)
		{
			DeleteDC(mem1DC);
			mem1DC = CreateCompatibleDC(hdc);
			if (hbit3 == NULL) // hBit1을 hdc와 호환되게 만들어준다.
				hbit3 = CreateCompatibleBitmap(hdc, 3000, 2000);
			oldBit1 = (HBITMAP)SelectObject(mem1DC, hbit3); // mem1dc에는 hBit1
			oldBit2 = (HBITMAP)SelectObject(mem2DC, stage1); // mem2dc에는 hBit2
			StretchBlt(mem1DC, 0, 0, rectview.right, rectview.bottom, mem2DC, 0, 0, 484, 330, SRCCOPY);//배경
			for (int y = 0; y < 9; y++){
				for (int x = 0; x < 13; x++){
					if (map[y][x].check >= 11 && map[y][x].check <= 15){
						if (chracter.y >= map[y][x].y&&chracter.x >= map[y][x].x)
							if ((chracter.y - map[y][x].y < 0.0975*rectview.bottom / 2 && chracter.x - map[y][x].x < 0.0661*rectview.right / 2)){
								chracter.check = 5;
								SetTimer(hwnd, 7, 500, NULL);
								KillTimer(hwnd, 6);
							}
						if (chracter.y <= map[y][x].y&&chracter.x >= map[y][x].x)
							if ((map[y][x].y - chracter.y < 0.0975*rectview.bottom / 2 && chracter.x - map[y][x].x < 0.0661*rectview.right / 2)){
								chracter.check = 5;
								SetTimer(hwnd, 7, 500, NULL);
								KillTimer(hwnd, 6);
							}
						if (chracter.y >= map[y][x].y&&chracter.x <= map[y][x].x)
							if ((chracter.y - map[y][x].y < 0.0975*rectview.bottom / 2 && map[y][x].x - chracter.x < 0.0661*rectview.right / 2)){
								chracter.check = 5;
								SetTimer(hwnd, 7, 500, NULL);
								KillTimer(hwnd, 6);
							}
						if (chracter.y <= map[y][x].y&&chracter.x <= map[y][x].x)
							if ((map[y][x].y - chracter.y < 0.0975*rectview.bottom / 2 && map[y][x].x - chracter.x < 0.0661*rectview.right / 2)){
								chracter.check = 5;
								SetTimer(hwnd, 7, 500, NULL);
								KillTimer(hwnd, 6);
							}

					}
				}
			}

			for (int k = 0; k < 3; k++){
				if (monster[k].check != 5){
						if (monster[k].y >= chracter.y&&monster[k].x >= chracter.x)
						if ((monster[k].y - chracter.y < 0.0975*rectview.bottom/2&&monster[k].x - chracter.x < 0.0661*rectview.right/2)){
							chracter.check = 5;
							SetTimer(hwnd, 7, 500, NULL);
							KillTimer(hwnd, 6);
						}
					if (monster[k].y <= chracter.y&&monster[k].x >= chracter.x)
						if ((chracter.y - monster[k].y < 0.0975*rectview.bottom/2&&monster[k].x - chracter.x < 0.0661*rectview.right/2)){
							chracter.check = 5;
							SetTimer(hwnd, 7, 500, NULL);
							KillTimer(hwnd, 6);
						}
					if (monster[k].y >= chracter.y&&monster[k].x <= chracter.x)
						if ((monster[k].y - chracter.y < 0.0975*rectview.bottom/2&&chracter.x - monster[k].x < 0.0661*rectview.right/2)){
							chracter.check = 5;
							SetTimer(hwnd, 7, 500, NULL);
							KillTimer(hwnd, 6);
						}
					if (monster[k].y <= chracter.y&&monster[k].x <= chracter.x)
						if ((chracter.y - monster[k].y < 0.0975*rectview.bottom/2&&chracter.x - monster[k].x < 0.0661*rectview.right/2)){
							SetTimer(hwnd, 7, 500, NULL);
							KillTimer(hwnd, 6);
							chracter.check = 5;
						}
					
					}
				for (int y = 0; y < 9; y++){
					for (int x = 0; x < 13; x++){
						if (map[y][x].check >= 11 && map[y][x].check <= 15){
							if (monster[k].y >= map[y][x].y&&monster[k].x >= map[y][x].x)
								if ((monster[k].y - map[y][x].y < 0.0975*rectview.bottom/2&&monster[k].x - map[y][x].x < 0.0661*rectview.right/2))
									monster[k].check = 5;
							if (monster[k].y <= map[y][x].y&&monster[k].x >= map[y][x].x)
								if ((map[y][x].y - monster[k].y < 0.0975*rectview.bottom / 2 && monster[k].x - map[y][x].x < 0.0661*rectview.right / 2))
									monster[k].check = 5;
							if (monster[k].y >= map[y][x].y&&monster[k].x <= map[y][x].x)
								if ((monster[k].y - map[y][x].y < 0.0975*rectview.bottom / 2 && map[y][x].x - monster[k].x < 0.0661*rectview.right / 2))
									monster[k].check = 5;
							if (monster[k].y <= map[y][x].y&&monster[k].x <= map[y][x].x)
								if ((map[y][x].y - monster[k].y < 0.0975*rectview.bottom / 2 && map[y][x].x - monster[k].x < 0.0661*rectview.right / 2))
									monster[k].check = 5;

						}
					}
				}

			}
			if (monster[0].check == 5 && monster[1].check == 5 && monster[2].check == 5){
				start = 3;
				animation = 0;
				timen = 0;
				movecheck = 0;
				chracter.xpos = 0;
				for (int i = 0; i < 5; i++)		bombtime[i] = 0;
				chracter.ypos = 0;
				chracter.bomb = 1;
				numberbomb = 1;
				chracter.power = 1;
				chracter.check = 0;
				chracter.x = map2[chracter.ypos][chracter.xpos].x;
				chracter.y = map2[chracter.ypos][chracter.xpos].y;
				map[0][0].check = 10;
				monster[0].xpos = 12;
				monster[0].ypos = 0;
				monster[0].check = 0;
				monster[1].xpos = 0;
				monster[1].ypos = 10;
				monster[1].check = 0;
				monster[2].xpos = 12;
				monster[2].ypos = 10;
				monster[2].check = 0;
				monster[3].xpos = 6;
				ghosttime = 0;
				monster[3].ypos = 0;
				monster[3].check = 0;
				monster[4].xpos = 6;
				monster[4].ypos = 10;
				monster[4].check = 0;
				map2[0][0].check = 10;
				useOFF[0] = 1;
				useOFF[1] = 1;
				speed = 100;
				bossmove = 0;
				time = 0;
				SetTimer(hwnd, 10, 1000, NULL);
				for (int a = 0; a < 5; a++){
				//	map2[monster[a].ypos][monster[a].xpos].check = 31;
					monster[a].x = map2[monster[a].ypos][monster[a].xpos].x;
					monster[a].y = map2[monster[a].ypos][monster[a].xpos].y;
					monstermove[a] = 0;
				}

			}


			/*for (int y = 0; y < 9; y++){
				for (int x = 0; x < 13; x++){
				if (map[y][x].check==23)
				Rectangle(mem1DC, map[y][x].x, map[y][x].y, map[y][x].x + 0.0661*rectview.right, map[y][x].y + 0.0975*rectview.bottom);//게임헬프
				}
				}*/
			for (int y = 0; y < 9; y++){
				for (int x = 0; x < 13; x++){
					if (map[y][x].check > 1 && map[y][x].check < 6){
						oldBit2 = (HBITMAP)SelectObject(mem2DC, bbox); // mem2dc에는 hBit2
						StretchBlt(mem1DC, map[y][x].x, map[y][x].y, 0.0661*rectview.right, 0.0975*rectview.bottom, mem2DC, 0, 0, 30, 30, SRCCOPY);
					}
					if (map[y][x].check == 6){
						oldBit2 = (HBITMAP)SelectObject(mem2DC, bomb); // mem2dc에는 hBit2
						TransparentBlt(mem1DC, map[y][x].x, map[y][x].y, 0.0661*rectview.right, 0.0975*rectview.bottom, mem2DC, 0, 0, 34, 32, RGB(255, 0, 255));//폭탄
					}
					if (map[y][x].check == 7){
						oldBit2 = (HBITMAP)SelectObject(mem2DC, bomb); // mem2dc에는 hBit2
						TransparentBlt(mem1DC, map[y][x].x, map[y][x].y, 0.0661*rectview.right, 0.0975*rectview.bottom, mem2DC, 32, 0, 34, 32, RGB(255, 0, 255));//폭탄
					}
					if (map[y][x].check == 8){
						oldBit2 = (HBITMAP)SelectObject(mem2DC, bomb); // mem2dc에는 hBit2
						TransparentBlt(mem1DC, map[y][x].x, map[y][x].y, 0.0661*rectview.right, 0.0975*rectview.bottom, mem2DC, 64, 0, 33, 32, RGB(255, 0, 255));//폭탄
					}
					if (map[y][x].check == 9){
						oldBit2 = (HBITMAP)SelectObject(mem2DC, bomb); // mem2dc에는 hBit2
						TransparentBlt(mem1DC, map[y][x].x, map[y][x].y, 0.0661*rectview.right, 0.0975*rectview.bottom, mem2DC, 32, 0, 34, 32, RGB(255, 0, 255));//폭탄
					}
					if (map[y][x].check == 11){
						oldBit2 = (HBITMAP)SelectObject(mem2DC, after); // mem2dc에는 hBit2
						TransparentBlt(mem1DC, map[y][x].x, map[y][x].y, 0.0661*rectview.right, 0.0975*rectview.bottom, mem2DC, 64, 32, 34, 32, RGB(255, 0, 255));//폭탄
					}
					if (map[y][x].check == 12){
						oldBit2 = (HBITMAP)SelectObject(mem2DC, after); // mem2dc에는 hBit2
						TransparentBlt(mem1DC, map[y][x].x, map[y][x].y, 0.0661*rectview.right, 0.0975*rectview.bottom, mem2DC, 64, 64, 34, 32, RGB(255, 0, 255));//폭탄
					}
					if (map[y][x].check == 13){
						oldBit2 = (HBITMAP)SelectObject(mem2DC, after); // mem2dc에는 hBit2
						TransparentBlt(mem1DC, map[y][x].x, map[y][x].y, 0.0661*rectview.right, 0.0975*rectview.bottom, mem2DC, 64, 96, 34, 32, RGB(255, 0, 255));//폭탄
					}
					if (map[y][x].check == 14){
						oldBit2 = (HBITMAP)SelectObject(mem2DC, after); // mem2dc에는 hBit2
						TransparentBlt(mem1DC, map[y][x].x, map[y][x].y, 0.0661*rectview.right, 0.0975*rectview.bottom, mem2DC, 64, 129, 34, 32, RGB(255, 0, 255));//폭탄
					}
					if (map[y][x].check == 15){
						oldBit2 = (HBITMAP)SelectObject(mem2DC, after); // mem2dc에는 hBit2
						TransparentBlt(mem1DC, map[y][x].x, map[y][x].y, 0.0661*rectview.right, 0.0975*rectview.bottom, mem2DC, 62, 160, 34, 32, RGB(255, 0, 255));//폭탄
					}
					if (map[y][x].check == 23){
						oldBit2 = (HBITMAP)SelectObject(mem2DC, item[0]); // mem2dc에는 hBit2
						TransparentBlt(mem1DC, map[y][x].x, map[y][x].y, 0.0661*rectview.right, 0.0975*rectview.bottom, mem2DC, 0, 0, 53, 52, RGB(255, 255, 255));//이속
					}
					if (map[y][x].check == 24){
						oldBit2 = (HBITMAP)SelectObject(mem2DC, item[2]); // mem2dc에는 hBit2
						TransparentBlt(mem1DC, map[y][x].x, map[y][x].y, 0.0661*rectview.right, 0.0975*rectview.bottom, mem2DC, 0, 0, 41, 37, RGB(0, 0, 0));//화력
					}
					if (map[y][x].check == 25){
						oldBit2 = (HBITMAP)SelectObject(mem2DC, item[1]); // mem2dc에는 hBit2
						TransparentBlt(mem1DC, map[y][x].x, map[y][x].y, 0.0661*rectview.right, 0.0975*rectview.bottom, mem2DC, 0, 0, 355, 215, RGB(0, 0, 0));//폭탄
					}

				}
			}

			if (chracter.check == 5 && animation % 5 == 0){
				oldBit2 = (HBITMAP)SelectObject(mem2DC, dead); // mem2dc에는 hBit2
				TransparentBlt(mem1DC, chracter.x, chracter.y - 0.0975*rectview.bottom / 6, 0.0661*rectview.right, 0.0975*rectview.bottom + 0.0975*rectview.bottom / 6, mem2DC, 0, 84, 48, 84, RGB(255, 0, 255));//죽음
			}
			if (chracter.check == 5 && animation % 5 == 1){
				oldBit2 = (HBITMAP)SelectObject(mem2DC, dead); // mem2dc에는 hBit2
				TransparentBlt(mem1DC, chracter.x, chracter.y - 0.0975*rectview.bottom / 3, 0.0661*rectview.right, 0.0975*rectview.bottom + 0.0975*rectview.bottom / 3, mem2DC, 48, 70, 48, 98, RGB(255, 0, 255));//죽음
			}
			if (chracter.check == 5 && animation % 5 == 2){
				oldBit2 = (HBITMAP)SelectObject(mem2DC, dead); // mem2dc에는 hBit2
				TransparentBlt(mem1DC, chracter.x, chracter.y - 0.0975*rectview.bottom / 6, 0.0661*rectview.right, 0.0975*rectview.bottom + 0.0975*rectview.bottom / 6, mem2DC, 0, 84, 48, 84, RGB(255, 0, 255));//죽음
			}
			if (chracter.check == 5 && animation % 5 == 3){
				oldBit2 = (HBITMAP)SelectObject(mem2DC, dead); // mem2dc에는 hBit2
				TransparentBlt(mem1DC, chracter.x, chracter.y, 0.0661*rectview.right, 0.0975*rectview.bottom, mem2DC, 144, 122, 48, 46, RGB(255, 0, 255));//죽음
			}
			if (chracter.check == 5 && animation % 5 == 4){
				oldBit2 = (HBITMAP)SelectObject(mem2DC, dead); // mem2dc에는 hBit2
				TransparentBlt(mem1DC, chracter.x, chracter.y, 0.0661*rectview.right, 0.0975*rectview.bottom, mem2DC, 192, 122, 48, 46, RGB(255, 0, 255));//죽음
			}
			if (chracter.check == 0 && animation % 4 == 0){
				oldBit2 = (HBITMAP)SelectObject(mem2DC, hero); // mem2dc에는 hBit2
				TransparentBlt(mem1DC, chracter.x, chracter.y, 0.0661*rectview.right, 0.0975*rectview.bottom, mem2DC, 33, 0, 32, 48, RGB(255, 0, 255));//앞모습
			}
			if (chracter.check == 0 && animation % 4 == 1){
				oldBit2 = (HBITMAP)SelectObject(mem2DC, hero); // mem2dc에는 hBit2
				TransparentBlt(mem1DC, chracter.x, chracter.y, 0.0661*rectview.right, 0.0975*rectview.bottom, mem2DC, 0, 0, 32, 48, RGB(255, 0, 255));//앞모습
			}
			if (chracter.check == 0 && animation % 4 == 2){
				oldBit2 = (HBITMAP)SelectObject(mem2DC, hero); // mem2dc에는 hBit2
				TransparentBlt(mem1DC, chracter.x, chracter.y, 0.0661*rectview.right, 0.0975*rectview.bottom, mem2DC, 0, 0, 32, 48, RGB(255, 0, 255));//앞모습
			}
			if (chracter.check == 0 && animation % 4 == 3){
				oldBit2 = (HBITMAP)SelectObject(mem2DC, hero); // mem2dc에는 hBit2
				TransparentBlt(mem1DC, chracter.x, chracter.y, 0.0661*rectview.right, 0.0975*rectview.bottom, mem2DC, 66, 0, 32, 48, RGB(255, 0, 255));// 앞모습
			}


			if (chracter.check == 1 && animation % 4 == 0){
				oldBit2 = (HBITMAP)SelectObject(mem2DC, hero); // mem2dc에는 hBit2
				TransparentBlt(mem1DC, chracter.x, chracter.y, 0.0661*rectview.right, 0.0975*rectview.bottom, mem2DC, 33, 48, 32, 48, RGB(255, 0, 255));//뒷습
			}
			if (chracter.check == 1 && animation % 4 == 1){
				oldBit2 = (HBITMAP)SelectObject(mem2DC, hero); // mem2dc에는 hBit2
				TransparentBlt(mem1DC, chracter.x, chracter.y, 0.0661*rectview.right, 0.0975*rectview.bottom, mem2DC, 0, 48, 32, 48, RGB(255, 0, 255));//뒷모습
			}
			if (chracter.check == 1 && animation % 4 == 2){
				oldBit2 = (HBITMAP)SelectObject(mem2DC, hero); // mem2dc에는 hBit2
				TransparentBlt(mem1DC, chracter.x, chracter.y, 0.0661*rectview.right, 0.0975*rectview.bottom, mem2DC, 0, 48, 32, 48, RGB(255, 0, 255));//뒷습
			}
			if (chracter.check == 1 && animation % 4 == 3){
				oldBit2 = (HBITMAP)SelectObject(mem2DC, hero); // mem2dc에는 hBit2
				TransparentBlt(mem1DC, chracter.x, chracter.y, 0.0661*rectview.right, 0.0975*rectview.bottom, mem2DC, 66, 48, 32, 48, RGB(255, 0, 255));//뒷습
			}


			if (chracter.check == 2 && animation % 4 == 0){
				oldBit2 = (HBITMAP)SelectObject(mem2DC, hero); // mem2dc에는 hBit2
				TransparentBlt(mem1DC, chracter.x, chracter.y, 0.0661*rectview.right, 0.0975*rectview.bottom, mem2DC, 33, 144, 32, 48, RGB(255, 0, 255));//옆모습
			}
			if (chracter.check == 2 && animation % 4 == 1){
				oldBit2 = (HBITMAP)SelectObject(mem2DC, hero); // mem2dc에는 hBit2
				TransparentBlt(mem1DC, chracter.x, chracter.y, 0.0661*rectview.right, 0.0975*rectview.bottom, mem2DC, 0, 144, 32, 48, RGB(255, 0, 255));//게옆모습
			}
			if (chracter.check == 2 && animation % 4 == 2){
				oldBit2 = (HBITMAP)SelectObject(mem2DC, hero); // mem2dc에는 hBit2
				TransparentBlt(mem1DC, chracter.x, chracter.y, 0.0661*rectview.right, 0.0975*rectview.bottom, mem2DC, 0, 144, 32, 48, RGB(255, 0, 255));//옆모습
			}
			if (chracter.check == 2 && animation % 4 == 3){
				oldBit2 = (HBITMAP)SelectObject(mem2DC, hero); // mem2dc에는 hBit2
				TransparentBlt(mem1DC, chracter.x, chracter.y, 0.0661*rectview.right, 0.0975*rectview.bottom, mem2DC, 66, 144, 32, 48, RGB(255, 0, 255));//옆모습
			}
			if (chracter.check == 3 && animation % 4 == 0){
				oldBit2 = (HBITMAP)SelectObject(mem2DC, hero); // mem2dc에는 hBit2
				TransparentBlt(mem1DC, chracter.x, chracter.y, 0.0661*rectview.right, 0.0975*rectview.bottom, mem2DC, 33, 96, 30, 48, RGB(255, 0, 255));//옆습
			}
			if (chracter.check == 3 && animation % 4 == 1){
				oldBit2 = (HBITMAP)SelectObject(mem2DC, hero); // mem2dc에는 hBit2
				TransparentBlt(mem1DC, chracter.x, chracter.y, 0.0661*rectview.right, 0.0975*rectview.bottom, mem2DC, 0, 96, 30, 48, RGB(255, 0, 255));//옆습
			}
			if (chracter.check == 3 && animation % 4 == 2){
				oldBit2 = (HBITMAP)SelectObject(mem2DC, hero); // mem2dc에는 hBit2
				TransparentBlt(mem1DC, chracter.x, chracter.y, 0.0661*rectview.right, 0.0975*rectview.bottom, mem2DC, 0, 96, 30, 48, RGB(255, 0, 255));//옆습
			}
			if (chracter.check == 3 && animation % 4 == 3){
				oldBit2 = (HBITMAP)SelectObject(mem2DC, hero); // mem2dc에는 hBit2
				TransparentBlt(mem1DC, chracter.x, chracter.y, 0.0661*rectview.right, 0.0975*rectview.bottom, mem2DC, 66, 96, 30, 48, RGB(255, 0, 255));//옆습
			}

			for (int t = 0; t < 3; t++){


				if (monster[t].check == 0 && monstermove[t] % 4 == 0){
					oldBit2 = (HBITMAP)SelectObject(mem2DC, monsterpicture); // 몬스터 움직이는 거
					TransparentBlt(mem1DC, monster[t].x, monster[t].y, 0.0661*rectview.right, 0.0975*rectview.bottom, mem2DC, 0, 0, 33, 48, RGB(255, 0, 255));//앞모습
				}
				if (monster[t].check == 0 && monstermove[t] % 4 == 1){
					oldBit2 = (HBITMAP)SelectObject(mem2DC, monsterpicture); // 
					TransparentBlt(mem1DC, monster[t].x, monster[t].y, 0.0661*rectview.right, 0.0975*rectview.bottom, mem2DC, 33, 0, 33, 48, RGB(255, 0, 255));//앞모습
				}
				if (monster[t].check == 0 && monstermove[t] % 4 == 2){
					oldBit2 = (HBITMAP)SelectObject(mem2DC, monsterpicture);
					TransparentBlt(mem1DC, monster[t].x, monster[t].y, 0.0661*rectview.right, 0.0975*rectview.bottom, mem2DC, 66, 0, 33, 48, RGB(255, 0, 255));//앞모습
				}
				if (monster[t].check == 0 && monstermove[t] == 3){
					oldBit2 = (HBITMAP)SelectObject(mem2DC, monsterpicture);
					TransparentBlt(mem1DC, monster[t].x, monster[t].y, 0.0661*rectview.right, 0.0975*rectview.bottom, mem2DC, 95, 0, 33, 48, RGB(255, 0, 255));// 앞모습
				}

				if (monster[t].check == 1 && monstermove[t] % 4 == 0){
					oldBit2 = (HBITMAP)SelectObject(mem2DC, monsterpicture); // 몬스터 움직이는 거
					TransparentBlt(mem1DC, monster[t].x, monster[t].y, 0.0661*rectview.right, 0.0975*rectview.bottom, mem2DC, 0, 48, 33, 48, RGB(255, 0, 255));//앞모습
				}
				if (monster[t].check == 1 && monstermove[t] % 4 == 1){
					oldBit2 = (HBITMAP)SelectObject(mem2DC, monsterpicture); // 
					TransparentBlt(mem1DC, monster[t].x, monster[t].y, 0.0661*rectview.right, 0.0975*rectview.bottom, mem2DC, 33, 48, 33, 48, RGB(255, 0, 255));//앞모습
				}
				if (monster[t].check == 1 && monstermove[t] % 4 == 2){
					oldBit2 = (HBITMAP)SelectObject(mem2DC, monsterpicture);
					TransparentBlt(mem1DC, monster[t].x, monster[t].y, 0.0661*rectview.right, 0.0975*rectview.bottom, mem2DC, 66, 48, 33, 48, RGB(255, 0, 255));//앞모습
				}
				if (monster[t].check == 1 && monstermove[t] == 3){
					oldBit2 = (HBITMAP)SelectObject(mem2DC, monsterpicture);
					TransparentBlt(mem1DC, monster[t].x, monster[t].y, 0.0661*rectview.right, 0.0975*rectview.bottom, mem2DC, 95, 48, 33, 48, RGB(255, 0, 255));// 앞모습
				}

				if (monster[t].check == 2 && monstermove[t] % 4 == 0){
					oldBit2 = (HBITMAP)SelectObject(mem2DC, monsterpicture); // 몬스터 움직이는 거
					TransparentBlt(mem1DC, monster[t].x, monster[t].y, 0.0661*rectview.right, 0.0975*rectview.bottom, mem2DC, 0, 95, 33, 48, RGB(255, 0, 255));//앞모습
				}
				if (monster[t].check == 2 && monstermove[t] % 4 == 1){
					oldBit2 = (HBITMAP)SelectObject(mem2DC, monsterpicture); // 
					TransparentBlt(mem1DC, monster[t].x, monster[t].y, 0.0661*rectview.right, 0.0975*rectview.bottom, mem2DC, 33, 95, 33, 48, RGB(255, 0, 255));//앞모습
				}
				if (monster[t].check == 2 && monstermove[t] % 4 == 2){
					oldBit2 = (HBITMAP)SelectObject(mem2DC, monsterpicture);
					TransparentBlt(mem1DC, monster[t].x, monster[t].y, 0.0661*rectview.right, 0.0975*rectview.bottom, mem2DC, 66, 95, 31, 48, RGB(255, 0, 255));//앞모습
				}
				if (monster[t].check == 2 && monstermove[t] == 3){
					oldBit2 = (HBITMAP)SelectObject(mem2DC, monsterpicture);
					TransparentBlt(mem1DC, monster[t].x, monster[t].y, 0.0661*rectview.right, 0.0975*rectview.bottom, mem2DC, 95, 95, 31, 48, RGB(255, 0, 255));// 앞모습
				}

				if (monster[t].check == 3 && monstermove[t] % 4 == 0){
					oldBit2 = (HBITMAP)SelectObject(mem2DC, monsterpicture); // 몬스터 움직이는 거
					TransparentBlt(mem1DC, monster[t].x, monster[t].y, 0.0661*rectview.right, 0.0975*rectview.bottom, mem2DC, 0, 143, 33, 48, RGB(255, 0, 255));//앞모습
				}
				if (monster[t].check == 3 && monstermove[t] % 4 == 1){
					oldBit2 = (HBITMAP)SelectObject(mem2DC, monsterpicture); // 
					TransparentBlt(mem1DC, monster[t].x, monster[t].y, 0.0661*rectview.right, 0.0975*rectview.bottom, mem2DC, 33, 143, 33, 48, RGB(255, 0, 255));//앞모습
				}
				if (monster[t].check == 3 && monstermove[t] % 4 == 2){
					oldBit2 = (HBITMAP)SelectObject(mem2DC, monsterpicture);
					TransparentBlt(mem1DC, monster[t].x, monster[t].y, 0.0661*rectview.right, 0.0975*rectview.bottom, mem2DC, 66, 143, 33, 48, RGB(255, 0, 255));//앞모습
				}
				if (monster[t].check == 3 && monstermove[t] == 3){
					oldBit2 = (HBITMAP)SelectObject(mem2DC, monsterpicture);
					TransparentBlt(mem1DC, monster[t].x, monster[t].y, 0.0661*rectview.right, 0.0975*rectview.bottom, mem2DC, 95, 143, 33, 48, RGB(255, 0, 255));// 앞모습
				}

				


			}





		}
		if (start == 3){
		
		
			if (star[0] == 1)
			for (int y = 0; y < 11; y++){
				for (int x = 0; x < 13; x++){
					if (map2[y][x].check >= 11 && map2[y][x].check <= 15){
						if (chracter.y >= map2[y][x].y&&chracter.x >= map2[y][x].x)
							if ((chracter.y - map2[y][x].y <= 0.0670*rectview.bottom / 2 && chracter.x - map2[y][x].x <= 0.0692*rectview.right / 2)){
								chracter.check = 5;
								SetTimer(hwnd, 7, 500, NULL);
								KillTimer(hwnd, 6);
							}
						if (chracter.y <= map2[y][x].y&&chracter.x >= map2[y][x].x)
							if ((map2[y][x].y - chracter.y <= 0.0670*rectview.bottom / 2 && chracter.x - map2[y][x].x <= 0.0692*rectview.right / 2)){
								chracter.check = 5;
								SetTimer(hwnd, 7, 500, NULL);
								KillTimer(hwnd, 6);
							}
						if (chracter.y >= map2[y][x].y&&chracter.x <= map2[y][x].x)
							if ((chracter.y - map2[y][x].y <= 0.0670*rectview.bottom / 2 && map2[y][x].x - chracter.x <= 0.0692*rectview.right / 2)){
								chracter.check = 5;
								SetTimer(hwnd, 7, 500, NULL);
								KillTimer(hwnd, 6);
							}
						if (chracter.y <= map2[y][x].y&&chracter.x <= map2[y][x].x)
							if ((map2[y][x].y - chracter.y <= 0.0670*rectview.bottom / 2 && map2[y][x].x - chracter.x <= 0.0692*rectview.right / 2)){
								SetTimer(hwnd, 7, 500, NULL);
								KillTimer(hwnd, 6);
								chracter.check = 5;
							}

					}
				}
			}
			if (time==120)
			SetTimer(hwnd, 11, 500, NULL);

			DeleteDC(mem1DC);
			mem1DC = CreateCompatibleDC(hdc);
			if (hbit3 == NULL) // hBit1을 hdc와 호환되게 만들어준다.
				hbit3 = CreateCompatibleBitmap(hdc, 3000, 2000);
			oldBit1 = (HBITMAP)SelectObject(mem1DC, hbit3); // mem1dc에는 hBit1
			oldBit2 = (HBITMAP)SelectObject(mem2DC, stage2); // mem2dc에는 hBit2
			StretchBlt(mem1DC, 0, 0, rectview.right, rectview.bottom, mem2DC, 0, 0, 440, 336, SRCCOPY);//배경
			/*for (int y = 0; y < 11; y++){
				for (int x = 0; x < 13; x++){
					if (map2[y][x].check==1)
				Rectangle(mem1DC, map2[y][x].x, map2[y][x].y, map2[y][x].x + 0.0670*rectview.right, map2[y][x].y + 0.0692*rectview.bottom);//게임헬프
				}
			}*/
		
			for (int k = 0; k < 5; k++){
				if (monster[k].check != 5){
					if (star[0] == 1){
						if (monster[k].y >= chracter.y&&monster[k].x >= chracter.x)
							if ((monster[k].y - chracter.y < 0.0670*rectview.bottom / 2 && monster[k].x - chracter.x < 0.0692*rectview.right / 2)){
								chracter.check = 5;
								SetTimer(hwnd, 7, 500, NULL);
								KillTimer(hwnd, 6);
							}
						if (monster[k].y <= chracter.y&&monster[k].x >= chracter.x)
							if ((chracter.y - monster[k].y < 0.0670*rectview.bottom / 2 && monster[k].x - chracter.x < 0.0692*rectview.right / 2)){
								chracter.check = 5;
								SetTimer(hwnd, 7, 500, NULL);
								KillTimer(hwnd, 6);
							}
						if (monster[k].y >= chracter.y&&monster[k].x <= chracter.x)
							if ((monster[k].y - chracter.y < 0.0670*rectview.bottom / 2 && chracter.x - monster[k].x < 0.0692*rectview.right / 2)){
								chracter.check = 5;
								SetTimer(hwnd, 7, 500, NULL);
								KillTimer(hwnd, 6);
							}
						if (monster[k].y <= chracter.y&&monster[k].x <= chracter.x)
							if ((chracter.y - monster[k].y < 0.0670*rectview.bottom / 2 && chracter.x - monster[k].x < 0.0692*rectview.right / 2)){
								SetTimer(hwnd, 7, 500, NULL);
								KillTimer(hwnd, 6);
								chracter.check = 5;
							}

					}
				}
				for (int y = 0; y < 11; y++){
					for (int x = 0; x < 13; x++){
						if (map2[y][x].check >= 11 && map2[y][x].check <= 15){
							if (monster[k].y >= map2[y][x].y&&monster[k].x >= map2[y][x].x)
								if ((monster[k].y - map2[y][x].y <= 0.0670*rectview.bottom / 2 && monster[k].x - map2[y][x].x <= 0.0692*rectview.right / 2))
									monster[k].check = 5;
							if (monster[k].y <= map2[y][x].y&&monster[k].x >= map2[y][x].x)
								if ((map2[y][x].y - monster[k].y <= 0.0670*rectview.bottom / 2 && monster[k].x - map2[y][x].x <= 0.0692*rectview.right / 2))
									monster[k].check = 5;
							if (monster[k].y >= map2[y][x].y&&monster[k].x <= map2[y][x].x)
								if ((monster[k].y - map2[y][x].y <= 0.0670*rectview.bottom / 2 && map2[y][x].x - monster[k].x <= 0.0692*rectview.right / 2))
									monster[k].check = 5;
							if (monster[k].y <= map2[y][x].y&&monster[k].x <= map2[y][x].x)
								if ((map2[y][x].y - monster[k].y <= 0.0670*rectview.bottom / 2 && map2[y][x].x - monster[k].x <= 0.0692*rectview.right / 2))
									monster[k].check = 5;

						}
					}
				}

			}
			if (monster[0].check == 5 && monster[1].check == 5 && monster[2].check == 5 && monster[3].check == 5 && monster[4].check == 5){
				start = 4;
				animation = 0;
				timen = 0;
				movecheck = 0;

				for (int i = 0; i < 5; i++)		bombtime[i] = 0;
				chracter.xpos = 4;

				time = 0;
				SetTimer(hwnd, 10, 1000, NULL);
				chracter.ypos = 1;
				chracter.bomb = 5;
				numberbomb = 1;
				chracter.power = 1;
				chracter.check = 0;
				speed = 100;
				chracter.x = map3[chracter.ypos][chracter.xpos].x;
				chracter.y = map3[chracter.ypos][chracter.xpos].y;
				map3[chracter.ypos][chracter.xpos].check = 10;
				ghosttime = 0;
				ghost.xpos = rand() % 11;
				ghost.ypos = rand() % 15;
				ghost.x = map3[ghost.ypos][ghost.xpos].x;
				ghost.y = map3[ghost.ypos][ghost.xpos].y;



			}

			for (int y = 0; y < 11; y++){
				for (int x = 0; x < 13; x++){
					if (map2[y][x].check > 1 && map2[y][x].check < 6){
						oldBit2 = (HBITMAP)SelectObject(mem2DC, bbox2); // mem2dc에는 hBit2
						StretchBlt(mem1DC, map2[y][x].x, map2[y][x].y, 0.0670*rectview.right, 0.0692*rectview.bottom, mem2DC, 0, 0, 40, 46, SRCCOPY);
					}
					if (map2[y][x].check == 6){
						oldBit2 = (HBITMAP)SelectObject(mem2DC, bomb); // mem2dc에는 hBit2
						TransparentBlt(mem1DC, map2[y][x].x, map2[y][x].y, 0.0670*rectview.right, 0.0692*rectview.bottom, mem2DC, 0, 0, 34, 32, RGB(255, 0, 255));//폭탄
					}
					if (map2[y][x].check == 7){
						oldBit2 = (HBITMAP)SelectObject(mem2DC, bomb); // mem2dc에는 hBit2
						TransparentBlt(mem1DC, map2[y][x].x, map2[y][x].y, 0.0670*rectview.right, 0.0692*rectview.bottom, mem2DC, 32, 0, 34, 32, RGB(255, 0, 255));//폭탄
					}
					if (map2[y][x].check == 8){
						oldBit2 = (HBITMAP)SelectObject(mem2DC, bomb); // mem2dc에는 hBit2
						TransparentBlt(mem1DC, map2[y][x].x, map2[y][x].y, 0.0670*rectview.right, 0.0692*rectview.bottom, mem2DC, 64, 0, 33, 32, RGB(255, 0, 255));//폭탄
					}
					if (map2[y][x].check == 9){
						oldBit2 = (HBITMAP)SelectObject(mem2DC, bomb); // mem2dc에는 hBit2
						TransparentBlt(mem1DC, map2[y][x].x, map2[y][x].y, 0.0670*rectview.right, 0.0692*rectview.bottom, mem2DC, 32, 0, 34, 32, RGB(255, 0, 255));//폭탄
					}
					if (map2[y][x].check == 11){
						oldBit2 = (HBITMAP)SelectObject(mem2DC, after); // mem2dc에는 hBit2
						TransparentBlt(mem1DC, map2[y][x].x, map2[y][x].y, 0.0670*rectview.right, 0.0692*rectview.bottom, mem2DC, 64, 32, 34, 32, RGB(255, 0, 255));//폭탄
					}
					if (map2[y][x].check == 12){
						oldBit2 = (HBITMAP)SelectObject(mem2DC, after); // mem2dc에는 hBit2
						TransparentBlt(mem1DC, map2[y][x].x, map2[y][x].y, 0.0670*rectview.right, 0.0692*rectview.bottom, mem2DC, 64, 64, 34, 32, RGB(255, 0, 255));//폭탄
					}
					if (map2[y][x].check == 13){
						oldBit2 = (HBITMAP)SelectObject(mem2DC, after); // mem2dc에는 hBit2
						TransparentBlt(mem1DC, map2[y][x].x, map2[y][x].y, 0.0670*rectview.right, 0.0692*rectview.bottom, mem2DC, 64, 96, 34, 32, RGB(255, 0, 255));//폭탄
					}
					if (map2[y][x].check == 14){
						oldBit2 = (HBITMAP)SelectObject(mem2DC, after); // mem2dc에는 hBit2
						TransparentBlt(mem1DC, map2[y][x].x, map2[y][x].y, 0.0670*rectview.right, 0.0692*rectview.bottom, mem2DC, 64, 129, 34, 32, RGB(255, 0, 255));//폭탄
					}
					if (map2[y][x].check == 15){
						oldBit2 = (HBITMAP)SelectObject(mem2DC, after); // mem2dc에는 hBit2
						TransparentBlt(mem1DC, map2[y][x].x, map2[y][x].y, 0.0670*rectview.right, 0.0692*rectview.bottom, mem2DC, 62, 160, 34, 32, RGB(255, 0, 255));//폭탄
					}
					if (map2[y][x].check == 23){
						oldBit2 = (HBITMAP)SelectObject(mem2DC, item[0]); // mem2dc에는 hBit2
						TransparentBlt(mem1DC, map2[y][x].x, map2[y][x].y, 0.0670*rectview.right, 0.0692*rectview.bottom, mem2DC, 0, 0, 53, 52, RGB(255, 255, 255));//이속
					}
					if (map2[y][x].check == 24){
						oldBit2 = (HBITMAP)SelectObject(mem2DC, item[2]); // mem2dc에는 hBit2
						TransparentBlt(mem1DC, map2[y][x].x, map2[y][x].y, 0.0670*rectview.right, 0.0692*rectview.bottom, mem2DC, 0, 0, 41, 37, RGB(0, 0, 0));//화력
					}
					if (map2[y][x].check == 25){
						oldBit2 = (HBITMAP)SelectObject(mem2DC, item[1]); // mem2dc에는 hBit2
						TransparentBlt(mem1DC, map2[y][x].x, map2[y][x].y, 0.0670*rectview.right, 0.0692*rectview.bottom, mem2DC, 0, 0, 355, 215, RGB(0, 0, 0));//폭탄
					}

				}
			}

			if (chracter.check == 5 && animation % 5 == 0){
				oldBit2 = (HBITMAP)SelectObject(mem2DC, dead); // mem2dc에는 hBit2
				TransparentBlt(mem1DC, chracter.x, chracter.y - 0.0692*rectview.bottom / 6, 0.0670*rectview.right, 0.0692*rectview.bottom + 0.0692*rectview.bottom / 6, mem2DC, 0, 84, 48, 84, RGB(255, 0, 255));//죽음
			}
			if (chracter.check == 5 && animation % 5 == 1){
				oldBit2 = (HBITMAP)SelectObject(mem2DC, dead); // mem2dc에는 hBit2
				TransparentBlt(mem1DC, chracter.x, chracter.y - 0.0692*rectview.bottom / 3, 0.0670*rectview.right, 0.0692*rectview.bottom + 0.0692*rectview.bottom / 3, mem2DC, 48, 70, 48, 98, RGB(255, 0, 255));//죽음
			}
			if (chracter.check == 5 && animation % 5 == 2){
				oldBit2 = (HBITMAP)SelectObject(mem2DC, dead); // mem2dc에는 hBit2
				TransparentBlt(mem1DC, chracter.x, chracter.y - 0.0692*rectview.bottom / 6, 0.0670*rectview.right, 0.0692*rectview.bottom + 0.0692*rectview.bottom / 6, mem2DC, 0, 84, 48, 84, RGB(255, 0, 255));//죽음
			}
			if (chracter.check == 5 && animation % 5 == 3){
				oldBit2 = (HBITMAP)SelectObject(mem2DC, dead); // mem2dc에는 hBit2
				TransparentBlt(mem1DC, chracter.x, chracter.y, 0.0670*rectview.right, 0.0692*rectview.bottom, mem2DC, 144, 122, 48, 46, RGB(255, 0, 255));//죽음
			}
			if (chracter.check == 5 && animation % 5 == 4){
				oldBit2 = (HBITMAP)SelectObject(mem2DC, dead); // mem2dc에는 hBit2
				TransparentBlt(mem1DC, chracter.x, chracter.y, 0.0670*rectview.right, 0.0692*rectview.bottom, mem2DC, 192, 122, 48, 46, RGB(255, 0, 255));//죽음
			}




			if (chracter.check == 0 && animation % 4 == 0){
				oldBit2 = (HBITMAP)SelectObject(mem2DC, hero); // mem2dc에는 hBit2
				TransparentBlt(mem1DC, chracter.x, chracter.y, 0.0670*rectview.right, 0.0692*rectview.bottom, mem2DC, 33, 0, 32, 48, RGB(255, 0, 255));//앞모습
			}
			if (chracter.check == 0 && animation % 4 == 1){
				oldBit2 = (HBITMAP)SelectObject(mem2DC, hero); // mem2dc에는 hBit2
				TransparentBlt(mem1DC, chracter.x, chracter.y, 0.0670*rectview.right, 0.0692*rectview.bottom, mem2DC, 0, 0, 32, 48, RGB(255, 0, 255));//앞모습
			}
			if (chracter.check == 0 && animation % 4 == 2){
				oldBit2 = (HBITMAP)SelectObject(mem2DC, hero); // mem2dc에는 hBit2
				TransparentBlt(mem1DC, chracter.x, chracter.y, 0.0670*rectview.right, 0.0692*rectview.bottom, mem2DC, 0, 0, 32, 48, RGB(255, 0, 255));//앞모습
			}
			if (chracter.check == 0 && animation % 4 == 3){
				oldBit2 = (HBITMAP)SelectObject(mem2DC, hero); // mem2dc에는 hBit2
				TransparentBlt(mem1DC, chracter.x, chracter.y, 0.0670*rectview.right, 0.0692*rectview.bottom, mem2DC, 66, 0, 32, 48, RGB(255, 0, 255));// 앞모습
			}


			if (chracter.check == 1 && animation % 4 == 0){
				oldBit2 = (HBITMAP)SelectObject(mem2DC, hero); // mem2dc에는 hBit2
				TransparentBlt(mem1DC, chracter.x, chracter.y, 0.0670*rectview.right, 0.0692*rectview.bottom, mem2DC, 33, 48, 32, 48, RGB(255, 0, 255));//뒷습
			}
			if (chracter.check == 1 && animation % 4 == 1){
				oldBit2 = (HBITMAP)SelectObject(mem2DC, hero); // mem2dc에는 hBit2
				TransparentBlt(mem1DC, chracter.x, chracter.y, 0.0670*rectview.right, 0.0692*rectview.bottom, mem2DC, 0, 48, 32, 48, RGB(255, 0, 255));//뒷모습
			}
			if (chracter.check == 1 && animation % 4 == 2){
				oldBit2 = (HBITMAP)SelectObject(mem2DC, hero); // mem2dc에는 hBit2
				TransparentBlt(mem1DC, chracter.x, chracter.y, 0.0670*rectview.right, 0.0692*rectview.bottom, mem2DC, 0, 48, 32, 48, RGB(255, 0, 255));//뒷습
			}
			if (chracter.check == 1 && animation % 4 == 3){
				oldBit2 = (HBITMAP)SelectObject(mem2DC, hero); // mem2dc에는 hBit2
				TransparentBlt(mem1DC, chracter.x, chracter.y, 0.0670*rectview.right, 0.0692*rectview.bottom, mem2DC, 66, 48, 32, 48, RGB(255, 0, 255));//뒷습
			}


			if (chracter.check == 2 && animation % 4 == 0){
				oldBit2 = (HBITMAP)SelectObject(mem2DC, hero); // mem2dc에는 hBit2
				TransparentBlt(mem1DC, chracter.x, chracter.y, 0.0670*rectview.right, 0.0692*rectview.bottom, mem2DC, 33, 144, 32, 48, RGB(255, 0, 255));//옆모습
			}
			if (chracter.check == 2 && animation % 4 == 1){
				oldBit2 = (HBITMAP)SelectObject(mem2DC, hero); // mem2dc에는 hBit2
				TransparentBlt(mem1DC, chracter.x, chracter.y, 0.0670*rectview.right, 0.0692*rectview.bottom, mem2DC, 0, 144, 32, 48, RGB(255, 0, 255));//게옆모습
			}
			if (chracter.check == 2 && animation % 4 == 2){
				oldBit2 = (HBITMAP)SelectObject(mem2DC, hero); // mem2dc에는 hBit2
				TransparentBlt(mem1DC, chracter.x, chracter.y, 0.0670*rectview.right, 0.0692*rectview.bottom, mem2DC, 0, 144, 32, 48, RGB(255, 0, 255));//옆모습
			}
			if (chracter.check == 2 && animation % 4 == 3){
				oldBit2 = (HBITMAP)SelectObject(mem2DC, hero); // mem2dc에는 hBit2
				TransparentBlt(mem1DC, chracter.x, chracter.y, 0.0670*rectview.right, 0.0692*rectview.bottom, mem2DC, 66, 144, 32, 48, RGB(255, 0, 255));//옆모습
			}
			if (chracter.check == 3 && animation % 4 == 0){
				oldBit2 = (HBITMAP)SelectObject(mem2DC, hero); // mem2dc에는 hBit2
				TransparentBlt(mem1DC, chracter.x, chracter.y, 0.0670*rectview.right, 0.0692*rectview.bottom, mem2DC, 33, 96, 30, 48, RGB(255, 0, 255));//옆습
			}
			if (chracter.check == 3 && animation % 4 == 1){
				oldBit2 = (HBITMAP)SelectObject(mem2DC, hero); // mem2dc에는 hBit2
				TransparentBlt(mem1DC, chracter.x, chracter.y, 0.0670*rectview.right, 0.0692*rectview.bottom, mem2DC, 0, 96, 30, 48, RGB(255, 0, 255));//옆습
			}
			if (chracter.check == 3 && animation % 4 == 2){
				oldBit2 = (HBITMAP)SelectObject(mem2DC, hero); // mem2dc에는 hBit2
				TransparentBlt(mem1DC, chracter.x, chracter.y, 0.0670*rectview.right, 0.0692*rectview.bottom, mem2DC, 0, 96, 30, 48, RGB(255, 0, 255));//옆습
			}
			if (chracter.check == 3 && animation % 4 == 3){
				oldBit2 = (HBITMAP)SelectObject(mem2DC, hero); // mem2dc에는 hBit2
				TransparentBlt(mem1DC, chracter.x, chracter.y, 0.0670*rectview.right, 0.0692*rectview.bottom, mem2DC, 66, 96, 30, 48, RGB(255, 0, 255));//옆습
			}





			///별먹은거
			if (chracter.check == 0 && star[0]==0 && animation % 4 == 0){
				oldBit2 = (HBITMAP)SelectObject(mem2DC, gold); // mem2dc에는 hBit2
				TransparentBlt(mem1DC, chracter.x, chracter.y, 0.0670*rectview.right, 0.0692*rectview.bottom, mem2DC, 33, 0, 32, 48, RGB(255, 0, 255));//앞모습
			}
			if (chracter.check == 0 && star[0]==0 && animation % 4 == 1){
				oldBit2 = (HBITMAP)SelectObject(mem2DC, gold); // mem2dc에는 hBit2
				TransparentBlt(mem1DC, chracter.x, chracter.y, 0.0670*rectview.right, 0.0692*rectview.bottom, mem2DC, 0, 0, 32, 48, RGB(255, 0, 255));//앞모습
			}
			if (chracter.check == 0 && star[0]==0 && animation % 4 == 2){
				oldBit2 = (HBITMAP)SelectObject(mem2DC, gold); // mem2dc에는 hBit2
				TransparentBlt(mem1DC, chracter.x, chracter.y, 0.0670*rectview.right, 0.0692*rectview.bottom, mem2DC, 0, 0, 32, 48, RGB(255, 0, 255));//앞모습
			}
			if (chracter.check == 0 && star[0]==0 && animation % 4 == 3){
				oldBit2 = (HBITMAP)SelectObject(mem2DC, gold); // mem2dc에는 hBit2
				TransparentBlt(mem1DC, chracter.x, chracter.y, 0.0670*rectview.right, 0.0692*rectview.bottom, mem2DC, 66, 0, 32, 48, RGB(255, 0, 255));// 앞모습
			}


			if (chracter.check == 1 && star[0]==0 && animation % 4 == 0){
				oldBit2 = (HBITMAP)SelectObject(mem2DC, gold); // mem2dc에는 hBit2
				TransparentBlt(mem1DC, chracter.x, chracter.y, 0.0670*rectview.right, 0.0692*rectview.bottom, mem2DC, 33, 48, 32, 48, RGB(255, 0, 255));//뒷습
			}
			if (chracter.check == 1 && star[0]==0 && animation % 4 == 1){
				oldBit2 = (HBITMAP)SelectObject(mem2DC, gold); // mem2dc에는 hBit2
				TransparentBlt(mem1DC, chracter.x, chracter.y, 0.0670*rectview.right, 0.0692*rectview.bottom, mem2DC, 0, 48, 32, 48, RGB(255, 0, 255));//뒷모습
			}
			if (chracter.check == 1 && star[0]==0 && animation % 4 == 2){
				oldBit2 = (HBITMAP)SelectObject(mem2DC, gold); // mem2dc에는 hBit2
				TransparentBlt(mem1DC, chracter.x, chracter.y, 0.0670*rectview.right, 0.0692*rectview.bottom, mem2DC, 0, 48, 32, 48, RGB(255, 0, 255));//뒷습
			}
			if (chracter.check == 1 && star[0]==0 && animation % 4 == 3){
				oldBit2 = (HBITMAP)SelectObject(mem2DC, gold); // mem2dc에는 hBit2
				TransparentBlt(mem1DC, chracter.x, chracter.y, 0.0670*rectview.right, 0.0692*rectview.bottom, mem2DC, 66, 48, 32, 48, RGB(255, 0, 255));//뒷습
			}



			if (chracter.check == 2 && star[0]==0 && animation % 4 == 0){
				oldBit2 = (HBITMAP)SelectObject(mem2DC, gold); // mem2dc에는 hBit2
				TransparentBlt(mem1DC, chracter.x, chracter.y, 0.0670*rectview.right, 0.0692*rectview.bottom, mem2DC, 33, 144, 32, 48, RGB(255, 0, 255));//옆모습
			}
			if (chracter.check == 2 && star[0]==0 && animation % 4 == 1){
				oldBit2 = (HBITMAP)SelectObject(mem2DC, gold); // mem2dc에는 hBit2
				TransparentBlt(mem1DC, chracter.x, chracter.y, 0.0670*rectview.right, 0.0692*rectview.bottom, mem2DC, 0, 144, 32, 48, RGB(255, 0, 255));//게옆모습
			}
			if (chracter.check == 2 && star[0]==0 && animation % 4 == 2){
				oldBit2 = (HBITMAP)SelectObject(mem2DC, gold); // mem2dc에는 hBit2
				TransparentBlt(mem1DC, chracter.x, chracter.y, 0.0670*rectview.right, 0.0692*rectview.bottom, mem2DC, 0, 144, 32, 48, RGB(255, 0, 255));//옆모습
			}
			if (chracter.check == 2 && star[0]==0 && animation % 4 == 3){
				oldBit2 = (HBITMAP)SelectObject(mem2DC, gold); // mem2dc에는 hBit2
				TransparentBlt(mem1DC, chracter.x, chracter.y, 0.0670*rectview.right, 0.0692*rectview.bottom, mem2DC, 66, 144, 32, 48, RGB(255, 0, 255));//옆모습
			}
			if (chracter.check == 3 && star[0]==0 && animation % 4 == 0){
				oldBit2 = (HBITMAP)SelectObject(mem2DC, gold); // mem2dc에는 hBit2
				TransparentBlt(mem1DC, chracter.x, chracter.y, 0.0670*rectview.right, 0.0692*rectview.bottom, mem2DC, 33, 96, 30, 48, RGB(255, 0, 255));//옆습
			}
			if (chracter.check == 3 && star[0]==0 && animation % 4 == 1){
				oldBit2 = (HBITMAP)SelectObject(mem2DC, gold); // mem2dc에는 hBit2
				TransparentBlt(mem1DC, chracter.x, chracter.y, 0.0670*rectview.right, 0.0692*rectview.bottom, mem2DC, 0, 96, 30, 48, RGB(255, 0, 255));//옆습
			}
			if (chracter.check == 3 && star[0]==0 && animation % 4 == 2){
				oldBit2 = (HBITMAP)SelectObject(mem2DC, gold); // mem2dc에는 hBit2
				TransparentBlt(mem1DC, chracter.x, chracter.y, 0.0670*rectview.right, 0.0692*rectview.bottom, mem2DC, 0, 96, 30, 48, RGB(255, 0, 255));//옆습
			}
			if (chracter.check == 3 && star[0]==0 && animation % 4 == 3){
				oldBit2 = (HBITMAP)SelectObject(mem2DC, gold); // mem2dc에는 hBit2
				TransparentBlt(mem1DC, chracter.x, chracter.y, 0.0670*rectview.right, 0.0692*rectview.bottom, mem2DC, 66, 96, 30, 48, RGB(255, 0, 255));//옆습
			}
			if (useOFF[0] == 1){
				oldBit2 = (HBITMAP)SelectObject(mem2DC, useitem[0]); // mem2dc에는 hBit2
				TransparentBlt(mem1DC, rectview.right * 11.8/ 16, rectview.bottom*9.9/11, 0.0670*rectview.right, 0.0692*rectview.bottom, mem2DC, 0, 0, 172, 180, RGB(255, 255, 255));//옆습
			}



			for (int t = 0; t < 5; t++){


				if (monster[t].check == 0 && monstermove[t] % 4 == 0){
					oldBit2 = (HBITMAP)SelectObject(mem2DC, monsterpicture); // 몬스터 움직이는 거
					TransparentBlt(mem1DC, monster[t].x, monster[t].y, 0.0670*rectview.right, 0.0692*rectview.bottom, mem2DC, 0, 0, 33, 48, RGB(255, 0, 255));//앞모습
				}
				if (monster[t].check == 0 && monstermove[t] % 4 == 1){
					oldBit2 = (HBITMAP)SelectObject(mem2DC, monsterpicture); // 
					TransparentBlt(mem1DC, monster[t].x, monster[t].y, 0.0670*rectview.right, 0.0692*rectview.bottom, mem2DC, 33, 0, 33, 48, RGB(255, 0, 255));//앞모습
				}
				if (monster[t].check == 0 && monstermove[t] % 4 == 2){
					oldBit2 = (HBITMAP)SelectObject(mem2DC, monsterpicture);
					TransparentBlt(mem1DC, monster[t].x, monster[t].y, 0.0670*rectview.right, 0.0692*rectview.bottom, mem2DC, 66, 0, 33, 48, RGB(255, 0, 255));//앞모습
				}
				if (monster[t].check == 0 && monstermove[t] == 3){
					oldBit2 = (HBITMAP)SelectObject(mem2DC, monsterpicture);
					TransparentBlt(mem1DC, monster[t].x, monster[t].y, 0.0670*rectview.right, 0.0692*rectview.bottom, mem2DC, 95, 0, 33, 48, RGB(255, 0, 255));// 앞모습
				}

				if (monster[t].check == 1 && monstermove[t] % 4 == 0){
					oldBit2 = (HBITMAP)SelectObject(mem2DC, monsterpicture); // 몬스터 움직이는 거
					TransparentBlt(mem1DC, monster[t].x, monster[t].y, 0.0670*rectview.right, 0.0692*rectview.bottom, mem2DC, 0, 48, 33, 48, RGB(255, 0, 255));//앞모습
				}
				if (monster[t].check == 1 && monstermove[t] % 4 == 1){
					oldBit2 = (HBITMAP)SelectObject(mem2DC, monsterpicture); // 
					TransparentBlt(mem1DC, monster[t].x, monster[t].y, 0.0670*rectview.right, 0.0692*rectview.bottom, mem2DC, 33, 48, 33, 48, RGB(255, 0, 255));//앞모습
				}
				if (monster[t].check == 1 && monstermove[t] % 4 == 2){
					oldBit2 = (HBITMAP)SelectObject(mem2DC, monsterpicture);
					TransparentBlt(mem1DC, monster[t].x, monster[t].y, 0.0670*rectview.right, 0.0692*rectview.bottom, mem2DC, 66, 48, 33, 48, RGB(255, 0, 255));//앞모습
				}
				if (monster[t].check == 1 && monstermove[t] == 3){
					oldBit2 = (HBITMAP)SelectObject(mem2DC, monsterpicture);
					TransparentBlt(mem1DC, monster[t].x, monster[t].y, 0.0670*rectview.right, 0.0692*rectview.bottom, mem2DC, 95, 48, 33, 48, RGB(255, 0, 255));// 앞모습
				}

				if (monster[t].check == 2 && monstermove[t] % 4 == 0){
					oldBit2 = (HBITMAP)SelectObject(mem2DC, monsterpicture); // 몬스터 움직이는 거
					TransparentBlt(mem1DC, monster[t].x, monster[t].y, 0.0670*rectview.right, 0.0692*rectview.bottom, mem2DC, 0, 95, 33, 48, RGB(255, 0, 255));//앞모습
				}
				if (monster[t].check == 2 && monstermove[t] % 4 == 1){
					oldBit2 = (HBITMAP)SelectObject(mem2DC, monsterpicture); // 
					TransparentBlt(mem1DC, monster[t].x, monster[t].y, 0.0670*rectview.right, 0.0692*rectview.bottom, mem2DC, 33, 95, 33, 48, RGB(255, 0, 255));//앞모습
				}
				if (monster[t].check == 2 && monstermove[t] % 4 == 2){
					oldBit2 = (HBITMAP)SelectObject(mem2DC, monsterpicture);
					TransparentBlt(mem1DC, monster[t].x, monster[t].y, 0.0670*rectview.right, 0.0692*rectview.bottom, mem2DC, 66, 95, 31, 48, RGB(255, 0, 255));//앞모습
				}
				if (monster[t].check == 2 && monstermove[t] == 3){
					oldBit2 = (HBITMAP)SelectObject(mem2DC, monsterpicture);
					TransparentBlt(mem1DC, monster[t].x, monster[t].y, 0.0670*rectview.right, 0.0692*rectview.bottom, mem2DC, 95, 95, 31, 48, RGB(255, 0, 255));// 앞모습
				}

				if (monster[t].check == 3 && monstermove[t] % 4 == 0){
					oldBit2 = (HBITMAP)SelectObject(mem2DC, monsterpicture); // 몬스터 움직이는 거
					TransparentBlt(mem1DC, monster[t].x, monster[t].y, 0.0670*rectview.right, 0.0692*rectview.bottom, mem2DC, 0, 143, 33, 48, RGB(255, 0, 255));//앞모습
				}
				if (monster[t].check == 3 && monstermove[t] % 4 == 1){
					oldBit2 = (HBITMAP)SelectObject(mem2DC, monsterpicture); // 
					TransparentBlt(mem1DC, monster[t].x, monster[t].y, 0.0670*rectview.right, 0.0692*rectview.bottom, mem2DC, 33, 143, 33, 48, RGB(255, 0, 255));//앞모습
				}
				if (monster[t].check == 3 && monstermove[t] % 4 == 2){
					oldBit2 = (HBITMAP)SelectObject(mem2DC, monsterpicture);
					TransparentBlt(mem1DC, monster[t].x, monster[t].y, 0.0670*rectview.right, 0.0692*rectview.bottom, mem2DC, 66, 143, 33, 48, RGB(255, 0, 255));//앞모습
				}
				if (monster[t].check == 3 && monstermove[t] == 3){
					oldBit2 = (HBITMAP)SelectObject(mem2DC, monsterpicture);
					TransparentBlt(mem1DC, monster[t].x, monster[t].y, 0.0670*rectview.right, 0.0692*rectview.bottom, mem2DC, 95, 143, 33, 48, RGB(255, 0, 255));// 앞모습
				}


			}


			if (time>120){
				oldBit2 = (HBITMAP)SelectObject(mem2DC, ghostP);
				TransparentBlt(mem1DC, ghost.x, ghost.y, 0.0670*rectview.right, 0.0692*rectview.bottom, mem2DC, 0, 0, 73, 69, RGB(255, 255, 255));// 앞모습
			}



		}


		if (start == 4){

			if (star[1] == 1)
			if (map3[chracter.ypos][chracter.xpos].check != 10 && !(map3[chracter.ypos][chracter.xpos].check >= 6 && map3[chracter.ypos][chracter.xpos].check <= 9)){
				chracter.check = 5;
				SetTimer(hwnd, 7, 500, NULL);
				KillTimer(hwnd, 6);
			}

			if (time==100)
				SetTimer(hwnd, 11, 500, NULL);
			DeleteDC(mem1DC);
			mem1DC = CreateCompatibleDC(hdc);
			if (hbit3 == NULL) // hBit1을 hdc와 호환되게 만들어준다.
				hbit3 = CreateCompatibleBitmap(hdc, 3000, 2000);
			oldBit1 = (HBITMAP)SelectObject(mem1DC, hbit3); // mem1dc에는 hBit1
			oldBit2 = (HBITMAP)SelectObject(mem2DC, stage3); // mem2dc에는 hBit2
			StretchBlt(mem1DC, 0, 0, rectview.right, rectview.bottom, mem2DC, 0, 0, 454, 351, SRCCOPY);//배경
			/*for (int y = 0; y < 10; y++){
				for (int x = 0; x < 15; x++){

				Rectangle(mem1DC, map3[y][x].x, map3[y][x].y, map3[y][x].x + 0.0661*rectview.right, map3[y][x].y + 0.0975*rectview.bottom);//게임헬프
				}
				}*/
			oldBit2 = (HBITMAP)SelectObject(mem2DC, bossbit);//boss 애니메이션
			TransparentBlt(mem1DC, map3[0][3].x, map3[0][3].y, 0.0661*rectview.right * 9, 0.0975*rectview.bottom * 3, mem2DC, 200 * (bossbomb % 3), 0, 200, 150, RGB(255, 255, 255));

			
			for (int y = 0; y < 9; y++){
				for (int x = 0; x < 15; x++){
					if (map3[y][x].check > 1 && map3[y][x].check < 6){
						oldBit2 = (HBITMAP)SelectObject(mem2DC, bbox); // mem2dc에는 hBit2
						StretchBlt(mem1DC, map3[y][x].x, map3[y][x].y, 0.0661*rectview.right, 0.0975*rectview.bottom, mem2DC, 0, 0, 30, 30, SRCCOPY);
					}
					if (map3[y][x].check == 6){
						oldBit2 = (HBITMAP)SelectObject(mem2DC, bomb); // mem2dc에는 hBit2
						TransparentBlt(mem1DC, map3[y][x].x, map3[y][x].y, 0.0661*rectview.right, 0.0975*rectview.bottom, mem2DC, 0, 0, 34, 32, RGB(255, 0, 255));//폭탄
					}
					if (map3[y][x].check == 7){
						oldBit2 = (HBITMAP)SelectObject(mem2DC, bomb); // mem2dc에는 hBit2
						TransparentBlt(mem1DC, map3[y][x].x, map3[y][x].y, 0.0661*rectview.right, 0.0975*rectview.bottom, mem2DC, 32, 0, 34, 32, RGB(255, 0, 255));//폭탄
					}
					if (map3[y][x].check == 8){
						oldBit2 = (HBITMAP)SelectObject(mem2DC, bomb); // mem2dc에는 hBit2
						TransparentBlt(mem1DC, map3[y][x].x, map3[y][x].y, 0.0661*rectview.right, 0.0975*rectview.bottom, mem2DC, 64, 0, 33, 32, RGB(255, 0, 255));//폭탄
					}
					if (map3[y][x].check == 9){
						oldBit2 = (HBITMAP)SelectObject(mem2DC, bomb); // mem2dc에는 hBit2
						TransparentBlt(mem1DC, map3[y][x].x, map3[y][x].y, 0.0661*rectview.right, 0.0975*rectview.bottom, mem2DC, 32, 0, 34, 32, RGB(255, 0, 255));//폭탄
					}
					if (map3[y][x].check == 11){
						oldBit2 = (HBITMAP)SelectObject(mem2DC, after); // mem2dc에는 hBit2
						TransparentBlt(mem1DC, map3[y][x].x, map3[y][x].y, 0.0661*rectview.right, 0.0975*rectview.bottom, mem2DC, 64, 32, 34, 32, RGB(255, 0, 255));//폭탄
					}
					if (map3[y][x].check == 12){
						oldBit2 = (HBITMAP)SelectObject(mem2DC, after); // mem2dc에는 hBit2
						TransparentBlt(mem1DC, map3[y][x].x, map3[y][x].y, 0.0661*rectview.right, 0.0975*rectview.bottom, mem2DC, 64, 64, 34, 32, RGB(255, 0, 255));//폭탄
					}
					if (map3[y][x].check == 13){
						oldBit2 = (HBITMAP)SelectObject(mem2DC, after); // mem2dc에는 hBit2
						TransparentBlt(mem1DC, map3[y][x].x, map3[y][x].y, 0.0661*rectview.right, 0.0975*rectview.bottom, mem2DC, 64, 96, 34, 32, RGB(255, 0, 255));//폭탄
					}
					if (map3[y][x].check == 14){
						oldBit2 = (HBITMAP)SelectObject(mem2DC, after); // mem2dc에는 hBit2
						TransparentBlt(mem1DC, map3[y][x].x, map3[y][x].y, 0.0661*rectview.right, 0.0975*rectview.bottom, mem2DC, 64, 129, 34, 32, RGB(255, 0, 255));//폭탄
					}
					if (map3[y][x].check == 15){
						oldBit2 = (HBITMAP)SelectObject(mem2DC, after); // mem2dc에는 hBit2
						TransparentBlt(mem1DC, map3[y][x].x, map3[y][x].y, 0.0661*rectview.right, 0.0975*rectview.bottom, mem2DC, 62, 160, 34, 32, RGB(255, 0, 255));//폭탄
					}
					if (map3[y][x].check == 23){
						oldBit2 = (HBITMAP)SelectObject(mem2DC, item[0]); // mem2dc에는 hBit2
						TransparentBlt(mem1DC, map3[y][x].x, map3[y][x].y, 0.0661*rectview.right, 0.0975*rectview.bottom, mem2DC, 0, 0, 53, 52, RGB(255, 255, 255));//이속
					}
					if (map3[y][x].check == 24){
						oldBit2 = (HBITMAP)SelectObject(mem2DC, item[2]); // mem2dc에는 hBit2
						TransparentBlt(mem1DC, map3[y][x].x, map3[y][x].y, 0.0661*rectview.right, 0.0975*rectview.bottom, mem2DC, 0, 0, 41, 37, RGB(0, 0, 0));//화력
					}
					if (map3[y][x].check == 25){
						oldBit2 = (HBITMAP)SelectObject(mem2DC, item[1]); // mem2dc에는 hBit2
						TransparentBlt(mem1DC, map3[y][x].x, map3[y][x].y, 0.0661*rectview.right, 0.0975*rectview.bottom, mem2DC, 0, 0, 355, 215, RGB(0, 0, 0));//폭탄
					}

				}
			}

			if (chracter.check == 5 && animation % 5 == 0){
				oldBit2 = (HBITMAP)SelectObject(mem2DC, dead); // mem2dc에는 hBit2
				TransparentBlt(mem1DC, chracter.x, chracter.y - 0.0975*rectview.bottom / 6, 0.0661*rectview.right, 0.0975*rectview.bottom + 0.0975*rectview.bottom / 6, mem2DC, 0, 84, 48, 84, RGB(255, 0, 255));//죽음
			}
			if (chracter.check == 5 && animation % 5 == 1){
				oldBit2 = (HBITMAP)SelectObject(mem2DC, dead); // mem2dc에는 hBit2
				TransparentBlt(mem1DC, chracter.x, chracter.y - 0.0975*rectview.bottom / 3, 0.0661*rectview.right, 0.0975*rectview.bottom + 0.0975*rectview.bottom / 3, mem2DC, 48, 70, 48, 98, RGB(255, 0, 255));//죽음
			}
			if (chracter.check == 5 && animation % 5 == 2){
				oldBit2 = (HBITMAP)SelectObject(mem2DC, dead); // mem2dc에는 hBit2
				TransparentBlt(mem1DC, chracter.x, chracter.y - 0.0975*rectview.bottom / 6, 0.0661*rectview.right, 0.0975*rectview.bottom + 0.0975*rectview.bottom / 6, mem2DC, 0, 84, 48, 84, RGB(255, 0, 255));//죽음
			}
			if (chracter.check == 5 && animation % 5 == 3){
				oldBit2 = (HBITMAP)SelectObject(mem2DC, dead); // mem2dc에는 hBit2
				TransparentBlt(mem1DC, chracter.x, chracter.y, 0.0661*rectview.right, 0.0975*rectview.bottom, mem2DC, 144, 122, 48, 46, RGB(255, 0, 255));//죽음
			}
			if (chracter.check == 5 && animation % 5 == 4){
				oldBit2 = (HBITMAP)SelectObject(mem2DC, dead); // mem2dc에는 hBit2
				TransparentBlt(mem1DC, chracter.x, chracter.y, 0.0661*rectview.right, 0.0975*rectview.bottom, mem2DC, 192, 122, 48, 46, RGB(255, 0, 255));//죽음
			}

			if (chracter.check == 0 && animation % 4 == 0){
				oldBit2 = (HBITMAP)SelectObject(mem2DC, hero); // mem2dc에는 hBit2
				TransparentBlt(mem1DC, chracter.x, chracter.y, 0.0661*rectview.right, 0.0975*rectview.bottom, mem2DC, 33, 0, 32, 48, RGB(255, 0, 255));//앞모습
			}
			if (chracter.check == 0 && animation % 4 == 1){
				oldBit2 = (HBITMAP)SelectObject(mem2DC, hero); // mem2dc에는 hBit2
				TransparentBlt(mem1DC, chracter.x, chracter.y, 0.0661*rectview.right, 0.0975*rectview.bottom, mem2DC, 0, 0, 32, 48, RGB(255, 0, 255));//앞모습
			}
			if (chracter.check == 0 && animation % 4 == 2){
				oldBit2 = (HBITMAP)SelectObject(mem2DC, hero); // mem2dc에는 hBit2
				TransparentBlt(mem1DC, chracter.x, chracter.y, 0.0661*rectview.right, 0.0975*rectview.bottom, mem2DC, 0, 0, 32, 48, RGB(255, 0, 255));//앞모습
			}
			if (chracter.check == 0 && animation % 4 == 3){
				oldBit2 = (HBITMAP)SelectObject(mem2DC, hero); // mem2dc에는 hBit2
				TransparentBlt(mem1DC, chracter.x, chracter.y, 0.0661*rectview.right, 0.0975*rectview.bottom, mem2DC, 66, 0, 32, 48, RGB(255, 0, 255));// 앞모습
			}


			if (chracter.check == 1 && animation % 4 == 0){
				oldBit2 = (HBITMAP)SelectObject(mem2DC, hero); // mem2dc에는 hBit2
				TransparentBlt(mem1DC, chracter.x, chracter.y, 0.0661*rectview.right, 0.0975*rectview.bottom, mem2DC, 33, 48, 32, 48, RGB(255, 0, 255));//게임종료버튼
			}
			if (chracter.check == 1 && animation % 4 == 1){
				oldBit2 = (HBITMAP)SelectObject(mem2DC, hero); // mem2dc에는 hBit2
				TransparentBlt(mem1DC, chracter.x, chracter.y, 0.0661*rectview.right, 0.0975*rectview.bottom, mem2DC, 0, 48, 32, 48, RGB(255, 0, 255));//게임종료버튼
			}
			if (chracter.check == 1 && animation % 4 == 2){
				oldBit2 = (HBITMAP)SelectObject(mem2DC, hero); // mem2dc에는 hBit2
				TransparentBlt(mem1DC, chracter.x, chracter.y, 0.0661*rectview.right, 0.0975*rectview.bottom, mem2DC, 0, 48, 32, 48, RGB(255, 0, 255));//게임종료버튼
			}
			if (chracter.check == 1 && animation % 4 == 3){
				oldBit2 = (HBITMAP)SelectObject(mem2DC, hero); // mem2dc에는 hBit2
				TransparentBlt(mem1DC, chracter.x, chracter.y, 0.0661*rectview.right, 0.0975*rectview.bottom, mem2DC, 66, 48, 32, 48, RGB(255, 0, 255));//게임종료버튼
			}


			if (chracter.check == 2 && animation % 4 == 0){
				oldBit2 = (HBITMAP)SelectObject(mem2DC, hero); // mem2dc에는 hBit2
				TransparentBlt(mem1DC, chracter.x, chracter.y, 0.0661*rectview.right, 0.0975*rectview.bottom, mem2DC, 33, 144, 32, 48, RGB(255, 0, 255));//게임종료버튼
			}
			if (chracter.check == 2 && animation % 4 == 1){
				oldBit2 = (HBITMAP)SelectObject(mem2DC, hero); // mem2dc에는 hBit2
				TransparentBlt(mem1DC, chracter.x, chracter.y, 0.0661*rectview.right, 0.0975*rectview.bottom, mem2DC, 0, 144, 32, 48, RGB(255, 0, 255));//게임종료버튼
			}
			if (chracter.check == 2 && animation % 4 == 2){
				oldBit2 = (HBITMAP)SelectObject(mem2DC, hero); // mem2dc에는 hBit2
				TransparentBlt(mem1DC, chracter.x, chracter.y, 0.0661*rectview.right, 0.0975*rectview.bottom, mem2DC, 0, 144, 32, 48, RGB(255, 0, 255));//게임종료버튼
			}
			if (chracter.check == 2 && animation % 4 == 3){
				oldBit2 = (HBITMAP)SelectObject(mem2DC, hero); // mem2dc에는 hBit2
				TransparentBlt(mem1DC, chracter.x, chracter.y, 0.0661*rectview.right, 0.0975*rectview.bottom, mem2DC, 66, 144, 32, 48, RGB(255, 0, 255));//게임종료버튼
			}
			if (chracter.check == 3 && animation % 4 == 0){
				oldBit2 = (HBITMAP)SelectObject(mem2DC, hero); // mem2dc에는 hBit2
				TransparentBlt(mem1DC, chracter.x, chracter.y, 0.0661*rectview.right, 0.0975*rectview.bottom, mem2DC, 33, 96, 30, 48, RGB(255, 0, 255));//게임종료버튼
			}
			if (chracter.check == 3 && animation % 4 == 1){
				oldBit2 = (HBITMAP)SelectObject(mem2DC, hero); // mem2dc에는 hBit2
				TransparentBlt(mem1DC, chracter.x, chracter.y, 0.0661*rectview.right, 0.0975*rectview.bottom, mem2DC, 0, 96, 30, 48, RGB(255, 0, 255));//게임종료버튼
			}
			if (chracter.check == 3 && animation % 4 == 2){
				oldBit2 = (HBITMAP)SelectObject(mem2DC, hero); // mem2dc에는 hBit2
				TransparentBlt(mem1DC, chracter.x, chracter.y, 0.0661*rectview.right, 0.0975*rectview.bottom, mem2DC, 0, 96, 30, 48, RGB(255, 0, 255));//게임종료버튼
			}
			if (chracter.check == 3 && animation % 4 == 3){
				oldBit2 = (HBITMAP)SelectObject(mem2DC, hero); // mem2dc에는 hBit2
				TransparentBlt(mem1DC, chracter.x, chracter.y, 0.0661*rectview.right, 0.0975*rectview.bottom, mem2DC, 66, 96, 30, 48, RGB(255, 0, 255));//게임종료버튼
			}
			///별먹은거
			if (chracter.check == 0 && star[1] == 0 && animation % 4 == 0){
				oldBit2 = (HBITMAP)SelectObject(mem2DC, gold); // mem2dc에는 hBit2
				TransparentBlt(mem1DC, chracter.x, chracter.y, 0.0661*rectview.right, 0.0975*rectview.bottom, mem2DC, 33, 0, 32, 48, RGB(255, 0, 255));//앞모습
			}
			if (chracter.check == 0 && star[1] == 0 && animation % 4 == 1){
				oldBit2 = (HBITMAP)SelectObject(mem2DC, gold); // mem2dc에는 hBit2
				TransparentBlt(mem1DC, chracter.x, chracter.y, 0.0661*rectview.right, 0.0975*rectview.bottom, mem2DC, 0, 0, 32, 48, RGB(255, 0, 255));//앞모습
			}
			if (chracter.check == 0 && star[1] == 0 && animation % 4 == 2){
				oldBit2 = (HBITMAP)SelectObject(mem2DC, gold); // mem2dc에는 hBit2
				TransparentBlt(mem1DC, chracter.x, chracter.y, 0.0661*rectview.right, 0.0975*rectview.bottom, mem2DC, 0, 0, 32, 48, RGB(255, 0, 255));//앞모습
			}
			if (chracter.check == 0 && star[1] == 0 && animation % 4 == 3){
				oldBit2 = (HBITMAP)SelectObject(mem2DC, gold); // mem2dc에는 hBit2
				TransparentBlt(mem1DC, chracter.x, chracter.y, 0.0661*rectview.right, 0.0975*rectview.bottom, mem2DC, 66, 0, 32, 48, RGB(255, 0, 255));// 앞모습
			}


			if (chracter.check == 1 && star[1] == 0 && animation % 4 == 0){
				oldBit2 = (HBITMAP)SelectObject(mem2DC, gold); // mem2dc에는 hBit2
				TransparentBlt(mem1DC, chracter.x, chracter.y, 0.0661*rectview.right, 0.0975*rectview.bottom, mem2DC, 33, 48, 32, 48, RGB(255, 0, 255));//뒷습
			}
			if (chracter.check == 1 && star[1] == 0 && animation % 4 == 1){
				oldBit2 = (HBITMAP)SelectObject(mem2DC, gold); // mem2dc에는 hBit2
				TransparentBlt(mem1DC, chracter.x, chracter.y, 0.0661*rectview.right, 0.0975*rectview.bottom, mem2DC, 0, 48, 32, 48, RGB(255, 0, 255));//뒷모습
			}
			if (chracter.check == 1 && star[1] == 0 && animation % 4 == 2){
				oldBit2 = (HBITMAP)SelectObject(mem2DC, gold); // mem2dc에는 hBit2
				TransparentBlt(mem1DC, chracter.x, chracter.y, 0.0661*rectview.right, 0.0975*rectview.bottom, mem2DC, 0, 48, 32, 48, RGB(255, 0, 255));//뒷습
			}
			if (chracter.check == 1 && star[1] == 0 && animation % 4 == 3){
				oldBit2 = (HBITMAP)SelectObject(mem2DC, gold); // mem2dc에는 hBit2
				TransparentBlt(mem1DC, chracter.x, chracter.y, 0.0661*rectview.right, 0.0975*rectview.bottom, mem2DC, 66, 48, 32, 48, RGB(255, 0, 255));//뒷습
			}



			if (chracter.check == 2 && star[1] == 0 && animation % 4 == 0){
				oldBit2 = (HBITMAP)SelectObject(mem2DC, gold); // mem2dc에는 hBit2
				TransparentBlt(mem1DC, chracter.x, chracter.y, 0.0661*rectview.right, 0.0975*rectview.bottom, mem2DC, 33, 144, 32, 48, RGB(255, 0, 255));//옆모습
			}
			if (chracter.check == 2 && star[1] == 0 && animation % 4 == 1){
				oldBit2 = (HBITMAP)SelectObject(mem2DC, gold); // mem2dc에는 hBit2
				TransparentBlt(mem1DC, chracter.x, chracter.y, 0.0661*rectview.right, 0.0975*rectview.bottom, mem2DC, 0, 144, 32, 48, RGB(255, 0, 255));//게옆모습
			}
			if (chracter.check == 2 && star[1] == 0 && animation % 4 == 2){
				oldBit2 = (HBITMAP)SelectObject(mem2DC, gold); // mem2dc에는 hBit2
				TransparentBlt(mem1DC, chracter.x, chracter.y, 0.0661*rectview.right, 0.0975*rectview.bottom, mem2DC, 0, 144, 32, 48, RGB(255, 0, 255));//옆모습
			}
			if (chracter.check == 2 && star[1] == 0 && animation % 4 == 3){
				oldBit2 = (HBITMAP)SelectObject(mem2DC, gold); // mem2dc에는 hBit2
				TransparentBlt(mem1DC, chracter.x, chracter.y, 0.0661*rectview.right, 0.0975*rectview.bottom, mem2DC, 66, 144, 32, 48, RGB(255, 0, 255));//옆모습
			}
			if (chracter.check == 3 && star[1] == 0 && animation % 4 == 0){
				oldBit2 = (HBITMAP)SelectObject(mem2DC, gold); // mem2dc에는 hBit2
				TransparentBlt(mem1DC, chracter.x, chracter.y, 0.0661*rectview.right, 0.0975*rectview.bottom, mem2DC, 33, 96, 30, 48, RGB(255, 0, 255));//옆습
			}
			if (chracter.check == 3 && star[1] == 0 && animation % 4 == 1){
				oldBit2 = (HBITMAP)SelectObject(mem2DC, gold); // mem2dc에는 hBit2
				TransparentBlt(mem1DC, chracter.x, chracter.y, 0.0661*rectview.right, 0.0975*rectview.bottom, mem2DC, 0, 96, 30, 48, RGB(255, 0, 255));//옆습
			}
			if (chracter.check == 3 && star[1] == 0 && animation % 4 == 2){
				oldBit2 = (HBITMAP)SelectObject(mem2DC, gold); // mem2dc에는 hBit2
				TransparentBlt(mem1DC, chracter.x, chracter.y, 0.0661*rectview.right, 0.0975*rectview.bottom, mem2DC, 0, 96, 30, 48, RGB(255, 0, 255));//옆습
			}
			if (chracter.check == 3 && star[1] == 0 && animation % 4 == 3){
				oldBit2 = (HBITMAP)SelectObject(mem2DC, gold); // mem2dc에는 hBit2
				TransparentBlt(mem1DC, chracter.x, chracter.y, 0.0661*rectview.right, 0.0975*rectview.bottom, mem2DC, 66, 96, 30, 48, RGB(255, 0, 255));//옆습
			}

			if (useOFF[1] == 1){
				oldBit2 = (HBITMAP)SelectObject(mem2DC, useitem[0]); // mem2dc에는 hBit2
				TransparentBlt(mem1DC, rectview.right * 11.7 / 16, rectview.bottom*9.85/ 11, 0.0670*rectview.right, 0.0692*rectview.bottom, mem2DC, 0, 0, 172, 180, RGB(255, 255, 255));//옆습
			}
			if (time>100){
				oldBit2 = (HBITMAP)SelectObject(mem2DC, ghostP);
				TransparentBlt(mem1DC, ghost.x, ghost.y,0.0661*rectview.right, 0.0975*rectview.bottom, mem2DC, 0, 0, 73, 69, RGB(255, 255, 255));// 앞모습
			}



		}
		if (start == 10){
			oldBit2 = (HBITMAP)SelectObject(mem2DC, end); // mem2dc에는 hBit2			
			StretchBlt(mem1DC, 0, 0, rectview.right, rectview.bottom, mem2DC, 0, 0, 226, 81, SRCCOPY);//종료
			if (musicon == 0){
				musicon = 1;
				PlaySound(MAKEINTRESOURCE(IDR_WAVE4), g_hInst, SND_RESOURCE | SND_ASYNC | SND_LOOP);
			}
			oldBit2 = (HBITMAP)SelectObject(mem2DC, exitbutton); // mem2dc에는 hBit2
			StretchBlt(mem1DC, rectview.right / 2 - 75+300, rectview.bottom / 2 + 50+300, 132, 25, mem2DC, 0, 0, 132, 25, SRCCOPY);//게임종료버튼
		}

		if (start == 5){
			KillTimer(hwnd, 10);
			KillTimer(hwnd, 11);
			oldBit2 = (HBITMAP)SelectObject(mem2DC, endingP); // mem2dc에는 hBit2			
			StretchBlt(mem1DC, 0, 0, rectview.right, rectview.bottom, mem2DC, 0, 0, 600, 408, SRCCOPY);//완료
			oldBit2 = (HBITMAP)SelectObject(mem2DC, exitbutton); // mem2dc에는 hBit2
			StretchBlt(mem1DC, rectview.right / 2 - 75 + 300, rectview.bottom / 2 + 50 + 300, 132, 25, mem2DC, 0, 0, 132, 25, SRCCOPY);//게임종료버튼
		}


		// hBit1에는 배경과 텍스트가 출력된 비트맵이 저장, mem1dc에 설정
		//  oldBit1 = (HBITMAP)SelectObject(mem1DC, hbit1);
		// mem1dc에 있는 내용을 hdc에 뿌려준다.
		BitBlt(hdc, 0, 0, rectview.right, rectview.bottom, mem1DC, 0, 0, SRCCOPY);

		SelectObject(mem1DC, oldBit1);
		SelectObject(mem2DC, oldBit2);
		DeleteDC(mem2DC);
		DeleteDC(mem1DC);
		//DeleteObject(hBit2);
		EndPaint(hwnd, &ps);
		break;

	case WM_TIMER:
		switch (wParam) {
		case 1: // 0.1초 간격으로 실행

			if (start == 2){
				if (chracter.ypos < 8){
					KillTimer(hwnd, 2);
					KillTimer(hwnd, 3);
					KillTimer(hwnd, 4);
					chracter.check = 0;
					if (!(map[chracter.ypos + 1][chracter.xpos].check > 0 && map[chracter.ypos + 1][chracter.xpos].check < 10)){
						chracter.y += 0.0975*rectview.bottom / 4;
						animation++;
						timen++;
					}
					if (map[chracter.ypos + 1][chracter.xpos].check != 0 && (map[chracter.ypos + 1][chracter.xpos].check>0 && map[chracter.ypos + 1][chracter.xpos].check < 10)){
						KillTimer(hwnd, 1);
						KillTimer(hwnd, 2);
						KillTimer(hwnd, 3);
						KillTimer(hwnd, 4);
					}
				}
				if (chracter.ypos == 8)
					KillTimer(hwnd, 1);
				if (timen == 4){
					timen = 0;
					if (map[chracter.ypos][chracter.xpos].check == 10)
						map[chracter.ypos][chracter.xpos].check = 0;

					chracter.ypos++;
					chracter.y = map[chracter.ypos][chracter.xpos].y;
					if (map[chracter.ypos][chracter.xpos].check == 23 && speed > 50)
						speed -= 20;
					if (map[chracter.ypos][chracter.xpos].check == 24 && chracter.power < 3)
						chracter.power++;
					if (map[chracter.ypos][chracter.xpos].check == 25 && chracter.bomb < 5)
						chracter.bomb++;
					map[chracter.ypos][chracter.xpos].check = 10;
					animation = 0;
					if (moveup==0)
					KillTimer(hwnd, 1);
				}
			}
			if (start == 3){
				if (chracter.ypos < 10){
					KillTimer(hwnd, 2);
					KillTimer(hwnd, 3);
					KillTimer(hwnd, 4);
					chracter.check = 0;
					if (map2[chracter.ypos + 1][chracter.xpos].check != 0 && (map2[chracter.ypos + 1][chracter.xpos].check>0 && map2[chracter.ypos + 1][chracter.xpos].check < 10)
						|| (map2[chracter.ypos + 1][chracter.xpos].check == 30)){
						KillTimer(hwnd, 1);
						KillTimer(hwnd, 2);
						KillTimer(hwnd, 3);
						KillTimer(hwnd, 4);
					}
					else if (!(map2[chracter.ypos + 1][chracter.xpos].check > 0 && map2[chracter.ypos + 1][chracter.xpos].check < 10)){
						chracter.y += 0.0975*rectview.bottom / 4;
						animation++;
						timen++;
					}

				}
				if (chracter.ypos == 10)
					KillTimer(hwnd, 1);
				if (timen == 4){
					timen = 0;
					if (map2[chracter.ypos][chracter.xpos].check == 10)
						map2[chracter.ypos][chracter.xpos].check = 0;

					chracter.ypos++;
					chracter.y = map2[chracter.ypos][chracter.xpos].y;
					if (map2[chracter.ypos][chracter.xpos].check == 23 && speed > 50)
						speed -= 20;
					if (map2[chracter.ypos][chracter.xpos].check == 24 && chracter.power < 3)
						chracter.power++;
					if (map2[chracter.ypos][chracter.xpos].check == 25 && chracter.bomb < 5)
						chracter.bomb++;
					map2[chracter.ypos][chracter.xpos].check = 10;
					animation = 0;
					if (moveup == 0)
					KillTimer(hwnd, 1);
				}

			}
			if (start == 4){
				if (chracter.ypos < 8){
					KillTimer(hwnd, 2);
					KillTimer(hwnd, 3);
					KillTimer(hwnd, 4);
					chracter.check = 0;
					if (map3[chracter.ypos + 1][chracter.xpos].check != 0 && (map3[chracter.ypos + 1][chracter.xpos].check>0 && map3[chracter.ypos + 1][chracter.xpos].check < 10)
						|| (map3[chracter.ypos + 1][chracter.xpos].check == 30)){
						KillTimer(hwnd, 1);
						KillTimer(hwnd, 2);
						KillTimer(hwnd, 3);
						KillTimer(hwnd, 4);
					}
					else if (!(map3[chracter.ypos + 1][chracter.xpos].check > 0 && map3[chracter.ypos + 1][chracter.xpos].check < 10)){
						chracter.y += 0.0975*rectview.bottom / 4;
						animation++;
						timen++;
					}

				}
				if (chracter.ypos == 8)
					KillTimer(hwnd, 1);
				if (timen == 4){
					timen = 0;
					if (map3[chracter.ypos][chracter.xpos].check == 10)
						map3[chracter.ypos][chracter.xpos].check = 0;

					chracter.ypos++;
					chracter.y = map3[chracter.ypos][chracter.xpos].y;
					if (map3[chracter.ypos][chracter.xpos].check == 23 && speed > 50)
						speed -= 20;
					if (map3[chracter.ypos][chracter.xpos].check == 24 && chracter.power < 3)
						chracter.power++;
					if (map3[chracter.ypos][chracter.xpos].check == 25 && chracter.bomb < 5)
						chracter.bomb++;
					map3[chracter.ypos][chracter.xpos].check = 10;
					animation = 0;
					if (moveup == 0)
					KillTimer(hwnd, 1);
				}

			}
			break;
		case 2: // 0.1초 간격으로 실행
			if (start == 2){
				if (chracter.ypos > 0){
					chracter.check = 1;
					KillTimer(hwnd, 1);
					KillTimer(hwnd, 3);
					KillTimer(hwnd, 4);
					if (!(map[chracter.ypos - 1][chracter.xpos].check >0 && map[chracter.ypos - 1][chracter.xpos].check < 10)){
						chracter.y -= 0.0975*rectview.bottom / 4;
						animation++;
						timen++;
					}
					if (map[chracter.ypos - 1][chracter.xpos].check != 0 && (map[chracter.ypos - 1][chracter.xpos].check>0 && map[chracter.ypos - 1][chracter.xpos].check < 10)
						){
						KillTimer(hwnd, 1);
						KillTimer(hwnd, 2);
						KillTimer(hwnd, 3);
						KillTimer(hwnd, 4);
					}
				}
				if (chracter.ypos == 0)
					KillTimer(hwnd, 2);
				if (timen == 4){
					timen = 0;
					if (map[chracter.ypos][chracter.xpos].check == 10)
						map[chracter.ypos][chracter.xpos].check = 0;

					chracter.ypos--;
					chracter.y = map[chracter.ypos][chracter.xpos].y;
					if (map[chracter.ypos][chracter.xpos].check == 23 && speed > 50)
						speed -= 20;
					if (map[chracter.ypos][chracter.xpos].check == 24 && chracter.power < 3)
						chracter.power++;
					if (map[chracter.ypos][chracter.xpos].check == 25 && chracter.bomb < 5)
						chracter.bomb++;
					map[chracter.ypos][chracter.xpos].check = 10;
					animation = 0;
					if (moveup == 0)
					KillTimer(hwnd, 2);
				}
			}
			if (start == 3){
				if (chracter.ypos > 0){
					chracter.check = 1;
					KillTimer(hwnd, 1);
					KillTimer(hwnd, 3);
					KillTimer(hwnd, 4);

					if (map2[chracter.ypos - 1][chracter.xpos].check != 0 && (map2[chracter.ypos - 1][chracter.xpos].check > 0 && map2[chracter.ypos - 1][chracter.xpos].check < 10)
						|| map2[chracter.ypos - 1][chracter.xpos].check == 30){
						KillTimer(hwnd, 1);
						KillTimer(hwnd, 2);
						KillTimer(hwnd, 3);
						KillTimer(hwnd, 4);
					}
					else if (!(map2[chracter.ypos - 1][chracter.xpos].check > 0 && map2[chracter.ypos - 1][chracter.xpos].check < 10)){
						chracter.y -= 0.0975*rectview.bottom / 4;
						animation++;
						timen++;
					}

				}
				if (chracter.ypos == 0)
					KillTimer(hwnd, 2);
				if (timen == 4){
					timen = 0;
					if (map2[chracter.ypos][chracter.xpos].check == 10)
						map2[chracter.ypos][chracter.xpos].check = 0;

					chracter.ypos--;
					chracter.y = map2[chracter.ypos][chracter.xpos].y;
					if (map2[chracter.ypos][chracter.xpos].check == 23 && speed > 50)
						speed -= 20;
					if (map2[chracter.ypos][chracter.xpos].check == 24 && chracter.power < 3)
						chracter.power++;
					if (map2[chracter.ypos][chracter.xpos].check == 25 && chracter.bomb < 5)
						chracter.bomb++;
					map2[chracter.ypos][chracter.xpos].check = 10;
					animation = 0;
					if (moveup == 0)
					KillTimer(hwnd, 2);
				}

			}

			if (start == 4){
				if (chracter.ypos > 0){
					chracter.check = 1;
					KillTimer(hwnd, 1);
					KillTimer(hwnd, 3);
					KillTimer(hwnd, 4);

					if (map3[chracter.ypos - 1][chracter.xpos].check != 0 && (map3[chracter.ypos - 1][chracter.xpos].check > 0 && map3[chracter.ypos - 1][chracter.xpos].check < 10)
						|| map3[chracter.ypos - 1][chracter.xpos].check == 30){
						KillTimer(hwnd, 1);
						KillTimer(hwnd, 2);
						KillTimer(hwnd, 3);
						KillTimer(hwnd, 4);
					}
					else if (!(map3[chracter.ypos - 1][chracter.xpos].check > 0 && map3[chracter.ypos - 1][chracter.xpos].check < 10)){
						chracter.y -= 0.0975*rectview.bottom / 4;
						animation++;
						timen++;
					}

				}
				if (chracter.ypos == 0)
					KillTimer(hwnd, 2);
				if (timen == 4){
					timen = 0;
					if (map3[chracter.ypos][chracter.xpos].check == 10)
						map3[chracter.ypos][chracter.xpos].check = 0;

					chracter.ypos--;
					chracter.y = map3[chracter.ypos][chracter.xpos].y;
					if (map3[chracter.ypos][chracter.xpos].check == 23 && speed > 50)
						speed -= 20;
					if (map3[chracter.ypos][chracter.xpos].check == 24 && chracter.power < 3)
						chracter.power++;
					if (map3[chracter.ypos][chracter.xpos].check == 25 && chracter.bomb < 5)
						chracter.bomb++;
					map3[chracter.ypos][chracter.xpos].check = 10;
					animation = 0;
					if (moveup == 0)
					KillTimer(hwnd, 2);
				}

			}
			break;
		case 3: // 0.1초 간격으로 실행
			if (start == 2){
				if (chracter.xpos < 12){
					chracter.check = 2;
					KillTimer(hwnd, 1);
					KillTimer(hwnd, 2);
					KillTimer(hwnd, 4);
					if (!(map[chracter.ypos][chracter.xpos + 1].check > 0 && map[chracter.ypos][chracter.xpos + 1].check < 10)){
						chracter.x += 0.0661*rectview.right / 4;
						animation++;
						timen++;
					}
					if (map[chracter.ypos][chracter.xpos + 1].check != 0 && (map[chracter.ypos][chracter.xpos + 1].check>0 && map[chracter.ypos][chracter.xpos + 1].check < 10)){
						KillTimer(hwnd, 1);
						KillTimer(hwnd, 2);
						KillTimer(hwnd, 3);
						KillTimer(hwnd, 4);
					}
				}
				if (chracter.xpos == 12)
					KillTimer(hwnd, 3);
				if (timen == 4){
					timen = 0;
					if (map[chracter.ypos][chracter.xpos].check == 10)
						map[chracter.ypos][chracter.xpos].check = 0;

					chracter.xpos++;
					chracter.x = map[chracter.ypos][chracter.xpos].x;
					animation = 0;
					if (map[chracter.ypos][chracter.xpos].check == 23 && speed > 50)
						speed -= 20;
					if (map[chracter.ypos][chracter.xpos].check == 24 && chracter.power < 3)
						chracter.power++;
					if (map[chracter.ypos][chracter.xpos].check == 25 && chracter.bomb < 5)
						chracter.bomb++;
					map[chracter.ypos][chracter.xpos].check = 10;
					if (moveup == 0)
					KillTimer(hwnd, 3);
				}
			}
			if (start == 3){
				if (chracter.xpos < 12){
					chracter.check = 2;
					KillTimer(hwnd, 1);
					KillTimer(hwnd, 2);
					KillTimer(hwnd, 4);
					if (map2[chracter.ypos][chracter.xpos + 1].check != 0 && (map2[chracter.ypos][chracter.xpos + 1].check>0 && map2[chracter.ypos][chracter.xpos + 1].check < 10)
						|| map2[chracter.ypos][chracter.xpos + 1].check == 30){
						KillTimer(hwnd, 1);
						KillTimer(hwnd, 2);
						KillTimer(hwnd, 3);
						KillTimer(hwnd, 4);
					}
					else if (!(map2[chracter.ypos][chracter.xpos + 1].check > 0 && map2[chracter.ypos][chracter.xpos + 1].check < 10)){
						chracter.x += 0.0661*rectview.right / 4;
						animation++;
						timen++;
					}

				}
				if (chracter.xpos == 12)
					KillTimer(hwnd, 3);
				if (timen == 4){
					timen = 0;
					if (map2[chracter.ypos][chracter.xpos].check == 10)
						map2[chracter.ypos][chracter.xpos].check = 0;

					chracter.xpos++;
					chracter.x = map2[chracter.ypos][chracter.xpos].x;
					animation = 0;
					if (map2[chracter.ypos][chracter.xpos].check == 23 && speed > 50)
						speed -= 20;
					if (map2[chracter.ypos][chracter.xpos].check == 24 && chracter.power < 3)
						chracter.power++;
					if (map2[chracter.ypos][chracter.xpos].check == 25 && chracter.bomb < 5)
						chracter.bomb++;
					map2[chracter.ypos][chracter.xpos].check = 10;
					if (moveup == 0)
					KillTimer(hwnd, 3);
				}
			}

			if (start == 4){
				if (chracter.xpos < 14){
					chracter.check = 2;
					KillTimer(hwnd, 1);
					KillTimer(hwnd, 2);
					KillTimer(hwnd, 4);
					if (map3[chracter.ypos][chracter.xpos + 1].check != 0 && (map3[chracter.ypos][chracter.xpos + 1].check>0 && map3[chracter.ypos][chracter.xpos + 1].check < 10)
						|| map3[chracter.ypos][chracter.xpos + 1].check == 30){
						KillTimer(hwnd, 1);
						KillTimer(hwnd, 2);
						KillTimer(hwnd, 3);
						KillTimer(hwnd, 4);
					}
					else if (!(map3[chracter.ypos][chracter.xpos + 1].check > 0 && map3[chracter.ypos][chracter.xpos + 1].check < 10)){
						chracter.x += 0.0661*rectview.right / 4;
						animation++;
						timen++;
					}

				}
				if (chracter.xpos == 14)
					KillTimer(hwnd, 3);
				if (timen == 4){
					timen = 0;
					if (map3[chracter.ypos][chracter.xpos].check == 10)
						map3[chracter.ypos][chracter.xpos].check = 0;

					chracter.xpos++;
					chracter.x = map3[chracter.ypos][chracter.xpos].x;
					animation = 0;
					if (map3[chracter.ypos][chracter.xpos].check == 23 && speed > 50)
						speed -= 20;
					if (map3[chracter.ypos][chracter.xpos].check == 24 && chracter.power < 3)
						chracter.power++;
					if (map3[chracter.ypos][chracter.xpos].check == 25 && chracter.bomb < 5)
						chracter.bomb++;
					map3[chracter.ypos][chracter.xpos].check = 10;
					if (moveup == 0)
					KillTimer(hwnd, 3);
				}
			}
			if (start == 4){
				if (chracter.xpos < 14){
					chracter.check = 2;
					KillTimer(hwnd, 1);
					KillTimer(hwnd, 2);
					KillTimer(hwnd, 4);
					if (map3[chracter.ypos][chracter.xpos + 1].check != 0 && (map3[chracter.ypos][chracter.xpos + 1].check>0 && map3[chracter.ypos][chracter.xpos + 1].check < 10)
						|| map3[chracter.ypos][chracter.xpos + 1].check == 30){
						KillTimer(hwnd, 1);
						KillTimer(hwnd, 2);
						KillTimer(hwnd, 3);
						KillTimer(hwnd, 4);
					}
					else if (!(map3[chracter.ypos][chracter.xpos + 1].check > 0 && map3[chracter.ypos][chracter.xpos + 1].check < 10)){
						chracter.x += 0.0661*rectview.right / 4;
						animation++;
						timen++;
					}

				}
				if (chracter.xpos == 14)
					KillTimer(hwnd, 3);
				if (timen == 4){
					timen = 0;
					if (map3[chracter.ypos][chracter.xpos].check == 10)
						map3[chracter.ypos][chracter.xpos].check = 0;

					chracter.xpos++;
					chracter.x = map3[chracter.ypos][chracter.xpos].x;
					animation = 0;
					if (map3[chracter.ypos][chracter.xpos].check == 23 && speed > 50)
						speed -= 20;
					if (map3[chracter.ypos][chracter.xpos].check == 24 && chracter.power < 3)
						chracter.power++;
					if (map3[chracter.ypos][chracter.xpos].check == 25 && chracter.bomb < 5)
						chracter.bomb++;
					map3[chracter.ypos][chracter.xpos].check = 10;
					if (moveup == 0)KillTimer(hwnd, 3);
				}
			}
			break;
		case 4: // 0.1초 간격으로 실행
			if (start == 2){
				if (chracter.xpos > 0){
					chracter.check = 3;
					KillTimer(hwnd, 1);
					KillTimer(hwnd, 2);
					KillTimer(hwnd, 3);

					if (!(map[chracter.ypos][chracter.xpos - 1].check > 0 && map[chracter.ypos][chracter.xpos - 1].check < 10)){
						chracter.x -= 0.0661*rectview.right / 4;
						animation++;
						timen++;
					}
					if (map[chracter.ypos][chracter.xpos - 1].check != 0 && (map[chracter.ypos][chracter.xpos - 1].check>0 && map[chracter.ypos][chracter.xpos - 1].check < 10)
						){
						KillTimer(hwnd, 1);
						KillTimer(hwnd, 2);
						KillTimer(hwnd, 3);
						KillTimer(hwnd, 4);
					}
				}
				if (chracter.xpos == 0)
					KillTimer(hwnd, 4);
				if (timen == 4){
					timen = 0;
					if (map[chracter.ypos][chracter.xpos].check == 10)
						map[chracter.ypos][chracter.xpos].check = 0;

					chracter.xpos--;
					chracter.x = map[chracter.ypos][chracter.xpos].x;
					if (map[chracter.ypos][chracter.xpos].check == 23 && speed > 50)
						speed -= 20;
					if (map[chracter.ypos][chracter.xpos].check == 24 && chracter.power < 3)
						chracter.power++;
					if (map[chracter.ypos][chracter.xpos].check == 25 && chracter.bomb < 5)
						chracter.bomb++;
					map[chracter.ypos][chracter.xpos].check = 10;
					animation = 0;
					if (moveup == 0)
					KillTimer(hwnd, 4);
				}
			}
			if (start == 3){
				if (chracter.xpos > 0){
					chracter.check = 3;
					KillTimer(hwnd, 1);
					KillTimer(hwnd, 2);
					KillTimer(hwnd, 3);
					if (map2[chracter.ypos][chracter.xpos - 1].check != 0 && (map2[chracter.ypos][chracter.xpos - 1].check > 0 && map2[chracter.ypos][chracter.xpos - 1].check < 10)
						|| map2[chracter.ypos][chracter.xpos - 1].check == 30){
						KillTimer(hwnd, 1);
						KillTimer(hwnd, 2);
						KillTimer(hwnd, 3);
						KillTimer(hwnd, 4);
					}
					else if (!(map2[chracter.ypos][chracter.xpos - 1].check > 0 && map2[chracter.ypos][chracter.xpos - 1].check < 10)){
						chracter.x -= 0.0661*rectview.right / 4;
						animation++;
						timen++;
					}

				}
				if (chracter.xpos == 0)
					KillTimer(hwnd, 4);
				if (timen == 4){
					timen = 0;
					if (map2[chracter.ypos][chracter.xpos].check == 10)
						map2[chracter.ypos][chracter.xpos].check = 0;

					chracter.xpos--;
					chracter.x = map2[chracter.ypos][chracter.xpos].x;
					if (map2[chracter.ypos][chracter.xpos].check == 23 && speed > 50)
						speed -= 20;
					if (map2[chracter.ypos][chracter.xpos].check == 24 && chracter.power < 3)
						chracter.power++;
					if (map2[chracter.ypos][chracter.xpos].check == 25 && chracter.bomb < 5)
						chracter.bomb++;
					map2[chracter.ypos][chracter.xpos].check = 10;
					animation = 0; 
					if (moveup == 0)
					KillTimer(hwnd, 4);
				}

			}

			if (start == 4){
				if (chracter.xpos > 0){
					chracter.check = 3;
					KillTimer(hwnd, 1);
					KillTimer(hwnd, 2);
					KillTimer(hwnd, 3);
					if (map3[chracter.ypos][chracter.xpos - 1].check != 0 && (map3[chracter.ypos][chracter.xpos - 1].check > 0 && map3[chracter.ypos][chracter.xpos - 1].check < 10)
						|| map3[chracter.ypos][chracter.xpos - 1].check == 30){
						KillTimer(hwnd, 1);
						KillTimer(hwnd, 2);
						KillTimer(hwnd, 3);
						KillTimer(hwnd, 4);
					}
					else if (!(map3[chracter.ypos][chracter.xpos - 1].check > 0 && map3[chracter.ypos][chracter.xpos - 1].check < 10)){
						chracter.x -= 0.0661*rectview.right / 4;
						animation++;
						timen++;
					}

				}
				if (chracter.xpos == 0)
					KillTimer(hwnd, 4);
				if (timen == 4){
					timen = 0;
					if (map3[chracter.ypos][chracter.xpos].check == 10)
						map3[chracter.ypos][chracter.xpos].check = 0;

					chracter.xpos--;
					chracter.x = map3[chracter.ypos][chracter.xpos].x;
					if (map3[chracter.ypos][chracter.xpos].check == 23 && speed > 50)
						speed -= 20;
					if (map3[chracter.ypos][chracter.xpos].check == 24 && chracter.power < 3)
						chracter.power++;
					if (map3[chracter.ypos][chracter.xpos].check == 25 && chracter.bomb < 5)
						chracter.bomb++;
					map3[chracter.ypos][chracter.xpos].check = 10;
					animation = 0;
					if (moveup == 0)KillTimer(hwnd, 4);
				}

			}
			break;


		case 5:
			animation++;
			if (animation == 4){
				start = 1;
				animation = 0;
				SetTimer(hwnd, 6, 500, NULL);
				KillTimer(hwnd, 5);
			}
			break;
		case 6:
			if (start == 2){
				for (int i = 0; i < chracter.bomb; i++){
				re:
					if (bombtime[i]>0){
						bombtime[i]++;
					}


					if (bombtime[i] == 2){
						map[bombI[i].y][bombI[i].x].check = 7;
					}

					if (bombtime[i] == 3){
						map[bombI[i].y][bombI[i].x].check = 8;
					}
					if (bombtime[i] == 4){
						map[bombI[i].y][bombI[i].x].check = 9;
					}

					if (bombtime[i] == 5){
						map[bombI[i].y][bombI[i].x].check = 7;
					}
					if (bombtime[i] == 6){
						map[bombI[i].y][bombI[i].x].check = 8;
					}
					if (bombtime[i] == 7){

						map[bombI[i].y][bombI[i].x].check = 9;
					}
					if (bombtime[i] == 8){
						map[bombI[i].y][bombI[i].x].check = 11;
						//PlaySound(NULL, 0, 0);
						PlaySound(MAKEINTRESOURCE(IDR_WAVE2), g_hInst, SND_RESOURCE | SND_ASYNC);
						if (chracter.power == 1){
							if (bombI[i].y != 0 && !(map[bombI[i].y - 1][bombI[i].x].check > 0 && map[bombI[i].y - 1][bombI[i].x].check < 6)){

								if (map[bombI[i].y - 1][bombI[i].x].check > 5 && map[bombI[i].y - 1][bombI[i].x].check < 10){
									for (int j = 0; j < chracter.bomb; j++){
										if (i != j)
											if (bombI[j].x == bombI[i].x&&bombI[j].y == bombI[i].y - 1){
												if (i < j)
													bombtime[j] = 7;
												if (i > j){
													bombtime[i] = 7;
													for (int k = j; k < i; k++)if (bombtime[k] == 8)
														bombtime[k] = 7;
													i = j;
													goto re;

												}

											}

									}
								}
								if (map[bombI[i].y - 1][bombI[i].x].check != 11)
									map[bombI[i].y - 1][bombI[i].x].check = 12;
							}


							if (bombI[i].y != 8 && !(map[bombI[i].y + 1][bombI[i].x].check > 0 && map[bombI[i].y + 1][bombI[i].x].check < 6)){

								if (map[bombI[i].y + 1][bombI[i].x].check > 5 && map[bombI[i].y + 1][bombI[i].x].check < 10){
									for (int j = 0; j < chracter.bomb; j++){
										if (i != j){
											if (bombI[j].x == bombI[i].x&&bombI[j].y == bombI[i].y + 1){
												if (i < j)
													bombtime[j] = 7;
												if (i > j){
													bombtime[i] = 7;
													for (int k = j; k < i; k++)if (bombtime[k] == 8)
														bombtime[k] = 7;
													i = j;
													goto re;

												}
											}

										}
									}
								}
								if (map[bombI[i].y + 1][bombI[i].x].check != 11)
									map[bombI[i].y + 1][bombI[i].x].check = 13;

							}

							if (bombI[i].x != 0 && !(map[bombI[i].y][bombI[i].x - 1].check > 0 && map[bombI[i].y][bombI[i].x - 1].check < 6)){

								if (map[bombI[i].y][bombI[i].x - 1].check > 5 && map[bombI[i].y][bombI[i].x - 1].check < 10){
									for (int j = 0; j < chracter.bomb; j++){
										if (i != j){
											if (bombI[j].x == bombI[i].x - 1 && bombI[j].y == bombI[i].y){
												if (i < j)
													bombtime[j] = 7;
												if (i > j){
													bombtime[i] = 7;
													for (int k = j; k < i; k++)if (bombtime[k] == 8)
														bombtime[k] = 7;
													i = j;
													goto re;

												}
											}

										}
									}
								}
								if (map[bombI[i].y][bombI[i].x - 1].check != 11)
									map[bombI[i].y][bombI[i].x - 1].check = 14;
							}

							if (bombI[i].x != 12 && !(map[bombI[i].y][bombI[i].x + 1].check > 0 && map[bombI[i].y][bombI[i].x + 1].check < 6)){

								if (map[bombI[i].y][bombI[i].x + 1].check > 5 && map[bombI[i].y][bombI[i].x + 1].check < 10){
									for (int j = 0; j < chracter.bomb; j++){
										if (i != j){
											if (bombI[j].x == bombI[i].x + 1 && bombI[j].y == bombI[i].y){
												if (i < j)
													bombtime[j] = 7;
												if (i > j){
													bombtime[i] = 7;
													for (int k = j; k < i; k++)if (bombtime[k] == 8)
														bombtime[k] = 7;
													i = j;
													goto re;

												}
											}

										}
									}
								}

								if (map[bombI[i].y][bombI[i].x + 1].check != 11)
									map[bombI[i].y][bombI[i].x + 1].check = 15;

							}


						}
						if (chracter.power == 2){

							if (bombI[i].y != 0 && !(map[bombI[i].y - 1][bombI[i].x].check > 0 && map[bombI[i].y - 1][bombI[i].x].check < 6)){

								if (map[bombI[i].y - 1][bombI[i].x].check > 5 && map[bombI[i].y - 1][bombI[i].x].check < 10){
									for (int j = 0; j < chracter.bomb; j++){
										if (i != j)
											if (bombI[j].x == bombI[i].x&&bombI[j].y == bombI[i].y - 1){
												if (i < j)
													bombtime[j] = 7;
												if (i > j){
													bombtime[i] = 7;
													for (int k = j; k < i; k++)if (bombtime[k] == 8)
														bombtime[k] = 7;
													i = j;
													goto re;
												}
											}
									}
								}
								map[bombI[i].y - 1][bombI[i].x].check = 11;
								if (bombI[i].y != 1 && !(map[bombI[i].y - 2][bombI[i].x].check > 0 && map[bombI[i].y - 2][bombI[i].x].check < 6)){

									if (map[bombI[i].y - 2][bombI[i].x].check > 5 && map[bombI[i].y - 2][bombI[i].x].check < 10){
										for (int j = 0; j < chracter.bomb; j++){
											if (i != j)
												if (bombI[j].x == bombI[i].x&&bombI[j].y == bombI[i].y - 2){
													if (i < j)
														bombtime[j] = 7;
													if (i > j){
														bombtime[i] = 7;
														for (int k = j; k < i; k++)if (bombtime[k] == 8)
															bombtime[k] = 7;
														i = j;
														goto re;

													}

												}

										}
									}
									if (map[bombI[i].y-2][bombI[i].x].check != 11)
									map[bombI[i].y - 2][bombI[i].x].check = 12;
								}
							}

							if (bombI[i].y != 8 && !(map[bombI[i].y + 1][bombI[i].x].check > 0 && map[bombI[i].y + 1][bombI[i].x].check < 6)){
								if (map[bombI[i].y + 1][bombI[i].x].check > 5 && map[bombI[i].y + 1][bombI[i].x].check < 10){
									for (int j = 0; j < chracter.bomb; j++){
										if (i != j){
											if (bombI[j].x == bombI[i].x&&bombI[j].y == bombI[i].y + 1){
												if (i < j)
													bombtime[j] = 7;
												if (i > j){
													bombtime[i] = 7;
													for (int k = j; k < i; k++)if (bombtime[k] == 8)
														bombtime[k] = 7;
													i = j;
													goto re;

												}
											}

										}
									}
								}
								map[bombI[i].y + 1][bombI[i].x].check = 11;

								if (bombI[i].y != 7 && !(map[bombI[i].y + 2][bombI[i].x].check > 0 && map[bombI[i].y + 2][bombI[i].x].check < 6)){
									if (map[bombI[i].y + 2][bombI[i].x].check > 5 && map[bombI[i].y + 2][bombI[i].x].check < 10){
										for (int j = 0; j < chracter.bomb; j++){
											if (i != j){
												if (bombI[j].x == bombI[i].x&&bombI[j].y == bombI[i].y + 2){
													if (i < j)
														bombtime[j] = 7;
													if (i > j){
														bombtime[i] = 7;
														for (int k = j; k < i; k++)if (bombtime[k] == 8)
															bombtime[k] = 7;
														i = j;
														goto re;

													}
												}

											}
										}
									}
									if (map[bombI[i].y + 2][bombI[i].x].check != 11)
									map[bombI[i].y + 2][bombI[i].x].check = 13;
								}
							}

							if (bombI[i].x != 0 && !(map[bombI[i].y][bombI[i].x - 1].check > 0 && map[bombI[i].y][bombI[i].x - 1].check < 6)){
								if (map[bombI[i].y][bombI[i].x - 1].check > 5 && map[bombI[i].y][bombI[i].x - 1].check < 10){
									for (int j = 0; j < chracter.bomb; j++){
										if (i != j){
											if (bombI[j].x == bombI[i].x - 1 && bombI[j].y == bombI[i].y){
												if (i < j)
													bombtime[j] = 7;
												if (i > j){
													bombtime[i] = 7;
													for (int k = j; k < i; k++)if (bombtime[k] == 8)
														bombtime[k] = 7;
													i = j;
													goto re;

												}
											}

										}
									}
								}

								map[bombI[i].y][bombI[i].x - 1].check = 11;
								if (bombI[i].x != 1 && !(map[bombI[i].y][bombI[i].x - 2].check > 0 && map[bombI[i].y][bombI[i].x - 2].check < 6)){
									if (map[bombI[i].y][bombI[i].x - 2].check > 5 && map[bombI[i].y][bombI[i].x - 2].check < 10){
										for (int j = 0; j < chracter.bomb; j++){
											if (i != j){
												if (bombI[j].x == bombI[i].x - 2 && bombI[j].y == bombI[i].y){
													if (i < j)
														bombtime[j] = 7;
													if (i > j){
														bombtime[i] = 7;
														for (int k = j; k < i; k++)if (bombtime[k] == 8)
															bombtime[k] = 7;
														i = j;
														goto re;

													}
												}

											}
										}
									}
									if (map[bombI[i].y][bombI[i].x-2].check != 11)
									map[bombI[i].y][bombI[i].x - 2].check = 14;
								}
							}

							if (bombI[i].x != 12 && !(map[bombI[i].y][bombI[i].x + 1].check > 0 && map[bombI[i].y][bombI[i].x + 1].check < 6)){
								if (map[bombI[i].y][bombI[i].x + 1].check > 5 && map[bombI[i].y][bombI[i].x + 1].check < 10){
									for (int j = 0; j < chracter.bomb; j++){
										if (i != j){
											if (bombI[j].x == bombI[i].x + 1 && bombI[j].y == bombI[i].y){
												if (i < j)
													bombtime[j] = 7;
												if (i > j){
													bombtime[i] = 7;
													for (int k = j; k < i; k++)if (bombtime[k] == 8)
														bombtime[k] = 7;
													i = j;
													goto re;

												}
											}

										}
									}
								}
								map[bombI[i].y][bombI[i].x + 1].check = 11;
								if (bombI[i].x != 11 && !(map[bombI[i].y][bombI[i].x + 2].check > 0 && map[bombI[i].y][bombI[i].x + 2].check < 6)){
									if (map[bombI[i].y][bombI[i].x + 2].check > 5 && map[bombI[i].y][bombI[i].x + 2].check < 10){
										for (int j = 0; j < chracter.bomb; j++){
											if (i != j){
												if (bombI[j].x == bombI[i].x + 2 && bombI[j].y == bombI[i].y){
													if (i < j)
														bombtime[j] = 7;
													if (i > j){
														bombtime[i] = 7;
														for (int k = j; k < i; k++)if (bombtime[k] == 8)
															bombtime[k] = 7;
														i = j;
														goto re;

													}
												}

											}
										}
									}
									if (map[bombI[i].y][bombI[i].x+2].check != 11)
									map[bombI[i].y][bombI[i].x + 2].check = 15;
								}
							}
						}




						if (chracter.power == 3){
							if (bombI[i].y != 0 && !(map[bombI[i].y - 1][bombI[i].x].check > 0 && map[bombI[i].y - 1][bombI[i].x].check < 6)){
								if (map[bombI[i].y - 1][bombI[i].x].check > 5 && map[bombI[i].y - 1][bombI[i].x].check < 10){
									for (int j = 0; j < chracter.bomb; j++){
										if (i != j)
											if (bombI[j].x == bombI[i].x&&bombI[j].y == bombI[i].y - 1){
												if (i < j)
													bombtime[j] = 7;
												if (i > j){
													bombtime[i] = 7;
													for (int k = j; k < i; k++)if (bombtime[k] == 8)
														bombtime[k] = 7;
													i = j;
													goto re;
												}
											}
									}
								}

								map[bombI[i].y - 1][bombI[i].x].check = 11;
								if (bombI[i].y != 1 && !(map[bombI[i].y - 2][bombI[i].x].check > 0 && map[bombI[i].y - 2][bombI[i].x].check < 6)){
									if (map[bombI[i].y - 2][bombI[i].x].check > 5 && map[bombI[i].y - 2][bombI[i].x].check < 10){
										for (int j = 0; j < chracter.bomb; j++){
											if (i != j)
												if (bombI[j].x == bombI[i].x&&bombI[j].y == bombI[i].y - 2){
													if (i < j)
														bombtime[j] = 7;
													if (i > j){
														bombtime[i] = 7;
														for (int k = j; k < i; k++)if (bombtime[k] == 8)
															bombtime[k] = 7;
														i = j;
														goto re;
													}
												}
										}
									}
									map[bombI[i].y - 2][bombI[i].x].check = 11;

									if (bombI[i].y != 2 && !(map[bombI[i].y - 3][bombI[i].x].check > 0 && map[bombI[i].y - 3][bombI[i].x].check < 6)){
										if (map[bombI[i].y - 3][bombI[i].x].check > 5 && map[bombI[i].y - 3][bombI[i].x].check < 10){
											for (int j = 0; j < chracter.bomb; j++){
												if (i != j)
													if (bombI[j].x == bombI[i].x&&bombI[j].y == bombI[i].y - 3){
														if (i < j)
															bombtime[j] = 7;
														if (i > j){
															bombtime[i] = 7;
															for (int k = j; k < i; k++)if (bombtime[k] == 8)
																bombtime[k] = 7;
															i = j;
															goto re;
														}
													}
											}
										}
										map[bombI[i].y - 3][bombI[i].x].check = 12;
									}
								}
							}


							if (bombI[i].y != 8 && !(map[bombI[i].y + 1][bombI[i].x].check > 0 && map[bombI[i].y + 1][bombI[i].x].check < 6)){
								if (map[bombI[i].y + 1][bombI[i].x].check > 5 && map[bombI[i].y + 1][bombI[i].x].check < 10){
									for (int j = 0; j < chracter.bomb; j++){
										if (i != j){
											if (bombI[j].x == bombI[i].x&&bombI[j].y == bombI[i].y + 1){
												if (i < j)
													bombtime[j] = 7;
												if (i > j){
													bombtime[i] = 7;
													for (int k = j; k < i; k++)if (bombtime[k] == 8)
														bombtime[k] = 7;
													i = j;
													goto re;

												}
											}

										}
									}
								}

								map[bombI[i].y + 1][bombI[i].x].check = 11;

								if (bombI[i].y != 7 && !(map[bombI[i].y + 2][bombI[i].x].check > 0 && map[bombI[i].y + 2][bombI[i].x].check < 6)){
									if (map[bombI[i].y + 2][bombI[i].x].check > 5 && map[bombI[i].y + 2][bombI[i].x].check < 10){
										for (int j = 0; j < chracter.bomb; j++){
											if (i != j){
												if (bombI[j].x == bombI[i].x&&bombI[j].y == bombI[i].y + 2){
													if (i < j)
														bombtime[j] = 7;
													if (i > j){
														bombtime[i] = 7;
														for (int k = j; k < i; k++)if (bombtime[k] == 8)
															bombtime[k] = 7;
														i = j;
														goto re;

													}
												}

											}
										}
									}

									map[bombI[i].y + 2][bombI[i].x].check = 11;
									if (bombI[i].y != 6 && !(map[bombI[i].y + 3][bombI[i].x].check > 0 && map[bombI[i].y + 3][bombI[i].x].check < 6)){
										if (map[bombI[i].y + 3][bombI[i].x].check > 5 && map[bombI[i].y + 3][bombI[i].x].check < 10){
											for (int j = 0; j < chracter.bomb; j++){
												if (i != j){
													if (bombI[j].x == bombI[i].x&&bombI[j].y == bombI[i].y + 3){
														if (i < j)
															bombtime[j] = 7;
														if (i > j){
															bombtime[i] = 7;
															for (int k = j; k < i; k++)if (bombtime[k] == 8)
																bombtime[k] = 7;
															i = j;
															goto re;

														}
													}

												}
											}
										}

										map[bombI[i].y + 3][bombI[i].x].check = 13;
									}

								}
							}

							if (bombI[i].x != 0 && !(map[bombI[i].y][bombI[i].x - 1].check > 0 && map[bombI[i].y][bombI[i].x - 1].check < 6)){
								if (map[bombI[i].y][bombI[i].x - 1].check > 5 && map[bombI[i].y][bombI[i].x - 1].check < 10){
									for (int j = 0; j < chracter.bomb; j++){
										if (i != j){
											if (bombI[j].x == bombI[i].x - 1 && bombI[j].y == bombI[i].y){
												if (i < j)
													bombtime[j] = 7;
												if (i > j){
													bombtime[i] = 7;
													for (int k = j; k < i; k++)if (bombtime[k] == 8)
														bombtime[k] = 7;
													i = j;
													goto re;

												}
											}

										}
									}
								}

								map[bombI[i].y][bombI[i].x - 1].check = 11;
								if (bombI[i].x != 1 && !(map[bombI[i].y][bombI[i].x - 2].check > 0 && map[bombI[i].y][bombI[i].x - 2].check < 6)){
									if (map[bombI[i].y][bombI[i].x - 2].check > 5 && map[bombI[i].y][bombI[i].x - 2].check < 10){
										for (int j = 0; j < chracter.bomb; j++){
											if (i != j){
												if (bombI[j].x == bombI[i].x - 2 && bombI[j].y == bombI[i].y){
													if (i < j)
														bombtime[j] = 7;
													if (i > j){
														bombtime[i] = 7;
														for (int k = j; k < i; k++)if (bombtime[k] == 8)
															bombtime[k] = 7;
														i = j;
														goto re;

													}
												}

											}
										}
									}


									map[bombI[i].y][bombI[i].x - 2].check = 11;
									if (bombI[i].x != 2 && !(map[bombI[i].y][bombI[i].x - 3].check > 0 && map[bombI[i].y][bombI[i].x - 3].check < 6)){
										if (map[bombI[i].y][bombI[i].x - 3].check > 5 && map[bombI[i].y][bombI[i].x - 3].check < 10){
											for (int j = 0; j < chracter.bomb; j++){
												if (i != j){
													if (bombI[j].x == bombI[i].x - 3 && bombI[j].y == bombI[i].y){
														if (i < j)
															bombtime[j] = 7;
														if (i > j){
															bombtime[i] = 7;
															for (int k = j; k < i; k++)if (bombtime[k] == 8)
																bombtime[k] = 7;
															i = j;
															goto re;

														}
													}

												}
											}
										}


										map[bombI[i].y][bombI[i].x - 3].check = 14;
									}
								}

							}
							if (bombI[i].x != 12 && !(map[bombI[i].y][bombI[i].x + 1].check > 0 && map[bombI[i].y][bombI[i].x + 1].check < 6)){
								if (map[bombI[i].y][bombI[i].x + 1].check > 5 && map[bombI[i].y][bombI[i].x + 1].check < 10){
									for (int j = 0; j < chracter.bomb; j++){
										if (i != j){
											if (bombI[j].x == bombI[i].x + 1 && bombI[j].y == bombI[i].y){
												if (i < j)
													bombtime[j] = 7;
												if (i > j){
													bombtime[i] = 7;
													for (int k = j; k < i; k++)if (bombtime[k] == 8)
														bombtime[k] = 7;
													i = j;
													goto re;

												}
											}

										}
									}
								}
								map[bombI[i].y][bombI[i].x + 1].check = 11;
								if (bombI[i].x != 11 && !(map[bombI[i].y][bombI[i].x + 2].check > 0 && map[bombI[i].y][bombI[i].x + 2].check < 6)){
									if (map[bombI[i].y][bombI[i].x + 2].check > 5 && map[bombI[i].y][bombI[i].x + 2].check < 10){
										for (int j = 0; j < chracter.bomb; j++){
											if (i != j){
												if (bombI[j].x == bombI[i].x + 2 && bombI[j].y == bombI[i].y){
													if (i < j)
														bombtime[j] = 7;
													if (i > j){
														bombtime[i] = 7;
														for (int k = j; k < i; k++)if (bombtime[k] == 8)
															bombtime[k] = 7;
														i = j;
														goto re;

													}
												}

											}
										}
									}
									map[bombI[i].y][bombI[i].x + 2].check = 11;
									if (bombI[i].x != 10 && !(map[bombI[i].y][bombI[i].x + 3].check > 0 && map[bombI[i].y][bombI[i].x + 3].check < 6)){
										if (map[bombI[i].y][bombI[i].x + 3].check > 5 && map[bombI[i].y][bombI[i].x + 3].check < 10){
											for (int j = 0; j < chracter.bomb; j++){
												if (i != j){
													if (bombI[j].x == bombI[i].x + 3 && bombI[j].y == bombI[i].y){
														if (i < j)
															bombtime[j] = 7;
														if (i > j){
															bombtime[i] = 7;
															for (int k = j; k < i; k++)if (bombtime[k] == 8)
																bombtime[k] = 7;
															i = j;
															goto re;

														}
													}

												}
											}
										}
										map[bombI[i].y][bombI[i].x + 3].check = 15;
									}
								}
							}
						}


					}
					if (bombtime[i] == 9){

						map[bombI[i].y][bombI[i].x].check = 0;


						if (chracter.power == 1){
							if (bombI[i].y != 0){
								if (map[bombI[i].y - 1][bombI[i].x].check == 2)
									map[bombI[i].y - 1][bombI[i].x].check = 0;
								else if (map[bombI[i].y - 1][bombI[i].x].check == 3)
									map[bombI[i].y - 1][bombI[i].x].check = 23;
								else if (map[bombI[i].y - 1][bombI[i].x].check == 4)
									map[bombI[i].y - 1][bombI[i].x].check = 24;
								else if (map[bombI[i].y - 1][bombI[i].x].check == 5)
									map[bombI[i].y - 1][bombI[i].x].check = 25;
								else if (map[bombI[i].y - 1][bombI[i].x].check == 12)
									map[bombI[i].y - 1][bombI[i].x].check = 0;

							}
							if (bombI[i].y != 8){
								if (map[bombI[i].y + 1][bombI[i].x].check == 2)
									map[bombI[i].y + 1][bombI[i].x].check = 0;
								else if (map[bombI[i].y + 1][bombI[i].x].check == 3)
									map[bombI[i].y + 1][bombI[i].x].check = 23;
								else if (map[bombI[i].y + 1][bombI[i].x].check == 4)
									map[bombI[i].y + 1][bombI[i].x].check = 24;
								else if (map[bombI[i].y + 1][bombI[i].x].check == 5)
									map[bombI[i].y + 1][bombI[i].x].check = 25;
								else if (map[bombI[i].y + 1][bombI[i].x].check == 13)
									map[bombI[i].y + 1][bombI[i].x].check = 0;
							}

							if (bombI[i].x != 0){
								if (map[bombI[i].y][bombI[i].x - 1].check == 2)
									map[bombI[i].y][bombI[i].x - 1].check = 0;

								else if (map[bombI[i].y][bombI[i].x - 1].check == 3)
									map[bombI[i].y][bombI[i].x - 1].check = 23;
								else if (map[bombI[i].y][bombI[i].x - 1].check == 4)
									map[bombI[i].y][bombI[i].x - 1].check = 24;
								else if (map[bombI[i].y][bombI[i].x - 1].check == 5)
									map[bombI[i].y][bombI[i].x - 1].check = 25;
								else if (map[bombI[i].y][bombI[i].x - 1].check == 14)
									map[bombI[i].y][bombI[i].x - 1].check = 0;
							}
							if (bombI[i].x != 12){
								if (map[bombI[i].y][bombI[i].x + 1].check == 2)
									map[bombI[i].y][bombI[i].x + 1].check = 0;
								else if (map[bombI[i].y][bombI[i].x + 1].check == 3)
									map[bombI[i].y][bombI[i].x + 1].check = 23;
								else if (map[bombI[i].y][bombI[i].x + 1].check == 4)
									map[bombI[i].y][bombI[i].x + 1].check = 24;
								else if (map[bombI[i].y][bombI[i].x + 1].check == 5)
									map[bombI[i].y][bombI[i].x + 1].check = 25;
								else if (map[bombI[i].y][bombI[i].x + 1].check == 15)
									map[bombI[i].y][bombI[i].x + 1].check = 0;
							}
						}
						if (chracter.power == 2){
							if (bombI[i].y != 0){
								if (map[bombI[i].y - 1][bombI[i].x].check == 11 || map[bombI[i].y - 1][bombI[i].x].check == 12)
									map[bombI[i].y - 1][bombI[i].x].check = 0;
								if (map[bombI[i].y - 2][bombI[i].x].check == 12)
									map[bombI[i].y - 2][bombI[i].x].check = 0;
								if (bombI[i].y != 1){
									if ((map[bombI[i].y - 1][bombI[i].x].check == 0) && (map[bombI[i].y - 2][bombI[i].x].check == 2))
										map[bombI[i].y - 2][bombI[i].x].check = 0;
									else if ((map[bombI[i].y - 1][bombI[i].x].check == 0) && (map[bombI[i].y - 2][bombI[i].x].check == 3))
										map[bombI[i].y - 2][bombI[i].x].check = 23;
									else if ((map[bombI[i].y - 1][bombI[i].x].check == 0) && (map[bombI[i].y - 2][bombI[i].x].check == 4))
										map[bombI[i].y - 2][bombI[i].x].check = 24;
									else if ((map[bombI[i].y - 1][bombI[i].x].check == 0) && (map[bombI[i].y - 2][bombI[i].x].check == 5))
										map[bombI[i].y - 2][bombI[i].x].check = 25;
								}
								if (map[bombI[i].y - 1][bombI[i].x].check == 2)
									map[bombI[i].y - 1][bombI[i].x].check = 0;
								if (map[bombI[i].y - 1][bombI[i].x].check == 3)
									map[bombI[i].y - 1][bombI[i].x].check = 23;
								if (map[bombI[i].y - 1][bombI[i].x].check == 4)
									map[bombI[i].y - 1][bombI[i].x].check = 24;
								if (map[bombI[i].y - 1][bombI[i].x].check == 5)
									map[bombI[i].y - 1][bombI[i].x].check = 25;
							}
							if (bombI[i].y != 8){
								if (map[bombI[i].y + 1][bombI[i].x].check == 11 || map[bombI[i].y + 1][bombI[i].x].check == 13)
									map[bombI[i].y + 1][bombI[i].x].check = 0;
								if (map[bombI[i].y + 2][bombI[i].x].check == 13)
									map[bombI[i].y + 2][bombI[i].x].check = 0;

								if (bombI[i].y != 7){
									if ((map[bombI[i].y + 1][bombI[i].x].check == 0) && (map[bombI[i].y + 2][bombI[i].x].check == 2))
										map[bombI[i].y + 2][bombI[i].x].check = 0;
									if ((map[bombI[i].y + 1][bombI[i].x].check == 0) && (map[bombI[i].y + 2][bombI[i].x].check == 3))
										map[bombI[i].y + 2][bombI[i].x].check = 23;
									if ((map[bombI[i].y + 1][bombI[i].x].check == 0) && (map[bombI[i].y + 2][bombI[i].x].check == 4))
										map[bombI[i].y + 2][bombI[i].x].check = 24;
									if ((map[bombI[i].y + 1][bombI[i].x].check == 0) && (map[bombI[i].y + 2][bombI[i].x].check == 5))
										map[bombI[i].y + 2][bombI[i].x].check = 25;
								}
								if (map[bombI[i].y + 1][bombI[i].x].check == 2)
									map[bombI[i].y + 1][bombI[i].x].check = 0;
								if (map[bombI[i].y + 1][bombI[i].x].check == 3)
									map[bombI[i].y + 1][bombI[i].x].check = 23;
								if (map[bombI[i].y + 1][bombI[i].x].check == 4)
									map[bombI[i].y + 1][bombI[i].x].check = 24;
								if (map[bombI[i].y + 1][bombI[i].x].check == 5)
									map[bombI[i].y + 1][bombI[i].x].check = 25;
							}
							if (bombI[i].x != 0){
								if (map[bombI[i].y][bombI[i].x - 1].check == 11 || map[bombI[i].y][bombI[i].x - 1].check == 14)
									map[bombI[i].y][bombI[i].x - 1].check = 0;
								if (map[bombI[i].y][bombI[i].x - 2].check == 14)
									map[bombI[i].y][bombI[i].x - 2].check = 0;
								if (bombI[i].x != 1){
									if ((map[bombI[i].y][bombI[i].x - 1].check == 0) && (map[bombI[i].y][bombI[i].x - 2].check == 2))
										map[bombI[i].y][bombI[i].x - 2].check = 0;
									if ((map[bombI[i].y][bombI[i].x - 1].check == 0) && (map[bombI[i].y][bombI[i].x - 2].check == 3))
										map[bombI[i].y][bombI[i].x - 2].check = 23;
									if ((map[bombI[i].y][bombI[i].x - 1].check == 0) && (map[bombI[i].y][bombI[i].x - 2].check == 4))
										map[bombI[i].y][bombI[i].x - 2].check = 24;
									if ((map[bombI[i].y][bombI[i].x - 1].check == 0) && (map[bombI[i].y][bombI[i].x - 2].check == 5))
										map[bombI[i].y][bombI[i].x - 2].check = 25;
								}
								if (map[bombI[i].y][bombI[i].x - 1].check == 2)
									map[bombI[i].y][bombI[i].x - 1].check = 0;
								if (map[bombI[i].y][bombI[i].x - 1].check == 3)
									map[bombI[i].y][bombI[i].x - 1].check = 23;
								if (map[bombI[i].y][bombI[i].x - 1].check == 4)
									map[bombI[i].y][bombI[i].x - 1].check = 24;
								if (map[bombI[i].y][bombI[i].x - 1].check == 5)
									map[bombI[i].y][bombI[i].x - 1].check = 25;
							}
							if (bombI[i].x != 12){
								if (map[bombI[i].y][bombI[i].x + 1].check == 11 || map[bombI[i].y][bombI[i].x + 1].check == 15)
									map[bombI[i].y][bombI[i].x + 1].check = 0;
								if (map[bombI[i].y][bombI[i].x + 2].check == 15)
									map[bombI[i].y][bombI[i].x + 2].check = 0;
								if (bombI[i].x != 11){
									if ((map[bombI[i].y][bombI[i].x + 1].check == 0) && (map[bombI[i].y][bombI[i].x + 2].check == 2))
										map[bombI[i].y][bombI[i].x + 2].check = 0;
									if ((map[bombI[i].y][bombI[i].x + 1].check == 0) && (map[bombI[i].y][bombI[i].x + 2].check == 3))
										map[bombI[i].y][bombI[i].x + 2].check = 23;
									if ((map[bombI[i].y][bombI[i].x + 1].check == 0) && (map[bombI[i].y][bombI[i].x + 2].check == 4))
										map[bombI[i].y][bombI[i].x + 2].check = 24;
									if ((map[bombI[i].y][bombI[i].x + 1].check == 0) && (map[bombI[i].y][bombI[i].x + 2].check == 5))
										map[bombI[i].y][bombI[i].x + 2].check = 25;
								}
								if (map[bombI[i].y][bombI[i].x + 1].check == 2)
									map[bombI[i].y][bombI[i].x + 1].check = 0;
								if (map[bombI[i].y][bombI[i].x + 1].check == 3)
									map[bombI[i].y][bombI[i].x + 1].check = 23;
								if (map[bombI[i].y][bombI[i].x + 1].check == 4)
									map[bombI[i].y][bombI[i].x + 1].check = 24;
								if (map[bombI[i].y][bombI[i].x + 1].check == 5)
									map[bombI[i].y][bombI[i].x + 1].check = 25;
							}
						}
						if (chracter.power == 3){
							if (bombI[i].y != 0){
								if (map[bombI[i].y - 1][bombI[i].x].check == 11 || map[bombI[i].y - 1][bombI[i].x].check == 12)
									map[bombI[i].y - 1][bombI[i].x].check = 0;
								if (map[bombI[i].y - 2][bombI[i].x].check == 11 || map[bombI[i].y - 2][bombI[i].x].check == 12)
									map[bombI[i].y - 2][bombI[i].x].check = 0;
								if (map[bombI[i].y - 3][bombI[i].x].check == 12)
									map[bombI[i].y - 3][bombI[i].x].check = 0;
								if (bombI[i].y != 2){
									if ((map[bombI[i].y - 1][bombI[i].x].check == 0) && (map[bombI[i].y - 2][bombI[i].x].check == 0) && (map[bombI[i].y - 3][bombI[i].x].check == 2))
										map[bombI[i].y - 3][bombI[i].x].check = 0;
									else if ((map[bombI[i].y - 1][bombI[i].x].check == 0) && (map[bombI[i].y - 2][bombI[i].x].check == 0) && (map[bombI[i].y - 3][bombI[i].x].check == 3))
										map[bombI[i].y - 3][bombI[i].x].check = 23;
									else if ((map[bombI[i].y - 1][bombI[i].x].check == 0) && (map[bombI[i].y - 2][bombI[i].x].check == 0) && (map[bombI[i].y - 3][bombI[i].x].check == 4))
										map[bombI[i].y - 3][bombI[i].x].check = 24;
									else if ((map[bombI[i].y - 1][bombI[i].x].check == 0) && (map[bombI[i].y - 2][bombI[i].x].check == 0) && (map[bombI[i].y - 3][bombI[i].x].check == 5))
										map[bombI[i].y - 3][bombI[i].x].check = 25;
								}
								if (bombI[i].y != 1){
									if ((map[bombI[i].y - 1][bombI[i].x].check == 0) && (map[bombI[i].y - 2][bombI[i].x].check == 2))
										map[bombI[i].y - 2][bombI[i].x].check = 0;
									else if ((map[bombI[i].y - 1][bombI[i].x].check == 0) && (map[bombI[i].y - 2][bombI[i].x].check == 3))
										map[bombI[i].y - 2][bombI[i].x].check = 23;
									else if ((map[bombI[i].y - 1][bombI[i].x].check == 0) && (map[bombI[i].y - 2][bombI[i].x].check == 4))
										map[bombI[i].y - 2][bombI[i].x].check = 24;
									else if ((map[bombI[i].y - 1][bombI[i].x].check == 0) && (map[bombI[i].y - 2][bombI[i].x].check == 5))
										map[bombI[i].y - 2][bombI[i].x].check = 25;
								}
								if (map[bombI[i].y - 1][bombI[i].x].check == 2)
									map[bombI[i].y - 1][bombI[i].x].check = 0;
								if (map[bombI[i].y - 1][bombI[i].x].check == 3)
									map[bombI[i].y - 1][bombI[i].x].check = 23;
								if (map[bombI[i].y - 1][bombI[i].x].check == 4)
									map[bombI[i].y - 1][bombI[i].x].check = 24;
								if (map[bombI[i].y - 1][bombI[i].x].check == 5)
									map[bombI[i].y - 1][bombI[i].x].check = 25;
							}
							if (bombI[i].y != 8){
								if (map[bombI[i].y + 1][bombI[i].x].check == 11 || map[bombI[i].y + 1][bombI[i].x].check == 13)
									map[bombI[i].y + 1][bombI[i].x].check = 0;
								if (map[bombI[i].y + 2][bombI[i].x].check == 11 || map[bombI[i].y + 2][bombI[i].x].check == 13)
									map[bombI[i].y + 2][bombI[i].x].check = 0;
								if (map[bombI[i].y + 3][bombI[i].x].check == 13)
									map[bombI[i].y + 3][bombI[i].x].check = 0;
								if (bombI[i].y != 6){
									if ((map[bombI[i].y + 1][bombI[i].x].check == 0) && (map[bombI[i].y + 2][bombI[i].x].check == 0) && (map[bombI[i].y + 3][bombI[i].x].check == 2))
										map[bombI[i].y + 3][bombI[i].x].check = 0;
									else if ((map[bombI[i].y + 1][bombI[i].x].check == 0) && (map[bombI[i].y + 2][bombI[i].x].check == 0) && (map[bombI[i].y + 3][bombI[i].x].check == 3))
										map[bombI[i].y + 3][bombI[i].x].check = 23;
									else if ((map[bombI[i].y + 1][bombI[i].x].check == 0) && (map[bombI[i].y + 2][bombI[i].x].check == 0) && (map[bombI[i].y + 3][bombI[i].x].check == 4))
										map[bombI[i].y + 3][bombI[i].x].check = 24;
									else if ((map[bombI[i].y + 1][bombI[i].x].check == 0) && (map[bombI[i].y + 2][bombI[i].x].check == 0) && (map[bombI[i].y + 3][bombI[i].x].check == 5))
										map[bombI[i].y + 3][bombI[i].x].check = 25;
								}

								if (bombI[i].y != 7){
									if ((map[bombI[i].y + 1][bombI[i].x].check == 0) && (map[bombI[i].y + 2][bombI[i].x].check == 2))
										map[bombI[i].y + 2][bombI[i].x].check = 0;
									if ((map[bombI[i].y + 1][bombI[i].x].check == 0) && (map[bombI[i].y + 2][bombI[i].x].check == 3))
										map[bombI[i].y + 2][bombI[i].x].check = 23;
									if ((map[bombI[i].y + 1][bombI[i].x].check == 0) && (map[bombI[i].y + 2][bombI[i].x].check == 4))
										map[bombI[i].y + 2][bombI[i].x].check = 24;
									if ((map[bombI[i].y + 1][bombI[i].x].check == 0) && (map[bombI[i].y + 2][bombI[i].x].check == 5))
										map[bombI[i].y + 2][bombI[i].x].check = 25;
								}
								if (map[bombI[i].y + 1][bombI[i].x].check == 2)
									map[bombI[i].y + 1][bombI[i].x].check = 0;
								if (map[bombI[i].y + 1][bombI[i].x].check == 3)
									map[bombI[i].y + 1][bombI[i].x].check = 23;
								if (map[bombI[i].y + 1][bombI[i].x].check == 4)
									map[bombI[i].y + 1][bombI[i].x].check = 24;
								if (map[bombI[i].y + 1][bombI[i].x].check == 5)
									map[bombI[i].y + 1][bombI[i].x].check = 25;
							}
							if (bombI[i].x != 0){
								if (map[bombI[i].y][bombI[i].x - 1].check == 11 || map[bombI[i].y][bombI[i].x - 1].check == 14)
									map[bombI[i].y][bombI[i].x - 1].check = 0;
								if (map[bombI[i].y][bombI[i].x - 2].check == 11 || map[bombI[i].y][bombI[i].x - 2].check == 14)
									map[bombI[i].y][bombI[i].x - 2].check = 0;
								if (map[bombI[i].y][bombI[i].x - 3].check == 14)
									map[bombI[i].y][bombI[i].x - 3].check = 0;
								if (bombI[i].x != 2){
									if ((map[bombI[i].y][bombI[i].x - 1].check == 0) && (map[bombI[i].y][bombI[i].x - 2].check == 0) && (map[bombI[i].y][bombI[i].x - 3].check == 2))
										map[bombI[i].y][bombI[i].x - 3].check = 0;
									else if ((map[bombI[i].y][bombI[i].x - 1].check == 0) && (map[bombI[i].y][bombI[i].x - 2].check == 0) && (map[bombI[i].y][bombI[i].x - 3].check == 3))
										map[bombI[i].y][bombI[i].x - 3].check = 23;
									else if ((map[bombI[i].y][bombI[i].x - 1].check == 0) && (map[bombI[i].y][bombI[i].x - 2].check == 0) && (map[bombI[i].y][bombI[i].x - 3].check == 4))
										map[bombI[i].y][bombI[i].x - 3].check = 24;
									else if ((map[bombI[i].y][bombI[i].x - 1].check == 0) && (map[bombI[i].y][bombI[i].x - 2].check == 0) && (map[bombI[i].y][bombI[i].x - 3].check == 5))
										map[bombI[i].y][bombI[i].x - 3].check = 25;
								}

								if (bombI[i].x != 1){
									if ((map[bombI[i].y][bombI[i].x - 1].check == 0) && (map[bombI[i].y][bombI[i].x - 2].check == 2))
										map[bombI[i].y][bombI[i].x - 2].check = 0;
									if ((map[bombI[i].y][bombI[i].x - 1].check == 0) && (map[bombI[i].y][bombI[i].x - 2].check == 3))
										map[bombI[i].y][bombI[i].x - 2].check = 23;
									if ((map[bombI[i].y][bombI[i].x - 1].check == 0) && (map[bombI[i].y][bombI[i].x - 2].check == 4))
										map[bombI[i].y][bombI[i].x - 2].check = 24;
									if ((map[bombI[i].y][bombI[i].x - 1].check == 0) && (map[bombI[i].y][bombI[i].x - 2].check == 5))
										map[bombI[i].y][bombI[i].x - 2].check = 25;
								}
								if (map[bombI[i].y][bombI[i].x - 1].check == 2)
									map[bombI[i].y][bombI[i].x - 1].check = 0;
								if (map[bombI[i].y][bombI[i].x - 1].check == 3)
									map[bombI[i].y][bombI[i].x - 1].check = 23;
								if (map[bombI[i].y][bombI[i].x - 1].check == 4)
									map[bombI[i].y][bombI[i].x - 1].check = 24;
								if (map[bombI[i].y][bombI[i].x - 1].check == 5)
									map[bombI[i].y][bombI[i].x - 1].check = 25;
							}
							if (bombI[i].x != 12){
								if (map[bombI[i].y][bombI[i].x + 1].check == 11 || map[bombI[i].y][bombI[i].x + 1].check == 15)
									map[bombI[i].y][bombI[i].x + 1].check = 0;
								if (map[bombI[i].y][bombI[i].x + 2].check == 11 || map[bombI[i].y][bombI[i].x + 2].check == 15)
									map[bombI[i].y][bombI[i].x + 2].check = 0;
								if (map[bombI[i].y][bombI[i].x + 3].check == 15)
									map[bombI[i].y][bombI[i].x + 3].check = 0;
								if (bombI[i].x != 10){
									if ((map[bombI[i].y][bombI[i].x + 1].check == 0) && (map[bombI[i].y][bombI[i].x + 2].check == 0) && (map[bombI[i].y][bombI[i].x + 3].check == 2))
										map[bombI[i].y][bombI[i].x + 3].check = 0;
									else if ((map[bombI[i].y][bombI[i].x + 1].check == 0) && (map[bombI[i].y][bombI[i].x + 2].check == 0) && (map[bombI[i].y][bombI[i].x + 3].check == 3))
										map[bombI[i].y][bombI[i].x + 3].check = 23;
									else if ((map[bombI[i].y][bombI[i].x + 1].check == 0) && (map[bombI[i].y][bombI[i].x + 2].check == 0) && (map[bombI[i].y][bombI[i].x + 3].check == 4))
										map[bombI[i].y][bombI[i].x + 3].check = 24;
									else if ((map[bombI[i].y][bombI[i].x + 1].check == 0) && (map[bombI[i].y][bombI[i].x + 2].check == 0) && (map[bombI[i].y][bombI[i].x + 3].check == 5))
										map[bombI[i].y][bombI[i].x + 3].check = 25;
								}

								if (bombI[i].x != 11){
									if ((map[bombI[i].y][bombI[i].x + 1].check == 0) && (map[bombI[i].y][bombI[i].x + 2].check == 2))
										map[bombI[i].y][bombI[i].x + 2].check = 0;
									if ((map[bombI[i].y][bombI[i].x + 1].check == 0) && (map[bombI[i].y][bombI[i].x + 2].check == 3))
										map[bombI[i].y][bombI[i].x + 2].check = 23;
									if ((map[bombI[i].y][bombI[i].x + 1].check == 0) && (map[bombI[i].y][bombI[i].x + 2].check == 4))
										map[bombI[i].y][bombI[i].x + 2].check = 24;
									if ((map[bombI[i].y][bombI[i].x + 1].check == 0) && (map[bombI[i].y][bombI[i].x + 2].check == 5))
										map[bombI[i].y][bombI[i].x + 2].check = 25;
								}
								if (map[bombI[i].y][bombI[i].x + 1].check == 2)
									map[bombI[i].y][bombI[i].x + 1].check = 0;
								if (map[bombI[i].y][bombI[i].x + 1].check == 3)
									map[bombI[i].y][bombI[i].x + 1].check = 23;
								if (map[bombI[i].y][bombI[i].x + 1].check == 4)
									map[bombI[i].y][bombI[i].x + 1].check = 24;
								if (map[bombI[i].y][bombI[i].x + 1].check == 5)
									map[bombI[i].y][bombI[i].x + 1].check = 25;
							}
						}
						bombtime[i] = 0;
						if (chracter.bomb > 0){
							if (bombtime[0] == 0)
								numberbomb = 1;
							if (chracter.bomb > 1){
								if (bombtime[1] == 0 && bombtime[0] != 0)
									numberbomb = 2;
								if (chracter.bomb > 2){
									if (bombtime[1] != 0 && bombtime[0] != 0 && bombtime[2] == 0)
										numberbomb = 3;
									if (chracter.bomb > 3){
										if (bombtime[1] != 0 && bombtime[0] != 0 && bombtime[2] != 0 && bombtime[3] == 0)
											numberbomb = 4;
										if (chracter.bomb > 4){
											if (bombtime[1] != 0 && bombtime[0] != 0 && bombtime[2] != 0 && bombtime[3] != 0 && bombtime[4] == 0)
												numberbomb = 5;
										}
									}
								}
							}
						}

					}

				}
				break;
			}
			if (start == 3){
				for (int i = 0; i < chracter.bomb; i++){
				re3:
					if (bombtime[i]>0){
						bombtime[i]++;
					}


					if (bombtime[i] == 2){
						map2[bombI[i].y][bombI[i].x].check = 7;
					}

					if (bombtime[i] == 3){
						map2[bombI[i].y][bombI[i].x].check = 8;
					}
					if (bombtime[i] == 4){
						map2[bombI[i].y][bombI[i].x].check = 9;
					}

					if (bombtime[i] == 5){
						map2[bombI[i].y][bombI[i].x].check = 7;
					}
					if (bombtime[i] == 6){
						map2[bombI[i].y][bombI[i].x].check = 8;
					}
					if (bombtime[i] == 7){

						map2[bombI[i].y][bombI[i].x].check = 9;
					}
					if (bombtime[i] == 8){
						map2[bombI[i].y][bombI[i].x].check = 11;
						//PlaySound(NULL, 0, 0);
						PlaySound(MAKEINTRESOURCE(IDR_WAVE2), g_hInst, SND_RESOURCE | SND_ASYNC);
						if (chracter.power == 1){
							if (bombI[i].y != 0 && !(map2[bombI[i].y - 1][bombI[i].x].check > 0 && map2[bombI[i].y - 1][bombI[i].x].check < 6)){

								if (map2[bombI[i].y - 1][bombI[i].x].check > 5 && map2[bombI[i].y - 1][bombI[i].x].check < 10){
									for (int j = 0; j < chracter.bomb; j++){
										if (i != j)
											if (bombI[j].x == bombI[i].x&&bombI[j].y == bombI[i].y - 1){
												if (i < j)
													bombtime[j] = 7;
												if (i > j){
													bombtime[i] = 7;
													for (int k = j; k < i; k++)if (bombtime[k] == 8)
														bombtime[k] = 7;
													i = j;
													goto re3;

												}

											}

									}
								}
								if (map2[bombI[i].y - 1][bombI[i].x].check != 11)
									map2[bombI[i].y - 1][bombI[i].x].check = 12;
							}


							if (bombI[i].y != 10 && !(map2[bombI[i].y + 1][bombI[i].x].check > 0 && map2[bombI[i].y + 1][bombI[i].x].check < 6)){

								if (map2[bombI[i].y + 1][bombI[i].x].check > 5 && map2[bombI[i].y + 1][bombI[i].x].check < 10){
									for (int j = 0; j < chracter.bomb; j++){
										if (i != j){
											if (bombI[j].x == bombI[i].x&&bombI[j].y == bombI[i].y + 1){
												if (i < j)
													bombtime[j] = 7;
												if (i > j){
													bombtime[i] = 7;
													for (int k = j; k < i; k++)if (bombtime[k] == 8)
														bombtime[k] = 7;
													i = j;
													goto re3;

												}
											}

										}
									}
								}
								if (map2[bombI[i].y + 1][bombI[i].x].check != 11)
									map2[bombI[i].y + 1][bombI[i].x].check = 13;

							}

							if (bombI[i].x != 0 && !(map2[bombI[i].y][bombI[i].x - 1].check > 0 && map2[bombI[i].y][bombI[i].x - 1].check < 6)){

								if (map2[bombI[i].y][bombI[i].x - 1].check > 5 && map2[bombI[i].y][bombI[i].x - 1].check < 10){
									for (int j = 0; j < chracter.bomb; j++){
										if (i != j){
											if (bombI[j].x == bombI[i].x - 1 && bombI[j].y == bombI[i].y){
												if (i < j)
													bombtime[j] = 7;
												if (i > j){
													bombtime[i] = 7;
													for (int k = j; k < i; k++)if (bombtime[k] == 8)
														bombtime[k] = 7;
													i = j;
													goto re3;

												}
											}

										}
									}
								}
								if (map2[bombI[i].y][bombI[i].x - 1].check != 11)
									map2[bombI[i].y][bombI[i].x - 1].check = 14;
							}

							if (bombI[i].x != 12 && !(map2[bombI[i].y][bombI[i].x + 1].check > 0 && map2[bombI[i].y][bombI[i].x + 1].check < 6)){

								if (map2[bombI[i].y][bombI[i].x + 1].check > 5 && map2[bombI[i].y][bombI[i].x + 1].check < 10){
									for (int j = 0; j < chracter.bomb; j++){
										if (i != j){
											if (bombI[j].x == bombI[i].x + 1 && bombI[j].y == bombI[i].y){
												if (i < j)
													bombtime[j] = 7;
												if (i > j){
													bombtime[i] = 7;
													for (int k = j; k < i; k++)if (bombtime[k] == 8)
														bombtime[k] = 7;
													i = j;
													goto re3;

												}
											}

										}
									}
								}

								if (map2[bombI[i].y][bombI[i].x + 1].check != 11)
									map2[bombI[i].y][bombI[i].x + 1].check = 15;

							}


						}
						if (chracter.power == 2){

							if (bombI[i].y != 0 && !(map2[bombI[i].y - 1][bombI[i].x].check > 0 && map2[bombI[i].y - 1][bombI[i].x].check < 6)){

								if (map2[bombI[i].y - 1][bombI[i].x].check > 5 && map2[bombI[i].y - 1][bombI[i].x].check < 10){
									for (int j = 0; j < chracter.bomb; j++){
										if (i != j)
											if (bombI[j].x == bombI[i].x&&bombI[j].y == bombI[i].y - 1){
												if (i < j)
													bombtime[j] = 7;
												if (i > j){
													bombtime[i] = 7;
													for (int k = j; k < i; k++)if (bombtime[k] == 8)
														bombtime[k] = 7;
													i = j;
													goto re3;
												}
											}
									}
								}
								map2[bombI[i].y - 1][bombI[i].x].check = 11;
								if (bombI[i].y != 1 && !(map2[bombI[i].y - 2][bombI[i].x].check > 0 && map2[bombI[i].y - 2][bombI[i].x].check < 6)){

									if (map2[bombI[i].y - 2][bombI[i].x].check > 5 && map2[bombI[i].y - 2][bombI[i].x].check < 10){
										for (int j = 0; j < chracter.bomb; j++){
											if (i != j)
												if (bombI[j].x == bombI[i].x&&bombI[j].y == bombI[i].y - 2){
													if (i < j)
														bombtime[j] = 7;
													if (i > j){
														bombtime[i] = 7;
														for (int k = j; k < i; k++)if (bombtime[k] == 8)
															bombtime[k] = 7;
														i = j;
														goto re3;

													}

												}

										}
									}
									map2[bombI[i].y - 2][bombI[i].x].check = 12;
								}
							}

							if (bombI[i].y != 11 && !(map2[bombI[i].y + 1][bombI[i].x].check > 0 && map2[bombI[i].y + 1][bombI[i].x].check < 6)){
								if (map2[bombI[i].y + 1][bombI[i].x].check > 5 && map2[bombI[i].y + 1][bombI[i].x].check < 10){
									for (int j = 0; j < chracter.bomb; j++){
										if (i != j){
											if (bombI[j].x == bombI[i].x&&bombI[j].y == bombI[i].y + 1){
												if (i < j)
													bombtime[j] = 7;
												if (i > j){
													bombtime[i] = 7;
													for (int k = j; k < i; k++)if (bombtime[k] == 8)
														bombtime[k] = 7;
													i = j;
													goto re3;

												}
											}

										}
									}
								}
								map2[bombI[i].y + 1][bombI[i].x].check = 11;

								if (bombI[i].y != 7 && !(map2[bombI[i].y + 2][bombI[i].x].check > 0 && map2[bombI[i].y + 2][bombI[i].x].check < 6)){
									if (map2[bombI[i].y + 2][bombI[i].x].check > 5 && map2[bombI[i].y + 1][bombI[i].x].check < 10){
										for (int j = 0; j < chracter.bomb; j++){
											if (i != j){
												if (bombI[j].x == bombI[i].x&&bombI[j].y == bombI[i].y + 2){
													if (i < j)
														bombtime[j] = 7;
													if (i > j){
														bombtime[i] = 7;
														for (int k = j; k < i; k++)if (bombtime[k] == 8)
															bombtime[k] = 7;
														i = j;
														goto re3;

													}
												}

											}
										}
									}
									map2[bombI[i].y + 2][bombI[i].x].check = 13;
								}
							}

							if (bombI[i].x != 0 && !(map2[bombI[i].y][bombI[i].x - 1].check > 0 && map2[bombI[i].y][bombI[i].x - 1].check < 6)){
								if (map2[bombI[i].y][bombI[i].x - 1].check > 5 && map2[bombI[i].y][bombI[i].x - 1].check < 10){
									for (int j = 0; j < chracter.bomb; j++){
										if (i != j){
											if (bombI[j].x == bombI[i].x - 1 && bombI[j].y == bombI[i].y){
												if (i < j)
													bombtime[j] = 7;
												if (i > j){
													bombtime[i] = 7;
													for (int k = j; k < i; k++)if (bombtime[k] == 8)
														bombtime[k] = 7;
													i = j;
													goto re3;

												}
											}

										}
									}
								}
								map2[bombI[i].y][bombI[i].x - 1].check = 11;
								if (bombI[i].x != 1 && !(map2[bombI[i].y][bombI[i].x - 2].check > 0 && map2[bombI[i].y][bombI[i].x - 2].check < 6)){
									if (map2[bombI[i].y][bombI[i].x - 2].check > 5 && map2[bombI[i].y][bombI[i].x - 2].check < 10){
										for (int j = 0; j < chracter.bomb; j++){
											if (i != j){
												if (bombI[j].x == bombI[i].x - 2 && bombI[j].y == bombI[i].y){
													if (i < j)
														bombtime[j] = 7;
													if (i > j){
														bombtime[i] = 7;
														for (int k = j; k < i; k++)if (bombtime[k] == 8)
															bombtime[k] = 7;
														i = j;
														goto re3;

													}
												}

											}
										}
									}

									map2[bombI[i].y][bombI[i].x - 2].check = 14;
								}
							}

							if (bombI[i].x != 12 && !(map2[bombI[i].y][bombI[i].x + 1].check > 0 && map2[bombI[i].y][bombI[i].x + 1].check < 6)){
								if (map2[bombI[i].y][bombI[i].x + 1].check > 5 && map2[bombI[i].y][bombI[i].x + 1].check < 10){
									for (int j = 0; j < chracter.bomb; j++){
										if (i != j){
											if (bombI[j].x == bombI[i].x + 1 && bombI[j].y == bombI[i].y){
												if (i < j)
													bombtime[j] = 7;
												if (i > j){
													bombtime[i] = 7;
													for (int k = j; k < i; k++)if (bombtime[k] == 8)
														bombtime[k] = 7;
													i = j;
													goto re3;

												}
											}

										}
									}
								}
								map2[bombI[i].y][bombI[i].x + 1].check = 11;
								if (bombI[i].x != 11 && !(map2[bombI[i].y][bombI[i].x + 2].check > 0 && map2[bombI[i].y][bombI[i].x + 2].check < 6)){
									if (map2[bombI[i].y][bombI[i].x + 2].check > 5 && map2[bombI[i].y][bombI[i].x + 2].check < 10){
										for (int j = 0; j < chracter.bomb; j++){
											if (i != j){
												if (bombI[j].x == bombI[i].x + 2 && bombI[j].y == bombI[i].y){
													if (i < j)
														bombtime[j] = 7;
													if (i > j){
														bombtime[i] = 7;
														for (int k = j; k < i; k++)if (bombtime[k] == 8)
															bombtime[k] = 7;
														i = j;
														goto re3;

													}
												}

											}
										}
									}

									map2[bombI[i].y][bombI[i].x + 2].check = 15;
								}
							}
						}




						if (chracter.power == 3){
							if (bombI[i].y != 0 && !(map2[bombI[i].y - 1][bombI[i].x].check > 0 && map2[bombI[i].y - 1][bombI[i].x].check < 6)){
								if (map2[bombI[i].y - 1][bombI[i].x].check > 5 && map2[bombI[i].y - 1][bombI[i].x].check < 10){
									for (int j = 0; j < chracter.bomb; j++){
										if (i != j)
											if (bombI[j].x == bombI[i].x&&bombI[j].y == bombI[i].y - 1){
												if (i < j)
													bombtime[j] = 7;
												if (i > j){
													bombtime[i] = 7;
													for (int k = j; k < i; k++)if (bombtime[k] == 8)
														bombtime[k] = 7;
													i = j;
													goto re3;
												}
											}
									}
								}

								map2[bombI[i].y - 1][bombI[i].x].check = 11;
								if (bombI[i].y != 1 && !(map2[bombI[i].y - 2][bombI[i].x].check > 0 && map2[bombI[i].y - 2][bombI[i].x].check < 6)){
									if (map2[bombI[i].y - 2][bombI[i].x].check > 5 && map2[bombI[i].y - 2][bombI[i].x].check < 10){
										for (int j = 0; j < chracter.bomb; j++){
											if (i != j)
												if (bombI[j].x == bombI[i].x&&bombI[j].y == bombI[i].y - 2){
													if (i < j)
														bombtime[j] = 7;
													if (i > j){
														bombtime[i] = 7;
														for (int k = j; k < i; k++)if (bombtime[k] == 8)
															bombtime[k] = 7;
														i = j;
														goto re3;
													}
												}
										}
									}
									map2[bombI[i].y - 2][bombI[i].x].check = 11;

									if (bombI[i].y != 2 && !(map2[bombI[i].y - 3][bombI[i].x].check > 0 && map2[bombI[i].y - 3][bombI[i].x].check < 6)){
										if (map2[bombI[i].y - 3][bombI[i].x].check > 5 && map2[bombI[i].y - 3][bombI[i].x].check < 10){
											for (int j = 0; j < chracter.bomb; j++){
												if (i != j)
													if (bombI[j].x == bombI[i].x&&bombI[j].y == bombI[i].y - 3){
														if (i < j)
															bombtime[j] = 7;
														if (i > j){
															bombtime[i] = 7;
															for (int k = j; k < i; k++)if (bombtime[k] == 8)
																bombtime[k] = 7;
															i = j;
															goto re3;
														}
													}
											}
										}
										map2[bombI[i].y - 3][bombI[i].x].check = 12;
									}
								}
							}


							if (bombI[i].y != 11 && !(map2[bombI[i].y + 1][bombI[i].x].check > 0 && map2[bombI[i].y + 1][bombI[i].x].check < 6)){
								if (map2[bombI[i].y + 1][bombI[i].x].check > 5 && map2[bombI[i].y + 1][bombI[i].x].check < 10){
									for (int j = 0; j < chracter.bomb; j++){
										if (i != j){
											if (bombI[j].x == bombI[i].x&&bombI[j].y == bombI[i].y + 1){
												if (i < j)
													bombtime[j] = 7;
												if (i > j){
													bombtime[i] = 7;
													for (int k = j; k < i; k++)if (bombtime[k] == 8)
														bombtime[k] = 7;
													i = j;
													goto re3;

												}
											}

										}
									}
								}

								map2[bombI[i].y + 1][bombI[i].x].check = 11;

								if (bombI[i].y != 10 && !(map2[bombI[i].y + 2][bombI[i].x].check > 0 && map2[bombI[i].y + 2][bombI[i].x].check < 6)){
									if (map2[bombI[i].y + 2][bombI[i].x].check > 5 && map2[bombI[i].y + 2][bombI[i].x].check < 10){
										for (int j = 0; j < chracter.bomb; j++){
											if (i != j){
												if (bombI[j].x == bombI[i].x&&bombI[j].y == bombI[i].y + 2){
													if (i < j)
														bombtime[j] = 7;
													if (i > j){
														bombtime[i] = 7;
														for (int k = j; k < i; k++)if (bombtime[k] == 8)
															bombtime[k] = 7;
														i = j;
														goto re3;

													}
												}

											}
										}
									}

									map2[bombI[i].y + 2][bombI[i].x].check = 11;
									if (bombI[i].y != 9 && !(map2[bombI[i].y + 3][bombI[i].x].check > 0 && map2[bombI[i].y + 3][bombI[i].x].check < 6)){
										if (map2[bombI[i].y + 3][bombI[i].x].check > 5 && map2[bombI[i].y + 3][bombI[i].x].check < 10){
											for (int j = 0; j < chracter.bomb; j++){
												if (i != j){
													if (bombI[j].x == bombI[i].x&&bombI[j].y == bombI[i].y + 3){
														if (i < j)
															bombtime[j] = 7;
														if (i > j){
															bombtime[i] = 7;
															for (int k = j; k < i; k++)if (bombtime[k] == 8)
																bombtime[k] = 7;
															i = j;
															goto re3;

														}
													}

												}
											}
										}

										map2[bombI[i].y + 3][bombI[i].x].check = 13;
									}

								}
							}

							if (bombI[i].x != 0 && !(map2[bombI[i].y][bombI[i].x - 1].check > 0 && map2[bombI[i].y][bombI[i].x - 1].check < 6)){
								if (map2[bombI[i].y][bombI[i].x - 1].check > 5 && map2[bombI[i].y][bombI[i].x - 1].check < 10){
									for (int j = 0; j < chracter.bomb; j++){
										if (i != j){
											if (bombI[j].x == bombI[i].x - 1 && bombI[j].y == bombI[i].y){
												if (i < j)
													bombtime[j] = 7;
												if (i > j){
													bombtime[i] = 7;
													for (int k = j; k < i; k++)if (bombtime[k] == 8)
														bombtime[k] = 7;
													i = j;
													goto re3;

												}
											}

										}
									}
								}

								map2[bombI[i].y][bombI[i].x - 1].check = 11;
								if (bombI[i].x != 1 && !(map2[bombI[i].y][bombI[i].x - 2].check > 0 && map2[bombI[i].y][bombI[i].x - 2].check < 6)){
									if (map2[bombI[i].y][bombI[i].x - 2].check > 5 && map2[bombI[i].y][bombI[i].x - 2].check < 10){
										for (int j = 0; j < chracter.bomb; j++){
											if (i != j){
												if (bombI[j].x == bombI[i].x - 2 && bombI[j].y == bombI[i].y){
													if (i < j)
														bombtime[j] = 7;
													if (i > j){
														bombtime[i] = 7;
														for (int k = j; k < i; k++)if (bombtime[k] == 8)
															bombtime[k] = 7;
														i = j;
														goto re3;

													}
												}

											}
										}
									}


									map2[bombI[i].y][bombI[i].x - 2].check = 11;
									if (bombI[i].x != 2 && !(map2[bombI[i].y][bombI[i].x - 3].check > 0 && map2[bombI[i].y][bombI[i].x - 3].check < 6)){
										if (map2[bombI[i].y][bombI[i].x - 3].check > 5 && map2[bombI[i].y][bombI[i].x - 3].check < 10){
											for (int j = 0; j < chracter.bomb; j++){
												if (i != j){
													if (bombI[j].x == bombI[i].x - 3 && bombI[j].y == bombI[i].y){
														if (i < j)
															bombtime[j] = 7;
														if (i > j){
															bombtime[i] = 7;
															for (int k = j; k < i; k++)if (bombtime[k] == 8)
																bombtime[k] = 7;
															i = j;
															goto re3;

														}
													}

												}
											}
										}


										map2[bombI[i].y][bombI[i].x - 3].check = 14;
									}
								}

							}
							if (bombI[i].x != 12 && !(map2[bombI[i].y][bombI[i].x + 1].check > 0 && map2[bombI[i].y][bombI[i].x + 1].check < 6)){
								if (map2[bombI[i].y][bombI[i].x + 1].check > 5 && map2[bombI[i].y][bombI[i].x + 1].check < 10){
									for (int j = 0; j < chracter.bomb; j++){
										if (i != j){
											if (bombI[j].x == bombI[i].x + 1 && bombI[j].y == bombI[i].y){
												if (i < j)
													bombtime[j] = 7;
												if (i > j){
													bombtime[i] = 7;
													for (int k = j; k < i; k++)if (bombtime[k] == 8)
														bombtime[k] = 7;
													i = j;
													goto re3;

												}
											}

										}
									}
								}
								map2[bombI[i].y][bombI[i].x + 1].check = 11;
								if (bombI[i].x != 11 && !(map2[bombI[i].y][bombI[i].x + 2].check > 0 && map2[bombI[i].y][bombI[i].x + 2].check < 6)){
									if (map2[bombI[i].y][bombI[i].x + 2].check > 5 && map2[bombI[i].y][bombI[i].x + 2].check < 10){
										for (int j = 0; j < chracter.bomb; j++){
											if (i != j){
												if (bombI[j].x == bombI[i].x + 2 && bombI[j].y == bombI[i].y){
													if (i < j)
														bombtime[j] = 7;
													if (i > j){
														bombtime[i] = 7;
														for (int k = j; k < i; k++)if (bombtime[k] == 8)
															bombtime[k] = 7;
														i = j;
														goto re3;

													}
												}

											}
										}
									}
									map2[bombI[i].y][bombI[i].x + 2].check = 11;
									if (bombI[i].x != 10 && !(map2[bombI[i].y][bombI[i].x + 3].check > 0 && map2[bombI[i].y][bombI[i].x + 3].check < 6)){
										if (map2[bombI[i].y][bombI[i].x + 3].check > 5 && map2[bombI[i].y][bombI[i].x + 3].check < 10){
											for (int j = 0; j < chracter.bomb; j++){
												if (i != j){
													if (bombI[j].x == bombI[i].x + 3 && bombI[j].y == bombI[i].y){
														if (i < j)
															bombtime[j] = 7;
														if (i > j){
															bombtime[i] = 7;
															for (int k = j; k < i; k++)if (bombtime[k] == 8)
																bombtime[k] = 7;
															i = j;
															goto re3;

														}
													}

												}
											}
										}
										map2[bombI[i].y][bombI[i].x + 3].check = 15;
									}
								}
							}
						}


					}
					if (bombtime[i] == 9){

						map2[bombI[i].y][bombI[i].x].check = 0;


						if (chracter.power == 1){
							if (bombI[i].y != 0){
								if (map2[bombI[i].y - 1][bombI[i].x].check == 2)
									map2[bombI[i].y - 1][bombI[i].x].check = 0;
								else if (map2[bombI[i].y - 1][bombI[i].x].check == 3)
									map2[bombI[i].y - 1][bombI[i].x].check = 23;
								else if (map2[bombI[i].y - 1][bombI[i].x].check == 4)
									map2[bombI[i].y - 1][bombI[i].x].check = 24;
								else if (map2[bombI[i].y - 1][bombI[i].x].check == 5)
									map2[bombI[i].y - 1][bombI[i].x].check = 25;
								else if (map2[bombI[i].y - 1][bombI[i].x].check == 12)
									map2[bombI[i].y - 1][bombI[i].x].check = 0;

							}
							if (bombI[i].y != 11){
								if (map2[bombI[i].y + 1][bombI[i].x].check == 2)
									map2[bombI[i].y + 1][bombI[i].x].check = 0;
								else if (map2[bombI[i].y + 1][bombI[i].x].check == 3)
									map2[bombI[i].y + 1][bombI[i].x].check = 23;
								else if (map2[bombI[i].y + 1][bombI[i].x].check == 4)
									map2[bombI[i].y + 1][bombI[i].x].check = 24;
								else if (map2[bombI[i].y + 1][bombI[i].x].check == 5)
									map2[bombI[i].y + 1][bombI[i].x].check = 25;
								else if (map2[bombI[i].y + 1][bombI[i].x].check == 13)
									map2[bombI[i].y + 1][bombI[i].x].check = 0;
							}

							if (bombI[i].x != 0){
								if (map2[bombI[i].y][bombI[i].x - 1].check == 2)
									map2[bombI[i].y][bombI[i].x - 1].check = 0;

								else if (map2[bombI[i].y][bombI[i].x - 1].check == 3)
									map2[bombI[i].y][bombI[i].x - 1].check = 23;
								else if (map2[bombI[i].y][bombI[i].x - 1].check == 4)
									map2[bombI[i].y][bombI[i].x - 1].check = 24;
								else if (map2[bombI[i].y][bombI[i].x - 1].check == 5)
									map2[bombI[i].y][bombI[i].x - 1].check = 25;
								else if (map2[bombI[i].y][bombI[i].x - 1].check == 14)
									map2[bombI[i].y][bombI[i].x - 1].check = 0;
							}
							if (bombI[i].x != 12){
								if (map2[bombI[i].y][bombI[i].x + 1].check == 2)
									map2[bombI[i].y][bombI[i].x + 1].check = 0;
								else if (map2[bombI[i].y][bombI[i].x + 1].check == 3)
									map2[bombI[i].y][bombI[i].x + 1].check = 23;
								else if (map2[bombI[i].y][bombI[i].x + 1].check == 4)
									map2[bombI[i].y][bombI[i].x + 1].check = 24;
								else if (map2[bombI[i].y][bombI[i].x + 1].check == 5)
									map2[bombI[i].y][bombI[i].x + 1].check = 25;
								else if (map2[bombI[i].y][bombI[i].x + 1].check == 15)
									map2[bombI[i].y][bombI[i].x + 1].check = 0;
							}
						}
						if (chracter.power == 2){
							if (bombI[i].y != 0){
								if (map2[bombI[i].y - 1][bombI[i].x].check == 11 || map2[bombI[i].y - 1][bombI[i].x].check == 12)
									map2[bombI[i].y - 1][bombI[i].x].check = 0;
								if (map2[bombI[i].y - 2][bombI[i].x].check == 12)
									map2[bombI[i].y - 2][bombI[i].x].check = 0;
								if (bombI[i].y != 1){
									if ((map2[bombI[i].y - 1][bombI[i].x].check == 0) && (map2[bombI[i].y - 2][bombI[i].x].check == 2))
										map2[bombI[i].y - 2][bombI[i].x].check = 0;
									else if ((map2[bombI[i].y - 1][bombI[i].x].check == 0) && (map2[bombI[i].y - 2][bombI[i].x].check == 3))
										map2[bombI[i].y - 2][bombI[i].x].check = 23;
									else if ((map2[bombI[i].y - 1][bombI[i].x].check == 0) && (map2[bombI[i].y - 2][bombI[i].x].check == 4))
										map2[bombI[i].y - 2][bombI[i].x].check = 24;
									else if ((map2[bombI[i].y - 1][bombI[i].x].check == 0) && (map2[bombI[i].y - 2][bombI[i].x].check == 5))
										map2[bombI[i].y - 2][bombI[i].x].check = 25;
								}
								if (map2[bombI[i].y - 1][bombI[i].x].check == 2)
									map2[bombI[i].y - 1][bombI[i].x].check = 0;
								if (map2[bombI[i].y - 1][bombI[i].x].check == 3)
									map2[bombI[i].y - 1][bombI[i].x].check = 23;
								if (map2[bombI[i].y - 1][bombI[i].x].check == 4)
									map2[bombI[i].y - 1][bombI[i].x].check = 24;
								if (map2[bombI[i].y - 1][bombI[i].x].check == 5)
									map2[bombI[i].y - 1][bombI[i].x].check = 25;
							}
							if (bombI[i].y != 11){
								if (map2[bombI[i].y + 1][bombI[i].x].check == 11 || map2[bombI[i].y + 1][bombI[i].x].check == 13)
									map2[bombI[i].y + 1][bombI[i].x].check = 0;
								if (map2[bombI[i].y + 2][bombI[i].x].check == 13)
									map2[bombI[i].y + 2][bombI[i].x].check = 0;

								if (bombI[i].y != 10){
									if ((map2[bombI[i].y + 1][bombI[i].x].check == 0) && (map2[bombI[i].y + 2][bombI[i].x].check == 2))
										map2[bombI[i].y + 2][bombI[i].x].check = 0;
									if ((map2[bombI[i].y + 1][bombI[i].x].check == 0) && (map2[bombI[i].y + 2][bombI[i].x].check == 3))
										map2[bombI[i].y + 2][bombI[i].x].check = 23;
									if ((map2[bombI[i].y + 1][bombI[i].x].check == 0) && (map2[bombI[i].y + 2][bombI[i].x].check == 4))
										map2[bombI[i].y + 2][bombI[i].x].check = 24;
									if ((map2[bombI[i].y + 1][bombI[i].x].check == 0) && (map2[bombI[i].y + 2][bombI[i].x].check == 5))
										map2[bombI[i].y + 2][bombI[i].x].check = 25;
								}
								if (map2[bombI[i].y + 1][bombI[i].x].check == 2)
									map2[bombI[i].y + 1][bombI[i].x].check = 0;
								if (map2[bombI[i].y + 1][bombI[i].x].check == 3)
									map2[bombI[i].y + 1][bombI[i].x].check = 23;
								if (map2[bombI[i].y + 1][bombI[i].x].check == 4)
									map2[bombI[i].y + 1][bombI[i].x].check = 24;
								if (map2[bombI[i].y + 1][bombI[i].x].check == 5)
									map2[bombI[i].y + 1][bombI[i].x].check = 25;
							}
							if (bombI[i].x != 0){
								if (map2[bombI[i].y][bombI[i].x - 1].check == 11 || map2[bombI[i].y][bombI[i].x - 1].check == 14)
									map2[bombI[i].y][bombI[i].x - 1].check = 0;
								if (map2[bombI[i].y][bombI[i].x - 2].check == 14)
									map2[bombI[i].y][bombI[i].x - 2].check = 0;
								if (bombI[i].x != 1){
									if ((map2[bombI[i].y][bombI[i].x - 1].check == 0) && (map2[bombI[i].y][bombI[i].x - 2].check == 2))
										map2[bombI[i].y][bombI[i].x - 2].check = 0;
									if ((map2[bombI[i].y][bombI[i].x - 1].check == 0) && (map2[bombI[i].y][bombI[i].x - 2].check == 3))
										map2[bombI[i].y][bombI[i].x - 2].check = 23;
									if ((map2[bombI[i].y][bombI[i].x - 1].check == 0) && (map2[bombI[i].y][bombI[i].x - 2].check == 4))
										map2[bombI[i].y][bombI[i].x - 2].check = 24;
									if ((map2[bombI[i].y][bombI[i].x - 1].check == 0) && (map2[bombI[i].y][bombI[i].x - 2].check == 5))
										map2[bombI[i].y][bombI[i].x - 2].check = 25;
								}
								if (map2[bombI[i].y][bombI[i].x - 1].check == 2)
									map2[bombI[i].y][bombI[i].x - 1].check = 0;
								if (map2[bombI[i].y][bombI[i].x - 1].check == 3)
									map2[bombI[i].y][bombI[i].x - 1].check = 23;
								if (map2[bombI[i].y][bombI[i].x - 1].check == 4)
									map2[bombI[i].y][bombI[i].x - 1].check = 24;
								if (map2[bombI[i].y][bombI[i].x - 1].check == 5)
									map2[bombI[i].y][bombI[i].x - 1].check = 25;
							}
							if (bombI[i].x != 12){
								if (map2[bombI[i].y][bombI[i].x + 1].check == 11 || map2[bombI[i].y][bombI[i].x + 1].check == 15)
									map2[bombI[i].y][bombI[i].x + 1].check = 0;
								if (map2[bombI[i].y][bombI[i].x + 2].check == 15)
									map2[bombI[i].y][bombI[i].x + 2].check = 0;
								if (bombI[i].x != 11){
									if ((map2[bombI[i].y][bombI[i].x + 1].check == 0) && (map2[bombI[i].y][bombI[i].x + 2].check == 2))
										map2[bombI[i].y][bombI[i].x + 2].check = 0;
									if ((map2[bombI[i].y][bombI[i].x + 1].check == 0) && (map2[bombI[i].y][bombI[i].x + 2].check == 3))
										map2[bombI[i].y][bombI[i].x + 2].check = 23;
									if ((map2[bombI[i].y][bombI[i].x + 1].check == 0) && (map2[bombI[i].y][bombI[i].x + 2].check == 4))
										map2[bombI[i].y][bombI[i].x + 2].check = 24;
									if ((map2[bombI[i].y][bombI[i].x + 1].check == 0) && (map2[bombI[i].y][bombI[i].x + 2].check == 5))
										map2[bombI[i].y][bombI[i].x + 2].check = 25;
								}
								if (map2[bombI[i].y][bombI[i].x + 1].check == 2)
									map2[bombI[i].y][bombI[i].x + 1].check = 0;
								if (map2[bombI[i].y][bombI[i].x + 1].check == 3)
									map2[bombI[i].y][bombI[i].x + 1].check = 23;
								if (map2[bombI[i].y][bombI[i].x + 1].check == 4)
									map2[bombI[i].y][bombI[i].x + 1].check = 24;
								if (map2[bombI[i].y][bombI[i].x + 1].check == 5)
									map2[bombI[i].y][bombI[i].x + 1].check = 25;
							}
						}
						if (chracter.power == 3){
							if (bombI[i].y != 0){
								if (map2[bombI[i].y - 1][bombI[i].x].check == 11 || map2[bombI[i].y - 1][bombI[i].x].check == 12)
									map2[bombI[i].y - 1][bombI[i].x].check = 0;
								if (map2[bombI[i].y - 2][bombI[i].x].check == 11 || map2[bombI[i].y - 2][bombI[i].x].check == 12)
									map2[bombI[i].y - 2][bombI[i].x].check = 0;
								if (map2[bombI[i].y - 3][bombI[i].x].check == 12)
									map2[bombI[i].y - 3][bombI[i].x].check = 0;
								if (bombI[i].y != 2){
									if ((map2[bombI[i].y - 1][bombI[i].x].check == 0) && (map2[bombI[i].y - 2][bombI[i].x].check == 0) && (map2[bombI[i].y - 3][bombI[i].x].check == 2))
										map2[bombI[i].y - 3][bombI[i].x].check = 0;
									else if ((map2[bombI[i].y - 1][bombI[i].x].check == 0) && (map2[bombI[i].y - 2][bombI[i].x].check == 0) && (map2[bombI[i].y - 3][bombI[i].x].check == 3))
										map2[bombI[i].y - 3][bombI[i].x].check = 23;
									else if ((map2[bombI[i].y - 1][bombI[i].x].check == 0) && (map2[bombI[i].y - 2][bombI[i].x].check == 0) && (map2[bombI[i].y - 3][bombI[i].x].check == 4))
										map2[bombI[i].y - 3][bombI[i].x].check = 24;
									else if ((map2[bombI[i].y - 1][bombI[i].x].check == 0) && (map2[bombI[i].y - 2][bombI[i].x].check == 0) && (map2[bombI[i].y - 3][bombI[i].x].check == 5))
										map2[bombI[i].y - 3][bombI[i].x].check = 25;
								}
								if (bombI[i].y != 1){
									if ((map2[bombI[i].y - 1][bombI[i].x].check == 0) && (map2[bombI[i].y - 2][bombI[i].x].check == 2))
										map2[bombI[i].y - 2][bombI[i].x].check = 0;
									else if ((map2[bombI[i].y - 1][bombI[i].x].check == 0) && (map2[bombI[i].y - 2][bombI[i].x].check == 3))
										map2[bombI[i].y - 2][bombI[i].x].check = 23;
									else if ((map2[bombI[i].y - 1][bombI[i].x].check == 0) && (map2[bombI[i].y - 2][bombI[i].x].check == 4))
										map2[bombI[i].y - 2][bombI[i].x].check = 24;
									else if ((map2[bombI[i].y - 1][bombI[i].x].check == 0) && (map2[bombI[i].y - 2][bombI[i].x].check == 5))
										map2[bombI[i].y - 2][bombI[i].x].check = 25;
								}
								if (map2[bombI[i].y - 1][bombI[i].x].check == 2)
									map2[bombI[i].y - 1][bombI[i].x].check = 0;
								if (map2[bombI[i].y - 1][bombI[i].x].check == 3)
									map2[bombI[i].y - 1][bombI[i].x].check = 23;
								if (map2[bombI[i].y - 1][bombI[i].x].check == 4)
									map2[bombI[i].y - 1][bombI[i].x].check = 24;
								if (map2[bombI[i].y - 1][bombI[i].x].check == 5)
									map2[bombI[i].y - 1][bombI[i].x].check = 25;
							}
							if (bombI[i].y != 11){
								if (map2[bombI[i].y + 1][bombI[i].x].check == 11 || map2[bombI[i].y + 1][bombI[i].x].check == 11)
									map2[bombI[i].y + 1][bombI[i].x].check = 0;
								if (map2[bombI[i].y + 2][bombI[i].x].check == 11 || map2[bombI[i].y + 2][bombI[i].x].check == 11)
									map2[bombI[i].y + 2][bombI[i].x].check = 0;
								if (map2[bombI[i].y + 3][bombI[i].x].check == 13)
									map2[bombI[i].y + 3][bombI[i].x].check = 0;
								if (bombI[i].y != 10){
									if ((map2[bombI[i].y + 1][bombI[i].x].check == 0) && (map2[bombI[i].y + 2][bombI[i].x].check == 0) && (map2[bombI[i].y + 3][bombI[i].x].check == 2))
										map2[bombI[i].y + 3][bombI[i].x].check = 0;
									else if ((map2[bombI[i].y + 1][bombI[i].x].check == 0) && (map2[bombI[i].y + 2][bombI[i].x].check == 0) && (map2[bombI[i].y + 3][bombI[i].x].check == 3))
										map2[bombI[i].y + 3][bombI[i].x].check = 23;
									else if ((map2[bombI[i].y + 1][bombI[i].x].check == 0) && (map2[bombI[i].y + 2][bombI[i].x].check == 0) && (map2[bombI[i].y + 3][bombI[i].x].check == 4))
										map2[bombI[i].y + 3][bombI[i].x].check = 24;
									else if ((map2[bombI[i].y + 1][bombI[i].x].check == 0) && (map2[bombI[i].y + 2][bombI[i].x].check == 0) && (map2[bombI[i].y + 3][bombI[i].x].check == 5))
										map2[bombI[i].y + 3][bombI[i].x].check = 25;
								}

								if (bombI[i].y != 9){
									if ((map2[bombI[i].y + 1][bombI[i].x].check == 0) && (map2[bombI[i].y + 2][bombI[i].x].check == 2))
										map2[bombI[i].y + 2][bombI[i].x].check = 0;
									if ((map2[bombI[i].y + 1][bombI[i].x].check == 0) && (map2[bombI[i].y + 2][bombI[i].x].check == 3))
										map2[bombI[i].y + 2][bombI[i].x].check = 23;
									if ((map2[bombI[i].y + 1][bombI[i].x].check == 0) && (map2[bombI[i].y + 2][bombI[i].x].check == 4))
										map2[bombI[i].y + 2][bombI[i].x].check = 24;
									if ((map2[bombI[i].y + 1][bombI[i].x].check == 0) && (map2[bombI[i].y + 2][bombI[i].x].check == 5))
										map2[bombI[i].y + 2][bombI[i].x].check = 25;
								}
								if (map2[bombI[i].y + 1][bombI[i].x].check == 2)
									map2[bombI[i].y + 1][bombI[i].x].check = 0;
								if (map2[bombI[i].y + 1][bombI[i].x].check == 3)
									map2[bombI[i].y + 1][bombI[i].x].check = 23;
								if (map2[bombI[i].y + 1][bombI[i].x].check == 4)
									map2[bombI[i].y + 1][bombI[i].x].check = 24;
								if (map2[bombI[i].y + 1][bombI[i].x].check == 5)
									map2[bombI[i].y + 1][bombI[i].x].check = 25;
							}
							if (bombI[i].x != 0){
								if (map2[bombI[i].y][bombI[i].x - 1].check == 11 || map2[bombI[i].y][bombI[i].x - 1].check == 14)
									map2[bombI[i].y][bombI[i].x - 1].check = 0;
								if (map2[bombI[i].y][bombI[i].x - 2].check == 11 || map2[bombI[i].y][bombI[i].x - 2].check == 14)
									map2[bombI[i].y][bombI[i].x - 2].check = 0;
								if (map2[bombI[i].y][bombI[i].x - 3].check == 14)
									map2[bombI[i].y][bombI[i].x - 3].check = 0;
								if (bombI[i].x != 2){
									if ((map2[bombI[i].y][bombI[i].x - 1].check == 0) && (map2[bombI[i].y][bombI[i].x - 2].check == 0) && (map2[bombI[i].y][bombI[i].x - 3].check == 2))
										map2[bombI[i].y][bombI[i].x - 3].check = 0;
									else if ((map2[bombI[i].y][bombI[i].x - 1].check == 0) && (map2[bombI[i].y][bombI[i].x - 2].check == 0) && (map2[bombI[i].y][bombI[i].x - 3].check == 3))
										map2[bombI[i].y][bombI[i].x - 3].check = 23;
									else if ((map2[bombI[i].y][bombI[i].x - 1].check == 0) && (map2[bombI[i].y][bombI[i].x - 2].check == 0) && (map2[bombI[i].y][bombI[i].x - 3].check == 4))
										map2[bombI[i].y][bombI[i].x - 3].check = 24;
									else if ((map2[bombI[i].y][bombI[i].x - 1].check == 0) && (map2[bombI[i].y][bombI[i].x - 2].check == 0) && (map2[bombI[i].y][bombI[i].x - 3].check == 5))
										map2[bombI[i].y][bombI[i].x - 3].check = 25;
								}

								if (bombI[i].x != 1){
									if ((map2[bombI[i].y][bombI[i].x - 1].check == 0) && (map2[bombI[i].y][bombI[i].x - 2].check == 2))
										map2[bombI[i].y][bombI[i].x - 2].check = 0;
									if ((map2[bombI[i].y][bombI[i].x - 1].check == 0) && (map2[bombI[i].y][bombI[i].x - 2].check == 3))
										map2[bombI[i].y][bombI[i].x - 2].check = 23;
									if ((map2[bombI[i].y][bombI[i].x - 1].check == 0) && (map2[bombI[i].y][bombI[i].x - 2].check == 4))
										map2[bombI[i].y][bombI[i].x - 2].check = 24;
									if ((map2[bombI[i].y][bombI[i].x - 1].check == 0) && (map2[bombI[i].y][bombI[i].x - 2].check == 5))
										map2[bombI[i].y][bombI[i].x - 2].check = 25;
								}
								if (map2[bombI[i].y][bombI[i].x - 1].check == 2)
									map2[bombI[i].y][bombI[i].x - 1].check = 0;
								if (map2[bombI[i].y][bombI[i].x - 1].check == 3)
									map2[bombI[i].y][bombI[i].x - 1].check = 23;
								if (map2[bombI[i].y][bombI[i].x - 1].check == 4)
									map2[bombI[i].y][bombI[i].x - 1].check = 24;
								if (map2[bombI[i].y][bombI[i].x - 1].check == 5)
									map2[bombI[i].y][bombI[i].x - 1].check = 25;
							}
							if (bombI[i].x != 12){
								if (map2[bombI[i].y][bombI[i].x + 1].check == 11 || map2[bombI[i].y][bombI[i].x + 1].check == 11)
									map2[bombI[i].y][bombI[i].x + 1].check = 0;
								if (map2[bombI[i].y][bombI[i].x + 2].check == 11||map2[bombI[i].y][bombI[i].x + 2].check == 11)
									map2[bombI[i].y][bombI[i].x + 2].check = 0;
								if (map2[bombI[i].y][bombI[i].x + 3].check == 15)
									map2[bombI[i].y][bombI[i].x + 3].check = 0;
								if (bombI[i].x != 10){
									if ((map2[bombI[i].y][bombI[i].x + 1].check == 0) && (map2[bombI[i].y][bombI[i].x + 2].check == 0) && (map2[bombI[i].y][bombI[i].x + 3].check == 2))
										map2[bombI[i].y][bombI[i].x + 3].check = 0;
									else if ((map2[bombI[i].y][bombI[i].x + 1].check == 0) && (map2[bombI[i].y][bombI[i].x + 2].check == 0) && (map2[bombI[i].y][bombI[i].x + 3].check == 3))
										map2[bombI[i].y][bombI[i].x + 3].check = 23;
									else if ((map2[bombI[i].y][bombI[i].x + 1].check == 0) && (map2[bombI[i].y][bombI[i].x + 2].check == 0) && (map2[bombI[i].y][bombI[i].x + 3].check == 4))
										map2[bombI[i].y][bombI[i].x + 3].check = 24;
									else if ((map2[bombI[i].y][bombI[i].x + 1].check == 0) && (map2[bombI[i].y][bombI[i].x + 2].check == 0) && (map2[bombI[i].y][bombI[i].x + 3].check == 5))
										map2[bombI[i].y][bombI[i].x + 3].check = 25;
								}

								if (bombI[i].x != 11){
									if ((map2[bombI[i].y][bombI[i].x + 1].check == 0) && (map2[bombI[i].y][bombI[i].x + 2].check == 2))
										map2[bombI[i].y][bombI[i].x + 2].check = 0;
									if ((map2[bombI[i].y][bombI[i].x + 1].check == 0) && (map2[bombI[i].y][bombI[i].x + 2].check == 3))
										map2[bombI[i].y][bombI[i].x + 2].check = 23;
									if ((map2[bombI[i].y][bombI[i].x + 1].check == 0) && (map2[bombI[i].y][bombI[i].x + 2].check == 4))
										map2[bombI[i].y][bombI[i].x + 2].check = 24;
									if ((map2[bombI[i].y][bombI[i].x + 1].check == 0) && (map2[bombI[i].y][bombI[i].x + 2].check == 5))
										map2[bombI[i].y][bombI[i].x + 2].check = 25;
								}
								if (map2[bombI[i].y][bombI[i].x + 1].check == 2)
									map2[bombI[i].y][bombI[i].x + 1].check = 0;
								if (map2[bombI[i].y][bombI[i].x + 1].check == 3)
									map2[bombI[i].y][bombI[i].x + 1].check = 23;
								if (map2[bombI[i].y][bombI[i].x + 1].check == 4)
									map2[bombI[i].y][bombI[i].x + 1].check = 24;
								if (map2[bombI[i].y][bombI[i].x + 1].check == 5)
									map2[bombI[i].y][bombI[i].x + 1].check = 25;
							}
						}
						bombtime[i] = 0;
						if (chracter.bomb > 0){
							if (bombtime[0] == 0)
								numberbomb = 1;
							if (chracter.bomb > 1){
								if (bombtime[1] == 0 && bombtime[0] != 0)
									numberbomb = 2;
								if (chracter.bomb > 2){
									if (bombtime[1] != 0 && bombtime[0] != 0 && bombtime[2] == 0)
										numberbomb = 3;
									if (chracter.bomb > 3){
										if (bombtime[1] != 0 && bombtime[0] != 0 && bombtime[2] != 0 && bombtime[3] == 0)
											numberbomb = 4;
										if (chracter.bomb > 4){
											if (bombtime[1] != 0 && bombtime[0] != 0 && bombtime[2] != 0 && bombtime[3] != 0 && bombtime[4] == 0)
												numberbomb = 5;
										}
									}
								}
							}
						}

					}

				}
				break;
			}
			if (start == 4){
				bossbomb++;
				if (bossbomb == 6){
				
				rebomb:
					if (bombtime[3] == 0){
						bombI[3].x = rand() % 15;
						bombI[3].y = rand() % 9;

						if (map3[bombI[3].y][bombI[3].x].check != 0) goto rebomb;
						map3[bombI[3].y][bombI[3].x].check = 6;
						bossbomb = 0;
						bombtime[3] = 1;//임의의위치로.
					}
					else if (bombtime[4] == 0){
						bombI[4].x = rand() % 15;
						bombI[4].y = rand() % 9;

						if (map3[bombI[4].y][bombI[4].x].check != 0) goto rebomb;
						map3[bombI[4].y][bombI[4].x].check = 6;
						bossbomb = 0;
						bombtime[4] = 1;//임의의위치로.
					}
					else bossbomb = 0;
					
					
				}
				for (int i = 0; i < 5; i++){
				re4:
					if (bombtime[i]>0){
						bombtime[i]++;
					}


					if (bombtime[i] == 2){
						map3[bombI[i].y][bombI[i].x].check = 7;
					}

					if (bombtime[i] == 3){
						map3[bombI[i].y][bombI[i].x].check = 8;
					}
					if (bombtime[i] == 4){
						map3[bombI[i].y][bombI[i].x].check = 9;
					}

					if (bombtime[i] == 5){
						map3[bombI[i].y][bombI[i].x].check = 7;
					}
					if (bombtime[i] == 6){
						map3[bombI[i].y][bombI[i].x].check = 8;
					}
					if (bombtime[i] == 7){

						map3[bombI[i].y][bombI[i].x].check = 9;
					}
					if (bombtime[i] == 8){
						map3[bombI[i].y][bombI[i].x].check = 11;
						//PlaySound(NULL, 0, 0);
						PlaySound(MAKEINTRESOURCE(IDR_WAVE2), g_hInst, SND_RESOURCE | SND_ASYNC| SND_LOOP);
						if (chracter.power == 1){
							if (bombI[i].y != 0 && !(map3[bombI[i].y - 1][bombI[i].x].check > 0 && map3[bombI[i].y - 1][bombI[i].x].check < 6)){

								if (map3[bombI[i].y - 1][bombI[i].x].check > 5 && map3[bombI[i].y - 1][bombI[i].x].check < 10){
									for (int j = 0; j < chracter.bomb; j++){
										if (i != j)
											if (bombI[j].x == bombI[i].x&&bombI[j].y == bombI[i].y - 1){
												if (i < j)
													bombtime[j] = 7;
												if (i > j){
													bombtime[i] = 7;
													for (int k = j; k < i; k++)if (bombtime[k] == 8)
														bombtime[k] = 7;
													i = j;
													goto re4;

												}

											}

									}
								}
								if (map3[bombI[i].y - 1][bombI[i].x].check != 11)
									map3[bombI[i].y - 1][bombI[i].x].check = 12;
							}


							if (bombI[i].y != 8 && !(map3[bombI[i].y + 1][bombI[i].x].check > 0 && map3[bombI[i].y + 1][bombI[i].x].check < 6)){

								if (map3[bombI[i].y + 1][bombI[i].x].check > 5 && map3[bombI[i].y + 1][bombI[i].x].check < 10){
									for (int j = 0; j < chracter.bomb; j++){
										if (i != j){
											if (bombI[j].x == bombI[i].x&&bombI[j].y == bombI[i].y + 1){
												if (i < j)
													bombtime[j] = 7;
												if (i > j){
													bombtime[i] = 7;
													for (int k = j; k < i; k++)if (bombtime[k] == 8)
														bombtime[k] = 7;
													i = j;
													goto re4;

												}
											}

										}
									}
								}
								if (map3[bombI[i].y + 1][bombI[i].x].check != 11)
									map3[bombI[i].y + 1][bombI[i].x].check = 13;

							}

							if (bombI[i].x != 0 && !(map3[bombI[i].y][bombI[i].x - 1].check > 0 && map3[bombI[i].y][bombI[i].x - 1].check < 6)){

								if (map3[bombI[i].y][bombI[i].x - 1].check > 5 && map3[bombI[i].y][bombI[i].x - 1].check < 10){
									for (int j = 0; j < chracter.bomb; j++){
										if (i != j){
											if (bombI[j].x == bombI[i].x - 1 && bombI[j].y == bombI[i].y){
												if (i < j)
													bombtime[j] = 7;
												if (i > j){
													bombtime[i] = 7;
													for (int k = j; k < i; k++)if (bombtime[k] == 8)
														bombtime[k] = 7;
													i = j;
													goto re4;

												}
											}

										}
									}
								}
								if (map3[bombI[i].y][bombI[i].x - 1].check != 11)
									map3[bombI[i].y][bombI[i].x - 1].check = 14;
							}

							if (bombI[i].x != 14 && !(map3[bombI[i].y][bombI[i].x + 1].check > 0 && map3[bombI[i].y][bombI[i].x + 1].check < 6)){

								if (map3[bombI[i].y][bombI[i].x + 1].check > 5 && map3[bombI[i].y][bombI[i].x + 1].check < 10){
									for (int j = 0; j < chracter.bomb; j++){
										if (i != j){
											if (bombI[j].x == bombI[i].x + 1 && bombI[j].y == bombI[i].y){
												if (i < j)
													bombtime[j] = 7;
												if (i > j){
													bombtime[i] = 7;
													for (int k = j; k < i; k++)if (bombtime[k] == 8)
														bombtime[k] = 7;
													i = j;
													goto re4;

												}
											}

										}
									}
								}

								if (map3[bombI[i].y][bombI[i].x + 1].check != 11)
									map3[bombI[i].y][bombI[i].x + 1].check = 15;

							}


						}
						if (chracter.power == 2){

							if (bombI[i].y != 0 && !(map3[bombI[i].y - 1][bombI[i].x].check > 0 && map3[bombI[i].y - 1][bombI[i].x].check < 6)){

								if (map3[bombI[i].y - 1][bombI[i].x].check > 5 && map3[bombI[i].y - 1][bombI[i].x].check < 10){
									for (int j = 0; j < chracter.bomb; j++){
										if (i != j)
											if (bombI[j].x == bombI[i].x&&bombI[j].y == bombI[i].y - 1){
												if (i < j)
													bombtime[j] = 7;
												if (i > j){
													bombtime[i] = 7;
													for (int k = j; k < i; k++)if (bombtime[k] == 8)
														bombtime[k] = 7;
													i = j;
													goto re4;
												}
											}
									}
								}
								map3[bombI[i].y - 1][bombI[i].x].check = 11;
								if (bombI[i].y != 1 && !(map3[bombI[i].y - 2][bombI[i].x].check > 0 && map3[bombI[i].y - 2][bombI[i].x].check < 6)){

									if (map3[bombI[i].y - 2][bombI[i].x].check > 5 && map3[bombI[i].y - 2][bombI[i].x].check < 10){
										for (int j = 0; j < chracter.bomb; j++){
											if (i != j)
												if (bombI[j].x == bombI[i].x&&bombI[j].y == bombI[i].y - 2){
													if (i < j)
														bombtime[j] = 7;
													if (i > j){
														bombtime[i] = 7;
														for (int k = j; k < i; k++)if (bombtime[k] == 8)
															bombtime[k] = 7;
														i = j;
														goto re4;

													}

												}

										}
									}
									map3[bombI[i].y - 2][bombI[i].x].check = 12;
								}
							}

							if (bombI[i].y != 8 && !(map3[bombI[i].y + 1][bombI[i].x].check > 0 && map3[bombI[i].y + 1][bombI[i].x].check < 6)){
								if (map3[bombI[i].y + 1][bombI[i].x].check > 5 && map3[bombI[i].y + 1][bombI[i].x].check < 10){
									for (int j = 0; j < chracter.bomb; j++){
										if (i != j){
											if (bombI[j].x == bombI[i].x&&bombI[j].y == bombI[i].y + 1){
												if (i < j)
													bombtime[j] = 7;
												if (i > j){
													bombtime[i] = 7;
													for (int k = j; k < i; k++)if (bombtime[k] == 8)
														bombtime[k] = 7;
													i = j;
													goto re4;

												}
											}

										}
									}
								}
								map3[bombI[i].y + 1][bombI[i].x].check = 11;

								if (bombI[i].y != 7 && !(map3[bombI[i].y + 2][bombI[i].x].check > 0 && map3[bombI[i].y + 2][bombI[i].x].check < 6)){
									if (map3[bombI[i].y + 2][bombI[i].x].check > 5 && map3[bombI[i].y + 2][bombI[i].x].check < 10){
										for (int j = 0; j < chracter.bomb; j++){
											if (i != j){
												if (bombI[j].x == bombI[i].x&&bombI[j].y == bombI[i].y + 2){
													if (i < j)
														bombtime[j] = 7;
													if (i > j){
														bombtime[i] = 7;
														for (int k = j; k < i; k++)if (bombtime[k] == 8)
															bombtime[k] = 7;
														i = j;
														goto re4;

													}
												}

											}
										}
									}
									map3[bombI[i].y + 2][bombI[i].x].check = 13;
								}
							}

							if (bombI[i].x != 0 && !(map3[bombI[i].y][bombI[i].x - 1].check > 0 && map3[bombI[i].y][bombI[i].x - 1].check < 6)){
								if (map3[bombI[i].y][bombI[i].x - 1].check > 5 && map3[bombI[i].y][bombI[i].x - 1].check < 10){
									for (int j = 0; j < chracter.bomb; j++){
										if (i != j){
											if (bombI[j].x == bombI[i].x - 1 && bombI[j].y == bombI[i].y){
												if (i < j)
													bombtime[j] = 7;
												if (i > j){
													bombtime[i] = 7;
													for (int k = j; k < i; k++)if (bombtime[k] == 8)
														bombtime[k] = 7;
													i = j;
													goto re4;

												}
											}

										}
									}
								}
								map3[bombI[i].y][bombI[i].x - 1].check = 11;
								if (bombI[i].x != 1 && !(map3[bombI[i].y][bombI[i].x - 2].check > 0 && map3[bombI[i].y][bombI[i].x - 2].check < 6)){
									if (map3[bombI[i].y][bombI[i].x - 2].check > 5 && map3[bombI[i].y][bombI[i].x - 2].check < 10){
										for (int j = 0; j < chracter.bomb; j++){
											if (i != j){
												if (bombI[j].x == bombI[i].x - 2 && bombI[j].y == bombI[i].y){
													if (i < j)
														bombtime[j] = 7;
													if (i > j){
														bombtime[i] = 7;
														for (int k = j; k < i; k++)if (bombtime[k] == 8)
															bombtime[k] = 7;
														i = j;
														goto re4;

													}
												}

											}
										}
									}

									map3[bombI[i].y][bombI[i].x - 2].check = 14;
								}
							}

							if (bombI[i].x != 14 && !(map3[bombI[i].y][bombI[i].x + 1].check > 0 && map3[bombI[i].y][bombI[i].x + 1].check < 6)){
								if (map3[bombI[i].y][bombI[i].x + 1].check > 5 && map3[bombI[i].y][bombI[i].x + 1].check < 10){
									for (int j = 0; j < chracter.bomb; j++){
										if (i != j){
											if (bombI[j].x == bombI[i].x + 1 && bombI[j].y == bombI[i].y){
												if (i < j)
													bombtime[j] = 7;
												if (i > j){
													bombtime[i] = 7;
													for (int k = j; k < i; k++)if (bombtime[k] == 8)
														bombtime[k] = 7;
													i = j;
													goto re4;

												}
											}

										}
									}
								}
								map3[bombI[i].y][bombI[i].x + 1].check = 11;
								if (bombI[i].x != 13 && !(map3[bombI[i].y][bombI[i].x + 2].check > 0 && map3[bombI[i].y][bombI[i].x + 2].check < 6)){
									if (map3[bombI[i].y][bombI[i].x + 2].check > 5 && map3[bombI[i].y][bombI[i].x + 2].check < 10){
										for (int j = 0; j < chracter.bomb; j++){
											if (i != j){
												if (bombI[j].x == bombI[i].x + 2 && bombI[j].y == bombI[i].y){
													if (i < j)
														bombtime[j] = 7;
													if (i > j){
														bombtime[i] = 7;
														for (int k = j; k < i; k++)if (bombtime[k] == 8)
															bombtime[k] = 7;
														i = j;
														goto re4;

													}
												}

											}
										}
									}

									map3[bombI[i].y][bombI[i].x + 2].check = 15;
								}
							}
						}




						if (chracter.power == 3){
							if (bombI[i].y != 0 && !(map3[bombI[i].y - 1][bombI[i].x].check > 0 && map3[bombI[i].y - 1][bombI[i].x].check < 6)){
								if (map3[bombI[i].y - 1][bombI[i].x].check > 5 && map3[bombI[i].y - 1][bombI[i].x].check < 10){
									for (int j = 0; j < chracter.bomb; j++){
										if (i != j)
											if (bombI[j].x == bombI[i].x&&bombI[j].y == bombI[i].y - 1){
												if (i < j)
													bombtime[j] = 7;
												if (i > j){
													bombtime[i] = 7;
													for (int k = j; k < i; k++)if (bombtime[k] == 8)
														bombtime[k] = 7;
													i = j;
													goto re4;
												}
											}
									}
								}

								map3[bombI[i].y - 1][bombI[i].x].check = 11;
								if (bombI[i].y != 1 && !(map3[bombI[i].y - 2][bombI[i].x].check > 0 && map3[bombI[i].y - 2][bombI[i].x].check < 6)){
									if (map3[bombI[i].y - 2][bombI[i].x].check > 5 && map3[bombI[i].y - 2][bombI[i].x].check < 10){
										for (int j = 0; j < chracter.bomb; j++){
											if (i != j)
												if (bombI[j].x == bombI[i].x&&bombI[j].y == bombI[i].y - 2){
													if (i < j)
														bombtime[j] = 7;
													if (i > j){
														bombtime[i] = 7;
														for (int k = j; k < i; k++)if (bombtime[k] == 8)
															bombtime[k] = 7;
														i = j;
														goto re4;
													}
												}
										}
									}
									map3[bombI[i].y - 2][bombI[i].x].check = 11;

									if (bombI[i].y != 2 && !(map3[bombI[i].y - 3][bombI[i].x].check > 0 && map3[bombI[i].y - 3][bombI[i].x].check < 6)){
										if (map3[bombI[i].y - 3][bombI[i].x].check > 5 && map3[bombI[i].y - 3][bombI[i].x].check < 10){
											for (int j = 0; j < chracter.bomb; j++){
												if (i != j)
													if (bombI[j].x == bombI[i].x&&bombI[j].y == bombI[i].y - 3){
														if (i < j)
															bombtime[j] = 7;
														if (i > j){
															bombtime[i] = 7;
															for (int k = j; k < i; k++)if (bombtime[k] == 8)
																bombtime[k] = 7;
															i = j;
															goto re4;
														}
													}
											}
										}
										map3[bombI[i].y - 3][bombI[i].x].check = 12;
									}
								}
							}


							if (bombI[i].y != 8 && !(map3[bombI[i].y + 1][bombI[i].x].check > 0 && map3[bombI[i].y + 1][bombI[i].x].check < 6)){
								if (map3[bombI[i].y + 1][bombI[i].x].check > 5 && map3[bombI[i].y + 1][bombI[i].x].check < 10){
									for (int j = 0; j < chracter.bomb; j++){
										if (i != j){
											if (bombI[j].x == bombI[i].x&&bombI[j].y == bombI[i].y + 1){
												if (i < j)
													bombtime[j] = 7;
												if (i > j){
													bombtime[i] = 7;
													for (int k = j; k < i; k++)if (bombtime[k] == 8)
														bombtime[k] = 7;
													i = j;
													goto re4;

												}
											}

										}
									}
								}

								map3[bombI[i].y + 1][bombI[i].x].check = 11;

								if (bombI[i].y != 7 && !(map3[bombI[i].y + 2][bombI[i].x].check > 0 && map3[bombI[i].y + 2][bombI[i].x].check < 6)){
									if (map3[bombI[i].y + 2][bombI[i].x].check > 5 && map3[bombI[i].y + 2][bombI[i].x].check < 10){
										for (int j = 0; j < chracter.bomb; j++){
											if (i != j){
												if (bombI[j].x == bombI[i].x&&bombI[j].y == bombI[i].y + 2){
													if (i < j)
														bombtime[j] = 7;
													if (i > j){
														bombtime[i] = 7;
														for (int k = j; k < i; k++)if (bombtime[k] == 8)
															bombtime[k] = 7;
														i = j;
														goto re4;

													}
												}

											}
										}
									}

									map3[bombI[i].y + 2][bombI[i].x].check = 11;
									if (bombI[i].y != 6 && !(map3[bombI[i].y + 3][bombI[i].x].check > 0 && map3[bombI[i].y + 3][bombI[i].x].check < 6)){
										if (map3[bombI[i].y + 3][bombI[i].x].check > 5 && map3[bombI[i].y + 3][bombI[i].x].check < 10){
											for (int j = 0; j < chracter.bomb; j++){
												if (i != j){
													if (bombI[j].x == bombI[i].x&&bombI[j].y == bombI[i].y + 3){
														if (i < j)
															bombtime[j] = 7;
														if (i > j){
															bombtime[i] = 7;
															for (int k = j; k < i; k++)if (bombtime[k] == 8)
																bombtime[k] = 7;
															i = j;
															goto re4;

														}
													}

												}
											}
										}

										map3[bombI[i].y + 3][bombI[i].x].check = 13;
									}

								}
							}

							if (bombI[i].x != 0 && !(map3[bombI[i].y][bombI[i].x - 1].check > 0 && map3[bombI[i].y][bombI[i].x - 1].check < 6)){
								if (map3[bombI[i].y][bombI[i].x - 1].check > 5 && map3[bombI[i].y][bombI[i].x - 1].check < 10){
									for (int j = 0; j < chracter.bomb; j++){
										if (i != j){
											if (bombI[j].x == bombI[i].x - 1 && bombI[j].y == bombI[i].y){
												if (i < j)
													bombtime[j] = 7;
												if (i > j){
													bombtime[i] = 7;
													for (int k = j; k < i; k++)if (bombtime[k] == 8)
														bombtime[k] = 7;
													i = j;
													goto re4;

												}
											}

										}
									}
								}

								map3[bombI[i].y][bombI[i].x - 1].check = 11;
								if (bombI[i].x != 1 && !(map3[bombI[i].y][bombI[i].x - 2].check > 0 && map3[bombI[i].y][bombI[i].x - 2].check < 6)){
									if (map3[bombI[i].y][bombI[i].x - 2].check > 5 && map3[bombI[i].y][bombI[i].x - 2].check < 10){
										for (int j = 0; j < chracter.bomb; j++){
											if (i != j){
												if (bombI[j].x == bombI[i].x - 2 && bombI[j].y == bombI[i].y){
													if (i < j)
														bombtime[j] = 7;
													if (i > j){
														bombtime[i] = 7;
														for (int k = j; k < i; k++)if (bombtime[k] == 8)
															bombtime[k] = 7;
														i = j;
														goto re4;

													}
												}

											}
										}
									}


									map3[bombI[i].y][bombI[i].x - 2].check = 11;
									if (bombI[i].x != 2 && !(map3[bombI[i].y][bombI[i].x - 3].check > 0 && map3[bombI[i].y][bombI[i].x - 3].check < 6)){
										if (map3[bombI[i].y][bombI[i].x - 3].check > 5 && map3[bombI[i].y][bombI[i].x - 3].check < 10){
											for (int j = 0; j < chracter.bomb; j++){
												if (i != j){
													if (bombI[j].x == bombI[i].x - 3 && bombI[j].y == bombI[i].y){
														if (i < j)
															bombtime[j] = 7;
														if (i > j){
															bombtime[i] = 7;
															for (int k = j; k < i; k++)if (bombtime[k] == 8)
																bombtime[k] = 7;
															i = j;
															goto re4;

														}
													}

												}
											}
										}


										map3[bombI[i].y][bombI[i].x - 3].check = 14;
									}
								}

							}

							if (bombI[i].x != 14 && !(map3[bombI[i].y][bombI[i].x + 1].check > 0 && map3[bombI[i].y][bombI[i].x + 1].check < 6)){
								if (map3[bombI[i].y][bombI[i].x + 1].check > 5 && map3[bombI[i].y][bombI[i].x + 1].check < 10){
									for (int j = 0; j < chracter.bomb; j++){
										if (i != j){
											if (bombI[j].x == bombI[i].x + 1 && bombI[j].y == bombI[i].y){
												if (i < j)
													bombtime[j] = 7;
												if (i > j){
													bombtime[i] = 7;
													for (int k = j; k < i; k++)if (bombtime[k] == 8)
														bombtime[k] = 7;
													i = j;
													goto re4;

												}
											}

										}
									}
								}

								map3[bombI[i].y][bombI[i].x + 1].check = 11;
								if (bombI[i].x != 13 && !(map3[bombI[i].y][bombI[i].x + 2].check > 0 && map3[bombI[i].y][bombI[i].x + 2].check < 6)){
									if (map3[bombI[i].y][bombI[i].x + 2].check > 5 && map3[bombI[i].y][bombI[i].x + 2].check < 10){
										for (int j = 0; j < chracter.bomb; j++){
											if (i != j){
												if (bombI[j].x == bombI[i].x + 2 && bombI[j].y == bombI[i].y){
													if (i < j)
														bombtime[j] = 7;
													if (i > j){
														bombtime[i] = 7;
														for (int k = j; k < i; k++)if (bombtime[k] == 8)
															bombtime[k] = 7;
														i = j;
														goto re4;

													}
												}

											}
										}
									}
									map3[bombI[i].y][bombI[i].x + 2].check = 11;
									if (bombI[i].x != 12 && !(map3[bombI[i].y][bombI[i].x + 3].check > 0 && map3[bombI[i].y][bombI[i].x + 3].check < 6)){
										if (map3[bombI[i].y][bombI[i].x + 3].check > 5 && map3[bombI[i].y][bombI[i].x + 3].check < 10){
											for (int j = 0; j < chracter.bomb; j++){
												if (i != j){
													if (bombI[j].x == bombI[i].x + 3 && bombI[j].y == bombI[i].y){
														if (i < j)
															bombtime[j] = 7;
														if (i > j){
															bombtime[i] = 7;
															for (int k = j; k < i; k++)if (bombtime[k] == 8)
																bombtime[k] = 7;
															i = j;
															goto re4;

														}
													}

												}
											}
										}

										map3[bombI[i].y][bombI[i].x + 3].check = 15;
									}
								}
							}
						}
			


					}
					if (bombtime[i] == 9){
						if (map3[0][5].check != 30 || map3[1][5].check != 30 || map3[2][5].check != 30 ||
							map3[2][6].check != 30 || map3[2][7].check != 30 || map3[2][8].check != 30 ||
							map3[0][9].check != 30 || map3[1][9].check != 30 || map3[2][9].check != 30){
							bossheal++;
							if (bossheal >= 12){
								start = 5;
								PlaySound(MAKEINTRESOURCE(IDR_WAVE3), g_hInst, SND_RESOURCE | SND_ASYNC);

							}
						}
						for (int y = 0; y < 3; y++){
							for (int x = 5; x < 10; x++){
								map3[y][x].check = 30;
							}
						}


						map3[bombI[i].y][bombI[i].x].check = 0;


						if (chracter.power == 1){
							if (bombI[i].y != 0){
								if (map3[bombI[i].y - 1][bombI[i].x].check == 2)
									map3[bombI[i].y - 1][bombI[i].x].check = 0;
								else if (map3[bombI[i].y - 1][bombI[i].x].check == 3)
									map3[bombI[i].y - 1][bombI[i].x].check = 23;
								else if (map3[bombI[i].y - 1][bombI[i].x].check == 4)
									map3[bombI[i].y - 1][bombI[i].x].check = 24;
								else if (map3[bombI[i].y - 1][bombI[i].x].check == 5)
									map3[bombI[i].y - 1][bombI[i].x].check = 25;
								else if (map3[bombI[i].y - 1][bombI[i].x].check == 12)
									map3[bombI[i].y - 1][bombI[i].x].check = 0;

							}
							if (bombI[i].y != 8){
								if (map3[bombI[i].y + 1][bombI[i].x].check == 2)
									map3[bombI[i].y + 1][bombI[i].x].check = 0;
								else if (map3[bombI[i].y + 1][bombI[i].x].check == 3)
									map3[bombI[i].y + 1][bombI[i].x].check = 23;
								else if (map3[bombI[i].y + 1][bombI[i].x].check == 4)
									map3[bombI[i].y + 1][bombI[i].x].check = 24;
								else if (map3[bombI[i].y + 1][bombI[i].x].check == 5)
									map3[bombI[i].y + 1][bombI[i].x].check = 25;
								else if (map3[bombI[i].y + 1][bombI[i].x].check == 13)
									map3[bombI[i].y + 1][bombI[i].x].check = 0;
							}

							if (bombI[i].x != 0){
								if (map3[bombI[i].y][bombI[i].x - 1].check == 2)
									map3[bombI[i].y][bombI[i].x - 1].check = 0;

								else if (map3[bombI[i].y][bombI[i].x - 1].check == 3)
									map3[bombI[i].y][bombI[i].x - 1].check = 23;
								else if (map3[bombI[i].y][bombI[i].x - 1].check == 4)
									map3[bombI[i].y][bombI[i].x - 1].check = 24;
								else if (map3[bombI[i].y][bombI[i].x - 1].check == 5)
									map3[bombI[i].y][bombI[i].x - 1].check = 25;
								else if (map3[bombI[i].y][bombI[i].x - 1].check == 14)
									map3[bombI[i].y][bombI[i].x - 1].check = 0;
							}
							if (bombI[i].x != 14){
								if (map3[bombI[i].y][bombI[i].x + 1].check == 2)
									map3[bombI[i].y][bombI[i].x + 1].check = 0;
								else if (map3[bombI[i].y][bombI[i].x + 1].check == 3)
									map3[bombI[i].y][bombI[i].x + 1].check = 23;
								else if (map3[bombI[i].y][bombI[i].x + 1].check == 4)
									map3[bombI[i].y][bombI[i].x + 1].check = 24;
								else if (map3[bombI[i].y][bombI[i].x + 1].check == 5)
									map3[bombI[i].y][bombI[i].x + 1].check = 25;
								else if (map3[bombI[i].y][bombI[i].x + 1].check == 15)
									map3[bombI[i].y][bombI[i].x + 1].check = 0;
							}
						}
						if (chracter.power == 2){
							if (bombI[i].y != 0){
								if (map3[bombI[i].y - 1][bombI[i].x].check == 11 || map3[bombI[i].y - 1][bombI[i].x].check == 12)
									map3[bombI[i].y - 1][bombI[i].x].check = 0;
								if (map3[bombI[i].y - 2][bombI[i].x].check == 12)
									map3[bombI[i].y - 2][bombI[i].x].check = 0;
								if (bombI[i].y != 1){
									if ((map3[bombI[i].y - 1][bombI[i].x].check == 0) && (map3[bombI[i].y - 2][bombI[i].x].check == 2))
										map3[bombI[i].y - 2][bombI[i].x].check = 0;
									else if ((map3[bombI[i].y - 1][bombI[i].x].check == 0) && (map3[bombI[i].y - 2][bombI[i].x].check == 3))
										map3[bombI[i].y - 2][bombI[i].x].check = 23;
									else if ((map3[bombI[i].y - 1][bombI[i].x].check == 0) && (map3[bombI[i].y - 2][bombI[i].x].check == 4))
										map3[bombI[i].y - 2][bombI[i].x].check = 24;
									else if ((map3[bombI[i].y - 1][bombI[i].x].check == 0) && (map3[bombI[i].y - 2][bombI[i].x].check == 5))
										map3[bombI[i].y - 2][bombI[i].x].check = 25;
								}
								if (map3[bombI[i].y - 1][bombI[i].x].check == 2)
									map3[bombI[i].y - 1][bombI[i].x].check = 0;
								if (map3[bombI[i].y - 1][bombI[i].x].check == 3)
									map3[bombI[i].y - 1][bombI[i].x].check = 23;
								if (map3[bombI[i].y - 1][bombI[i].x].check == 4)
									map3[bombI[i].y - 1][bombI[i].x].check = 24;
								if (map3[bombI[i].y - 1][bombI[i].x].check == 5)
									map3[bombI[i].y - 1][bombI[i].x].check = 25;
							}
							if (bombI[i].y != 9){
								if (map3[bombI[i].y + 1][bombI[i].x].check == 11 || map3[bombI[i].y + 1][bombI[i].x].check == 13)
									map3[bombI[i].y + 1][bombI[i].x].check = 0;
								if (map3[bombI[i].y + 2][bombI[i].x].check == 13)
									map3[bombI[i].y + 2][bombI[i].x].check = 0;

								if (bombI[i].y != 8){
									if ((map3[bombI[i].y + 1][bombI[i].x].check == 0) && (map3[bombI[i].y + 2][bombI[i].x].check == 2))
										map3[bombI[i].y + 2][bombI[i].x].check = 0;
									if ((map3[bombI[i].y + 1][bombI[i].x].check == 0) && (map3[bombI[i].y + 2][bombI[i].x].check == 3))
										map3[bombI[i].y + 2][bombI[i].x].check = 23;
									if ((map3[bombI[i].y + 1][bombI[i].x].check == 0) && (map3[bombI[i].y + 2][bombI[i].x].check == 4))
										map3[bombI[i].y + 2][bombI[i].x].check = 24;
									if ((map3[bombI[i].y + 1][bombI[i].x].check == 0) && (map3[bombI[i].y + 2][bombI[i].x].check == 5))
										map3[bombI[i].y + 2][bombI[i].x].check = 25;
								}
								if (map3[bombI[i].y + 1][bombI[i].x].check == 2)
									map3[bombI[i].y + 1][bombI[i].x].check = 0;
								if (map3[bombI[i].y + 1][bombI[i].x].check == 3)
									map3[bombI[i].y + 1][bombI[i].x].check = 23;
								if (map3[bombI[i].y + 1][bombI[i].x].check == 4)
									map3[bombI[i].y + 1][bombI[i].x].check = 24;
								if (map3[bombI[i].y + 1][bombI[i].x].check == 5)
									map3[bombI[i].y + 1][bombI[i].x].check = 25;
							}
							if (bombI[i].x != 0){
								if (map3[bombI[i].y][bombI[i].x - 1].check == 11 || map3[bombI[i].y][bombI[i].x - 1].check == 14)
									map3[bombI[i].y][bombI[i].x - 1].check = 0;
								if (map3[bombI[i].y][bombI[i].x - 2].check == 14)
									map3[bombI[i].y][bombI[i].x - 2].check = 0;
								if (bombI[i].x != 1){
									if ((map3[bombI[i].y][bombI[i].x - 1].check == 0) && (map3[bombI[i].y][bombI[i].x - 2].check == 2))
										map3[bombI[i].y][bombI[i].x - 2].check = 0;
									if ((map3[bombI[i].y][bombI[i].x - 1].check == 0) && (map3[bombI[i].y][bombI[i].x - 2].check == 3))
										map3[bombI[i].y][bombI[i].x - 2].check = 23;
									if ((map3[bombI[i].y][bombI[i].x - 1].check == 0) && (map3[bombI[i].y][bombI[i].x - 2].check == 4))
										map3[bombI[i].y][bombI[i].x - 2].check = 24;
									if ((map3[bombI[i].y][bombI[i].x - 1].check == 0) && (map3[bombI[i].y][bombI[i].x - 2].check == 5))
										map3[bombI[i].y][bombI[i].x - 2].check = 25;
								}
								if (map3[bombI[i].y][bombI[i].x - 1].check == 2)
									map3[bombI[i].y][bombI[i].x - 1].check = 0;
								if (map3[bombI[i].y][bombI[i].x - 1].check == 3)
									map3[bombI[i].y][bombI[i].x - 1].check = 23;
								if (map3[bombI[i].y][bombI[i].x - 1].check == 4)
									map3[bombI[i].y][bombI[i].x - 1].check = 24;
								if (map3[bombI[i].y][bombI[i].x - 1].check == 5)
									map3[bombI[i].y][bombI[i].x - 1].check = 25;
							}
							if (bombI[i].x != 14){
								if (map3[bombI[i].y][bombI[i].x + 1].check == 11 || map3[bombI[i].y][bombI[i].x + 1].check == 15)
									map3[bombI[i].y][bombI[i].x + 1].check = 0;
								if (map3[bombI[i].y][bombI[i].x + 2].check == 15)
									map3[bombI[i].y][bombI[i].x + 2].check = 0;
								if (bombI[i].x != 13){
									if ((map3[bombI[i].y][bombI[i].x + 1].check == 0) && (map3[bombI[i].y][bombI[i].x + 2].check == 2))
										map3[bombI[i].y][bombI[i].x + 2].check = 0;
									if ((map3[bombI[i].y][bombI[i].x + 1].check == 0) && (map3[bombI[i].y][bombI[i].x + 2].check == 3))
										map3[bombI[i].y][bombI[i].x + 2].check = 23;
									if ((map3[bombI[i].y][bombI[i].x + 1].check == 0) && (map3[bombI[i].y][bombI[i].x + 2].check == 4))
										map3[bombI[i].y][bombI[i].x + 2].check = 24;
									if ((map3[bombI[i].y][bombI[i].x + 1].check == 0) && (map3[bombI[i].y][bombI[i].x + 2].check == 5))
										map3[bombI[i].y][bombI[i].x + 2].check = 25;
								}
								if (map3[bombI[i].y][bombI[i].x + 1].check == 2)
									map3[bombI[i].y][bombI[i].x + 1].check = 0;
								if (map3[bombI[i].y][bombI[i].x + 1].check == 3)
									map3[bombI[i].y][bombI[i].x + 1].check = 23;
								if (map3[bombI[i].y][bombI[i].x + 1].check == 4)
									map3[bombI[i].y][bombI[i].x + 1].check = 24;
								if (map3[bombI[i].y][bombI[i].x + 1].check == 5)
									map3[bombI[i].y][bombI[i].x + 1].check = 25;
							}
						}
						if (chracter.power == 3){
							if (bombI[i].y != 0){
								if (map3[bombI[i].y - 1][bombI[i].x].check == 11 || map3[bombI[i].y - 1][bombI[i].x].check == 12)
									map3[bombI[i].y - 1][bombI[i].x].check = 0;
								if (map3[bombI[i].y - 2][bombI[i].x].check == 11 || map3[bombI[i].y - 2][bombI[i].x].check == 12)
									map3[bombI[i].y - 2][bombI[i].x].check = 0;
								if (map3[bombI[i].y - 3][bombI[i].x].check == 12)
									map3[bombI[i].y - 3][bombI[i].x].check = 0;
								if (bombI[i].y != 2){
									if ((map3[bombI[i].y - 1][bombI[i].x].check == 0) && (map3[bombI[i].y - 2][bombI[i].x].check == 0) && (map3[bombI[i].y - 3][bombI[i].x].check == 2))
										map3[bombI[i].y - 3][bombI[i].x].check = 0;
									else if ((map3[bombI[i].y - 1][bombI[i].x].check == 0) && (map3[bombI[i].y - 2][bombI[i].x].check == 0) && (map3[bombI[i].y - 3][bombI[i].x].check == 3))
										map3[bombI[i].y - 3][bombI[i].x].check = 23;
									else if ((map3[bombI[i].y - 1][bombI[i].x].check == 0) && (map3[bombI[i].y - 2][bombI[i].x].check == 0) && (map3[bombI[i].y - 3][bombI[i].x].check == 4))
										map3[bombI[i].y - 3][bombI[i].x].check = 24;
									else if ((map3[bombI[i].y - 1][bombI[i].x].check == 0) && (map3[bombI[i].y - 2][bombI[i].x].check == 0) && (map3[bombI[i].y - 3][bombI[i].x].check == 5))
										map3[bombI[i].y - 3][bombI[i].x].check = 25;
								}
								if (bombI[i].y != 1){
									if ((map3[bombI[i].y - 1][bombI[i].x].check == 0) && (map3[bombI[i].y - 2][bombI[i].x].check == 2))
										map3[bombI[i].y - 2][bombI[i].x].check = 0;
									else if ((map3[bombI[i].y - 1][bombI[i].x].check == 0) && (map3[bombI[i].y - 2][bombI[i].x].check == 3))
										map3[bombI[i].y - 2][bombI[i].x].check = 23;
									else if ((map3[bombI[i].y - 1][bombI[i].x].check == 0) && (map3[bombI[i].y - 2][bombI[i].x].check == 4))
										map3[bombI[i].y - 2][bombI[i].x].check = 24;
									else if ((map3[bombI[i].y - 1][bombI[i].x].check == 0) && (map3[bombI[i].y - 2][bombI[i].x].check == 5))
										map3[bombI[i].y - 2][bombI[i].x].check = 25;
								}
								if (map3[bombI[i].y - 1][bombI[i].x].check == 2)
									map3[bombI[i].y - 1][bombI[i].x].check = 0;
								if (map3[bombI[i].y - 1][bombI[i].x].check == 3)
									map3[bombI[i].y - 1][bombI[i].x].check = 23;
								if (map3[bombI[i].y - 1][bombI[i].x].check == 4)
									map3[bombI[i].y - 1][bombI[i].x].check = 24;
								if (map3[bombI[i].y - 1][bombI[i].x].check == 5)
									map3[bombI[i].y - 1][bombI[i].x].check = 25;
							}
							if (bombI[i].y != 8){
								if (map3[bombI[i].y + 1][bombI[i].x].check == 11 || map3[bombI[i].y + 1][bombI[i].x].check == 13)
									map3[bombI[i].y + 1][bombI[i].x].check = 0;
								if (map3[bombI[i].y + 2][bombI[i].x].check == 11 || map3[bombI[i].y + 2][bombI[i].x].check == 13)
									map3[bombI[i].y + 2][bombI[i].x].check = 0;
								if (map3[bombI[i].y + 3][bombI[i].x].check == 13)
									map3[bombI[i].y + 3][bombI[i].x].check = 0;
								if (bombI[i].y !=6){
									if ((map3[bombI[i].y + 1][bombI[i].x].check == 0) && (map3[bombI[i].y + 2][bombI[i].x].check == 0) && (map3[bombI[i].y + 3][bombI[i].x].check == 2))
										map3[bombI[i].y + 3][bombI[i].x].check = 0;
									else if ((map3[bombI[i].y + 1][bombI[i].x].check == 0) && (map3[bombI[i].y + 2][bombI[i].x].check == 0) && (map3[bombI[i].y + 3][bombI[i].x].check == 3))
										map3[bombI[i].y + 3][bombI[i].x].check = 23;
									else if ((map3[bombI[i].y + 1][bombI[i].x].check == 0) && (map3[bombI[i].y + 2][bombI[i].x].check == 0) && (map3[bombI[i].y + 3][bombI[i].x].check == 4))
										map3[bombI[i].y + 3][bombI[i].x].check = 24;
									else if ((map3[bombI[i].y + 1][bombI[i].x].check == 0) && (map3[bombI[i].y + 2][bombI[i].x].check == 0) && (map3[bombI[i].y + 3][bombI[i].x].check == 5))
										map3[bombI[i].y + 3][bombI[i].x].check = 25;
								}

								if (bombI[i].y !=7){
									if ((map3[bombI[i].y + 1][bombI[i].x].check == 0) && (map3[bombI[i].y + 2][bombI[i].x].check == 2))
										map3[bombI[i].y + 2][bombI[i].x].check = 0;
									if ((map3[bombI[i].y + 1][bombI[i].x].check == 0) && (map3[bombI[i].y + 2][bombI[i].x].check == 3))
										map3[bombI[i].y + 2][bombI[i].x].check = 23;
									if ((map3[bombI[i].y + 1][bombI[i].x].check == 0) && (map3[bombI[i].y + 2][bombI[i].x].check == 4))
										map3[bombI[i].y + 2][bombI[i].x].check = 24;
									if ((map3[bombI[i].y + 1][bombI[i].x].check == 0) && (map3[bombI[i].y + 2][bombI[i].x].check == 5))
										map3[bombI[i].y + 2][bombI[i].x].check = 25;
								}
								if (map3[bombI[i].y + 1][bombI[i].x].check == 2)
									map3[bombI[i].y + 1][bombI[i].x].check = 0;
								if (map3[bombI[i].y + 1][bombI[i].x].check == 3)
									map3[bombI[i].y + 1][bombI[i].x].check = 23;
								if (map3[bombI[i].y + 1][bombI[i].x].check == 4)
									map3[bombI[i].y + 1][bombI[i].x].check = 24;
								if (map3[bombI[i].y + 1][bombI[i].x].check == 5)
									map3[bombI[i].y + 1][bombI[i].x].check = 25;
							}
							if (bombI[i].x != 0){
								if (map3[bombI[i].y][bombI[i].x - 1].check == 11 || map3[bombI[i].y][bombI[i].x - 1].check == 14)
									map3[bombI[i].y][bombI[i].x - 1].check = 0;
								if (map3[bombI[i].y][bombI[i].x - 2].check == 11 || map3[bombI[i].y][bombI[i].x - 2].check == 14)
									map3[bombI[i].y][bombI[i].x - 2].check = 0;
								if (map3[bombI[i].y][bombI[i].x - 3].check == 14)
									map3[bombI[i].y][bombI[i].x - 3].check = 0;
								if (bombI[i].x != 2){
									if ((map3[bombI[i].y][bombI[i].x - 1].check == 0) && (map3[bombI[i].y][bombI[i].x - 2].check == 0) && (map3[bombI[i].y][bombI[i].x - 3].check == 2))
										map3[bombI[i].y][bombI[i].x - 3].check = 0;
									else if ((map3[bombI[i].y][bombI[i].x - 1].check == 0) && (map3[bombI[i].y][bombI[i].x - 2].check == 0) && (map3[bombI[i].y][bombI[i].x - 3].check == 3))
										map3[bombI[i].y][bombI[i].x - 3].check = 23;
									else if ((map3[bombI[i].y][bombI[i].x - 1].check == 0) && (map3[bombI[i].y][bombI[i].x - 2].check == 0) && (map3[bombI[i].y][bombI[i].x - 3].check == 4))
										map3[bombI[i].y][bombI[i].x - 3].check = 24;
									else if ((map3[bombI[i].y][bombI[i].x - 1].check == 0) && (map3[bombI[i].y][bombI[i].x - 2].check == 0) && (map3[bombI[i].y][bombI[i].x - 3].check == 5))
										map3[bombI[i].y][bombI[i].x - 3].check = 25;
								}

								if (bombI[i].x != 1){
									if ((map3[bombI[i].y][bombI[i].x - 1].check == 0) && (map3[bombI[i].y][bombI[i].x - 2].check == 2))
										map3[bombI[i].y][bombI[i].x - 2].check = 0;
									if ((map3[bombI[i].y][bombI[i].x - 1].check == 0) && (map3[bombI[i].y][bombI[i].x - 2].check == 3))
										map3[bombI[i].y][bombI[i].x - 2].check = 23;
									if ((map3[bombI[i].y][bombI[i].x - 1].check == 0) && (map3[bombI[i].y][bombI[i].x - 2].check == 4))
										map3[bombI[i].y][bombI[i].x - 2].check = 24;
									if ((map3[bombI[i].y][bombI[i].x - 1].check == 0) && (map3[bombI[i].y][bombI[i].x - 2].check == 5))
										map3[bombI[i].y][bombI[i].x - 2].check = 25;
								}
								if (map3[bombI[i].y][bombI[i].x - 1].check == 2)
									map3[bombI[i].y][bombI[i].x - 1].check = 0;
								if (map3[bombI[i].y][bombI[i].x - 1].check == 3)
									map3[bombI[i].y][bombI[i].x - 1].check = 23;
								if (map3[bombI[i].y][bombI[i].x - 1].check == 4)
									map3[bombI[i].y][bombI[i].x - 1].check = 24;
								if (map3[bombI[i].y][bombI[i].x - 1].check == 5)
									map3[bombI[i].y][bombI[i].x - 1].check = 25;
							}
							if (bombI[i].x != 14){
								if (map3[bombI[i].y][bombI[i].x + 1].check == 11 || map3[bombI[i].y][bombI[i].x + 1].check == 15)
									map3[bombI[i].y][bombI[i].x + 1].check = 0;
								if (map3[bombI[i].y][bombI[i].x + 2].check == 11 || map3[bombI[i].y][bombI[i].x + 2].check == 15)
									map3[bombI[i].y][bombI[i].x + 2].check = 0;
								if (map3[bombI[i].y][bombI[i].x + 3].check == 15)
									map3[bombI[i].y][bombI[i].x + 3].check = 0;
								if (bombI[i].x != 12){
									if ((map3[bombI[i].y][bombI[i].x + 1].check == 0) && (map3[bombI[i].y][bombI[i].x + 2].check == 0) && (map3[bombI[i].y][bombI[i].x + 3].check == 2))
										map3[bombI[i].y][bombI[i].x + 3].check = 0;
									else if ((map3[bombI[i].y][bombI[i].x + 1].check == 0) && (map3[bombI[i].y][bombI[i].x + 2].check == 0) && (map3[bombI[i].y][bombI[i].x + 3].check == 3))
										map3[bombI[i].y][bombI[i].x + 3].check = 23;
									else if ((map3[bombI[i].y][bombI[i].x + 1].check == 0) && (map3[bombI[i].y][bombI[i].x + 2].check == 0) && (map3[bombI[i].y][bombI[i].x + 3].check == 4))
										map3[bombI[i].y][bombI[i].x + 3].check = 24;
									else if ((map3[bombI[i].y][bombI[i].x + 1].check == 0) && (map3[bombI[i].y][bombI[i].x + 2].check == 0) && (map3[bombI[i].y][bombI[i].x + 3].check == 5))
										map3[bombI[i].y][bombI[i].x + 3].check = 25;
								}

								if (bombI[i].x != 13){
									if ((map3[bombI[i].y][bombI[i].x + 1].check == 0) && (map3[bombI[i].y][bombI[i].x + 2].check == 2))
										map3[bombI[i].y][bombI[i].x + 2].check = 0;
									if ((map3[bombI[i].y][bombI[i].x + 1].check == 0) && (map3[bombI[i].y][bombI[i].x + 2].check == 3))
										map3[bombI[i].y][bombI[i].x + 2].check = 23;
									if ((map3[bombI[i].y][bombI[i].x + 1].check == 0) && (map3[bombI[i].y][bombI[i].x + 2].check == 4))
										map3[bombI[i].y][bombI[i].x + 2].check = 24;
									if ((map3[bombI[i].y][bombI[i].x + 1].check == 0) && (map3[bombI[i].y][bombI[i].x + 2].check == 5))
										map3[bombI[i].y][bombI[i].x + 2].check = 25;
								}
								if (map3[bombI[i].y][bombI[i].x + 1].check == 2)
									map3[bombI[i].y][bombI[i].x + 1].check = 0;
								if (map3[bombI[i].y][bombI[i].x + 1].check == 3)
									map3[bombI[i].y][bombI[i].x + 1].check = 23;
								if (map3[bombI[i].y][bombI[i].x + 1].check == 4)
									map3[bombI[i].y][bombI[i].x + 1].check = 24;
								if (map3[bombI[i].y][bombI[i].x + 1].check == 5)
									map3[bombI[i].y][bombI[i].x + 1].check = 25;
							}
						}
						bombtime[i] = 0;
						if (chracter.bomb > 0){
							if (bombtime[0] == 0)
								numberbomb = 1;
							if (chracter.bomb > 1){
								if (bombtime[1] == 0 && bombtime[0] != 0)
									numberbomb = 2;
								if (chracter.bomb > 2){
									if (bombtime[1] != 0 && bombtime[0] != 0 && bombtime[2] == 0)
										numberbomb = 3;
									if (chracter.bomb > 3){
										if (bombtime[1] != 0 && bombtime[0] != 0 && bombtime[2] != 0 && bombtime[3] == 0)
											numberbomb = 4;
										if (chracter.bomb > 4){
											if (bombtime[1] != 0 && bombtime[0] != 0 && bombtime[2] != 0 && bombtime[3] != 0 && bombtime[4] == 0)
												numberbomb = 5;
										}
									}
								}
							}
						}
					}

							
			

				}
				break;
			}

			break;


		case 7:
			deadcheck = 1;
			chracter.check = 5;
			animation++;
			if (animation == 5){
				start = 10;
			}
			break;


		case 8:
			if (start == 2){

				for (int i = 0; i < 3; i++){

				mon:
					if ((map[monster[i].ypos + 1][monster[i].xpos].check > 0 && map[monster[i].ypos + 1][monster[i].xpos].check < 10) &&
						(map[monster[i].ypos - 1][monster[i].xpos].check > 0 && map[monster[i].ypos - 1][monster[i].xpos].check < 10) &&
						(map[monster[i].ypos][monster[i].xpos - 1].check > 0 && map[monster[i].ypos][monster[i].xpos - 1].check < 10) &&
						(map[monster[i].ypos][monster[i].xpos + 1].check > 0 && map[monster[i].ypos][monster[i].xpos + 1].check <10)){
						i++;
						if (i == 3)
							goto end2;
					}
					if (monstermove[i] == 0) a[i] = rand() % 4;


					if (monster[i].check != 5){
						if (a[i] == 0 && monster[i].ypos != 8){
							if (!(map[monster[i].ypos + 1][monster[i].xpos].check > 0 && map[monster[i].ypos + 1][monster[i].xpos].check < 10)){
								monster[i].check = 0;
								monster[i].y += 0.0975*rectview.bottom / 4;
								monstermove[i]++;
								if (monstermove[i] == 4){
									monster[i].y = map[monster[i].ypos + 1][monster[i].xpos].y;
								//	map[monster[i].ypos][monster[i].xpos].check = 0;
									monster[i].ypos++;
								//	map[monster[i].ypos][monster[i].xpos].check = 31;
									monstermove[i] = 0;
								}


							}
							else {
								monster[i].y = map[monster[i].ypos][monster[i].xpos].y;
								a[i] = rand() % 4;
								goto mon;
							}
						}
						else if (a[i] == 1 && monster[i].ypos != 0){
							if (!(map[monster[i].ypos - 1][monster[i].xpos].check > 0 && map[monster[i].ypos - 1][monster[i].xpos].check < 10)){
								monster[i].check = 1;
								monster[i].y -= 0.0975*rectview.bottom / 4;
								monstermove[i]++;
								if (monstermove[i] == 4){
									monster[i].y = map[monster[i].ypos - 1][monster[i].xpos].y;
								//	map[monster[i].ypos][monster[i].xpos].check = 0;
									monster[i].ypos--;
								//	map[monster[i].ypos][monster[i].xpos].check = 32;
									monstermove[i] = 0;
								}


							}
							else {
								monstermove[i] = 0;
								monster[i].y = map[monster[i].ypos][monster[i].xpos].y;
								a[i] = rand() % 4;
								goto mon;
							}
						}
						else if (a[i] == 2 && monster[i].xpos != 0){
							if (!(map[monster[i].ypos][monster[i].xpos - 1].check > 0 && map[monster[i].ypos][monster[i].xpos - 1].check < 10)){
								monster[i].check = 2;
								monster[i].x -= 0.0661*rectview.right / 4;
								monstermove[i]++;
								if (monstermove[i] == 4){
									monster[i].x = map[monster[i].ypos][monster[i].xpos - 1].x;
								//	map[monster[i].ypos][monster[i].xpos].check = 0;
									monster[i].xpos--;
								//	map[monster[i].ypos][monster[i].xpos].check = 33;
									monstermove[i] = 0;
								}


							}
							else {
								monstermove[i] = 0;
								monster[i].x = map[monster[i].ypos][monster[i].xpos].x;
								a[i] = rand() % 4;
								goto mon;
							}
						}

						else if (a[i] == 3 && monster[i].xpos != 12){
							if (!(map[monster[i].ypos][monster[i].xpos + 1].check > 0 && map[monster[i].ypos][monster[i].xpos + 1].check < 10)){
								monster[i].check = 3;
								monster[i].x += 0.0661*rectview.right / 4;
								monstermove[i]++;
								if (monstermove[i] == 4){
									monster[i].x = map[monster[i].ypos][monster[i].xpos + 1].x;
									//map[monster[i].ypos][monster[i].xpos].check = 0;
									monster[i].xpos++;
								//	map[monster[i].ypos][monster[i].xpos].check = 34;
									monstermove[i] = 0;
								}


							}
							else {
								monstermove[i] = 0;
								monster[i].x = map[monster[i].ypos][monster[i].xpos].x;
								a[i] = rand() % 4;
								goto mon;
							}
						}
						
						
					}



				}
			}
			if (start == 3){

				for (int i = 0; i < 5; i++){

				mon2:
					if ((map2[monster[i].ypos + 1][monster[i].xpos].check > 0 && map2[monster[i].ypos + 1][monster[i].xpos].check < 10) &&
						(map2[monster[i].ypos - 1][monster[i].xpos].check > 0 && map2[monster[i].ypos - 1][monster[i].xpos].check <10) &&
						(map2[monster[i].ypos][monster[i].xpos - 1].check > 0 && map2[monster[i].ypos][monster[i].xpos - 1].check <10) &&
						(map2[monster[i].ypos][monster[i].xpos + 1].check > 0 && map2[monster[i].ypos][monster[i].xpos + 1].check <10)){
						i++;
						if (i == 5)
							goto end3;
					}
					if (monstermove[i] == 0) a[i] = rand() % 4;
					if (monster[i].check != 5){
						if (a[i] == 0 && monster[i].ypos != 10){
							if (!(map2[monster[i].ypos + 1][monster[i].xpos].check > 0 && map2[monster[i].ypos + 1][monster[i].xpos].check < 10)){
								monster[i].check = 0;
								monster[i].y += 0.0975*rectview.bottom / 4;
								monstermove[i]++;
								if (monstermove[i] == 4){
									monster[i].y = map2[monster[i].ypos + 1][monster[i].xpos].y;
								//	map2[monster[i].ypos][monster[i].xpos].check = 0;
									monster[i].ypos++;
								//	map2[monster[i].ypos][monster[i].xpos].check = 31;
									monstermove[i] = 0;
								}


							}
							else {
								monstermove[i] = 0;
								monster[i].y = map2[monster[i].ypos][monster[i].xpos].y;
								a[i] = rand() % 4;
								goto mon2;
							}
						}
						else if (a[i] == 1 && monster[i].ypos != 0){
							if (!(map2[monster[i].ypos - 1][monster[i].xpos].check > 0 && map2[monster[i].ypos - 1][monster[i].xpos].check < 10)){
								monster[i].check = 1;
								monster[i].y -= 0.0975*rectview.bottom / 4;
								monstermove[i]++;
								if (monstermove[i] == 4){
									monster[i].y = map2[monster[i].ypos - 1][monster[i].xpos].y;
								//	map2[monster[i].ypos][monster[i].xpos].check = 0;
									monster[i].ypos--;
								//	map2[monster[i].ypos][monster[i].xpos].check = 32;
									monstermove[i] = 0;
								}


							}
							else {
								monstermove[i] = 0;
								monster[i].y = map2[monster[i].ypos][monster[i].xpos].y;
								a[i] = rand() % 4;
								goto mon2; 
							}
						}
						else if (a[i] == 2 && monster[i].xpos != 0){
							if (!(map2[monster[i].ypos][monster[i].xpos - 1].check > 0 && map2[monster[i].ypos][monster[i].xpos - 1].check < 10)){
								monster[i].check = 2;
								monster[i].x -= 0.0661*rectview.right / 4;
								monstermove[i]++;
								if (monstermove[i] == 4){
									monster[i].x = map2[monster[i].ypos][monster[i].xpos - 1].x;
								//	map2[monster[i].ypos][monster[i].xpos].check = 0;
									monster[i].xpos--;
								//	map2[monster[i].ypos][monster[i].xpos].check = 33;
									monstermove[i] = 0;
								}


							}
							else {
								monstermove[i] = 0;
								monster[i].x = map2[monster[i].ypos][monster[i].xpos].x;
								a[i] = rand() % 4;
								goto mon2;
							}
						}

						else if (a[i] == 3 && monster[i].xpos != 12){
							if (!(map2[monster[i].ypos][monster[i].xpos + 1].check > 0 && map2[monster[i].ypos][monster[i].xpos + 1].check < 10)){
								monster[i].check = 3;
								monster[i].x += 0.0661*rectview.right / 4;
								monstermove[i]++;
								if (monstermove[i] == 4){
									monster[i].x = map2[monster[i].ypos][monster[i].xpos + 1].x;
								//	map2[monster[i].ypos][monster[i].xpos].check = 0;
									monster[i].xpos++;
									//map2[monster[i].ypos][monster[i].xpos].check = 34;
									monstermove[i] = 0;
								}


							}
							else{
								monstermove[i] = 0;
								monster[i].x = map2[monster[i].ypos][monster[i].xpos].x;
								a[i] = rand() % 4;
								goto mon2; 
							}

						}
						
						
					}



				}
			}



			break;

			case 9:

				if (start == 3){
					star[0] = 1;

				}
				if (start == 4){
					star[1] = 1;

				}

				break;


			case 10:
				time++;
				if (start==3||start==4)
				if (time == 300){
					SetTimer(hwnd, 7, 100, NULL);
				}

					break;

			case 11:
				ghosttime++;
					if (chracter.xpos >= ghost.xpos&&chracter.ypos > ghost.ypos){
						ghost.y += 0.0975*rectview.bottom / 4;
						if (ghosttime % 4 == 0){
							ghost.ypos++;
							ghost.y = map2[ghost.ypos][ghost.xpos].y;
						}

					}
					 if (chracter.xpos > ghost.xpos&&chracter.ypos <= ghost.ypos){
						ghost.x += 0.0661*rectview.right / 4;
						if (ghosttime % 4 == 0){
							ghost.xpos++;
							ghost.x = map2[ghost.ypos][ghost.xpos].x;
						}

					}
					 if (chracter.xpos <= ghost.xpos&&chracter.ypos < ghost.ypos){
						ghost.y -= 0.0661*rectview.right / 4;
						if (ghosttime % 4 == 0){
							ghost.ypos--;
							ghost.y = map2[ghost.ypos][ghost.xpos].y;
						}

					}
					 if (chracter.xpos < ghost.xpos&&chracter.ypos >= ghost.ypos){
						ghost.x -= 0.0661*rectview.right / 4;
						if (ghosttime % 4 == 0){
							ghost.xpos--;
							ghost.x = map2[ghost.ypos][ghost.xpos].x;
						}

					}
					 if (start == 3 && star[0] == 1 || start == 4 && star[1] == 1)
					 if (chracter.xpos == ghost.xpos&&chracter.ypos == ghost.ypos){
						SetTimer(hwnd, 7, 500, NULL);
						KillTimer(hwnd, 6);
						KillTimer(hwnd, 11);
						KillTimer(hwnd, 10);
					}
				

				

				
				break;

		}
	end2:
		end3:
		InvalidateRect(hwnd, FALSE, NULL);
		break;

	case WM_KEYDOWN:
		movecheck = 0;
		if (deadcheck == 0){
			switch (wParam){
				
			case VK_DOWN:
				if (chracter.check != 5 && moveup == 0 && timen == 0){
					SetTimer(hwnd, 1, speed, NULL);
					moveup++;
				}
				break;
			case VK_UP:
				if (chracter.check != 5 && moveup == 0&&timen == 0){
					SetTimer(hwnd, 2, speed, NULL);
					moveup++;
				}
				break;

			case VK_RIGHT:
				if (chracter.check != 5 && moveup == 0&&timen==0){
					SetTimer(hwnd, 3, speed, NULL);
					moveup++;
				}
				break;

			case VK_LEFT:
				if (chracter.check != 5 && moveup == 0 && timen == 0){
					SetTimer(hwnd, 4, speed, NULL);
					moveup++;
				}
				break;
				
			case 'T':
				if (start == 2){
					start = 3;
					animation = 0;
					timen = 0;
					movecheck = 0;
					chracter.xpos = 0;
					for (int i = 0; i < 5; i++)		bombtime[i] = 0;
					chracter.ypos = 0;
					chracter.bomb = 1;
					numberbomb = 1;
					chracter.power = 1;
					chracter.check = 0;
					chracter.x = map2[chracter.ypos][chracter.xpos].x;
					chracter.y = map2[chracter.ypos][chracter.xpos].y;
					map[0][0].check = 10;
					monster[0].xpos = 12;
					monster[0].ypos = 0;
					SetTimer(hwnd, 10, 1000, NULL);
					monster[0].check = 0;
					monster[1].xpos = 0;
					monster[1].ypos = 10;
					monster[1].check = 0;
					monster[2].xpos = 12;
					monster[2].ypos = 10;
					monster[2].check = 0;
					monster[3].xpos = 6;
					monster[3].ypos = 0;
					monster[3].check = 0;
					monster[4].xpos = 6;
					monster[4].ypos = 10;
					monster[4].check = 0;
					map2[0][0].check = 10;
					useOFF[0] = 1;
					useOFF[1] = 1;
					speed = 100;
					bossmove = 0;
					for (int a = 0; a < 5; a++){
						//	map2[monster[a].ypos][monster[a].xpos].check = 31;
						monster[a].x = map2[monster[a].ypos][monster[a].xpos].x;
						monster[a].y = map2[monster[a].ypos][monster[a].xpos].y;
						monstermove[a] = 0;
					}
					SetTimer(hwnd, 10, 1000, NULL);

				}
				else if (start == 3){
					start = 4;
					animation = 0;
					timen = 0;
					movecheck = 0;

					for (int i = 0; i < 5; i++)		bombtime[i] = 0;
					chracter.xpos = 4;
					chracter.ypos = 1;
					chracter.bomb = 5;
					numberbomb = 1;
					chracter.power = 1;
					chracter.check = 0;
					speed = 100;
					SetTimer(hwnd, 10, 1000, NULL);
					chracter.x = map3[chracter.ypos][chracter.xpos].x;
					chracter.y = map3[chracter.ypos][chracter.xpos].y;
					map3[chracter.ypos][chracter.xpos].check = 10;
					start = 4;

				}

				
				break;
			case 'Z':
				
				if (start == 3){
					star[0] = 0;
					useOFF[0] = 0;
				}
				if (start == 4){
					useOFF[1] = 0;
					star[1] = 0;
				}

				SetTimer(hwnd, 9, 7000, NULL);
				break;

			case VK_SPACE:
				if (start == 2){
					if ((map[chracter.ypos][chracter.xpos].check == 10 || map[chracter.ypos][chracter.xpos].check == 0) && numberbomb <= chracter.bomb && bombtime[numberbomb - 1] == 0){
						bombI[numberbomb - 1].x = chracter.xpos;
						bombI[numberbomb - 1].y = chracter.ypos;
						map[bombI[numberbomb - 1].y][bombI[numberbomb - 1].x].check = 6;
						bombtime[numberbomb - 1] = 1;
						numberbomb++;

						PlaySound(MAKEINTRESOURCE(IDR_WAVE1), g_hInst, SND_RESOURCE | SND_ASYNC);

					}
				}
				if (start == 3){
					if ((map2[chracter.ypos][chracter.xpos].check == 10 || map2[chracter.ypos][chracter.xpos].check == 0) && numberbomb <= chracter.bomb && bombtime[numberbomb - 1] == 0){
						bombI[numberbomb - 1].x = chracter.xpos;
						bombI[numberbomb - 1].y = chracter.ypos;
						map2[bombI[numberbomb - 1].y][bombI[numberbomb - 1].x].check = 6;
						bombtime[numberbomb - 1] = 1;
						numberbomb++;

						PlaySound(MAKEINTRESOURCE(IDR_WAVE1), g_hInst, SND_RESOURCE | SND_ASYNC);

					}

				}
				if (start == 4){
					if ((map3[chracter.ypos][chracter.xpos].check == 10 || map3[chracter.ypos][chracter.xpos].check == 0) && numberbomb <= 3 && bombtime[numberbomb - 1] == 0){
						bombI[numberbomb - 1].x = chracter.xpos;
						bombI[numberbomb - 1].y = chracter.ypos;
						map3[bombI[numberbomb - 1].y][bombI[numberbomb - 1].x].check = 6;
						bombtime[numberbomb - 1] = 1;
						numberbomb++;

						PlaySound(MAKEINTRESOURCE(IDR_WAVE1), g_hInst, SND_RESOURCE | SND_ASYNC);

					}

				}

			}
			break;
		}


		InvalidateRect(hwnd, FALSE, NULL);

		break;



		case WM_KEYUP:
			if (wParam == VK_LEFT || wParam == VK_RIGHT || wParam == VK_DOWN || wParam == VK_UP )
			moveup = 0;
			InvalidateRect(hwnd, FALSE, NULL);
			break;
	case WM_DESTROY:

		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hwnd, iMsg, wParam, lParam));
}

