#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
using std::fstream;
using std::ios;
using std::string;
typedef struct filemessage
{
	char name[200] = "";//文件名
	int namelength = 0;//文件名长度
	long size = 0;//文件大小
	long offset = 0;//文件偏移量
	filemessage* next;
}FM, *PFM;


///////////////////////////////////类/////////////////////////
class FilePack//文件打包类
{
	FM *p;
	int FileNumber;
	///打包专用数据
	string FilePath;//打包路径
	string SavePath; //保存路径

	long FMSize;//文件头大小
	long GetSize(string &str);//获得文件长度
	void Clear(void);//清空
	void UpData(void);//更新数据
					  //void NameLink(string &path,string &name);//文件名链接函数
public:
	bool open(string Path);//需要解压文件的路径
	void SetSavePath(string Path) { SavePath = Path; };//设置保存路径
	void SetPackPath(string Path) { FilePath = Path; };//设置打包路径
	bool SetFileM(const char *name, const char *name2);//修改文件名
	bool AddFileM(const char* name);//添加文件信息FileNumber++
	bool DelFileM(const char *name);//删除文件信息FileNumber--
	bool Pack(void);//开始打包
	bool OneUnpack(const char *name);//解压指定文件名
	bool Unpack(void)const;//开始解包
						   //	FilePack(string FilePath, string savepath);
	FilePack(void);
	~FilePack();
};