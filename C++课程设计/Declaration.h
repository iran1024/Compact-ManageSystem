#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

#pragma region 定义全局变量
const char *_ptr[] = { _T("◆ 公司管理"),"◆ 合同管理","◆ 输出所有合同信息",
"◆ 文件操作","◆ 合同排序",NULL };
static int g_count = 0;			// 合同数量
static int index = 0;			// 标记是否存在链表
static char CompanyName[20] = { 0 };
static char CompanyPass[12] = { 0 };
static char name[20] = { 0 };	// 保存从文件读取的公司名称
IMAGE _img;

#pragma endregion

inline void UI_X()				// 重绘
{
	cleardevice();
	loadimage(&_img, "./res/111.jpg", 640, 450);
	putimage(0, 0, &_img);
	int y = 10, index = 0;

	while (_ptr[index] != NULL)
	{
		outtextxy(240, 80 + y, _ptr[index++]);
		y += 30;
	}
	settextstyle(25, 0, _T("华文楷体"));
	outtextxy(50, 30, "公司名称:");
	outtextxy(170, 30, CompanyName);
	settextstyle(15, 0, _T("新宋体"));
}
void CreateWindowAX()			// 创建窗口
{
	HWND hWnd = initgraph(640, 450);
	SetWindowText(hWnd, _T("企业合同管理系统"));
	setbkmode(TRANSPARENT);
	settextcolor(BLACK);
	settextstyle(15, 0, _T("新宋体"));
	UI_X();
}
__int64 GetSecretKey(char _pass[])				// 获取密钥
{
	__int64 key = 9989979949959;
	char pass[12] = "";
	strcpy(pass, _pass);
	unsigned int len = strlen(pass);
	
	for (int i = 0; i < len; i++)
		key ^= pass[i] | 128;
	return key;
}	
char* ECP(char pass[])							// 加密
{
	int len = strlen(pass);
	__int64 key = GetSecretKey(pass);
	for (int i = 0; i < len; i++)
	{
		pass[i] ^= key;
		pass[i] ^= len;
	}
	return pass;
}
char* DCP(char pass[])							// 解密
{
	int len = strlen(pass);
	__int64 key = GetSecretKey(pass);
	for (int i = 0; i < len; i++)
	{
		pass[i] ^= key;
		pass[i] ^= len;
	}
	return pass;
}
void BindCompany()			// 公司绑定
{
loop:if ((InputBox(CompanyName, 20, "请输入公司名称:", "企业合同管理系统第一次使用,请先绑定公司...", NULL, 0, 0, false) == IDOK))
	{
		if (strcmp(CompanyName, "") == 0)
		{
			if ((MessageBox(NULL, "请输入正确的公司名称...", "温馨提示", MB_OKCANCEL | MB_ICONINFORMATION) == IDOK))
				goto loop;
			else
				exit(-1);
		}
	pp:if ((InputBox(CompanyPass, 11, "请输入密码:", "企业合同管理系统第一次使用,请先设置密码", NULL, 0, 0, false) == IDOK))
		{
			if (strcmp(CompanyPass, "") == 0)
			{
				if ((MessageBox(NULL, "请输入正确的密码...", "温馨提示", MB_OKCANCEL | MB_ICONINFORMATION) == IDOK))
					goto pp;
				else
					exit(-1);
			}
		}
		char *IndexPass = ECP(CompanyPass);
		ofstream record("company", ios::out);
		record <<  CompanyName << "\t" << IndexPass;
		record.close();
	}
	else
		exit(-1);
}
void VerifyIdentiy()			// 身份验证
{
	ifstream in("company", ios::in);
	if (!in)
	{
		if ((MessageBox(NULL, "尚未绑定公司,请按确定进行绑定操作...", "温馨提示", MB_OKCANCEL | MB_ICONINFORMATION)) == IDOK)
			BindCompany();
		else
			exit(-1);
	}
	else
	{
		char _pass[12] = "";
		while (!in.eof())
		{
			in >> CompanyName >> _pass;
		}
		char password[12] = { 0 };
		char *p = DCP(_pass);	// 将从文件中读取出来的已加密的乱码解析成原密码
		int time = 1;
	loop:if ((InputBox(password, 11, "请输入密码:", "企业合同管理系统安全检测...", NULL, 0, 0, false) == IDOK))
		{
			if (strcmp(password,p) != 0)
			{
				if (time == 3)
				{
					MessageBox(NULL, "错误次数已达到三次,禁止使用...", "温馨提示", MB_OK | MB_ICONERROR);
					exit(-1);
				}
				if ((MessageBox(NULL, "请输入正确密码...", "温馨提示", MB_OKCANCEL | MB_ICONINFORMATION) == IDOK))
				{
					time++;
					goto loop;
				}
				else
					exit(-1);
			}
		}
		 else
			 exit(-1);	
	}
	in.close();
}
#pragma region 类/结构定义
typedef struct Date
{
	short year, month, day;
}Date;
typedef struct com
{
	string id;						// 合同编号
	string name;					// 合同名称
	string firstparty, secondparty;	// 甲方,乙方
	Date time;						// 签约时间
	char lv;						// 合同等级
	char *path;						// 合同路径
}Com;
class Compact
{
private:
	Com c;
public:
	Compact *pPrior, *pNext;			// 指向前一个结点与后一个结点
	Compact();
	Compact(string id, string na, string first, string second, Date t, char *p);
	~Compact();
	void InputCompactInfo();			// 输入合同信息
	void SetInfo(string i, string na, string fi, string se, Date t, char l);// 设置基本信息
	void SetPath(char *p);				// 设置路径
	void SetID(string i);				// 设置编号
	void ReplaceInfo(short choice);		// 修改信息
	void ShowHStuData();				// 显示此合同数据
	string GetID();						// 得到合同的编号
	string GetName();					// 得到合同的名称
	string GetFirst();					// 得到合同的甲方
	string GetSecond();					// 得到合同的乙方
	char* GetPath(char *p);				// 得到合同的路径
	Date GetTime();						// 得到合同的签约时间
	char GetLv();						// 得到合同的签约等级
	Compact *GetAddress();				// 得到当前结点的地址
	void SetNextAddress(Compact *a);	// 设置当前结点的下一个结点的地址
	bool IsAbleMatchingTxt(Compact* r, string key);// 是否能够匹配关键字
};
typedef Compact* pCompact;
class Company
{
	pCompact pHead;
public:
	void CreateList();					// 创建链表
	void ManageCompany();				// 管理公司
	void BindingCompany();				// 绑定公司
	void DeBindingCompany();			// 解除绑定
	void Show_All_CompactData();		// 显示所有合同信息
	void ManageCompact();				// 管理合同(增删改查)
	void AddCompact();					// 添加合同
	void FileOperate();					// 文件操作
	void SortCompact();					// 合同排序
	void WriteCompactTxt();				// 编写合同正文
	void ViewCompactTxt();				// 查看合同正文
	pCompact SearchCompact(pCompact re,string i);// 查找合同
	pCompact SearchTxt(pCompact re, string ip, pCompact he);
	void DealTxtSearch();
	void SearchMenu();					// 查找菜单
	bool IsexistsID(string id);			// 检查是否存在相同的ID
	void DealSearchRes();				// 处理查找后的结果
	void DeleteData();					// 删除合同信息
	pCompact GetComPosition(string pos);// 获取合同定位
	pCompact GetEndPosition();			// 获取最后一个合同定位
	void Extracting_From_File();		// 将数据从硬盘提取到内存
	void Storage_To_File();				// 将数据从内存存放到硬盘
	bool CreateBinaryCatch();			// 将数据以二进制缓存到硬盘已提高读写速度
	bool Sort(short index);				// 按编号,名称或等级排序
	void changeCompact();				// 修改合同信息
	pCompact GetpHead();				// 得到链表头结点

};
#pragma endregion

