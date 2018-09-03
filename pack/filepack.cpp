#include "filepack.h"
long FilePack::GetSize(string & str)//����ļ���С
{
	fstream fp;
	long len;
	fp.open(str, ios::in | ios::binary);//��Դ�ļ�
	fp.seekp(0, ios::end);//�ƶ��ļ�ָ�뵽�ļ�β
	len = (long)fp.tellp();
	fp.close();
	return 	len;	//���ָ��λ��ps:�ļ�����
}
void FilePack::UpData(void)//��������
{
	PFM pl = p;
	PFM pi = p;
	FMSize = sizeof(long) + sizeof(int);
	p->offset = 0;
	for (int i = 0; i < FileNumber; i++)
	{
		FMSize += pl->namelength + \
			sizeof(long) + \
			sizeof(long) + \
			sizeof(int);
		pl = pl->next;
	}
	pl = pi = p;
	while (pl->next != NULL)
	{
		if (pl != p)
			pl->offset = pi->size + pi->offset;
		pi = pl;
		pl = pi->next;
	}
	pl->offset = pi->size + pi->offset;
	return;
}
bool FilePack::SetFileM(const char * name, const char * name2)
//�ı��ļ���
{
	PFM pl = p;
	if (p == NULL)
		return false;
	if (!strcmp(p->name, name))//���ͷ������Ҫ�޸ĵ��ļ�
	{
		strcpy(p->name, name2);
		p->namelength = strlen(name2);//�޸��ļ�����
		return true;
	}
	while ((pl->next != NULL) && strcmp(pl->name, name))
		pl = pl->next;
	if (!strcmp(pl->name, name))
	{
		strcpy(pl->name, name2);
		pl->namelength = strlen(name2);//�޸��ļ�����
		return true;
	}
	return false;
}
bool FilePack::AddFileM(const char * name)//����ļ�
{
	PFM pi = p;
	PFM pl = p;
	if (p == NULL)
	{
		p = new FM;
		if (p == NULL)//ʧ�ܷ���false
			return false;
		p->next = NULL;//��ֵΪNULL
		pl = p;
	}
	else {
		while (pl->next != NULL)//ѭ�����ұ�β
		{
			pl = pi->next;
			pi = pl;
		}
		//pi = pi->next;
		pl->next = new FM;
		if (pl == NULL)//ʧ�ܷ���false
			return false;
		pl = pl->next;
		pl->next = NULL;//��ֵΪNULL
	}
	strcpy(pl->name, name);//�����ļ���
	pl->namelength = strlen(name);//����ļ�������
	pl->size = GetSize(FilePath + '\\' + pl->name);//����Ŀ¼���ļ���
	if (pl == p)//ͷ��Ϊƫ��Ϊ0
		pl->offset = 0;
	else//����ͷ���ۼ��ϸ��ļ�ƫ�������ļ���С
		pl->offset = pi->offset + pi->size;
	FileNumber++;
	return true;
}
bool FilePack::DelFileM(const char * name)
{
	if (p == NULL)
		return false;
	PFM pi = p;
	PFM pl = p;
	if ((strcmp(p->name, name) == 0))
		//ɾ��ͷ������
	{
		pl = pl->next;
		delete p;
		p = pl;
		FileNumber--;
		return true;
	}
	do
	{
		pl = pi->next;
		if (strcmp(pl->name, name) == 0)//�����ļ�����ͬ
			break;
		pi = pl;
	} while (pl->next != NULL);//ѭ��������β��
	if (strcmp(pl->name, name) != 0)//���Ҳ���
		return false;
	pi->next = pl->next;
	delete pl;
	pl = NULL;
	FileNumber--;
	return true;
}
FilePack::FilePack()
{
	FileNumber = 0;
	p = NULL;
	//	FMSize = sizeof(long) + sizeof(int);
	//FMSize = 0;
}
bool FilePack::Pack(void)//��ʼ���
{
	fstream fp, fp1;
	PFM pl = p;
	UpData();//��������
	fp1.open(SavePath, ios::out | ios::binary);//�����ļ�
	fp1.write((char*)&FileNumber, sizeof(int));
	fp1.write((char*)&FMSize, sizeof(long));
	for (int i = 0; i < FileNumber; i++)//д���ļ�ͷ��Ϣ
	{
		fp1.write((char*)&pl->namelength, sizeof(int));//д���ļ�name����
		fp1.write(pl->name, sizeof(char)*pl->namelength);//д��name
		fp1.write((char *)&pl->size, sizeof(long));
		fp1.write((char *)&pl->offset, sizeof(long));
		pl = pl->next;
	}
	pl = p;
	for (int i = 0; i<FileNumber; i++)//д���ļ�
	{
		fp.open(FilePath + '\\' + pl->name, ios::in | ios::binary);//��Դ�ļ�
		for (int i = 0; i<pl->size; i++)//���ֽ�д���ļ�
			fp1.put(fp.get());
		pl = pl->next;
		fp.close();
	}
	fp1.close();
	return true;
}
FilePack::~FilePack()
{
	if (p != NULL)
	{
		PFM lp = p;
		do
		{
			lp = lp->next;
			delete p;
			p = lp;
		} while (lp != NULL);
		p = NULL;
	}
}

