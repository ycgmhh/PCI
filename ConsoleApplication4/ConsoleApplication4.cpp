// ConsoleApplication4.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <stdio.h>
#include <windows.h>
#include "winio.h"  //导入winio头文件
//#include  <minwindef.h>
//#include <stdlib.h>
#define PCI_CONFIG_ADDRESS 0xcf8  //pci地址映射
#define PCI_CONFIG_DATA 0xcfc  //pci数据映射
#pragma comment(lib,"winio.lib")  //导入winio库文件




void printf_a();
void printfSpecDev();
void ChangeValue1();
void ChangeValue2();



int bus, dev, func,count=0;
//int bus = 00, dev = 2, func = 0;
int bus1 = 0x00, dev1 = 0x1F, func1 = 0x04;
int a[20][20], reg, i, j;
DWORD dwAddr, dwData, VID, DID;


int main(){



	int Data;

	if (InitializeWinIo())
	{
		/*读取全部*/

//		printf_a();


		/*对固定设备的读取*/

		printfSpecDev();
		ChangeValue1();
		
		/* 再次打印 */
		printfSpecDev();





		ShutdownWinIo();
	}
	else
	{
		printf("Initialize Win IO Fail\n");

	}
	return 0;
}




void printf_a()
{
	printf("VID\tDID\tBUS\tDEV\tFUNC\t\n");
	for (bus = 0; bus < 8; bus++)
	{
		for (dev = 0; dev < 32; dev++)
		{
			for (func = 0; func < 8; func++)
			{
				dwAddr = 0x80000000 + (bus << 16) + (dev << 11) + (func << 8);//bus，dev，func都是我们设定的初始值然后需要移动到32位地址寄存器中
				SetPortVal(PCI_CONFIG_ADDRESS, dwAddr, 4);//I/O口写操作，第一个端口地址，16位整变量或常量；第二个为要写入端口的32位整型变量或常量
				GetPortVal(PCI_CONFIG_DATA, &dwData, 4);//I/O读操作，第一个为端口地址，第二个为存放端口内容的32为整型变量指针
				if (dwData != 0xffffffff) //数据这里如果全“1”，表示无效
				{
					//						count++;
					VID = dwData & 0xffff;
					DID = (dwData >> 16) & 0xffff;
					printf("%04x\t%04x\t%02x\t%02x\t%02x\t\n", VID, DID, bus, dev, func);  //02,04表示不足2，4位，前面补0输出
				}
			}
		}
	}
	printf("\nFind PCI Devic %d\n", count);
}



void ChangeValue1()
{
	dwAddr = 0x80000000 + (bus1 << 17) + (dev1 << 11) + (func1 << 8) + (0x10 << 2);	//if offset , base address + (offset)
	SetPortVal(PCI_CONFIG_ADDRESS, dwAddr, 4);
	GetPortVal(PCI_CONFIG_DATA, &dwData, 4);
	printf("  \n%08x\n", dwData);
	dwData =(dwData | 0x0013) & 0x0013;				//确定dwData的值，直接赋值会使相应地址不能改变
	SetPortVal(PCI_CONFIG_DATA, dwData, 2);
	//		GetPortVal(PCI_CONFIG_DATA, &dwData, 4);
	printf("  \n%08x\n", dwData);
	printf("  \n\n\n", dwData);
}








void printfSpecDev()
{
	printf("%d    %02X  %02X  %02X  %02X  %02X  %02X  %02X  %02X  %02X  %02X  %02X  %02X  %02X  %02X  %02X  %02X\n", 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
	for (i = 1; i <= 16; i++)
	{
		a[i][0] = i - 1;
		printf("%02X0  ", a[i][0]);
		for (reg = 4 * (i - 1); reg < 4 * i; reg++)
		{
			dwAddr = 0x80000000 + (bus1 << 17) + (dev1 << 11) + (func1 << 8) + (reg << 2);
			//					printf("%04x\n", dwAddr);
			SetPortVal(PCI_CONFIG_ADDRESS, dwAddr, 4);
			GetPortVal(PCI_CONFIG_DATA, &dwData, 4);
			for (j = 4 * reg + 1; j <= 4 * reg + 4; j++)
			{
				a[i][j] = dwData & 0xff;
				dwData = (dwData >> 8);
				printf("%02X  ", a[i][j]);
			}
		}
		printf("\n");

	}
}
