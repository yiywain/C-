#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
using std::fstream;
using std::ios;
using std::string;
typedef struct filemessage
{
	char name[200] = "";//�ļ���
	int namelength = 0;//�ļ�������
	long size = 0;//�ļ���С
	long offset = 0;//�ļ�ƫ����
	filemessage* next;
}FM, *PFM;


///////////////////////////////////��/////////////////////////
class FilePack//�ļ������
{
	FM *p;
	int FileNumber;
	///���ר������
	string FilePath;//���·��
	string SavePath; //����·��

	long FMSize;//�ļ�ͷ��С
	long GetSize(string &str);//����ļ�����
	void Clear(void);//���
	void UpData(void);//��������
					  //void NameLink(string &path,string &name);//�ļ������Ӻ���
public:
	bool open(string Path);//��Ҫ��ѹ�ļ���·��
	void SetSavePath(string Path) { SavePath = Path; };//���ñ���·��
	void SetPackPath(string Path) { FilePath = Path; };//���ô��·��
	bool SetFileM(const char *name, const char *name2);//�޸��ļ���
	bool AddFileM(const char* name);//����ļ���ϢFileNumber++
	bool DelFileM(const char *name);//ɾ���ļ���ϢFileNumber--
	bool Pack(void);//��ʼ���
	bool OneUnpack(const char *name);//��ѹָ���ļ���
	bool Unpack(void)const;//��ʼ���
						   //	FilePack(string FilePath, string savepath);
	FilePack(void);
	~FilePack();
};