bool FilePack::open(string Path)
{
	fstream fp;
	PFM pl = NULL;
	if (p != NULL)
		Clear();
	FilePath = Path;//�������·��
	fp.open(Path, ios::in | ios::binary);
	if (!fp.is_open())
	{
		throw "�ļ���ʧ��";
	}
	fp.read((char*)&FileNumber, sizeof(int));//��ȡ�ļ�����
	fp.read((char*)&FMSize, sizeof(long));//��ȡ�ļ�ͷ��С
	for (int i = 0; i < FileNumber; i++)
	{
		if (i == 0) {
			p = new FM;
			pl = p;
		}
		else {
			pl->next = new FM;
			pl = pl->next;
		}
		fp.read((char*)&pl->namelength, sizeof(int));//��ȡ�ļ�������
		fp.read(pl->name, sizeof(char)*pl->namelength);//��ȡ�ļ���
		fp.read((char*)&pl->size, sizeof(long));//��ȡ�ļ���С
		fp.read((char*)&pl->offset, sizeof(long));//��ȡ�ļ�ƫ����
		pl->next = NULL;
	}
	pl->next = NULL;
	//pl = NULL;
	fp.close();
	return false;
}
void FilePack::Clear(void)
{
	if (p != NULL)
	{
		PFM lp = p;
		do
		{
			lp = lp->next;
			delete p;
			p = lp;
		} while (lp != NULL);
		p = NULL;
	}
	FileNumber = 0;
	FilePath = "";
	FMSize = 0;
}

bool FilePack::Unpack(void)const//���
{
	fstream fp;
	fstream fp1;
	PFM pl = p;
	fp.open(FilePath, ios::in | ios::binary);//�򿪰�
	for (int i = 0; i < FileNumber; i++)
	{
		fp1.open(SavePath + "\\" + pl->name, ios::out | ios::binary);//�򿪵����ļ�
		if (!fp1.is_open())
			throw "��ʧ��";
		fp.seekp(pl->offset + FMSize, ios::beg);//��λ���ļ���ʼλ��
		for (int j = 0; j < pl->size; j++)//��ȡָ���ֽڴ�С
			fp1.put(fp.get());
		fp1.close();
		pl = pl->next;
	}
	fp.close();
	return false;
}
bool FilePack::OneUnpack(const char * name)//�������
{
	fstream fp;
	fstream fp1;
	PFM pl = p;
	fp.open(FilePath, ios::in | ios::binary);//�򿪰�
	if (!fp.is_open())
		throw "��ʧ��";
	for (int i = 0; i < FileNumber; i++)
	{
		if (!strcmp(pl->name, name))
			break;
		pl = pl->next;
	}
	if (strcmp(pl->name, name))//û���ҵ�ָ���ļ�
		return false;
	fp1.open(SavePath + "\\" + pl->name, ios::out | ios::binary);//�򿪵����ļ�
	if (!fp1.is_open())
		throw "��ʧ��";
	fp.seekp(pl->offset + FMSize, ios::beg);//��λ���ļ���ʼλ��
	for (int j = 0; j < pl->size; j++)//��ȡָ���ֽڴ�С
		fp1.put(fp.get());
	fp1.close();
	fp.close();
	return false;
}