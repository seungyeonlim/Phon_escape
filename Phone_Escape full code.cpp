#include<stdio.h>
#include<stdlib.h>
#include<windows.h>
#include<time.h>
#include<conio.h>
#include<mmsystem.h>
#pragma warning (disable:4996)
#pragma comment(lib,"winmm.lib")

#define SPACE 32
#define GBOARD_ORIGIN_X 8
#define GBOARD_ORIGIN_Y 4
#define DRAWX 5
#define DRAWY 2
#define XSize 35
#define YSize 15
#define ATTACK 97
#define P 112
#define START 115
#define RULE 114
#define EXIT 120
#define BACK 98
#define ABOUT 97

HANDLE hC = GetStdHandle(STD_OUTPUT_HANDLE);
COORD PC_pos = { 10,14 }, PC_rightpos = { 12, 14 }; //rightpos는 NPC랑 충돌비교를 위한 pc의 맨 오른쪽 좌표임
COORD Life_pos;
COORD NPC_pos = { 0,11 };
COORD NPC_Attackpos1 = { 60,15 };
COORD PC_Attackpos_MAX = { 10 + 12 ,15 };
COORD PC_Attackpos = { 10,15 };
COORD PC_Attackpos_origin = PC_rightpos;
COORD text_pos = { 90, 5 };


int id = 1;
int speed = 200;
int NPC_randnum = 1;
int cnt = 0; // 현재 스테이지 스타트 위치
int life = 3;
int isJump = 0; //현재 점프를 하고 있는지 아닌지
int jumped = 0;
int GBoardInfo[15][35] = { { 0 } };
int NPCAttack_rand = 0;
int isAttack = 0;
int attacked = 0;
int NPCAttack_count = 0;
void NPC_Attack();
void DrawEnding();
int makeNPCAttack2 = 0;
int PCAttack_rand = 0;
int PCAttack_count = 0;
int NPCAttack1_die = 0;
void DrawLife();
void DetectCollisionPC_NPCAttack();
int bullet = 0; // 총알 개수
int arrow = 0; // 화살 개수
int NPC_faceborn = 0, NPC_Instaborn = 0, NPC_Youtubeborn = 0;
int NPC_facedead = 0, NPC_Instadead, NPC_Youtubedead = 0;
int NPC_Attack1born = 0, NPC_Attack2born = 0;
int NPCattacktime = 3;
int issuper = 0, supercnt = 0;
int npccnt = 0, n = 0;
int stage_num = 1;
void menu();
void menuKeyInput();
void game_over();
void DrawPC();

int PC[4][3][3] = {
	{
		{ 100,0,0 },
{ 101,106,104 },
{ 102,0,0 },
	},
   {
	   { 100,104,0 },
{ 101,105,0 },
{ 102,0,0 }
   },
   {
	   { 100,0,0 },
{ 101,103,0 },
{ 102,0,0 }
   },
   {
	   { 100,0,0 },
{ 101,107,0 },
{ 102,0,0 }
   }
};

int NPC[3][3][3] = {
	{
		{ -2,60,60 },
{ 60,10,60 },
{ 60,60,65 }
	},
   {
	   { -2,60,60 },
{ 60,11,60 },
{ 60,60,65 }
   },
   {
	   { -3,-1,-1 },
{ -1,12,-1 },
{ -1,-1,66 }
   }
};

int stage[15][450] = { { 0 } };

void stair_1f(int x) // 1단 계단 생성
{
	stage[13][x] = 1;
	stage[13][x + 1] = 1;

	return;
}

void stair_2f(int x) // 2단 계단 생성
{
	stage[13][x] = 1;
	stage[13][x + 1] = 1;
	stage[12][x] = 1;
	stage[12][x + 1] = 1;

	return;
}

void tack(int x, int y) // 압정
{
	stage[y][x] = 4;
}

void in_triangle(int x) // 역삼각형
{
	stage[10][x] = 5;
}

void triangle(int x) // 삼각형
{
	stage[12][x] = 6;
}

void gap(int x) { // 구덩이
	stage[14][x] = 7;
}

void flower(int x, int y) { // 식인식물
	stage[y][x] = 8;
}

void gun(int x, int y) { // 무기 총
	stage[y][x] = 21;
}

void arro(int x, int y) { // 무기 활
	stage[y][x] = 22;
}

void pan(int x, int y) { // 무기 후라이팬
	stage[y][x] = 23;
}

void bull(int x, int y) { // 총알
	stage[y][x] = 24;
}

void arr_bull(int x, int y) { // 화살
	stage[y][x] = 25;
}

void super(int x, int y) { // 무적
	stage[y][x] = 20;
}

void NPCfacebook(int x)
{
	stage[11][x] = -3;
	stage[11][x + 1] = -1;
	stage[11][x + 2] = -1;
	stage[12][x] = -1;
	stage[12][x + 1] = 12;
	stage[12][x + 2] = -1;
	stage[13][x] = -1;
	stage[13][x + 1] = -1;
	stage[13][x + 2] = 66;
}

void NPCYoutube(int x)
{
	stage[11][x] = -4;
	stage[11][x + 1] = 61;
	stage[11][x + 2] = 61;
	stage[12][x] = 61;
	stage[12][x + 1] = 11;
	stage[12][x + 2] = 61;
	stage[13][x] = 61;
	stage[13][x + 1] = 61;
	stage[13][x + 2] = 65;
}

void NPCInstagram(int x)
{
	stage[11][x] = -2;
	stage[11][x + 1] = 60;
	stage[11][x + 2] = 60;
	stage[12][x] = 60;
	stage[12][x + 1] = 10;
	stage[12][x + 2] = 60;
	stage[13][x] = 60;
	stage[13][x + 1] = 60;
	stage[13][x + 2] = 64;
}

void setStage() {
   stage_num = 1;
   int location = 15;

   for (int i = 0; i < 15; i++) {
      for (int j = 0; j < 400; j++) {
         stage[i][j] = 0;
      }
   }
   for (int i = 0; i < 400; i++) {
      stage[14][i] = 1;
   }

   location = 15;

   location += 3;
   gun(location, 13); // 아이테에에에에에에ㅔㅁ
   location += 5;
   stair_1f(location);
   location += 2;
   stair_2f(location);
   location += 7;
   tack(location, 13);
   location += 6;
   tack(location, 13);
   location += 2;
   super(location, 13);
   location += 3;
   location += 5;
   in_triangle(location);
   location += 4;
   triangle(location);
   location += 5;
   stair_1f(location);
   location += 1;
   pan(location, 12);
   location += 1;
   stair_1f(location);
   tack(location, 12);
   location += 7;
   gap(location);
   location += 6;
   NPCInstagram(location); // 엔피씨이이이이이ㅣㅇ
   location += 4;
   pan(location, 12); // 아이테에에에에에메
   location += 6;
   gap(location);
   location += 1;
   stair_1f(location);
   location += 2;
   gap(location);
   location += 1;
   stair_1f(location);
   location += 2;
   stair_2f(location);
   location += 2;
   stair_1f(location);
   tack(location, 12);
   location += 7;
   gap(location);
   location += 5;
   triangle(location);
   location += 6;
   triangle(location);
   location += 6;
   in_triangle(location);
   location += 6;
   tack(location, 13);
   location += 5;

   flower(location, 13);
   location += 5;
   stair_1f(location);
   location += 2;
   triangle(location);
   location += 1;
   stair_1f(location);
   location += 4;
   arro(location, 11); // 아이템
   location += 5;
   gap(location);
   location += 5;
   in_triangle(location);
   location += 4;
   in_triangle(location);
   location += 4;
   gap(location);
   location += 6;
   stair_1f(location);
   location += 5;
   flower(location, 13);
   location += 5;
   stair_1f(location);
   location += 4;
   super(location, 13);
   location += 6;
   NPCYoutube(location); // 엔피씨
   location += 4;
   pan(location, 13); // 아이테ㅁ
   location += 5;
   stair_1f(location);
   location += 2;
   stair_2f(location);
   location += 1;
   super(location, 11);
   location += 2;
   stair_1f(location);
   location += 2;
   stair_2f(location);
   location += 5;
   triangle(location);
   location += 5;
   stair_1f(location);
   location += 1;
   flower(location, 12);
   location += 9;
   NPCfacebook(location); // 엔피씨
   location += 9;

   //227
}