#pragma region 定义函数
//void swap(pCompact a, pCompact b)
//{
//	a->pPrior->pNext = b;
//	b->pPrior->pNext = a;
//	pCompact pTemp = new Compact;
//	//pTemp->SetInfo(a->GetID(), a->GetName(),a->GetFirst(),a->GetSecond(),a->GetTime(),a->GetLv());
//	pTemp->pNext = a->pNext;
//	pTemp->pPrior = a->pPrior;
//
//	//a->SetInfo(b->GetID(), b->GetName(), b->GetFirst(), b->GetSecond(), b->GetTime(), b->GetLv());
//	a->pNext = b->pNext;
//	a->pPrior = b->pPrior;
//
//	//b->SetInfo(pTemp->GetID(), pTemp->GetName(), pTemp->GetFirst(), pTemp->GetSecond(), pTemp->GetTime(), pTemp->GetLv());
//	b->pNext = pTemp->pNext;
//	b->pPrior = pTemp->pPrior;
//
//	if (a->pNext == NULL)
//	{
//		a->pNext = b->pNext;
//		b->pNext = NULL;
//	}
//}
vector<string> split(string strTime)	// 重写vector库中的split函数便于替代正则表达式进行字符串的条件分割(提高速度)
{
	vector<string> result;
	string temp("");
	strTime += '-';//字符串结束标记，方便将最后一个单词入vector  
	for (size_t i = 0; i < strTime.size(); i++)
	{
		if (strTime[i] == '-' || strTime[i] == '.' || strTime[i] == ' ' || strTime[i] == ':')
		{
			result.push_back(temp);
			temp = "";
		}
		else
		{
			temp += strTime[i];
		}
	}
	return result;
}
void Exception_HandlingX(char* str)		// 异常处理函数
{
	if (MessageBox(NULL, str, "温馨提示...", MB_OK | MB_ICONERROR) == IDOK)
		return;
}
pCompact getSeparator(pCompact pBegin, pCompact pEnd)		// 找到分割点
{
	pCompact p = pBegin;
	pCompact q = pBegin->pNext;
	if (p == NULL)
		return NULL;
	string key = p->GetID();
	while (q != pEnd)
	{
		if (q->GetID() < key)
		{
			p = p->pNext;
			swap(p, q);
		}
		q = q->pNext;
	}
	swap(pBegin, p);
	return p;
}
void QuickSort(pCompact pBegin, pCompact pEnd)
{
	if (pBegin != pEnd)
	{
		pCompact separator = getSeparator(pBegin, pEnd);
		QuickSort(pBegin, separator);
		QuickSort(separator->pNext, pEnd);
	}
}

