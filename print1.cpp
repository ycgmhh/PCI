#include <stdio.h>  
#include <windows.h>  
#include <conio.h>  

HANDLE handle_in;
HANDLE handle_out;
CONSOLE_SCREEN_BUFFER_INFO csbi;        //定义窗口缓冲区信息结构体  

void DisplayMousePosition(COORD pos);   //显示鼠标所在位置  

void gotoxy(int x, int y);  //将光标移到坐标为(x,y)的位置  

int main()
{
	handle_in = GetStdHandle(STD_INPUT_HANDLE);      //获得标准输入设备句柄  
	handle_out = GetStdHandle(STD_OUTPUT_HANDLE);    //获得标准输出设备句柄  
	INPUT_RECORD mouserec;      //定义输入事件结构体  
	DWORD res;      //用于存储读取记录  
	COORD pos;      //用于存储鼠标当前位置  
	COORD size = { 80, 25 };  //窗口缓冲区大小  
	GetConsoleScreenBufferInfo(handle_out, &csbi);  //获得窗口缓冲区信息  
	SetConsoleScreenBufferSize(handle_out, size);   //设置窗口缓冲区大小  
	for (;;)
	{
		ReadConsoleInput(handle_in, &mouserec, 1, &res);      //读取输入事件  
		pos = mouserec.Event.MouseEvent.dwMousePosition;    //获得当前鼠标位置  
		gotoxy(0, 24);  //在第25行显示鼠标位置  
		DisplayMousePosition(pos);      //显示鼠标位置  
		if (mouserec.EventType == MOUSE_EVENT)    //如果当前为鼠标事件  
		{
			gotoxy(pos.X, pos.Y);
			//单击鼠标左键，输出字符A  
			if (mouserec.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
			{
				putchar('A');
			}
			//单击鼠标右键，输出字符B  
			if (mouserec.Event.MouseEvent.dwButtonState == RIGHTMOST_BUTTON_PRESSED)
			{
				putchar('B');
			}
			//双击退出  
			if (mouserec.Event.MouseEvent.dwEventFlags == DOUBLE_CLICK)
			{
				break;
			}
		}
	}
	CloseHandle(handle_out);
	CloseHandle(handle_in);
	return 0;
}

void DisplayMousePosition(COORD pos)
{
	COORD dis = { 0, 24 };        //在第24行显示鼠标位置  
	WORD att = FOREGROUND_GREEN | FOREGROUND_INTENSITY; //文本属性  
	GetConsoleScreenBufferInfo(handle_out, &csbi);  //获得窗口缓冲区信息  
	printf("X = %3d, Y = %3d", (int)pos.X, (int)pos.Y);
	FillConsoleOutputAttribute(handle_out, att, 16, dis, NULL);  //填充文本属性  
	return;
}

void gotoxy(int x, int y)
{
	COORD pos = { x, y };
	SetConsoleCursorPosition(handle_out, pos);
}