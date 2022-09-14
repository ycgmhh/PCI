#include <stdio.h>  
#include <windows.h>  
#include <conio.h>  

HANDLE handle_in;
HANDLE handle_out;
CONSOLE_SCREEN_BUFFER_INFO csbi;        //���崰�ڻ�������Ϣ�ṹ��  

void DisplayMousePosition(COORD pos);   //��ʾ�������λ��  

void gotoxy(int x, int y);  //������Ƶ�����Ϊ(x,y)��λ��  

int main()
{
	handle_in = GetStdHandle(STD_INPUT_HANDLE);      //��ñ�׼�����豸���  
	handle_out = GetStdHandle(STD_OUTPUT_HANDLE);    //��ñ�׼����豸���  
	INPUT_RECORD mouserec;      //���������¼��ṹ��  
	DWORD res;      //���ڴ洢��ȡ��¼  
	COORD pos;      //���ڴ洢��굱ǰλ��  
	COORD size = { 80, 25 };  //���ڻ�������С  
	GetConsoleScreenBufferInfo(handle_out, &csbi);  //��ô��ڻ�������Ϣ  
	SetConsoleScreenBufferSize(handle_out, size);   //���ô��ڻ�������С  
	for (;;)
	{
		ReadConsoleInput(handle_in, &mouserec, 1, &res);      //��ȡ�����¼�  
		pos = mouserec.Event.MouseEvent.dwMousePosition;    //��õ�ǰ���λ��  
		gotoxy(0, 24);  //�ڵ�25����ʾ���λ��  
		DisplayMousePosition(pos);      //��ʾ���λ��  
		if (mouserec.EventType == MOUSE_EVENT)    //�����ǰΪ����¼�  
		{
			gotoxy(pos.X, pos.Y);
			//����������������ַ�A  
			if (mouserec.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
			{
				putchar('A');
			}
			//��������Ҽ�������ַ�B  
			if (mouserec.Event.MouseEvent.dwButtonState == RIGHTMOST_BUTTON_PRESSED)
			{
				putchar('B');
			}
			//˫���˳�  
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
	COORD dis = { 0, 24 };        //�ڵ�24����ʾ���λ��  
	WORD att = FOREGROUND_GREEN | FOREGROUND_INTENSITY; //�ı�����  
	GetConsoleScreenBufferInfo(handle_out, &csbi);  //��ô��ڻ�������Ϣ  
	printf("X = %3d, Y = %3d", (int)pos.X, (int)pos.Y);
	FillConsoleOutputAttribute(handle_out, att, 16, dis, NULL);  //����ı�����  
	return;
}

void gotoxy(int x, int y)
{
	COORD pos = { x, y };
	SetConsoleCursorPosition(handle_out, pos);
}