#pragma region Company类函数定义
																	// Company类函数定义
inline void Company::Show_All_CompactData()
{
	pCompact p = pHead->pNext;
	if (p == NULL)
	{
		Exception_HandlingX(const_cast<char*>("暂无数据..."));
		return;
	}
	closegraph();
	printf("编号\t名称\t甲方\t乙方\t签约时间\t合同等级\n");
	while (p != NULL)
	{
		p->ShowHStuData();
		p = p->pNext;
	}
	cout << "\n\n\n\n\t\t\t请按任意键返回...\n";
	_getch();
	CreateWindowAX();
	system("cls");
}
inline void Company::ManageCompact()
{
	closegraph();
loop:system("cls");
	cout << "\n\n\t\t\t\t1.添加合同";
	cout << "\n\n\t\t\t\t2.删除合同";
	cout << "\n\n\t\t\t\t3.修改合同";
	cout << "\n\n\t\t\t\t4.查找合同";
	cout << "\n\n\t\t\t\t5.编写合同正文";
	cout << "\n\n\t\t\t\t6.查看合同正文";
	cout << "\n\n\t\t\t\t0.返回\n";
	int choice = 0;
	while (1)
	{
		tp:cout << "请选择:";
		cin >> choice;
		switch (choice)
		{
			case 1:AddCompact(); break;
			case 2:DeleteData(); break;
			case 3:changeCompact(); goto loop; break;
			case 4:SearchMenu(); goto loop; break;
			case 5:WriteCompactTxt(); break;
			case 6:ViewCompactTxt(); goto loop; break;
			case 0:CreateWindowAX(); system("cls"); return;
			default: cout << "\n\n\t请输入正确的选项...\n"; goto tp; break;
		}
	}
}
inline void Company::AddCompact()
{
	pCompact pTail = GetEndPosition();						// 创建一个指向最后一个结点的指针
	int StuNum = 0;
	cout << "请输入合同的数量:";
	cin >> StuNum;
	if (StuNum == 0)
	{
		cout << "\n\t取消添加..." << endl;
		return;
	}
	cout << "请先输入合同的编号,以检查编号是否存在:";
	for (int i = 1; i <= StuNum; i++)
	{
		pCompact pNew = new Compact;	// 创建一个新结点
		string id;
lp:		cin >> id;
		if (IsexistsID(id))
		{
			cout << "已存在的编号,请重新输入编号:";
			goto lp;
		}
		cout << "请再依次输入名称, 甲方, 乙方, 签约时间和合同等级(以空格隔开) :\n";
		pNew->InputCompactInfo();
		pNew->pNext = pTail->pNext;
		pTail->pNext = pNew;
		pNew->pPrior = pTail;
		pTail = pNew;
		cout << "成功..." << endl;
	}
	g_count += StuNum;
}
inline void Company::FileOperate()
{
	closegraph();
	cout << "\n\n\t\t\t\t1.存储到文件\n";
	cout << "\n\n\t\t\t\t2.从文件读取\n";
	cout << "\n\n\t\t\t\t0.返回\n";
	int choice = 0;
	while (1)
	{
	tp:	cout << "请选择:";
		cin >> choice;
		switch (choice)
		{
			case 1:Storage_To_File(); break;
			case 2:Extracting_From_File(); break;
			case 0:CreateWindowAX(); system("cls"); return;
			default: cout << "\n\n\t请输入正确的选项...\n"; goto tp; break;
		}
	}
}
inline void Company::SortCompact()
{
	closegraph();
	cout << "\n\n\t\t\t\t1.按编号排序";
	cout << "\n\n\t\t\t\t2.按名称排序";
	cout << "\n\n\t\t\t\t3.按等级排序";
	cout << "\n\n\t\t\t\t0.返回\n";
	int choice = 0;
	while (1)
	{
	lp:cout << "请选择:";
	gp:cin >> choice;
		switch (choice)
		{
			case 1:if (!Sort(1)) goto lp; break;			// 按照编号排序
			case 2:if (!Sort(2)) goto lp; break;			// 按照名称排序	因为名称为中文,而编码方式为多字节编码,只有GBK编码排序才会按照读音顺序
			case 3:if (!Sort(3)) goto lp; break;			// 按照等级排序
			case 0:CreateWindowAX(); system("cls"); return;
			default: cout << "\n\n\t请输入正确的选项...\n"; goto gp; break;
		}
		cout << "\n\t排序成功...\n";
	}
}
inline void Company::DealSearchRes()
{
	string i;
	cout << "请输入查找的编号:";
	cin >> i;
	pCompact res = SearchCompact(res,i);
	if (res == NULL)
	{
		cout << "\n\t查无此数据...\n";
		return;
	}
	printf("编号\t名称\t甲方\t乙方\t签约时间\t合同等级\n");
	res->ShowHStuData();
}
inline void Company::DealTxtSearch()
{
	string key;
	cout << "请输入需要查找的正文关键字:";
	cin >> key;
	pCompact res = pHead->pNext;
	res = SearchTxt(res, key, res);
	if (res != NULL)
	{
		printf("编号\t名称\t甲方\t乙方\t签约时间\t合同等级\n");
		res->ShowHStuData();
	lo:	res = res->pNext;
		res = SearchTxt(res, key, res);
		if (res != NULL)
		{
			res->ShowHStuData();
			goto lo;
		}
	}
	else
		cout << "\n\t查无此数据...\n";
}
inline void Company::SearchMenu()
{
loop:system("cls");
	cout << "\n\n\t\t\t\t1.按编号查找";
	cout << "\n\n\t\t\t\t2.按正文关键字查找";
	cout << "\n\n\t\t\t\t0.返回\n";
	int choice = 0;
	while (1)
	{
		cout << "请选择:";
	 tp:cin >> choice;
		switch (choice)
		{
			case 1:DealSearchRes(); break;
			case 2:DealTxtSearch(); break;
			case 0:system("cls"); return;
			default: cout << "\n\n\t请输入正确的选项...\n"; goto tp; break;
		}
	}
}
inline pCompact Company::SearchTxt(pCompact re, string i,pCompact he)	// 第三个参数 当前所在结点
{
	Sort(1);
	pCompact p = he, s;
	while (p)
	{
		if (p->IsAbleMatchingTxt(p, i))
		{
			re = p;
			return p;
		}
		p = p->pNext;
	}
	return NULL;
}
inline bool Compact::IsAbleMatchingTxt(Compact* r,string key)
{
	string na = r->GetName() + ".txt";
	const char *path = na.c_str();
	ifstream read(path, ios::in);
	if (!read)
	{
		Exception_HandlingX(const_cast<char*>("当前没有记录该编号指定的合同数据,请先添加合同或者读取文件..."));
		return false;
	}
	char ch;
	int val = 0;
	string index = "";
	while (read.get(ch))
	{
		index += ch;	
		if (val == 2000)
		{
			if (index.find(key, 0) == -1) 
			{
				val = 0;
				index = "";
			}
			else
				return true;
		}
		val++;
	}
	return false;
}
inline void Company::DeleteData()
{
	if (pHead->pNext == NULL)
	{
		Exception_HandlingX(const_cast<char*>("当前不存在任何合同信息..."));
		return;
	}
	string pos = "";
	pCompact p;
	cout << "请输入要删除的合同的编号:";
	cin >> pos;
	p = GetComPosition(pos);
	if (p->pNext == NULL )
	{
		cout << "不存在该合同的数据..." << endl;
		return;
	}
	else
	{
		pCompact pTemp = p->pNext;
		p->pNext = p->pNext->pNext;
		if (p->pNext->pNext == NULL)
			goto loop;
		p->pNext->pNext->pPrior = p;
		loop:string p = "del " + pTemp->GetName() + ".txt";
		const char *path = p.c_str();
		system(path);
		free(pTemp);
	}
	cout << "\n\n\t删除成功...\n";
	g_count--;
}
inline pCompact Company::GetComPosition(string pos)
{
	pCompact p = pHead;
	while (p->pNext != NULL && p->pNext->GetID() != pos)
	{
		p = p->pNext;
	}
	return p;
}
inline pCompact Company::GetEndPosition()
{
	pCompact p = pHead;
	while (p->pNext != NULL)
	{
		p = p->pNext;
	}
	return p;
}
inline void Company::Extracting_From_File()
{
	ifstream extrac("Compact.txt", ios::in);
	if (!extrac)
	{
		Exception_HandlingX(const_cast<char*>("文件打开失败,请检查路径中是否存在已存储的数据文件..."));
		return;
	}
	extrac.seekg(39, ios::beg);
	pCompact pTail = GetEndPosition();
	string id,name,first,second;
	Date t;
	char lv;
	while (!extrac.eof())
	{
		if (!(extrac >> id >> name >> first >> second >> t.year >> t.month >> t.day >> lv))
			break;
loop:	if (IsexistsID(id)) 
		{
			cout << "已存在编号为 " + id << " 的合同,请修改编号为:";
			cin >> id;
			goto loop;
		}
		pCompact pNew = new Compact;	// 创建一个新结点
		pNew->SetInfo(id, name,first,second,t,lv);
		string path = name + ".txt";
		char *p = path._Get_data()._Myptr();
		ofstream f(p, ios::_Noreplace);
		f.close();
		pNew->SetPath(p);
		pNew->pNext = pTail->pNext;
		pTail->pNext = pNew;
		pNew->pPrior = pTail;
		pTail = pNew;
		g_count++;
	}
	cout << "\n\t读取成功..." << endl;
	extrac.close();
}
inline void Company::Storage_To_File()
{
	pCompact p = pHead->pNext;
	if (p == NULL)
		return;
	ofstream save("Compact.txt", ios::out);
	save << "编号\t名称\t甲方\t乙方\t签约时间\t合同等级\n";
	while (p)
	{
		Date t = p->GetTime();
		save <<p->GetID() << "\t" << p->GetName() << "\t" << p->GetFirst() << "\t" << p->GetSecond() << "\t" << t.year << " " << t.month << " " << t.day << "\t" << p->GetLv();
		if (p->pNext != NULL)
			save << endl;
		p = p->pNext;
	}
	save.close();
	cout << "\n\t存储成功...\n";
}
inline bool Company::CreateBinaryCatch()
{
	ofstream BinaryCatch("CompactData.catch", ios::binary);
	if (!BinaryCatch)
	{
		cout << "缓存文件生成失败..." << endl;
		return false;
	}
	pCompact p = pHead;
	while (p != NULL)
	{
		//BinaryCatch.write(p->GetID(), sizeof(p->GetID()));
	}
	BinaryCatch.close();
	return true;
}
	//vector<string> result = split(name);
	//for (size_t i = 0; i < result.size(); i++)
	//{
	//	cout << result[i] << "  ";
	//}
