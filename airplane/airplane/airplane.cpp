// ConsoleApplication2.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <stdlib.h>
#include <conio.h>

#include <stdio.h>
#include <windows.h>

// 全局变量
int position_x, position_y; // 飞机位置
int bullet_x, bullet_y; // 子弹位置
int enemy_x, enemy_y; // 敌机位置
int high, width; //  游戏画面尺寸
int score; // 得分

void gotoxy(int x, int y) //类似于清屏函数，光标移动到原点位置进行重画
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos;
	pos.X = x;
	pos.Y = y;
	SetConsoleCursorPosition(handle, pos);
}

void startup() // 数据初始化
{
	high = 20;
	width = 30;
	position_x = high / 2;
	position_y = width / 2;
	bullet_x = -2;
	bullet_y = position_y;
	enemy_x = 0;
	enemy_y = position_y;
	score = 0;
}

void show() // 显示画面
{
	gotoxy(0, 0); // 光标移动到原点位置进行重画清屏
	for (int i = 0; i < high; i++)
	{
		for (int j = 0; j < width; j++)
		{
			if ((i == position_x) && (j == position_y))
				printf("*"); //   输出飞机*
			else if ((i == enemy_x) && (j == enemy_y))
				printf("@"); //   输出敌机@
			else if ((i == bullet_x) && (j == bullet_y))
				printf("|"); //   输出子弹|
			else
				printf(" "); //   输出空格
		}
		printf("\n");
	}
	printf("得分：%d\n", score);
}

void updateWithoutInput() // 与用户输入无关的更新
{
	if (bullet_x > -1)
		bullet_x--;

	if ((bullet_x == enemy_x) && (bullet_y == enemy_y)) // 子弹击中敌机
	{
		score++; // 分数加1
		enemy_x = -1; // 产生新的飞机
		enemy_y = rand() % width;
		bullet_x = -2; // 子弹无效
	}
	if (enemy_x > high) // 敌机跑出显示屏幕
	{
		enemy_x = -1; // 产生新的飞机
		enemy_y = rand() % width;
	}

	// 用来控制敌机向下移动的速度。每隔几次循环，才移动一次敌机
	// 这样修改的话，用户按键交互速度还是保持很快，但我们NPC的移动显示可以降速
	static int speed = 0;
	if (speed < 20)
		speed++;
	if (speed == 20)
	{
		enemy_x++;
		speed = 0;
	}
}

void updateWithInput() // 与用户输入有关的更新
{
	if (_kbhit()) // 判断是否有输入
	{
		const char input = _getch(); // 根据用户的不同输入来移动，不必输入回车
		if (input == 'a')
			position_y--; // 位置左移
		if (input == 'd')
			position_y++; // 位置右移
		if (input == 'w')
			position_x--; // 位置上移
		if (input == 's')
			position_x++; // 位置下移
		if (input == ' ') // 发射子弹
		{
			bullet_x = position_x - 1; // 发射子弹的初始位置在飞机的正上方
			bullet_y = position_y;
		}
	}
}


int _tmain(int argc, _TCHAR* argv[])
{
	startup(); // 数据初始化	
	while (true) //  游戏循环执行
	{
		show(); // 显示画面
		updateWithoutInput(); // 与用户输入无关的更新
		updateWithInput(); // 与用户输入有关的更新
	}
}
