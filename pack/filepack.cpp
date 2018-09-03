#include "filepack.h"
long FilePack::GetSize(string & str)//获得文件大小
{
	fstream fp;
	long len;
	fp.open(str, ios::in | ios::binary);//打开源文件
	fp.seekp(0, ios::end);//移动文件指针到文件尾
	len = (long)fp.tellp();
	fp.close();
	return 	len;	//获得指针位置ps:文件长度
}
void FilePack::UpData(void)//更新数据
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
//改变文件名
{
	PFM pl = p;
	if (p == NULL)
		return false;
	if (!strcmp(p->name, name))//如果头表是需要修改的文件
	{
		strcpy(p->name, name2);
		p->namelength = strlen(name2);//修改文件长度
		return true;
	}
	while ((pl->next != NULL) && strcmp(pl->name, name))
		pl = pl->next;
	if (!strcmp(pl->name, name))
	{
		strcpy(pl->name, name2);
		pl->namelength = strlen(name2);//修改文件长度
		return true;
	}
	return false;
}
bool FilePack::AddFileM(const char * name)//添加文件
{
	PFM pi = p;
	PFM pl = p;
	if (p == NULL)
	{
		p = new FM;
		if (p == NULL)//失败返回false
			return false;
		p->next = NULL;//赋值为NULL
		pl = p;
	}
	else {
		while (pl->next != NULL)//循环查找表尾
		{
			pl = pi->next;
			pi = pl;
		}
		//pi = pi->next;
		pl->next = new FM;
		if (pl == NULL)//失败返回false
			return false;
		pl = pl->next;
		pl->next = NULL;//赋值为NULL
	}
	strcpy(pl->name, name);//设置文件名
	pl->namelength = strlen(name);//获得文件名长度
	pl->size = GetSize(FilePath + '\\' + pl->name);//链接目录和文件名
	if (pl == p)//头表为偏移为0
		pl->offset = 0;
	else//不是头表累加上个文件偏移量加文件大小
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
		//删除头表的情况
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
		if (strcmp(pl->name, name) == 0)//查找文件名相同
			break;
		pi = pl;
	} while (pl->next != NULL);//循环到链表尾部
	if (strcmp(pl->name, name) != 0)//查找不到
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
bool FilePack::Pack(void)//开始打包
{
	fstream fp, fp1;
	PFM pl = p;
	UpData();//更新数据
	fp1.open(SavePath, ios::out | ios::binary);//创建文件
	fp1.write((char*)&FileNumber, sizeof(int));
	fp1.write((char*)&FMSize, sizeof(long));
	for (int i = 0; i < FileNumber; i++)//写入文件头信息
	{
		fp1.write((char*)&pl->namelength, sizeof(int));//写入文件name长度
		fp1.write(pl->name, sizeof(char)*pl->namelength);//写入name
		fp1.write((char *)&pl->size, sizeof(long));
		fp1.write((char *)&pl->offset, sizeof(long));
		pl = pl->next;
	}
	pl = p;
	for (int i = 0; i<FileNumber; i++)//写入文件
	{
		fp.open(FilePath + '\\' + pl->name, ios::in | ios::binary);//打开源文件
		for (int i = 0; i<pl->size; i++)//逐字节写入文件
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
	FilePath = Path;//保存包的路径
	fp.open(Path, ios::in | ios::binary);
	if (!fp.is_open())
	{
		throw "文件打开失败";
	}
	fp.read((char*)&FileNumber, sizeof(int));//读取文件数量
	fp.read((char*)&FMSize, sizeof(long));//读取文件头大小
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
		fp.read((char*)&pl->namelength, sizeof(int));//读取文件名长度
		fp.read(pl->name, sizeof(char)*pl->namelength);//读取文件名
		fp.read((char*)&pl->size, sizeof(long));//读取文件大小
		fp.read((char*)&pl->offset, sizeof(long));//读取文件偏移量
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

bool FilePack::Unpack(void)const//解包
{
	fstream fp;
	fstream fp1;
	PFM pl = p;
	fp.open(FilePath, ios::in | ios::binary);//打开包
	for (int i = 0; i < FileNumber; i++)
	{
		fp1.open(SavePath + "\\" + pl->name, ios::out | ios::binary);//打开单个文件
		if (!fp1.is_open())
			throw "打开失败";
		fp.seekp(pl->offset + FMSize, ios::beg);//定位到文件开始位置
		for (int j = 0; j < pl->size; j++)//读取指定字节大小
			fp1.put(fp.get());
		fp1.close();
		pl = pl->next;
	}
	fp.close();
	return false;
}
bool FilePack::OneUnpack(const char * name)//单个解包
{
	fstream fp;
	fstream fp1;
	PFM pl = p;
	fp.open(FilePath, ios::in | ios::binary);//打开包
	if (!fp.is_open())
		throw "打开失败";
	for (int i = 0; i < FileNumber; i++)
	{
		if (!strcmp(pl->name, name))
			break;
		pl = pl->next;
	}
	if (strcmp(pl->name, name))//没有找到指定文件
		return false;
	fp1.open(SavePath + "\\" + pl->name, ios::out | ios::binary);//打开单个文件
	if (!fp1.is_open())
		throw "打开失败";
	fp.seekp(pl->offset + FMSize, ios::beg);//定位到文件开始位置
	for (int j = 0; j < pl->size; j++)//读取指定字节大小
		fp1.put(fp.get());
	fp1.close();
	fp.close();
	return false;
}