void Quick_Sort(pCompact *a, int left, int right,short index)			// 快速排序
{
	if (left > right)
		return;
	pCompact temp = a[left];					// 以待排序列最左边的数为基准数
	int i = left, j = right;			// i哨兵负责探测比基准数大的数,j哨兵负责探测比基准数小的数
	pCompact val = NULL;
	if (index == 1)
	{
		while (i != j)						// 直到哨兵相遇才停止探测	
		{
			while (a[j]->GetID() >= temp->GetID() && i < j)	// 注意哨兵i不能越过哨兵j
				j--;
			while (a[i]->GetID() <= temp->GetID() && i < j)
				i++;
			if (i < j)
			{
				val = a[j];
				a[j] = a[i];
				a[i] = val;
			}
		}
	}
	else if (index == 2)
	{
		while (i != j)						// 直到哨兵相遇才停止探测	
		{
			while (a[j]->GetName() >= temp->GetName() && i < j)	// 注意哨兵i不能越过哨兵j
				j--;
			while (a[i]->GetName() <= temp->GetName() && i < j)
				i++;
			if (i < j)
			{
				val = a[j];
				a[j] = a[i];
				a[i] = val;
			}
		}
	}
	else if (index == 3)
	{
		while (i != j)						// 直到哨兵相遇才停止探测	
		{
			while (a[j]->GetLv() >= temp->GetLv() && i < j)	// 注意哨兵i不能越过哨兵j
				j--;
			while (a[i]->GetLv() <= temp->GetLv() && i < j)
				i++;
			if (i < j)
			{
				val = a[j];
				a[j] = a[i];
				a[i] = val;
			}
		}
	}
	// 当循环结束,i == j 的时候,将基准数归位,此时哨兵i和哨兵j相遇,将他们所对应的位置就是基准数归位的位置
	a[left] = a[i];
	a[i] = temp;
	Quick_Sort(a, left, i - 1,index);			// 继续处理左边的序列,这是一个递归的过程
	Quick_Sort(a, i + 1, right,index);			// 继续处理右边的序列
}
inline bool Company::Sort(short index)
{
	pCompact p = pHead->pNext, q;
	// 成功排序
	pCompact val[128] = { NULL };
	int i = 0, j = 0;
	pCompact pt = NULL;
	while (p)
	{
		val[i++] = p->GetAddress();
		p = p->pNext;
	}
	Quick_Sort(val, 0, g_count - 1,index);		// 对地址指向的属性进行快速排序
	p = pHead;
	i = 0;
	while (p != NULL)
	{
		p->SetNextAddress(val[i++]);
		p = p->pNext;
	}
	return true;
}
inline void Company::changeCompact()
{
	pCompact re;
	string i;
	cout << "输入要修改合同的编号:";
	cin >> i;
	re = SearchCompact(re, i);
	printf("\n编号\t名称\t甲方\t乙方\t签约时间\t合同等级\n");
	re->ShowHStuData();
cp:cout << "是否继续修改(Y/N)";
	char flag;
	cin >> flag; 
	if (flag == 'Y' || flag == 'y')
	{
		printf("\n1.编号\t2.名称\t3.甲方\t4.乙方\t5.签约时间\t6.合同等级\n");
		int choice = 0;
		cout << "请输入你的选择:";
		hp:cin >> choice;
		if(choice < 0 || choice > 6)
		{
			cout << "请输入正确的选择:";
			goto hp;
		}
		if (choice == 1)
		{
			string NewId;
			cout << "请输入新的编号:";
		lp:cin >> NewId;
			if (IsexistsID(NewId))
			{
				cout << "已存在的编号,请重新输入:";
				goto lp;
			}
			re->SetID(NewId);
			cout << "\n\t修改成功...\n";
			_getch();
			return;
		}
		else
		{
			re->ReplaceInfo(choice);
			goto cp;
		}
	}
}
								// 容器分离字符串,正则表达式
