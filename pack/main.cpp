#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include "filepack.h"
int main(void)
{
	try {
		FilePack k;
		k.SetPackPath(".\\");
		k.SetSavePath("ccc.bin");
		k.AddFileM("1.jpg");
		k.AddFileM("2.jpg");
		k.AddFileM("3.jpg");
		k.AddFileM("4.jpg");
		k.SetFileM("1.jpg", "12.jpg");
		k.DelFileM("12.jpg");
		k.Pack();
		FilePack m;
		m.open("ccc.bin");
		m.SetSavePath(".\\11");
		m.OneUnpack("3.jpg");
		//m.Unpack();
		int i = 0;
		system("pause");

	}
	catch (char *ch)
	{
		std::cout << ch<<std::endl;
	}
	int i = 0;
	system("pause");
	return 0;
}