void setStage2() {
   stage_num = 2;
   int location = 15;

   for (int i = 0; i < 15; i++) {
      for (int j = 0; j < 400; j++) {
         stage[i][j] = 0;
      }
   }
   for (int i = 0; i < 400; i++) {
      stage[14][i] = 1;
   }

   location = 15; 

   stair_1f(location);
   location += 2; 
   stair_2f(location);
   location += 6; 
   triangle(location);
   location += 4; 
   stair_1f(location);
   location += 2; 
   stair_1f(location);
   location += 2;
   triangle(location);
   location += 1; 
   stair_1f(location);
   location += 6; 
   tack(location, 13);
   location += 6; 
   NPCInstagram(location); 
   location += 4;
   gap(location);
   location += 4; 
   tack(location, 13);
   location += 5; 
   tack(location, 13);
   location += 1; 
   stair_1f(location);
   location += 2;
   stair_1f(location);
   pan(location, 12);
   location += 2; 
   stair_2f(location);
   location += 2; 
   stair_2f(location);
   location += 2; 
   stair_1f(location);
   tack(location, 12);
   location += 7;
   tack(location, 13);
   location += 5;
   triangle(location);
   location += 5;
   in_triangle(location);
   location += 2;
   super(location, 13);
   location += 2;
   triangle(location);
   location += 4;
   gap(location);
   location += 7;
   NPCYoutube(location); 
   location += 4; 
   gap(location);
   location += 4;
   tack(location, 13);
   location += 4;
   in_triangle(location);
   location += 5;
   triangle(location);
   location += 5;
   stair_1f(location);
   location += 2;
   stair_2f(location);
   location += 2;
   stair_2f(location);
   location += 1;
   tack(location, 11);
   location += 1; 
   stair_1f(location);
   location += 2; 
   gap(location);
   location += 1;
   stair_2f(location);
   location += 3;
   gun(location, 13);
   location += 5;
   flower(location, 13);
   location += 1; 
   stair_1f(location);
   location += 2;
   stair_1f(location);
   location += 2;
   flower(location, 13);
   location += 4; 
   flower(location, 13);
   location += 2; 
   super(location, 13);
   location += 3; 
   gap(location);
   location += 4; 
   tack(location, 13);
   location += 4; 
   stair_1f(location);
   location += 2;
   tack(location, 13);
   location += 7; 
   NPCYoutube(location); 
   location += 2;
   stair_1f(location);
   location += 3; 
   stair_2f(location);
   location += 3;
   stair_2f(location);
   location += 3;
   in_triangle(location);
   location += 4;
   triangle(location);
   arro(location, 11);
   location += 4;
   in_triangle(location);
   location += 5; 
   gap(location);
   location += 1; 
   stair_1f(location);
   location += 4;
   flower(location, 13);
   location += 4;
   stair_1f(location);
   location += 2;

   stair_1f(location);
   pan(location, 10);
   location += 2; 
   triangle(location);
   location += 1;
   stair_1f(location);
   location += 3;
   gap(location);
   location += 1;
   stair_1f(location);
   location += 2;
   stair_1f(location);
   gun(location, 12);
   location += 1;

   flower(location, 12);
   location += 6; 
   NPCYoutube(location); 
   location += 5; 
   gap(location);
   location += 3; 
   gap(location);
   location += 2; 
   super(location, 10);
   location += 2; 
   gap(location);
   location += 2; 
   tack(location, 13);
   location += 1; 
   gap(location);
   location += 3; 
   stair_1f(location);
   location += 2;
   gap(location);
   location += 1; 
   stair_2f(location);
   location += 5; 
   tack(location, 13);
   location += 2;
   gun(location, 10);
   location += 3; 
   tack(location, 13);
   location += 4;
   in_triangle(location);
   location += 5; 
   tack(location, 13);
   location += 1; 
   pan(location, 13);
   location += 4;
   stair_1f(location);
   location += 2; 
   stair_2f(location);
   location += 2;
   stair_2f(location);
   location += 2;
   tack(location, 13);
   location += 7; 
   NPCInstagram(location);
   location += 4; 
   gap(location);
   location += 4;
   in_triangle(location);
   location += 4; 
   triangle(location);
   location += 5;
   stair_1f(location);
   location += 2; 
   triangle(location);
   location += 1; 
   stair_1f(location);
}
void setStage3() {
   stage_num = 3;
   int location = 15;

   for (int i = 0; i < 15; i++) {
      for (int j = 0; j < 500; j++) {
         stage[i][j] = 0;
      }
   }
   for (int i = 0; i < 500; i++) {
      stage[14][i] = 1;
   }

   location = 15; // 15
   stair_1f(location);
   location += 2; // 17
   gap(location);
   location += 1; // 18
   stair_1f(location);
   location += 2; // 20
   stair_1f(location);
   location += 2;
   tack(location, 13);
   location += 5; // 23
   tack(location, 13);
   location += 5; // 27
   in_triangle(location);
   location += 5; // 30
   stair_1f(location);
   location += 2;
   stair_1f(location);
   location += 1; // 31
   tack(location, 12);
   location += 6; // 35
   flower(location, 13);
   location += 4; // 38
   stair_1f(location);
   location += 2; // 40
   triangle(location);
   location += 1; // 41
   stair_1f(location);
   location += 2; // 43
   gap(location);
   location += 1; // 44
   stair_2f(location);
   location += 3; // 47
   triangle(location);
   location += 5; // 50
   in_triangle(location);
   location += 5; // 53
   triangle(location);
   location += 3; // 56
   gap(location);
   location += 4; // 60
   flower(location, 13);
   location += 4; // 63
   in_triangle(location);
   location += 4; // 69
   flower(location, 13);

   location += 5; // 74
   stair_1f(location);
   location += 2; // 76
   gap(location);
   location += 1; // 77
   stair_1f(location);
   location += 2; // 79
   gap(location);
   location += 1; // 80
   stair_2f(location);
   location += 2; // 82
   gap(location);
   location += 1; // 83
   gap(location);
   location += 1; // 84
   stair_2f(location);
   location += 7; // 91
   NPCInstagram(location); // 엔피씨이이이이이이잉
   location += 5; // 96
   stair_1f(location);
   location += 5; // 99
   in_triangle(location);
   arro(location, 13); // 아이테에에에에에엠
   location += 6; // 100
   stair_1f(location);
   location += 2; // 102
   gap(location);
   location += 5; // 107
   flower(location, 13);
   location += 3; // 110
   stair_1f(location);
   location += 3; // 113
   in_triangle(location);
   location += 6; // 114
   stair_1f(location);
   location += 2; // 116
   stair_1f(location);
   location += 2;
   tack(location, 13);
   location += 5; // 119
   tack(location, 13);
   location += 5; // 122
   tack(location, 13);
   location += 5; // 125
   tack(location, 13);
   location += 5; // 128
   in_triangle(location);
   location += 5; // 131
   tack(location, 13);

   location += 6; // 136
   stair_1f(location);
   location += 2; // 138
   stair_1f(location);
   flower(location, 12);
   location += 2; // 140
   stair_2f(location);
   location += 2; // 143
   gun(location, 10); // 아이테에에에에에엠
   gap(location);
   location += 1; // 144
   stair_2f(location);
   flower(location, 11);
   location += 2; // 148
   stair_1f(location);
   location += 7; // 154
   NPCfacebook(location); // 엔피씨이이이이이이잉
   location += 1; // 155
   gap(location);
   location += 1; // 156
   gap(location);
   location += 6; // 159
   in_triangle(location);
   location += 5; // 162
   triangle(location);
   location += 5; // 165
   in_triangle(location);
   location += 5; // 168
   flower(location, 13);
   location += 5; // 171
   in_triangle(location);
   location += 5; // 174
   tack(location, 13);
   location += 5; // 177
   in_triangle(location);
   location += 5; // 179
   super(location, 13);
   location += 3;
   stair_1f(location);
   location += 2; // 181
   stair_2f(location);
   location += 2; // 183
   in_triangle(location);
   gap(location);
   location += 1;
   stair_2f(location);
   location += 7; // 188
   gap(location);
   location += 2; // 190
   pan(location, 12); // 아이테에에에에에엠
   location += 5; // 195
   //super(location, 13);
   location += 2;
   stair_1f(location);
   location += 2; // 197
   stair_2f(location);
   location += 2; // 199
   in_triangle(location);
   gap(location);
   location += 1; // 201
   stair_2f(location);
   location += 2; // 203
   flower(location, 13);
   location += 7; // 210
   NPCYoutube(location); // 엔피씨이이이이이이잉
   location += 3; // 213
   triangle(location);
   location += 4; // 216
   triangle(location);
   location += 1; // 217
   stair_1f(location);
   location += 5; // 220
   tack(location, 13);
   location += 5; // 224
   flower(location, 13);
   location += 5; // 227
   in_triangle(location);
   location += 5; // 230
   stair_1f(location);
   location += 2; // 232
   //super(location, 13); // 아이테에에엠
   gap(location);
   location += 1; // 233
   stair_1f(location);
   location += 2; // 235
   gap(location);
   location += 1;
   stair_2f(location);
   location += 1; // 239
   tack(location, 11);
   location += 5;// 243
   in_triangle(location);
   location += 5; // 245
   stair_1f(location);
   location += 2; // 247
   stair_1f(location);
   location += 2; // 249
   gap(location);
   location += 1; // 250
   gap(location);
   location += 1; // 251
   stair_2f(location);
   location += 7; // 258
   NPCfacebook(location); // 엔피씨이이이이잉
   location += 5; // 261
   in_triangle(location);
   location += 4; // 263
   gap(location);
   location += 4; // 265
   triangle(location);
   location += 4; // 267
   gap(location);
   location += 3; // 270
   tack(location, 13);
   location += 4; // 272
   tack(location, 13);
   location += 3; // 275
   gap(location);
   location += 3; // 278
   flower(location, 13);
   location += 4; // 280
   flower(location, 13);
   location += 3; // 283
   gap(location);
   location += 4; // 287
   flower(location, 13);
   location += 4; // 290
   tack(location, 13);
   location += 4; // 294
   flower(location, 13);
   location += 3; // 297
   in_triangle(location);
   location += 5; // 299
   tack(location, 13);
   location += 4; // 301
   in_triangle(location);
   location += 5; // 304
   stair_1f(location);
   location += 5; // 306
   tack(location, 13);
   location += 4; // 310

   stair_1f(location);
   location += 2; // 312
   stair_2f(location);
   location += 2; // 315
   stair_1f(location);
   location += 2; // 317
   stair_1f(location);
   tack(location, 12);
   location += 2; // 319
   gap(location);
   location += 1; // 320
   stair_2f(location);
   location += 2; // 322
   in_triangle(location);
   location += 1; // 323
   stair_1f(location);
   location += 2; // 325
   flower(location, 13);
   location += 4; // 329


               //   printf("%d", location);
}
void SetCurrentCursorPos(int x, int y) {
	COORD pos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void textcolor(int foreground, int background)
{
	int color = foreground + background * 16;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void NPC_Delete()
{
	int i = 0, j = 0;

	SetCurrentCursorPos(NPC_pos.X, NPC_pos.Y);
	printf("      ");
	SetCurrentCursorPos(NPC_pos.X, NPC_pos.Y + 1);
	printf("      ");
	SetCurrentCursorPos(NPC_pos.X, NPC_pos.Y + 2);
	printf("      ");
}

COORD GetCurrentCursorPos(void) {
	COORD curPoint;
	CONSOLE_SCREEN_BUFFER_INFO curInfo;

	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
	curPoint.X = curInfo.dwCursorPosition.X;
	curPoint.Y = curInfo.dwCursorPosition.Y;

	return curPoint;
}

void RemoveCursor(void) {
	CONSOLE_CURSOR_INFO curInfo;

	GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
	curInfo.bVisible = 0;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
}

void GboardUpdate(int start) {
	int i = 0, j = 0;
	int x = id;

	for (i = 0; i < YSize; i++) {
		for (j = start; j < start + 35; j++) {
			GBoardInfo[i][j - (start)] = stage[i][j];
		}
	}
}

void DrawGboard() {
	int i = 0, j = 0, n = 0;
	int x = 0, y = 0;

	for (i = 10; i<YSize; i++) {
		for (j = 0; j < XSize; j++) {
			textcolor(0, 15);
			SetCurrentCursorPos(7, 13);
			printf(" ");
			SetCurrentCursorPos(7, 14);
			printf(" ");
			SetCurrentCursorPos(7, 15);
			printf(" ");
			SetCurrentCursorPos(7, 16);
			printf(" ");
			textcolor(15, 15);

			SetCurrentCursorPos(GBOARD_ORIGIN_X + (j * 2) + 2, GBOARD_ORIGIN_Y + i - 1);
			x = j;
			y = i;
			n = GBoardInfo[y][x];

			switch (n) {
			case -4:
				if (NPC_Youtubedead == 0)
				{
					printf("  ");
					SetCurrentCursorPos(GBOARD_ORIGIN_X + j * 2 - 1, GBOARD_ORIGIN_Y + i - 1);
					textcolor(12, 15);
					printf("■");
				}
				if (NPC_Youtubeborn == 0 && NPC_Youtubedead == 0)
				{
					npccnt = 1;
					NPCAttack_rand = (rand() % 2) + 100;
					NPC_Youtubeborn = 1;
					NPC_pos.X = GBOARD_ORIGIN_X + j * 2 - 1;
					NPC_pos.Y = GBOARD_ORIGIN_Y + i - 1;
					NPC_facedead = 0;
					NPC_Instadead = 0;
				}
				if (NPC_Youtubedead == 0 && NPC_Youtubeborn == 1)
				{
					if (NPC_Youtubeborn == 1 && NPC_pos.X <= PC_rightpos.X)
					{
						if (PC_rightpos.X != 16)
						{
							if (issuper == 0)
							{
								life -= 1;
							}
						}
						GBoardInfo[y][x] = 0;
						DrawLife();
						NPC_Delete();
						NPC_Youtubedead = 1;
						NPC_Youtubeborn = 0;
					}
					if (NPC_Youtubedead == 0)
					{
						NPC_pos.X = GBOARD_ORIGIN_X + j * 2 - 1;
					}
				}
				break;
			case -3:
				if (NPC_faceborn == 0 && NPC_facedead == 0)
				{
					npccnt = 1;
					NPCAttack_rand = (rand() % 2) + 100;
					NPC_faceborn = 1;
					NPC_pos.X = GBOARD_ORIGIN_X + j * 2 - 1;
					NPC_pos.Y = GBOARD_ORIGIN_Y + i - 1;
					NPC_Instadead = 0;
					NPC_Youtubedead = 0;
				}
				if (NPC_facedead == 0 && NPC_faceborn == 1)
				{
					if (NPC_faceborn == 1 && NPC_pos.X+2 <= PC_rightpos.X)
					{
						if (PC_rightpos.X != 16)
						{
							if (issuper == 0)
							{
								life -= 1;
							}
						}
						DrawLife();
						NPC_facedead = 1;

						GBoardInfo[y][x] = 0;
						NPC_Delete();
						NPC_faceborn = 0;
					}
					if (NPC_facedead == 0)
					{
						NPC_pos.X = GBOARD_ORIGIN_X + j * 2 - 1;
						NPC_pos.Y = GBOARD_ORIGIN_Y + i - 1;
					}
				}
				break;
			case -2:
				if (NPC_Instadead == 0)
				{
					printf("  ");
					SetCurrentCursorPos(GBOARD_ORIGIN_X + j * 2 - 1, GBOARD_ORIGIN_Y + i - 1);
					textcolor(13, 15);
					printf("■");
				}
				if (NPC_Instaborn == 0 && NPC_Instadead == 0)
				{
					npccnt = 1;
					NPCAttack_rand = (rand() % 2) + 100;
					NPC_Instaborn = 1;
					NPC_pos.X = GBOARD_ORIGIN_X + j * 2 - 1;
					NPC_pos.Y = GBOARD_ORIGIN_Y + i - 1;
					NPC_facedead = 0;
					NPC_Youtubedead = 0;
				}
				if (NPC_Instadead == 0 && NPC_Instaborn == 1)
				{

					if (NPC_Instaborn == 1 && NPC_pos.X <= PC_rightpos.X)
					{
						if (PC_rightpos.X != 16)
						{
							if (issuper == 0)
							{
								life -= 1;
							}
						}
						DrawLife();

						GBoardInfo[y][x] = 0;
						NPC_Delete();
						NPC_Instadead = 1;

						NPC_Instaborn = 0;
					}
					if (NPC_Instadead == 0)
					{
						NPC_pos.X = GBOARD_ORIGIN_X + j * 2 - 1;
						NPC_pos.Y = GBOARD_ORIGIN_Y + i - 1;
					}
				}
				break;
			case 1:
				if (GBoardInfo[y - 1][x - 1] != 1) {
					printf("  ");
				}
				SetCurrentCursorPos(GBOARD_ORIGIN_X + j * 2 - 1, GBOARD_ORIGIN_Y + i - 1);
				textcolor(0, 15);
				printf("■");
				break;

			case 4:
				if (GBoardInfo[y - 1][x - 1] != 4) {
					printf("  ");
				}
				SetCurrentCursorPos(GBOARD_ORIGIN_X + j * 2 - 1, GBOARD_ORIGIN_Y + i - 1);
				textcolor(0, 15);
				printf("ㅗ");
				break;

			case 5:
				if (GBoardInfo[y - 1][x - 1] != 5) {
					printf("  ");
				}
				SetCurrentCursorPos(GBOARD_ORIGIN_X + j * 2 - 1, GBOARD_ORIGIN_Y + i - 1);
				textcolor(0, 15);
				printf("▽");
				break;

			case 6:
				if (GBoardInfo[y - 1][x - 1] != 6) {
					printf("  ");
				}
				SetCurrentCursorPos(GBOARD_ORIGIN_X + j * 2 - 1, GBOARD_ORIGIN_Y + i - 1);
				textcolor(0, 15);
				printf("△");
				break;

			case 7:
				if (GBoardInfo[y - 1][x - 1] != 7) {
					printf("  ");
				}
				SetCurrentCursorPos(GBOARD_ORIGIN_X + j * 2 - 1, GBOARD_ORIGIN_Y + i - 1);
				printf("  ");
				break;

			case 8:
				if (GBoardInfo[y - 1][x - 1] != 8) {
					printf("  ");
				}
				SetCurrentCursorPos(GBOARD_ORIGIN_X + j * 2 - 1, GBOARD_ORIGIN_Y + i - 1);
				textcolor(4, 15);//밝은 초록색 
				printf("∪");
				break;
			case 10:
				if (NPC_Instadead == 0)
				{
					printf("  ");
					SetCurrentCursorPos(GBOARD_ORIGIN_X + j * 2 - 1, GBOARD_ORIGIN_Y + i - 1);
					textcolor(5, 15);//보라색
					printf("○");
				}
				else
				{
					GBoardInfo[y][x] = 0;
				}
				break;
			case 11:
				if (NPC_Youtubedead == 0)
				{
					printf("  ");
					SetCurrentCursorPos(GBOARD_ORIGIN_X + j * 2 - 1, GBOARD_ORIGIN_Y + i - 1);
					textcolor(12, 15);//빨강
					printf("▷");
				}
				else
				{
					GBoardInfo[y][x] = 0;
				}
				break;
			case 12:

				if (NPC_facedead == 0)
				{
					printf("  ");
					SetCurrentCursorPos(GBOARD_ORIGIN_X + j * 2 - 1, GBOARD_ORIGIN_Y + i - 1);
					textcolor(9, 15);
					printf("ⓕ");//파랑
				}
				else
				{
					GBoardInfo[y][x] = 0;
				}
				break;
			case 20:
				if (GBoardInfo[y - 1][x - 1] != 20) {
					printf("  ");
				}
				SetCurrentCursorPos(GBOARD_ORIGIN_X + j * 2 - 1, GBOARD_ORIGIN_Y + i - 1);
				textcolor(13, 15);
				printf("★");
				break;
			case 21:
				if (GBoardInfo[y - 1][x - 1] != 21) {
					printf("  ");
				}
				SetCurrentCursorPos(GBOARD_ORIGIN_X + j * 2 - 1, GBOARD_ORIGIN_Y + i - 1);
				textcolor(9, 15);//하늘
				printf("㉠");
				break;
			case 22:
				if (GBoardInfo[y - 1][x - 1] != 22) {
					printf("  ");
				}
				SetCurrentCursorPos(GBOARD_ORIGIN_X + j * 2 - 1, GBOARD_ORIGIN_Y + i - 1);
				textcolor(9, 15);//하늘
				printf("Ｄ");
				break;
			case 23:
				if (GBoardInfo[y - 1][x - 1] != 23) {
					printf("  ");
				}
				SetCurrentCursorPos(GBOARD_ORIGIN_X + j * 2 - 1, GBOARD_ORIGIN_Y + i - 1);
				textcolor(9, 15);//하늘
				printf("◎");
				break;
			case 24:
				if (GBoardInfo[y - 1][x - 1] != 24) {
					printf("  ");
				}
				SetCurrentCursorPos(GBOARD_ORIGIN_X + j * 2 - 1, GBOARD_ORIGIN_Y + i - 1);
				textcolor(0, 15);
				printf("…");
				break;
			case 25:
				if (GBoardInfo[y - 1][x - 1] != 25) {
					printf("  ");
				}
				SetCurrentCursorPos(GBOARD_ORIGIN_X + j * 2 - 1, GBOARD_ORIGIN_Y + i - 1);
				//textcolor(12, 15);
				printf("▶");
				break;
			case 60:

				if (NPC_Instadead == 0)
				{
					printf("  ");
					SetCurrentCursorPos(GBOARD_ORIGIN_X + j * 2 - 1, GBOARD_ORIGIN_Y + i - 1);
					textcolor(13, 15);
					printf("■");
				}
				else
				{
					GBoardInfo[y][x] = 0;
				}
				break;
			case 61:

				if (NPC_Youtubedead == 0)
				{
					printf("  ");
					SetCurrentCursorPos(GBOARD_ORIGIN_X + j * 2 - 1, GBOARD_ORIGIN_Y + i - 1);
					textcolor(12, 15);
					printf("■");
				}
				else
				{
					GBoardInfo[y][x] = 0;
				}
				break;
			case 64:
				if (NPC_Instadead == 0 && NPC_Instaborn == 1)
				{
					printf("  ");
					SetCurrentCursorPos(GBOARD_ORIGIN_X + j * 2 - 1, GBOARD_ORIGIN_Y + i - 1);
					textcolor(13, 15);
					printf("■");
				}
				if (NPC_Instadead == 1)
				{
					GBoardInfo[y][x] = 0;
					NPC_pos.X = 0;
					NPC_Instaborn = 0;
				}

				break;
			case 65:
				if (NPC_Youtubedead == 0 && NPC_Youtubeborn == 1)
				{
					printf("  ");
					SetCurrentCursorPos(GBOARD_ORIGIN_X + j * 2 - 1, GBOARD_ORIGIN_Y + i - 1);
					textcolor(12, 15);
					printf("■");
				}
				if (NPC_Youtubedead == 1)
				{
					GBoardInfo[y][x] = 0;
					NPC_pos.X = 0;
					NPC_Youtubeborn = 0;
				}
				break;
			case 66:
				if (NPC_facedead == 0 && NPC_faceborn == 1)
				{
					printf("  ");
					SetCurrentCursorPos(GBOARD_ORIGIN_X + j * 2 - 1, GBOARD_ORIGIN_Y + i - 1);
				}
				if (NPC_facedead == 1)
				{
					GBoardInfo[y][x] = 0;
					NPC_pos.X = 0;
					NPC_faceborn = 0;
				}
				break;
			}
			textcolor(15, 15);
		}
	}

	if (cnt - (supercnt - 1) == 15 && issuper == 1)
	{
		issuper = 0;
		supercnt = 0;
		DrawPC();
	}
	if (issuper == 1)
	{
		DrawPC();
	}
}

void DrawPC() {
	int x, y, z;
	if (issuper == 0)
	{
		textcolor(2, 15);
	}
	else
	{
		textcolor(13, 15);
	}
	for (y = 0; y < 3; y++) {
		for (x = 0; x < 3; x++) {
			SetCurrentCursorPos(PC_pos.X + x * 2, PC_pos.Y + y);

			if (PC[id][y][x] == 100) {
				printf("●");
			}
			else if (PC[id][y][x] == 101) {
				printf("┼");
			}
			else if (PC[id][y][x] == 102) {
				printf("∬");
			}
			else if (PC[id][y][x] == 103) {
				printf("┌");
			}
			else if (PC[id][y][x] == 104) {
				printf("◎");
			}
			else if (PC[id][y][x] == 105) {
				printf("│");
			}
			else if (PC[id][y][x] == 106) {
				printf("─");
			}
			else if (PC[id][y][x] == 107) {
				printf("Ｄ");
			}
		}
	}
	textcolor(15, 15);
}

void DeletePC() {
	int x, y;

	for (y = 0; y < 3; y++) {
		for (x = 0; x < 3; x++) {
			SetCurrentCursorPos(PC_pos.X + x * 2, PC_pos.Y + y);
			if (PC[id][y][x] != 0) {

				printf(" ");
			}
		}
	}
}

void DetectCollision_PC_Item() {

	int x, y;

	int arrX = ((PC_pos.X) - GBOARD_ORIGIN_X) / 2 + 1;

	int arrY = (PC_pos.Y - GBOARD_ORIGIN_Y);
	for (x = 0; x <= 2; x++) {
		for (y = 0; y <= 3; y++) {

			if (GBoardInfo[arrY + y][arrX] == 20 && PC[y][arrX + x] != 0) { //무적 아이템
				issuper = 1;
				supercnt = cnt;
				stage[arrY + y][arrX + x + cnt] = 0;

			}

			else if (GBoardInfo[arrY + y][arrX] == 21 && PC[y][arrX + x] != 0) { // 총 무기
				DeletePC();
				id = 2;
				DrawPC();
				stage[arrY + y][arrX + x + cnt] = 0;
			}

			else if (GBoardInfo[arrY + y][arrX] == 22 && PC[y][arrX + x] != 0) { // 화살
				DeletePC();
				id = 3;
				DrawPC();
				stage[arrY + y][arrX + x + cnt] = 0;
			}

			else if (GBoardInfo[arrY + y][arrX] == 23 && PC[y][arrX + x] != 0) { // 후라이팬
				DeletePC();
				id = 1;
				DrawPC();
				stage[arrY + y][arrX + x + cnt] = 0;
			}

			else if (GBoardInfo[arrY + y][arrX] == 24 && PC[y][arrX + x] != 0) { // 총알
				bullet = bullet + 5;
			}

			else if (GBoardInfo[arrY + y][arrX] == 25 && PC[y][arrX + x] != 0) { // 화살
				arrow = arrow + 5;
			}
		}
	}

}

void DetectCollision_PC_Barrier() {
	int x, y;
	int arrX = ((PC_pos.X) - GBOARD_ORIGIN_X) / 2;
	int arrY = (PC_pos.Y - GBOARD_ORIGIN_Y) + 1;


	for (int y = 0; y < 3; y++) {
		if (GBoardInfo[arrY + y][arrX + 1] != 0 && GBoardInfo[arrY + y][arrX + 1] != 1 && GBoardInfo[arrY + y][arrX + 1] != 20 && GBoardInfo[arrY + y][arrX + 1] != 21 && GBoardInfo[arrY + y][arrX + 1] != 22 && GBoardInfo[arrY + y][arrX + 1] != 23 && GBoardInfo[arrY + y][arrX + 1] != 24 && GBoardInfo[arrY + y][arrX + 1] != 25 && GBoardInfo[arrY + y][arrX + 1] != 6) {
			if (GBoardInfo[arrY + y][arrX + 1] != -3 && GBoardInfo[arrY + y][arrX + 1] != -1 && GBoardInfo[arrY + y][arrX + 1] != 12 && GBoardInfo[arrY + y][arrX + 1] != 66 && GBoardInfo[arrY + y][arrX + 1] != -4 && GBoardInfo[arrY + y][arrX + 1] != 61 && GBoardInfo[arrY + y][arrX + 1] != 11 && GBoardInfo[arrY + y][arrX + 1] != 65 && GBoardInfo[arrY + y][arrX + 1] != -2 && GBoardInfo[arrY + y][arrX + 1] != 60 && GBoardInfo[arrY + y][arrX + 1] != 64 && GBoardInfo[arrY + y][arrX + 1] != 10)
			{
				if (issuper == 0)
				{
					life -= 1;
					return;
				}
			}
		}
	}
	for (int y = 0; y < 3; y++)
	{
		if (GBoardInfo[arrY + y][arrX + 1] == 6)
		{
			if (issuper == 0)
			{
				life -= 1;
				return;
			}
		}
	}
	DrawLife();
}

void DrawNPC() {
	int x, y;

	for (y = 0; y < 3; y++) {
		for (x = 0; x < 3; x++) {
			SetCurrentCursorPos(NPC_pos.X + x * 2, NPC_pos.Y + y);
			switch (NPC[NPC_randnum][y][x]) {
			case 1:
				printf("■");
				break;

			case 10:
				printf("○");
				break;

			case 11:
				printf("▷");
				break;

			case 12:
				printf("ⓕ");
				break;
			}
		}
	}
}

void DeleteNPC() {
	int x, y;

	for (y = 0; y < 3; y++) {
		for (x = 0; x < 3; x++) {
			SetCurrentCursorPos(NPC_pos.X + x * 2, NPC_pos.Y + y);
			if (NPC[NPC_randnum][y][x] != 0) {
				printf(" ");
			}
		}
	}
}


void PC_Jump() {
	DeletePC();
	for (int i = 0; i < 2; i++) {
		PC_pos.Y--;
		SetCurrentCursorPos(PC_pos.X, PC_pos.Y);
		DrawPC();
		if (PC_pos.Y != 14) DetectCollision_PC_Barrier();
		DetectCollision_PC_Item();
		GboardUpdate(cnt++);
		DrawGboard();
		NPC_Attack();
		Sleep(speed / 7 * 3);
		DeletePC();
		PC_pos.Y--;
		SetCurrentCursorPos(PC_pos.X, PC_pos.Y);
		DrawPC();
		//DetectCollision_PC_Barrier();
		DetectCollision_PC_Item();
		Sleep(speed / 7 * 3);
		//NPC_Attack();
		DeletePC();
	}

	while (PC_pos.Y - GBOARD_ORIGIN_Y + 3 < 13 && GBoardInfo[PC_pos.Y - GBOARD_ORIGIN_Y + 4][(PC_pos.X - GBOARD_ORIGIN_X) / 2 + 1] != 1) {
		DeletePC();
		if (PC_pos.Y == 15 || GBoardInfo[PC_pos.Y - DRAWY + 1][(PC_pos.X - GBOARD_ORIGIN_X) / 2] == 7)
		{
			int n_life;
			DeletePC();
			DrawPC();
			if (issuper == 0)
			{
				n_life = life - 1;
			}
			else
			{
				n_life = life;
			}
			DetectCollision_PC_Item();
			while (!_kbhit())
			{
				if (_kbhit())
				{
					int key = _getch();
					if (key == SPACE)
					{
						PC_Jump();
						life = n_life;
						break;
					}
				}
			}
			DeletePC();
			break;
		}
		PC_pos.Y++;
		SetCurrentCursorPos(PC_pos.X, PC_pos.Y);
		DrawPC();
		DetectCollision_PC_Barrier();
		DetectCollision_PC_Item();
		GboardUpdate(cnt++);
		DrawGboard();
		NPC_Attack();
		if (GBoardInfo[PC_pos.Y - GBOARD_ORIGIN_Y + 4][(PC_pos.X - GBOARD_ORIGIN_X) / 2 + 1] != 1)
		{
			Sleep(speed / 7 * 3);
		}
		DeletePC();
		if (GBoardInfo[PC_pos.Y - GBOARD_ORIGIN_Y + 4][(PC_pos.X - GBOARD_ORIGIN_X) / 2 + 1] == 1)
		{
			DrawPC();
			break;
		}
		PC_pos.Y++;
		SetCurrentCursorPos(PC_pos.X, PC_pos.Y);
		DrawPC();
		DetectCollision_PC_Barrier();
		DetectCollision_PC_Item();
		if (PC_pos.Y == 15 || GBoardInfo[PC_pos.Y - DRAWY + 1][(PC_pos.X - GBOARD_ORIGIN_X) / 2] == 7)
		{
			int n_life;
			DeletePC();
			DrawPC();
			if (issuper == 0)
			{
				n_life = life - 1;
			}
			else
			{
				n_life = life;
			}
			DetectCollision_PC_Item();
			while (!_kbhit())
			{
				if (_kbhit())
				{
					int key = _getch();
					if (key == SPACE)
					{
						PC_Jump();
						life = n_life;
						break;
					}
				}
			}
			break;
		}

		if (GBoardInfo[PC_pos.Y - GBOARD_ORIGIN_Y + 4][(PC_pos.X - GBOARD_ORIGIN_X) / 2 + 1] != 1)
		{
			Sleep(speed / 7 * 3);
		}
		if (GBoardInfo[PC_pos.Y - DRAWY + 2][(PC_pos.X - GBOARD_ORIGIN_X) / 2] != 1)
		{
			DeletePC();
		}
	}
	cnt--;
	return;
}

void PC_down()
{
	while (PC_pos.Y + 2 < 15 + DRAWY && GBoardInfo[PC_pos.Y - DRAWY + 2][(PC_pos.X - GBOARD_ORIGIN_X) / 2] != 1) { // 나중에 while문으로 바꿔서 밑바닥이 1일 때까지 내려가기
		DeletePC();
		if (PC_pos.Y == 15 || GBoardInfo[PC_pos.Y - DRAWY + 1][(PC_pos.X - GBOARD_ORIGIN_X) / 2] == 7)
		{
			DeletePC();
			DrawPC();
			DetectCollision_PC_Item();
			int n_life = life - 1;
			while (!_kbhit())
			{
				if (_kbhit())
				{
					int key = _getch();
					if (key == SPACE)
					{
						PC_Jump();
						life = n_life + 1;
						break;
					}
				}
			}
			DeletePC();
			break;
		}
		PC_pos.Y++;
		SetCurrentCursorPos(PC_pos.X, PC_pos.Y);
		DrawPC();
		DetectCollision_PC_Barrier();
		DetectCollision_PC_Item();
		GboardUpdate(cnt++);
		DrawGboard();
		if (GBoardInfo[PC_pos.Y - GBOARD_ORIGIN_Y + 4][(PC_pos.X - GBOARD_ORIGIN_X) / 2] != 1)
		{
			Sleep(speed / 7 * 3);
		}
		DeletePC();
		if (PC_pos.Y == 15 || GBoardInfo[PC_pos.Y - DRAWY + 1][(PC_pos.X - GBOARD_ORIGIN_X) / 2] == 7)
		{
			DeletePC();
			DrawPC();

			int   n_life = life - 1;

			DetectCollision_PC_Item();
			while (!_kbhit())
			{
				if (_kbhit())
				{
					int key = _getch();
					if (key == SPACE)
					{
						PC_Jump();
						life = n_life + 1;
						break;
					}
				}
			}
			break;
		}
		cnt--;
	}

	if (GBoardInfo[PC_pos.Y - DRAWY + 1][(PC_pos.X - GBOARD_ORIGIN_X) / 2 + 2] == 1)
	{
		while (!_kbhit())
		{
			if (_kbhit())
			{
				int key = _getch();
				if (key == SPACE)
				{
					PC_Jump();
					break;
				}
			}
		}
	}

}


void DrawLife() {
	Life_pos.X = 65;
	Life_pos.Y = 3;
	textcolor(15, 15);
	SetCurrentCursorPos(Life_pos.X, Life_pos.Y);
	printf("              ");
	SetCurrentCursorPos(Life_pos.X, Life_pos.Y);
	textcolor(0, 15);
	printf("LIFE : ");
	textcolor(12, 15);
	for (int i = 0; i < life; i++) {
		printf("♥");
	}
	textcolor(0, 15);
	/*
	if (life <= 0)
	{
		game_over();
	}
	*/
	return;
}

void DrawGBox() {
	int i, j;
	SetCurrentCursorPos(0, 0);
	printf("┌");
	for (i = 0; i<XSize + 7; i++)
	{
		printf("─");
	}
	printf("┐\n");
	for (i = 0; i<YSize + 4; i++)
	{
		SetCurrentCursorPos(0, i + 1);
		printf("│");

		SetCurrentCursorPos((XSize + (DRAWX * 2 - 2)) * 2, i + 1);
		printf("│");
	}
	SetCurrentCursorPos(0, i + 1);
	printf("└");

	for (i = 0; i<XSize + (DRAWX * 2 - 3); i++)
	{
		printf("─");
	}
	printf("┘");

	SetCurrentCursorPos(DRAWX, DRAWY);
	printf("┌");

	for (i = 1; i < XSize + 2; i++) { // 사이즈 조정
		printf("─");
	}
	printf("┐\n");

	for (j = 0; j < YSize; j++) {
		SetCurrentCursorPos(DRAWX, DRAWY + j + 1);
		printf("│");

		for (i = 1; i < (XSize * 2) + 3; i++) { // 사이즈 조정
			printf(" ");
		}
		printf("│\n");
	}

	SetCurrentCursorPos(DRAWX, DRAWY + YSize + 1);
	printf("└");

	for (i = 0; i<XSize + 1; i++)
	{
		printf("─");
	}
	SetCurrentCursorPos(DRAWX + ((XSize + 2) * 2), DRAWY + YSize + 1);
	printf("┘");

	SetCurrentCursorPos((DRAWX + XSize + 1) * 2, 7);
	printf("┌");
	printf("┐");
	for (i = 0; i<5; i++)
	{
		SetCurrentCursorPos((DRAWX + XSize + 1) * 2, 8 + i);
		printf("│");
		printf("│");
	}
	SetCurrentCursorPos((DRAWX + XSize + 1) * 2, 13);
	printf("└");
	printf("┘");
}

void DeleteNPCAttack(COORD NPC_Attackpos)
{
	int i = 0;

	switch (NPCAttack_rand)
	{
	case 100:
		SetCurrentCursorPos(NPC_Attackpos.X, NPC_Attackpos.Y);
		printf(" ");
		break;
	case 101:
		SetCurrentCursorPos(NPC_Attackpos.X, NPC_Attackpos.Y);
		printf(" ");
		break;
	}

}

void DetectCollisionPC_NPCAttack()
{
	if (NPC_facedead == 1 || NPC_Youtubedead == 1 || NPC_Instadead == 1 || NPCAttack1_die == 1)
	{
		return;
	}
	if (PC_rightpos.X - 1 == NPC_Attackpos1.X)
	{
		return;
	}
	/*
	if (PC_Attackpos.X+2>=NPC_Attackpos1.X||PC_rightpos.X==16&&PC_rightpos.X==NPC_Attackpos1.X)
	{
	DeleteNPCAttack(NPC_Attackpos1);
	life+=1;
	DrawLife();
	}
	*/
	if (PC_pos.X + 1 >= NPC_Attackpos1.X && (PC_pos.Y + 2) - 1 >= NPC_Attackpos1.Y)
	{
		if (!((PC_rightpos.X == 16 && PC_rightpos.X >= NPC_Attackpos1.X) || PC_Attackpos.X + 2 == NPC_Attackpos1.X))
		{
			if (issuper == 0)
			{
				life -= 1;
			}
		}
		DrawLife();
		DeleteNPCAttack(NPC_Attackpos1);
		NPC_Attackpos1.X = 60;
		NPCAttack1_die = 1;
	}
}

void DrawNPC_Attack(COORD NPC_Attackpos)
{
	textcolor(0, 15);
	switch (NPCAttack_rand)
	{

	case 100:
		SetCurrentCursorPos(NPC_Attackpos.X, NPC_Attackpos.Y);
		printf("←");
		break;
	case 101:
		SetCurrentCursorPos(NPC_Attackpos.X, NPC_Attackpos.Y);
		printf("ㆁ");
		break;
	}
}


void NPC_Attack()
{

	if ((NPC_faceborn == 0 && NPC_Youtubeborn == 0 && NPC_Instaborn == 0) || (NPC_facedead == 1 || NPC_Instadead == 1 || NPC_Youtubedead == 1))
	{
		DeleteNPCAttack(NPC_Attackpos1);
		NPCAttack1_die = 0;
		return;
	}
	if (NPC_Attack1born == 0 && (NPC_faceborn == 1 || NPC_Youtubeborn == 1 || NPC_Instaborn == 1) && (npccnt == 1 || n - NPC_pos.X >= 6))
	{
		NPC_Attack1born = 1;
		NPCAttack1_die = 0;
		if (NPC_faceborn == 1)
		{
			NPC_Attackpos1.X = NPC_pos.X - 2;
		}
		else
		{
			NPC_Attackpos1.X = NPC_pos.X - 4;
		}
		NPC_Attackpos1.Y = 15;
		DeleteNPCAttack(NPC_Attackpos1);
		npccnt++;
	}

	else if (NPC_Attack1born == 1)
	{
		if (NPC_pos.X - NPC_Attackpos1.X<9)
		{
			DeleteNPCAttack(NPC_Attackpos1);
			NPC_Attackpos1.X -= 3;
			NPC_Attackpos1.Y = 15;
		}
		else
		{
			DeleteNPCAttack(NPC_Attackpos1);
			NPCAttack1_die = 1;
			NPC_Attackpos1.X = 60;
			n = NPC_pos.X;
			NPC_Attack1born = 0;
		}
	}
	if (NPC_Attack1born == 1 && NPCAttack1_die == 0)
	{
		DrawNPC_Attack(NPC_Attackpos1);
	}


}


void DrawPC_Attack() {
	textcolor(0, 15);
	switch (PCAttack_rand) {
	case 201:
		SetCurrentCursorPos(PC_Attackpos.X, PC_Attackpos.Y);
		printf("→");
		break;

	case 202:
		SetCurrentCursorPos(PC_Attackpos.X, PC_Attackpos.Y);
		printf("ㆁ");
		break;
	}
}

void DeletePC_Attack() {
	switch (PCAttack_rand) {
	case 201:
		SetCurrentCursorPos(PC_Attackpos.X, PC_Attackpos.Y);
		printf(" ");
		break;

	case 202:
		SetCurrentCursorPos(PC_Attackpos.X, PC_Attackpos.Y);
		printf(" ");
		break;
	}
}

int DetectCollision_PCAttack_NPC() {


	if (PC_Attackpos.X >= NPC_pos.X - 2)
	{
		if (NPC_Youtubeborn == 1)
		{
			NPC_Delete();
			NPC_Youtubedead = 1;
		}
		if (NPC_Instaborn == 1)
		{
			NPC_Delete();
			NPC_Instadead = 1;
		}
		if (NPC_faceborn == 1)
		{
			NPC_Delete();
			NPC_facedead = 1;
		}
		return 1;
	}

	else return -1;
}


void PC_Attack() {
	switch (id) {
	case 1: // 무기: 후라이팬
		DeletePC();
		id = 0;
		PC_rightpos.X += 4;
		DrawPC();
		GboardUpdate(cnt++);
		DrawGboard();
		Sleep(100);
		// 공격했을 때 무기의 좌표랑 NPC 좌표랑 비교하기 위해서 이 안에 넣었음! 
		NPC_Attack();
		DetectCollisionPC_NPCAttack();
		DeletePC();
		id = 1;

		DrawPC();

		PC_rightpos.X -= 4; //무기 원상복구되니까 좌표도 다시 기본 상태일때의 좌표로
		break;

	case 2: // 무기: 총
		PCAttack_rand = 202;
		PC_Attackpos.X = PC_pos.X + 4;
		PC_Attackpos.Y = PC_pos.Y + 1;

		DrawPC_Attack();
		DetectCollision_PC_Item();
		DetectCollision_PCAttack_NPC();

		Sleep(speed);
		DeletePC_Attack();
		GboardUpdate(cnt++);
		DrawGboard();
		PC_Attackpos.X += 3;
		DrawPC_Attack();
		DetectCollision_PC_Item();
		NPC_Attack();
		DetectCollisionPC_NPCAttack();
		DetectCollision_PCAttack_NPC();
		if (DetectCollision_PCAttack_NPC() == 0) {
			DeleteNPC();
			DeletePC_Attack();
		}
		Sleep(speed);
		DeletePC_Attack();
		GboardUpdate(cnt++);
		DrawGboard();
		NPC_Attack();
		DetectCollisionPC_NPCAttack();
		PC_Attackpos.X += 3;
		DrawPC_Attack();
		DetectCollision_PC_Item();
		DetectCollision_PCAttack_NPC();
		if (DetectCollision_PCAttack_NPC() == 0) {
			DeleteNPC();
			DeletePC_Attack();
		}
		Sleep(speed);
		GboardUpdate(cnt++);
		DrawGboard();
		DeletePC_Attack();
		NPC_Attack();
		DetectCollisionPC_NPCAttack();
		PC_Attackpos.X = PC_pos.X + 4;
		PC_Attackpos.Y = PC_pos.Y + 1;

		break;

	case 3: // 무기: 활
		PCAttack_rand = 201;
		PC_Attackpos.X = PC_pos.X + 4;
		PC_Attackpos.Y = PC_pos.Y + 1;

		DrawPC_Attack();
		DetectCollision_PC_Item();
		DetectCollision_PCAttack_NPC();
		if (DetectCollision_PCAttack_NPC() == 0) {
			DeleteNPC();
			DeletePC_Attack();
		}
		Sleep(speed);
		DeletePC_Attack();
		GboardUpdate(cnt++);
		DrawGboard();
		NPC_Attack();
		DetectCollisionPC_NPCAttack();
		PC_Attackpos.X += 2;
		DrawPC_Attack();
		DetectCollision_PC_Item();
		DetectCollision_PCAttack_NPC();
		if (DetectCollision_PCAttack_NPC() == 0) {
			DeleteNPC();
			DeletePC_Attack();
		}
		Sleep(speed);
		DeletePC_Attack();
		GboardUpdate(cnt++);
		DrawGboard();
		NPC_Attack();
		DetectCollisionPC_NPCAttack();
		PC_Attackpos.X += 2;
		DrawPC_Attack();
		DetectCollision_PC_Item();
		DetectCollision_PCAttack_NPC();
		if (DetectCollision_PCAttack_NPC() == 0) {
			DeleteNPC();
			DeletePC_Attack();
		}
		Sleep(speed);
		GboardUpdate(cnt++);
		DrawGboard();
		DeletePC_Attack();
		NPC_Attack();
		DetectCollisionPC_NPCAttack();
		PC_Attackpos.X = PC_pos.X + 4;
		PC_Attackpos.Y = PC_pos.Y + 1;

		break;

	default:

		break;
	}
	return;
}

void ProcessKeyInput() {
	int i, key;
	int speed = 10;

	for (int i = 0; i < 20; i++) {
		if (_kbhit() != 0) {
			key = _getch();

			switch (key) {
			case SPACE:
				jumped = 1;
				PC_Jump();
				break;

			case  ATTACK:
				isAttack = 1;
				PC_Attack();
				isAttack = 0;
				break;

			case P:
				system("cls");
				DrawEnding();
				break;
			}
		}
	}
}

void Now_progress()
{
	int stage1 = 224 - 35;
	int stage2 = 370 - 35;
	int stage3 = 440 - 35;

	if (stage_num == 1)
	{
		SetCurrentCursorPos(10, 3);
		printf("<STAGE 1>");
		SetCurrentCursorPos(25, 3);
		printf("현재 진행도! ");
		for (int i = 0; i < cnt / 22 + 1; i++)
		{
			printf("■");
		}
		for (int i = cnt / 22 + 1; i < 10; i++)
		{
			printf("□");
		}
	}
	else if (stage_num == 2)
	{
		SetCurrentCursorPos(10, 3);
		printf("<STAGE 2>");
		SetCurrentCursorPos(25, 3);
		printf("현재 진행도! ");
		for (int i = 0; i < cnt / (stage2 / 10) + 1; i++)
		{
			printf("■");
		}
		for (int i = cnt / (stage2 / 10) + 1; i < 10; i++)
		{
			printf("□");
		}
	}
	else
	{
		SetCurrentCursorPos(10, 3);
		printf("<STAGE 3>");
		SetCurrentCursorPos(25, 3);
		printf("현재 진행도! ");
		for (int i = 0; i < cnt / (stage3 / 10) + 1; i++)
		{
			printf("■");
		}
		for (int i = cnt / (stage3 / 10) + 1; i < 10; i++)
		{
			printf("□");
		}
	}

	return;
}

void stage_clear()
{

	DeletePC();
	for (int y = 0; y < 15; y++)
	{
		SetCurrentCursorPos(GBOARD_ORIGIN_X, 3 + y);
		for (int x = 0; x < 70; x++)
		{
			printf(" ");
		}
	}
	textcolor(9, 15);
	Sleep(200);
	SetCurrentCursorPos(GBOARD_ORIGIN_X + 2 * 3, 3 + 4);
	printf("■■");
	SetCurrentCursorPos(GBOARD_ORIGIN_X + 2 * 2, 3 + 5);
	printf("■    ■");
	SetCurrentCursorPos(GBOARD_ORIGIN_X + 2 * 2, 3 + 6);
	printf("■");
	SetCurrentCursorPos(GBOARD_ORIGIN_X + 2 * 2, 3 + 7);
	printf("■    ■");
	SetCurrentCursorPos(GBOARD_ORIGIN_X + 2 * 3, 3 + 8);
	printf("■■");

	Sleep(200);
	SetCurrentCursorPos(GBOARD_ORIGIN_X + 2 * 8, 3 + 4);
	printf("■");
	SetCurrentCursorPos(GBOARD_ORIGIN_X + 2 * 8, 3 + 5);
	printf("■");
	SetCurrentCursorPos(GBOARD_ORIGIN_X + 2 * 8, 3 + 6);
	printf("■");
	SetCurrentCursorPos(GBOARD_ORIGIN_X + 2 * 8, 3 + 7);
	printf("■");
	SetCurrentCursorPos(GBOARD_ORIGIN_X + 2 * 8, 3 + 8);
	printf("■■■");

	Sleep(200);
	SetCurrentCursorPos(GBOARD_ORIGIN_X + 2 * 13, 3 + 4);
	printf("■■■■");
	SetCurrentCursorPos(GBOARD_ORIGIN_X + 2 * 13, 3 + 5);
	printf("■");
	SetCurrentCursorPos(GBOARD_ORIGIN_X + 2 * 13, 3 + 6);
	printf("■■■■");
	SetCurrentCursorPos(GBOARD_ORIGIN_X + 2 * 13, 3 + 7);
	printf("■");
	SetCurrentCursorPos(GBOARD_ORIGIN_X + 2 * 13, 3 + 8);
	printf("■■■■");

	Sleep(200);
	SetCurrentCursorPos(GBOARD_ORIGIN_X + 2 * 20, 3 + 4);
	printf("■■");
	SetCurrentCursorPos(GBOARD_ORIGIN_X + 2 * 19, 3 + 5);
	printf("■    ■");
	SetCurrentCursorPos(GBOARD_ORIGIN_X + 2 * 19, 3 + 6);
	printf("■■■■");
	SetCurrentCursorPos(GBOARD_ORIGIN_X + 2 * 19, 3 + 7);
	printf("■    ■");
	SetCurrentCursorPos(GBOARD_ORIGIN_X + 2 * 19, 3 + 8);
	printf("■    ■");

	Sleep(200);
	SetCurrentCursorPos(GBOARD_ORIGIN_X + 2 * 25, 3 + 4);
	printf("■■■");
	SetCurrentCursorPos(GBOARD_ORIGIN_X + 2 * 25, 3 + 5);
	printf("■    ■");
	SetCurrentCursorPos(GBOARD_ORIGIN_X + 2 * 25, 3 + 6);
	printf("■■■");
	SetCurrentCursorPos(GBOARD_ORIGIN_X + 2 * 25, 3 + 7);
	printf("■    ■");
	SetCurrentCursorPos(GBOARD_ORIGIN_X + 2 * 25, 3 + 8);
	printf("■    ■");

	Sleep(200);
	SetCurrentCursorPos(GBOARD_ORIGIN_X + 2 * 31, 3 + 4);
	printf("■  ■");
	SetCurrentCursorPos(GBOARD_ORIGIN_X + 2 * 31, 3 + 5);
	printf("■  ■");
	SetCurrentCursorPos(GBOARD_ORIGIN_X + 2 * 31, 3 + 6);
	printf("■  ■");
	SetCurrentCursorPos(GBOARD_ORIGIN_X + 2 * 31, 3 + 8);
	printf("■  ■");



	Sleep(3000);
	for (int y = 0; y < 15; y++)
	{
		SetCurrentCursorPos(GBOARD_ORIGIN_X, 3 + y);
		for (int x = 0; x < 70; x++)
		{
			printf(" ");
		}
	}
}

void game_over()
{
	PlaySound(TEXT("only_you.wav"), NULL, SND_ASYNC | SND_LOOP);
	DeletePC();
	for (int y = 0; y < 15; y++)
	{
		SetCurrentCursorPos(GBOARD_ORIGIN_X, 3 + y);
		for (int x = 0; x < 70; x++)
		{
			printf(" ");
		}
	}
	textcolor(12, 15);
	SetCurrentCursorPos(GBOARD_ORIGIN_X + 2 * 7, 3 + 2);
	printf("■■■        ■■      ■      ■    ■■■■");
	SetCurrentCursorPos(GBOARD_ORIGIN_X + 2 * 6, 3 + 3);
	printf("■            ■    ■    ■■  ■■    ■");
	SetCurrentCursorPos(GBOARD_ORIGIN_X + 2 * 6, 3 + 4);
	printf("■    ■■    ■■■■    ■  ■  ■    ■■■■");
	SetCurrentCursorPos(GBOARD_ORIGIN_X + 2 * 6, 3 + 5);
	printf("■      ■    ■    ■    ■      ■    ■");
	SetCurrentCursorPos(GBOARD_ORIGIN_X + 2 * 7, 3 + 6);
	printf("■■■      ■    ■    ■      ■    ■■■■");

	Sleep(400);
	SetCurrentCursorPos(GBOARD_ORIGIN_X + 2 * 7, 3 + 8);
	printf("■■■      ■      ■    ■■■■    ■■■");
	SetCurrentCursorPos(GBOARD_ORIGIN_X + 2 * 6, 3 + 9);
	printf("■      ■    ■      ■    ■          ■    ■");
	SetCurrentCursorPos(GBOARD_ORIGIN_X + 2 * 6, 3 + 10);
	printf("■      ■    ■      ■    ■■■■    ■■■");
	SetCurrentCursorPos(GBOARD_ORIGIN_X + 2 * 6, 3 + 11);
	printf("■      ■      ■  ■      ■          ■    ■");
	SetCurrentCursorPos(GBOARD_ORIGIN_X + 2 * 7, 3 + 12);
	printf("■■■          ■        ■■■■    ■    ■");

	SetCurrentCursorPos(GBOARD_ORIGIN_X + 2 * 13, 3 + 14);
	printf("PRESS 'P' TO RESTART");

	while (1)
	{
		int key = _getch();
		if (key == P)
		{
			textcolor(0, 15);
			PlaySound(TEXT("main (online-audio-converter.com).wav"), NULL, SND_ASYNC | SND_LOOP);
			for (int y = 0; y < 15; y++)
			{
				SetCurrentCursorPos(GBOARD_ORIGIN_X, 3 + y);
				for (int x = 0; x < 70; x++)
				{
					printf(" ");
				}
			}

			menu();
			menuKeyInput();
			return;
		}
	}


}

void DrawEnding()
{
	RemoveCursor();
	PlaySound(TEXT("HD-Frontend 2013-12-31 18-36-47-149 (online-audio-converter.com).wav"), NULL, SND_ASYNC | SND_LOOP);
	textcolor(0, 15);
	int i, j;
	SetCurrentCursorPos(0, 0);
	for (i = 0; i<18; i++)
	{
		printf("─");
	}
	printf("┐");

	for (i = 0; i<19; i++)
	{
		SetCurrentCursorPos(2 * 18, i + 1);
		printf("│");
	}
	SetCurrentCursorPos(0, i + 1);
	for (i = 0; i<18; i++)
	{
		printf("─");
	}
	printf("┘");

	for (i = 0; i<40; i++)
	{
		printf("─");
	}

	SetCurrentCursorPos(0, 2);
	for (i = 0; i<14; i++)
	{
		printf("─");
	}
	printf("┐");
	for (i = 0; i<15; i++)
	{
		SetCurrentCursorPos(2 * 14, i + 3);
		printf("│");
	}
	SetCurrentCursorPos(0, 18);
	for (i = 0; i<14; i++)
	{
		printf("─");
	}
	printf("┘");
	SetCurrentCursorPos((2 * 14) + 3, 7);
	printf("┌");
	printf("┐");
	for (i = 0; i<5; i++)
	{
		SetCurrentCursorPos((2 * 14) + 3, 8 + i);
		printf("│");
		printf("│");
	}
	SetCurrentCursorPos((2 * 14) + 3, 8 + i);
	printf("└");
	printf("┘");

	SetCurrentCursorPos(0, 17);
	for (i = 0; i<14; i++)
	{
		printf("■");
	}

	for (i = 0; i<13; i++)
	{
		PC_pos.X = i * 2;
		PC_pos.Y = 14;
		DrawPC();
		Sleep(100);
		DeletePC();
	}
	for (i = 0; i<17; i++)
	{
		PC_pos.X = 39 + (i * 2);
		PC_pos.Y = 17;
		DrawPC();
		Sleep(100);
		DeletePC();
	}
	textcolor(0, 15);
	SetCurrentCursorPos(41, 4);
	printf("■■■■■■■■■■");
	SetCurrentCursorPos(41, 5);
	printf("■");
	SetCurrentCursorPos(41, 6);
	printf("■");
	SetCurrentCursorPos(41, 7);
	printf("■");
	SetCurrentCursorPos(41, 8);
	printf("■");
	SetCurrentCursorPos(41, 9);
	printf("■");
	SetCurrentCursorPos(41, 10);
	printf("■■■■■■■■■■");
	SetCurrentCursorPos(41, 11);
	printf("■");
	SetCurrentCursorPos(41, 12);
	printf("■");
	SetCurrentCursorPos(41, 13);
	printf("■");
	SetCurrentCursorPos(41, 14);
	printf("■");
	SetCurrentCursorPos(41, 15);
	printf("■");
	SetCurrentCursorPos(41, 16);
	printf("■■■■■■■■■■");

	SetCurrentCursorPos(64, 4);
	printf("■");
	SetCurrentCursorPos(64, 5);
	printf("■");
	SetCurrentCursorPos(64, 6);
	printf("■");
	SetCurrentCursorPos(64, 7);
	printf("■");
	SetCurrentCursorPos(64, 8);
	printf("■");
	SetCurrentCursorPos(64, 9);
	printf("■");
	SetCurrentCursorPos(64, 10);
	printf("■");
	SetCurrentCursorPos(64, 11);
	printf("■");
	SetCurrentCursorPos(64, 12);
	printf("■");
	SetCurrentCursorPos(64, 13);
	printf("■");
	SetCurrentCursorPos(64, 14);
	printf("■");
	SetCurrentCursorPos(64, 15);
	printf("■");
	SetCurrentCursorPos(64, 16);
	printf("■");

	SetCurrentCursorPos(66, 5);
	printf("■");
	SetCurrentCursorPos(68, 6);
	printf("■");
	SetCurrentCursorPos(70, 7);
	printf("■");
	SetCurrentCursorPos(72, 8);
	printf("■");
	SetCurrentCursorPos(74, 9);
	printf("■");
	SetCurrentCursorPos(75 + 1, 10);
	printf("■");
	SetCurrentCursorPos(77 + 1, 11);
	printf("■");
	SetCurrentCursorPos(79 + 1, 12);
	printf("■");
	SetCurrentCursorPos(81 + 1, 13);
	printf("■");
	SetCurrentCursorPos(83 + 1, 14);
	printf("■");
	SetCurrentCursorPos(85 + 1, 15);
	printf("■");
	SetCurrentCursorPos(87 + 1, 16);
	printf("■");


	SetCurrentCursorPos(88, 4);
	printf("■");
	SetCurrentCursorPos(88, 5);
	printf("■");
	SetCurrentCursorPos(88, 6);
	printf("■");
	SetCurrentCursorPos(88, 7);
	printf("■");
	SetCurrentCursorPos(88, 8);
	printf("■");
	SetCurrentCursorPos(88, 9);
	printf("■");
	SetCurrentCursorPos(88, 10);
	printf("■");
	SetCurrentCursorPos(88, 11);
	printf("■");
	SetCurrentCursorPos(88, 12);
	printf("■");
	SetCurrentCursorPos(88, 13);
	printf("■");
	SetCurrentCursorPos(88, 14);
	printf("■");
	SetCurrentCursorPos(88, 15);
	printf("■");

	SetCurrentCursorPos(93, 4);
	printf("■■■■■■");
	SetCurrentCursorPos(93, 5);
	printf("■          ■");
	SetCurrentCursorPos(93, 6);
	printf("■            ■");
	SetCurrentCursorPos(93, 7);
	printf("■              ■");
	SetCurrentCursorPos(93, 8);
	printf("■               ■");
	SetCurrentCursorPos(93, 9);
	printf("■                ■");
	SetCurrentCursorPos(93, 10);
	printf("■                ■");
	SetCurrentCursorPos(93, 11);
	printf("■                ■");
	SetCurrentCursorPos(93, 12);
	printf("■               ■");
	SetCurrentCursorPos(93, 13);
	printf("■              ■");
	SetCurrentCursorPos(93, 14);
	printf("■            ■");
	SetCurrentCursorPos(93, 15);
	printf("■          ■");
	SetCurrentCursorPos(93, 16);
	printf("■■■■■■");

	getchar();
}

void startGame()
{
	int a = 0, i = 0, q = 0;
	system("color F0");
	srand(time(NULL)); //r 난수 생성

	NPCAttack_rand = 101;
	
	setStage();
	PC_pos.X = 10;
	PC_pos.Y = 14;
	
	SetCurrentCursorPos(text_pos.X, text_pos.Y);
	printf("<조작법 설명>\n");
	SetCurrentCursorPos(text_pos.X, text_pos.Y + 2);
	printf("a: 공격\n");
	SetCurrentCursorPos(text_pos.X, text_pos.Y + 3);
	printf("space bar: 점프\n");
	SetCurrentCursorPos(text_pos.X, text_pos.Y + 5);
	printf("<아이템 설명>\n");
	SetCurrentCursorPos(text_pos.X, text_pos.Y + 6);
	printf("㉠: 총, 무기를 총으로 바꿔줍니다.\n");
	SetCurrentCursorPos(text_pos.X, text_pos.Y + 7);
	printf("◎: 프라이팬, 무기를 프라이팬으로 바꿔줍니다.\n");
	SetCurrentCursorPos(text_pos.X, text_pos.Y + 8);
	printf(" D: 활, 무기를 활로 바꿔줍니다.\n");
	SetCurrentCursorPos(text_pos.X, text_pos.Y + 9);
	printf("★: 무적, 5초간 무적 상태가 됩니다.\n");
	SetCurrentCursorPos(text_pos.X, text_pos.Y + 10);
	printf("    장애물, NPC와 닿아도 체력이 깎이지 않습니다.\n");

	cnt = 0;
	life = 3;
	stage_num = 1;
	while (cnt <= 220) {
		/*
		if (life <= 0) {
			DrawLife();
			return;
		}
		*/
		if (q == 0)
		{
			q++;
			PlaySound(TEXT("Main Theme (online-audio-converter.com).wav"), NULL, SND_ASYNC | SND_LOOP);
		}
		DrawLife();
		Now_progress();
		GboardUpdate(cnt);
		DrawGboard();
		PC_down();
		NPC_Attack();

		DrawPC();

		DetectCollisionPC_NPCAttack();
		DetectCollision_PC_Barrier();
		DetectCollision_PC_Item();
		ProcessKeyInput();

		Sleep(speed);

		cnt++;

	}


	q = 0;
	PlaySound(TEXT("clear.wav"), NULL, SND_ASYNC);
	stage_clear();

	setStage2();
	PC_pos.X = 10;
	PC_pos.Y = 14;
	stage_num = 2;
	cnt = 0;
	life = 3;
	while (cnt <= 281 + 10) {
		if (q == 0)
		{
			q++;
			PlaySound(TEXT("Main Theme (online-audio-converter.com).wav"), NULL, SND_ASYNC | SND_LOOP);
		}
		/*
		if (life <= 0) {
			DrawLife();
			return;
		}
		*/
		DrawLife();
		Now_progress();
		GboardUpdate(cnt);
		DrawGboard();
		PC_down();
		NPC_Attack();

		DrawPC();

		DetectCollisionPC_NPCAttack();
		DetectCollision_PC_Barrier();
		DetectCollision_PC_Item();
		ProcessKeyInput();

		Sleep(speed);

		cnt++;

	}
	
	q = 0;
	PlaySound(TEXT("clear.wav"), NULL, SND_ASYNC);
	stage_clear();
	setStage3();
	PC_pos.X = 10;
	PC_pos.Y = 14;
	life = 3;
	cnt = 0;
	while (cnt <= 415) {
		if (q == 0)
		{
			q++;
			PlaySound(TEXT("Main Theme (online-audio-converter.com).wav"), NULL, SND_ASYNC | SND_LOOP);
		}
		/*
		if (life <= 0) {
			DrawLife();
			return;
		}
		*/
		DrawLife();
		Now_progress();
		GboardUpdate(cnt);
		DrawGboard();
		PC_down();
		NPC_Attack();

		DrawPC();

		DetectCollisionPC_NPCAttack();
		DetectCollision_PC_Barrier();
		DetectCollision_PC_Item();
		ProcessKeyInput();

		Sleep(speed);

		cnt++;

	}
	system("cls");
	DrawEnding();
	return;
}

void AboutPhoneEscape() {

	SetCurrentCursorPos(GBOARD_ORIGIN_X, 3);
	printf("<BACKGROUND STORY>\n");
	SetCurrentCursorPos(GBOARD_ORIGIN_X, 4);
	printf("현재 시각 새벽 3시 30분, \n");
	SetCurrentCursorPos(GBOARD_ORIGIN_X, 5);
	printf("오늘도 이 시간이 되어서야 폰을 내려놓고 잘 준비를 한다.\n");
	SetCurrentCursorPos(GBOARD_ORIGIN_X, 6);
	printf("SNS와 각종 영상들을 보다보면 시간 가는 줄 모른다.\n");
	SetCurrentCursorPos(GBOARD_ORIGIN_X, 7);
	printf("자다가 눈을 떴는데... 여긴 어디지?\n");
	SetCurrentCursorPos(GBOARD_ORIGIN_X, 8);
	printf("- 달려라, 그러면 살 것이다.\n");
	SetCurrentCursorPos(GBOARD_ORIGIN_X, 9);
	printf("갑자기 핸드폰 안에 빠져버린 당신!\n");
	SetCurrentCursorPos(GBOARD_ORIGIN_X, 10);
	printf("과연 모든 스테이지를 클리어하고 원래 세계로 돌아갈 수 있을 것인가?\n");
	SetCurrentCursorPos(GBOARD_ORIGIN_X, 11);
	printf("<개발자 소개>\n");
	SetCurrentCursorPos(GBOARD_ORIGIN_X, 12);
	textcolor(4, 15);
	printf("최 강 경 영 학 과\n");
	SetCurrentCursorPos(GBOARD_ORIGIN_X, 13);
	textcolor(0, 15);
	printf("16010112 권나현\n");
	SetCurrentCursorPos(GBOARD_ORIGIN_X, 14);
	printf("16010120 임승연\n");
	SetCurrentCursorPos(GBOARD_ORIGIN_X, 15);
	printf("16010122 황희선\n");
	SetCurrentCursorPos(GBOARD_ORIGIN_X, 16);
	printf("p(´∇｀)q 예쁘게 봐주세요 p(´∇｀)q \n");
	SetCurrentCursorPos(GBOARD_ORIGIN_X, 17);
	printf("메뉴로 돌아가기 >> B\n");
	int key;
	while (1) {
		key = _getch();
		if (key == BACK)
		{
			break;
		}
	}
	for (int y = 0; y < 15; y++)
	{
		SetCurrentCursorPos(GBOARD_ORIGIN_X, 3 + y);
		for (int x = 0; x < 70; x++)
		{
			printf(" ");
		}
	}
	menu();
	menuKeyInput();
}


void showRule() {

	SetCurrentCursorPos(GBOARD_ORIGIN_X, 3);
	printf("<조작법 설명>\n");
	SetCurrentCursorPos(GBOARD_ORIGIN_X, 4);
	printf("a: 공격\n");
	SetCurrentCursorPos(GBOARD_ORIGIN_X, 5);
	printf("space bar: 점프\n");
	SetCurrentCursorPos(GBOARD_ORIGIN_X, 6);
	printf("<아이템 설명>\n");
	SetCurrentCursorPos(GBOARD_ORIGIN_X, 7);
	printf("㉠: 총, 무기를 총으로 바꿔줍니다.\n");
	SetCurrentCursorPos(GBOARD_ORIGIN_X, 8);
	printf("◎: 프라이팬, 무기를 프라이팬으로 바꿔줍니다.\n");
	SetCurrentCursorPos(GBOARD_ORIGIN_X, 9);
	printf(" D: 활, 무기를 활로 바꿔줍니다.\n");
	SetCurrentCursorPos(GBOARD_ORIGIN_X, 10);
	printf("★: 무적, 5초간 무적 상태가 됩니다.\n");
	SetCurrentCursorPos(GBOARD_ORIGIN_X, 11);
	printf("    장애물, NPC와 닿아도 체력이 깎이지 않습니다.\n");
	SetCurrentCursorPos(GBOARD_ORIGIN_X, 12);
	printf("<NPC 설명> - SNS의 형상을 본 따 제작\n");
	SetCurrentCursorPos(GBOARD_ORIGIN_X, 13);
	printf("■■■    ■■■ \n");
	SetCurrentCursorPos(GBOARD_ORIGIN_X, 14);
	printf("■▷■    ■○■         ⓕ\n");
	SetCurrentCursorPos(GBOARD_ORIGIN_X, 15);
	printf("■■■    ■■■\n");
	SetCurrentCursorPos(GBOARD_ORIGIN_X, 16);
	printf("유투브   인스타그램   페이스북\n");
	SetCurrentCursorPos(GBOARD_ORIGIN_X, 17);
	printf("메뉴로 돌아가기 >> B\n");
	int key;
	while (1) {
		key = _getch();
		if (key == BACK)
		{
			break;
		}
	}
	for (int y = 0; y < 15; y++)
	{
		SetCurrentCursorPos(GBOARD_ORIGIN_X, 3 + y);
		for (int x = 0; x < 70; x++)
		{
			printf(" ");
		}
	}
	menu();
	menuKeyInput();

}


void menuKeyInput() {
	int key;
	while (1) {
		key = _getch();
		switch (key) {
		case START:
			for (int y = 0; y < 15; y++)
			{
				SetCurrentCursorPos(GBOARD_ORIGIN_X, 3 + y);
				for (int x = 0; x < 70; x++)
				{
					printf(" ");
				}
			}
			startGame();
			break;
		case RULE:
			for (int y = 0; y < 15; y++)
			{
				SetCurrentCursorPos(GBOARD_ORIGIN_X, 3 + y);
				for (int x = 0; x < 70; x++)
				{
					printf(" ");
				}
			}
			showRule();
			//menuKeyInput();
			break;
		case ABOUT:
			for (int y = 0; y < 15; y++)
			{
				SetCurrentCursorPos(GBOARD_ORIGIN_X, 3 + y);
				for (int x = 0; x < 70; x++)
				{
					printf(" ");
				}
			}
			AboutPhoneEscape();
			break;
		case EXIT:
			for (int y = 0; y < 15; y++)
			{
				SetCurrentCursorPos(GBOARD_ORIGIN_X, 3 + y);
				for (int x = 0; x < 70; x++)
				{
					printf(" ");
				}
			}
			PlaySound(NULL, 0, 0);
			return;
			break;
		}
	}
}

void menu() {
	RemoveCursor();
	SetCurrentCursorPos(GBOARD_ORIGIN_X + 2, 2 + 2);
	printf("■■■  ■    ■  ■■■  ■     ■ ■■■\n");
	SetCurrentCursorPos(GBOARD_ORIGIN_X + 2, 2 + 3);
	printf("■   ■ ■    ■ ■    ■ ■■   ■ ■        GAME START>> S\n");
	SetCurrentCursorPos(GBOARD_ORIGIN_X + 2, 2 + 4);
	printf("■■■  ■■■■ ■    ■ ■ ■  ■ ■■■    RULE>> R\n");
	SetCurrentCursorPos(GBOARD_ORIGIN_X + 2, 2 + 5);
	printf("■      ■    ■ ■    ■ ■  ■ ■ ■        ABOUT PHONE ESCAPE>> A\n");
	SetCurrentCursorPos(GBOARD_ORIGIN_X + 2, 2 + 6);
	printf("■      ■    ■  ■■■  ■   ■■ ■■■    EXIT>> X\n");

	SetCurrentCursorPos(GBOARD_ORIGIN_X + 2 * 2, 2 + 10);
	printf("■■■ ■■■ ■■■   ■   ■■■  ■■■\n");
	SetCurrentCursorPos(GBOARD_ORIGIN_X + 2 * 2, 2 + 11);
	printf("■     ■     ■      ■■  ■   ■ ■                    HELP!\n");
	SetCurrentCursorPos(GBOARD_ORIGIN_X + 2 * 2, 2 + 12);
	printf("■■■ ■■■ ■     ■■■ ■■■  ■■■            ● ◎\n");
	SetCurrentCursorPos(GBOARD_ORIGIN_X + 2 * 2, 2 + 13);
	printf("■         ■ ■     ■  ■ ■      ■                ┼ │\n");
	SetCurrentCursorPos(GBOARD_ORIGIN_X + 2 * 2, 2 + 14);
	printf("■■■ ■■■ ■■■ ■  ■ ■      ■■■            ∬\n");

}


int main() {
	PlaySound(TEXT("main (online-audio-converter.com).wav"), NULL, SND_ASYNC | SND_LOOP);
	RemoveCursor();
	textcolor(0, 15);
	DrawGBox();
	menu();
	menuKeyInput();
}