//inline void Company::ReTest()
//{
//	if (!index)
//	{
//		cout << "\n\t请先建立班级..." << endl;
//		return;
//	}
//	pCompact p = pHead->pNext;
//	string re[4] = { "语文补考:","数学补考:","英语补考:","政治补考:" };
//	while (p)
//	{
//		for (int i = 0; i < 4; i++)
//		{
//			if (*(p->GetScore() + i) < 60)
//			{
//				re[i] += p->GetName();
//				re[i] += ".";
//			}
//		}		
//		p = p->pNext;
//	}
//	vector<string> result[4];
//	for(int k = 0;k < 4;k++)
//		result[k] = split(re[k]);
//	for (int m = 0; m < 4; m++)
//	{
//		for (size_t i = 0; i < result[m].size(); i++)
//		{
//			cout << result[m][i] << "  ";
//		}
//		cout << endl;
//	}
//	cout << endl;
//}
inline Compact* Company::GetpHead()
{
	return pHead;
}
inline void Company::CreateList()
{
	if (index)
	{
		Exception_HandlingX(const_cast<char*>("已绑定公司..."));
		return;
	}
	pHead = new Compact;
	pHead->pNext = NULL;
	index++;
}
inline void Company::ManageCompany()
{
	closegraph();
loop:system("cls");
	cout << "\n\n\t\t\t\t1.绑定公司";
	cout << "\n\n\t\t\t\t2.解除绑定\n";
	cout << "\n\n\t\t\t\t0.返回\n";
	int choice = 0;
	while (1)
	{
	tp:cout << "请选择:";
		cin >> choice;
		switch (choice)
		{
		case 1:BindingCompany(); break;
		case 2:DeBindingCompany(); break;
		case 0:CreateWindowAX(); system("cls"); return;
		default: cout << "\n\n\t请输入正确的选项...\n"; goto tp; break;
		}
	}
}

inline void Company::BindingCompany()
{
	ifstream in("company", ios::in);
	if (in)
	{
		if ((MessageBox(NULL, "已经绑定公司,请按确定进行解除绑定操作...", "温馨提示", MB_OKCANCEL | MB_ICONINFORMATION)) == IDOK)
		{
			in.close();
			DeBindingCompany();
		}
		else
			return;
	}
	else
	{
		BindCompany();
		cout << "\n\t绑定成功...\n";
	}
}

inline void Company::DeBindingCompany()
{
	ifstream in("company", ios::in);
	if (!in)
	{
		if ((MessageBox(NULL, "尚未绑定公司,请按确定进行绑定操作...", "温馨提示", MB_OKCANCEL | MB_ICONINFORMATION)) == IDOK)
			BindingCompany();
		else
			exit(-1);
	}
	else
	{
		in.close();
		system("del company");
		if ((MessageBox(NULL, "解绑成功,请重新绑定公司...", "温馨提示", MB_OKCANCEL | MB_ICONINFORMATION)) == IDOK)
			BindingCompany();
		else
			exit(-1);
	}
}

#pragma endregion 

#pragma region Compact类函数定义
																// Compact类函数定义
Compact::Compact()
{
	c.id = "";
	c.name = "";
	c.firstparty = "";
	c.secondparty = "";
	c.time = { 0,0,0 };
	c.lv = NULL;
	c.path = NULL;
}
Compact::Compact(string id, string na, string first, string second, Date t,char *p)
{
	c.id = id;
	c.name = na;
	c.firstparty = first;
	c.secondparty = second;
	c.time = t;
	c.path = p;
}
inline Compact::~Compact()
{
	
}
inline void Compact::InputCompactInfo()
{
	cin >> c.id >> c.name >> c.firstparty >> c.secondparty >> c.time.year >> c.time.month >> c.time.day >> c.lv;
	string p = c.name + ".txt";
	const char *path = p.c_str();
	ofstream txt(path, ios::_Noreplace);
	txt.close();
}
inline void Compact::SetInfo(string i, string na, string fi, string se, Date t, char l)
{
	c.id = i;
	c.name = na;
	c.firstparty = fi;
	c.secondparty = se;
	c.time = t;
	c.lv = l;
}
inline void Compact::SetPath(char * p)
{
	this->c.path = p;
}
inline void Compact::SetID(string i)
{
	this->c.id = i;
}
inline void Compact::ReplaceInfo(short choice)
{	
	switch (choice)
	{
		case 2:cout << "请输入新的名称:"; cin >> c.name; break;
		case 3:cout << "请输入新的甲方:"; cin >> c.firstparty; break;
		case 4:cout << "请输入新的乙方:"; cin >> c.secondparty; break;
		case 5:cout << "请输入新的时间:"; cin >> c.time.year >> c.time.month >> c.time.day; break;
		case 6:cout << "请输入新的等级:"; cin >> c.lv; break;
		case 0:CreateWindowAX(); system("cls"); return;
	}
	cout << "\n\t修改成功...\n";
}
inline void Compact::ShowHStuData()
{
	cout << c.id << "\t" << c.name << "\t" << c.firstparty << "\t" << c.secondparty << "\t" << c.time.year << "-" << c.time.month << "-" << c.time.day << "\t "<< c.lv << endl;
}
inline string Compact::GetID()
{
	return this->c.id;
}
inline string Compact::GetName()
{
	return this->c.name;
}
inline string Compact::GetFirst()
{
	return this->c.firstparty;
}
inline string Compact::GetSecond()
{
	return this->c.secondparty;
}
inline char * Compact::GetPath(char * p)
{
	p = this->c.path;
	return p;
}
inline Date Compact::GetTime()
{
	return this->c.time;
}
inline char Compact::GetLv()
{
	return this->c.lv;
}
inline Compact * Compact::GetAddress()
{
	return this;
}
inline void Compact::SetNextAddress(Compact * a)
{
	this->pNext = a;
}
inline void Company::WriteCompactTxt()
{
	cout << "\n请输入合同的编号:";
	string id = "";
	cin >> id;
	pCompact temp = NULL;
	string na;
	if (IsexistsID(id))
		na = SearchCompact(temp, id)->GetName() + ".txt";
	else
	{
		cout << "\n\t不存在此编号指定的合同,请仔细核查...\n";
		return;
	}
	//string na = id + ".txt";
	const char *path = na.c_str();
	system(path);
}
inline void Company::ViewCompactTxt()
{
	cout << "\n请输入合同的编号:";
	string id = "";
	cin >> id;
	pCompact temp = NULL;
	string na;
	if (IsexistsID(id))
		na = SearchCompact(temp, id)->GetName() + ".txt";
	else
	{
		Exception_HandlingX(const_cast<char*>("当前没有记录该编号指定的合同数据,请先添加合同或者读取文件..."));
		return;
	}
	const char *path = na.c_str();
	ifstream read(path, ios::in);
	if (!read)
	{
		Exception_HandlingX(const_cast<char*>("当前没有记录该编号指定的合同数据,请先添加合同或者读取文件..."));
		return;
	}
	system("cls");
	cout << "\t";
	char ch;
	while (read.get(ch))
	{
		cout << ch;
		Sleep(6);
	}
	cout << "\n\nPS:以上内容均来源于网络资源,无任何商业用途...\n";
	cout << "\n\n\t请按任意键返回..." << endl;
	_getch();
	system("cls");
}
inline pCompact Company::SearchCompact(pCompact re,string i)
{
	Sort(1);
	pCompact p = pHead, s;
	p = p->pNext;

	int mid, low = 0, high = g_count - 1;
	while (low <= high) {
		s = p;
		mid = (low + high) / 2;
		for (int j = 0; j < mid; j++) {
			s = s->pNext;
		}
		if (s->GetID() == i)
		{
			re = s;
			return re;
		}
		if (s->GetID() > i)
			high = mid - 1;
		else
			low = mid + 1;	
	}
	return NULL;
}
inline bool Company::IsexistsID(string id)
{
	pCompact r = SearchCompact(r, id);
	return (r == NULL) ? false : true;
}

#pragma endregion

#pragma endregion
