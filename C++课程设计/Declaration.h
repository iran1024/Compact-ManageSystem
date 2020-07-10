#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

#pragma region ����ȫ�ֱ���
const char *_ptr[] = { _T("�� ��˾����"),"�� ��ͬ����","�� ������к�ͬ��Ϣ",
"�� �ļ�����","�� ��ͬ����",NULL };
static int g_count = 0;			// ��ͬ����
static int index = 0;			// ����Ƿ��������
static char CompanyName[20] = { 0 };
static char CompanyPass[12] = { 0 };
static char name[20] = { 0 };	// ������ļ���ȡ�Ĺ�˾����
IMAGE _img;

#pragma endregion

inline void UI_X()				// �ػ�
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
	settextstyle(25, 0, _T("���Ŀ���"));
	outtextxy(50, 30, "��˾����:");
	outtextxy(170, 30, CompanyName);
	settextstyle(15, 0, _T("������"));
}
void CreateWindowAX()			// ��������
{
	HWND hWnd = initgraph(640, 450);
	SetWindowText(hWnd, _T("��ҵ��ͬ����ϵͳ"));
	setbkmode(TRANSPARENT);
	settextcolor(BLACK);
	settextstyle(15, 0, _T("������"));
	UI_X();
}
__int64 GetSecretKey(char _pass[])				// ��ȡ��Կ
{
	__int64 key = 9989979949959;
	char pass[12] = "";
	strcpy(pass, _pass);
	unsigned int len = strlen(pass);
	
	for (int i = 0; i < len; i++)
		key ^= pass[i] | 128;
	return key;
}	
char* ECP(char pass[])							// ����
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
char* DCP(char pass[])							// ����
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
void BindCompany()			// ��˾��
{
loop:if ((InputBox(CompanyName, 20, "�����빫˾����:", "��ҵ��ͬ����ϵͳ��һ��ʹ��,���Ȱ󶨹�˾...", NULL, 0, 0, false) == IDOK))
	{
		if (strcmp(CompanyName, "") == 0)
		{
			if ((MessageBox(NULL, "��������ȷ�Ĺ�˾����...", "��ܰ��ʾ", MB_OKCANCEL | MB_ICONINFORMATION) == IDOK))
				goto loop;
			else
				exit(-1);
		}
	pp:if ((InputBox(CompanyPass, 11, "����������:", "��ҵ��ͬ����ϵͳ��һ��ʹ��,������������", NULL, 0, 0, false) == IDOK))
		{
			if (strcmp(CompanyPass, "") == 0)
			{
				if ((MessageBox(NULL, "��������ȷ������...", "��ܰ��ʾ", MB_OKCANCEL | MB_ICONINFORMATION) == IDOK))
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
void VerifyIdentiy()			// �����֤
{
	ifstream in("company", ios::in);
	if (!in)
	{
		if ((MessageBox(NULL, "��δ�󶨹�˾,�밴ȷ�����а󶨲���...", "��ܰ��ʾ", MB_OKCANCEL | MB_ICONINFORMATION)) == IDOK)
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
		char *p = DCP(_pass);	// �����ļ��ж�ȡ�������Ѽ��ܵ����������ԭ����
		int time = 1;
	loop:if ((InputBox(password, 11, "����������:", "��ҵ��ͬ����ϵͳ��ȫ���...", NULL, 0, 0, false) == IDOK))
		{
			if (strcmp(password,p) != 0)
			{
				if (time == 3)
				{
					MessageBox(NULL, "��������Ѵﵽ����,��ֹʹ��...", "��ܰ��ʾ", MB_OK | MB_ICONERROR);
					exit(-1);
				}
				if ((MessageBox(NULL, "��������ȷ����...", "��ܰ��ʾ", MB_OKCANCEL | MB_ICONINFORMATION) == IDOK))
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
#pragma region ��/�ṹ����
typedef struct Date
{
	short year, month, day;
}Date;
typedef struct com
{
	string id;						// ��ͬ���
	string name;					// ��ͬ����
	string firstparty, secondparty;	// �׷�,�ҷ�
	Date time;						// ǩԼʱ��
	char lv;						// ��ͬ�ȼ�
	char *path;						// ��ͬ·��
}Com;
class Compact
{
private:
	Com c;
public:
	Compact *pPrior, *pNext;			// ָ��ǰһ��������һ�����
	Compact();
	Compact(string id, string na, string first, string second, Date t, char *p);
	~Compact();
	void InputCompactInfo();			// �����ͬ��Ϣ
	void SetInfo(string i, string na, string fi, string se, Date t, char l);// ���û�����Ϣ
	void SetPath(char *p);				// ����·��
	void SetID(string i);				// ���ñ��
	void ReplaceInfo(short choice);		// �޸���Ϣ
	void ShowHStuData();				// ��ʾ�˺�ͬ����
	string GetID();						// �õ���ͬ�ı��
	string GetName();					// �õ���ͬ������
	string GetFirst();					// �õ���ͬ�ļ׷�
	string GetSecond();					// �õ���ͬ���ҷ�
	char* GetPath(char *p);				// �õ���ͬ��·��
	Date GetTime();						// �õ���ͬ��ǩԼʱ��
	char GetLv();						// �õ���ͬ��ǩԼ�ȼ�
	Compact *GetAddress();				// �õ���ǰ���ĵ�ַ
	void SetNextAddress(Compact *a);	// ���õ�ǰ������һ�����ĵ�ַ
	bool IsAbleMatchingTxt(Compact* r, string key);// �Ƿ��ܹ�ƥ��ؼ���
};
typedef Compact* pCompact;
class Company
{
	pCompact pHead;
public:
	void CreateList();					// ��������
	void ManageCompany();				// ����˾
	void BindingCompany();				// �󶨹�˾
	void DeBindingCompany();			// �����
	void Show_All_CompactData();		// ��ʾ���к�ͬ��Ϣ
	void ManageCompact();				// �����ͬ(��ɾ�Ĳ�)
	void AddCompact();					// ��Ӻ�ͬ
	void FileOperate();					// �ļ�����
	void SortCompact();					// ��ͬ����
	void WriteCompactTxt();				// ��д��ͬ����
	void ViewCompactTxt();				// �鿴��ͬ����
	pCompact SearchCompact(pCompact re,string i);// ���Һ�ͬ
	pCompact SearchTxt(pCompact re, string ip, pCompact he);
	void DealTxtSearch();
	void SearchMenu();					// ���Ҳ˵�
	bool IsexistsID(string id);			// ����Ƿ������ͬ��ID
	void DealSearchRes();				// ������Һ�Ľ��
	void DeleteData();					// ɾ����ͬ��Ϣ
	pCompact GetComPosition(string pos);// ��ȡ��ͬ��λ
	pCompact GetEndPosition();			// ��ȡ���һ����ͬ��λ
	void Extracting_From_File();		// �����ݴ�Ӳ����ȡ���ڴ�
	void Storage_To_File();				// �����ݴ��ڴ��ŵ�Ӳ��
	bool CreateBinaryCatch();			// �������Զ����ƻ��浽Ӳ������߶�д�ٶ�
	bool Sort(short index);				// �����,���ƻ�ȼ�����
	void changeCompact();				// �޸ĺ�ͬ��Ϣ
	pCompact GetpHead();				// �õ�����ͷ���

};
#pragma endregion

#pragma region ���庯��
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
vector<string> split(string strTime)	// ��дvector���е�split�����������������ʽ�����ַ����������ָ�(����ٶ�)
{
	vector<string> result;
	string temp("");
	strTime += '-';//�ַ���������ǣ����㽫���һ��������vector  
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
void Exception_HandlingX(char* str)		// �쳣������
{
	if (MessageBox(NULL, str, "��ܰ��ʾ...", MB_OK | MB_ICONERROR) == IDOK)
		return;
}
pCompact getSeparator(pCompact pBegin, pCompact pEnd)		// �ҵ��ָ��
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

#pragma region Company�ຯ������
																	// Company�ຯ������
inline void Company::Show_All_CompactData()
{
	pCompact p = pHead->pNext;
	if (p == NULL)
	{
		Exception_HandlingX(const_cast<char*>("��������..."));
		return;
	}
	closegraph();
	printf("���\t����\t�׷�\t�ҷ�\tǩԼʱ��\t��ͬ�ȼ�\n");
	while (p != NULL)
	{
		p->ShowHStuData();
		p = p->pNext;
	}
	cout << "\n\n\n\n\t\t\t�밴���������...\n";
	_getch();
	CreateWindowAX();
	system("cls");
}
inline void Company::ManageCompact()
{
	closegraph();
loop:system("cls");
	cout << "\n\n\t\t\t\t1.��Ӻ�ͬ";
	cout << "\n\n\t\t\t\t2.ɾ����ͬ";
	cout << "\n\n\t\t\t\t3.�޸ĺ�ͬ";
	cout << "\n\n\t\t\t\t4.���Һ�ͬ";
	cout << "\n\n\t\t\t\t5.��д��ͬ����";
	cout << "\n\n\t\t\t\t6.�鿴��ͬ����";
	cout << "\n\n\t\t\t\t0.����\n";
	int choice = 0;
	while (1)
	{
		tp:cout << "��ѡ��:";
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
			default: cout << "\n\n\t��������ȷ��ѡ��...\n"; goto tp; break;
		}
	}
}
inline void Company::AddCompact()
{
	pCompact pTail = GetEndPosition();						// ����һ��ָ�����һ������ָ��
	int StuNum = 0;
	cout << "�������ͬ������:";
	cin >> StuNum;
	if (StuNum == 0)
	{
		cout << "\n\tȡ�����..." << endl;
		return;
	}
	cout << "���������ͬ�ı��,�Լ�����Ƿ����:";
	for (int i = 1; i <= StuNum; i++)
	{
		pCompact pNew = new Compact;	// ����һ���½��
		string id;
lp:		cin >> id;
		if (IsexistsID(id))
		{
			cout << "�Ѵ��ڵı��,������������:";
			goto lp;
		}
		cout << "����������������, �׷�, �ҷ�, ǩԼʱ��ͺ�ͬ�ȼ�(�Կո����) :\n";
		pNew->InputCompactInfo();
		pNew->pNext = pTail->pNext;
		pTail->pNext = pNew;
		pNew->pPrior = pTail;
		pTail = pNew;
		cout << "�ɹ�..." << endl;
	}
	g_count += StuNum;
}
inline void Company::FileOperate()
{
	closegraph();
	cout << "\n\n\t\t\t\t1.�洢���ļ�\n";
	cout << "\n\n\t\t\t\t2.���ļ���ȡ\n";
	cout << "\n\n\t\t\t\t0.����\n";
	int choice = 0;
	while (1)
	{
	tp:	cout << "��ѡ��:";
		cin >> choice;
		switch (choice)
		{
			case 1:Storage_To_File(); break;
			case 2:Extracting_From_File(); break;
			case 0:CreateWindowAX(); system("cls"); return;
			default: cout << "\n\n\t��������ȷ��ѡ��...\n"; goto tp; break;
		}
	}
}
inline void Company::SortCompact()
{
	closegraph();
	cout << "\n\n\t\t\t\t1.���������";
	cout << "\n\n\t\t\t\t2.����������";
	cout << "\n\n\t\t\t\t3.���ȼ�����";
	cout << "\n\n\t\t\t\t0.����\n";
	int choice = 0;
	while (1)
	{
	lp:cout << "��ѡ��:";
	gp:cin >> choice;
		switch (choice)
		{
			case 1:if (!Sort(1)) goto lp; break;			// ���ձ������
			case 2:if (!Sort(2)) goto lp; break;			// ������������	��Ϊ����Ϊ����,�����뷽ʽΪ���ֽڱ���,ֻ��GBK��������Żᰴ�ն���˳��
			case 3:if (!Sort(3)) goto lp; break;			// ���յȼ�����
			case 0:CreateWindowAX(); system("cls"); return;
			default: cout << "\n\n\t��������ȷ��ѡ��...\n"; goto gp; break;
		}
		cout << "\n\t����ɹ�...\n";
	}
}
inline void Company::DealSearchRes()
{
	string i;
	cout << "��������ҵı��:";
	cin >> i;
	pCompact res = SearchCompact(res,i);
	if (res == NULL)
	{
		cout << "\n\t���޴�����...\n";
		return;
	}
	printf("���\t����\t�׷�\t�ҷ�\tǩԼʱ��\t��ͬ�ȼ�\n");
	res->ShowHStuData();
}
inline void Company::DealTxtSearch()
{
	string key;
	cout << "��������Ҫ���ҵ����Ĺؼ���:";
	cin >> key;
	pCompact res = pHead->pNext;
	res = SearchTxt(res, key, res);
	if (res != NULL)
	{
		printf("���\t����\t�׷�\t�ҷ�\tǩԼʱ��\t��ͬ�ȼ�\n");
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
		cout << "\n\t���޴�����...\n";
}
inline void Company::SearchMenu()
{
loop:system("cls");
	cout << "\n\n\t\t\t\t1.����Ų���";
	cout << "\n\n\t\t\t\t2.�����Ĺؼ��ֲ���";
	cout << "\n\n\t\t\t\t0.����\n";
	int choice = 0;
	while (1)
	{
		cout << "��ѡ��:";
	 tp:cin >> choice;
		switch (choice)
		{
			case 1:DealSearchRes(); break;
			case 2:DealTxtSearch(); break;
			case 0:system("cls"); return;
			default: cout << "\n\n\t��������ȷ��ѡ��...\n"; goto tp; break;
		}
	}
}
inline pCompact Company::SearchTxt(pCompact re, string i,pCompact he)	// ���������� ��ǰ���ڽ��
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
		Exception_HandlingX(const_cast<char*>("��ǰû�м�¼�ñ��ָ���ĺ�ͬ����,������Ӻ�ͬ���߶�ȡ�ļ�..."));
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
		Exception_HandlingX(const_cast<char*>("��ǰ�������κκ�ͬ��Ϣ..."));
		return;
	}
	string pos = "";
	pCompact p;
	cout << "������Ҫɾ���ĺ�ͬ�ı��:";
	cin >> pos;
	p = GetComPosition(pos);
	if (p->pNext == NULL )
	{
		cout << "�����ڸú�ͬ������..." << endl;
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
	cout << "\n\n\tɾ���ɹ�...\n";
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
		Exception_HandlingX(const_cast<char*>("�ļ���ʧ��,����·�����Ƿ�����Ѵ洢�������ļ�..."));
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
			cout << "�Ѵ��ڱ��Ϊ " + id << " �ĺ�ͬ,���޸ı��Ϊ:";
			cin >> id;
			goto loop;
		}
		pCompact pNew = new Compact;	// ����һ���½��
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
	cout << "\n\t��ȡ�ɹ�..." << endl;
	extrac.close();
}
inline void Company::Storage_To_File()
{
	pCompact p = pHead->pNext;
	if (p == NULL)
		return;
	ofstream save("Compact.txt", ios::out);
	save << "���\t����\t�׷�\t�ҷ�\tǩԼʱ��\t��ͬ�ȼ�\n";
	while (p)
	{
		Date t = p->GetTime();
		save <<p->GetID() << "\t" << p->GetName() << "\t" << p->GetFirst() << "\t" << p->GetSecond() << "\t" << t.year << " " << t.month << " " << t.day << "\t" << p->GetLv();
		if (p->pNext != NULL)
			save << endl;
		p = p->pNext;
	}
	save.close();
	cout << "\n\t�洢�ɹ�...\n";
}
inline bool Company::CreateBinaryCatch()
{
	ofstream BinaryCatch("CompactData.catch", ios::binary);
	if (!BinaryCatch)
	{
		cout << "�����ļ�����ʧ��..." << endl;
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
void Quick_Sort(pCompact *a, int left, int right,short index)			// ��������
{
	if (left > right)
		return;
	pCompact temp = a[left];					// �Դ�����������ߵ���Ϊ��׼��
	int i = left, j = right;			// i�ڱ�����̽��Ȼ�׼�������,j�ڱ�����̽��Ȼ�׼��С����
	pCompact val = NULL;
	if (index == 1)
	{
		while (i != j)						// ֱ���ڱ�������ֹͣ̽��	
		{
			while (a[j]->GetID() >= temp->GetID() && i < j)	// ע���ڱ�i����Խ���ڱ�j
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
		while (i != j)						// ֱ���ڱ�������ֹͣ̽��	
		{
			while (a[j]->GetName() >= temp->GetName() && i < j)	// ע���ڱ�i����Խ���ڱ�j
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
		while (i != j)						// ֱ���ڱ�������ֹͣ̽��	
		{
			while (a[j]->GetLv() >= temp->GetLv() && i < j)	// ע���ڱ�i����Խ���ڱ�j
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
	// ��ѭ������,i == j ��ʱ��,����׼����λ,��ʱ�ڱ�i���ڱ�j����,����������Ӧ��λ�þ��ǻ�׼����λ��λ��
	a[left] = a[i];
	a[i] = temp;
	Quick_Sort(a, left, i - 1,index);			// ����������ߵ�����,����һ���ݹ�Ĺ���
	Quick_Sort(a, i + 1, right,index);			// ���������ұߵ�����
}
inline bool Company::Sort(short index)
{
	pCompact p = pHead->pNext, q;
	// �ɹ�����
	pCompact val[128] = { NULL };
	int i = 0, j = 0;
	pCompact pt = NULL;
	while (p)
	{
		val[i++] = p->GetAddress();
		p = p->pNext;
	}
	Quick_Sort(val, 0, g_count - 1,index);		// �Ե�ַָ������Խ��п�������
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
	cout << "����Ҫ�޸ĺ�ͬ�ı��:";
	cin >> i;
	re = SearchCompact(re, i);
	printf("\n���\t����\t�׷�\t�ҷ�\tǩԼʱ��\t��ͬ�ȼ�\n");
	re->ShowHStuData();
cp:cout << "�Ƿ�����޸�(Y/N)";
	char flag;
	cin >> flag; 
	if (flag == 'Y' || flag == 'y')
	{
		printf("\n1.���\t2.����\t3.�׷�\t4.�ҷ�\t5.ǩԼʱ��\t6.��ͬ�ȼ�\n");
		int choice = 0;
		cout << "���������ѡ��:";
		hp:cin >> choice;
		if(choice < 0 || choice > 6)
		{
			cout << "��������ȷ��ѡ��:";
			goto hp;
		}
		if (choice == 1)
		{
			string NewId;
			cout << "�������µı��:";
		lp:cin >> NewId;
			if (IsexistsID(NewId))
			{
				cout << "�Ѵ��ڵı��,����������:";
				goto lp;
			}
			re->SetID(NewId);
			cout << "\n\t�޸ĳɹ�...\n";
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
								// ���������ַ���,������ʽ
//inline void Company::ReTest()
//{
//	if (!index)
//	{
//		cout << "\n\t���Ƚ����༶..." << endl;
//		return;
//	}
//	pCompact p = pHead->pNext;
//	string re[4] = { "���Ĳ���:","��ѧ����:","Ӣ�ﲹ��:","���β���:" };
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
		Exception_HandlingX(const_cast<char*>("�Ѱ󶨹�˾..."));
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
	cout << "\n\n\t\t\t\t1.�󶨹�˾";
	cout << "\n\n\t\t\t\t2.�����\n";
	cout << "\n\n\t\t\t\t0.����\n";
	int choice = 0;
	while (1)
	{
	tp:cout << "��ѡ��:";
		cin >> choice;
		switch (choice)
		{
		case 1:BindingCompany(); break;
		case 2:DeBindingCompany(); break;
		case 0:CreateWindowAX(); system("cls"); return;
		default: cout << "\n\n\t��������ȷ��ѡ��...\n"; goto tp; break;
		}
	}
}

inline void Company::BindingCompany()
{
	ifstream in("company", ios::in);
	if (in)
	{
		if ((MessageBox(NULL, "�Ѿ��󶨹�˾,�밴ȷ�����н���󶨲���...", "��ܰ��ʾ", MB_OKCANCEL | MB_ICONINFORMATION)) == IDOK)
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
		cout << "\n\t�󶨳ɹ�...\n";
	}
}

inline void Company::DeBindingCompany()
{
	ifstream in("company", ios::in);
	if (!in)
	{
		if ((MessageBox(NULL, "��δ�󶨹�˾,�밴ȷ�����а󶨲���...", "��ܰ��ʾ", MB_OKCANCEL | MB_ICONINFORMATION)) == IDOK)
			BindingCompany();
		else
			exit(-1);
	}
	else
	{
		in.close();
		system("del company");
		if ((MessageBox(NULL, "���ɹ�,�����°󶨹�˾...", "��ܰ��ʾ", MB_OKCANCEL | MB_ICONINFORMATION)) == IDOK)
			BindingCompany();
		else
			exit(-1);
	}
}

#pragma endregion 

#pragma region Compact�ຯ������
																// Compact�ຯ������
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
		case 2:cout << "�������µ�����:"; cin >> c.name; break;
		case 3:cout << "�������µļ׷�:"; cin >> c.firstparty; break;
		case 4:cout << "�������µ��ҷ�:"; cin >> c.secondparty; break;
		case 5:cout << "�������µ�ʱ��:"; cin >> c.time.year >> c.time.month >> c.time.day; break;
		case 6:cout << "�������µĵȼ�:"; cin >> c.lv; break;
		case 0:CreateWindowAX(); system("cls"); return;
	}
	cout << "\n\t�޸ĳɹ�...\n";
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
	cout << "\n�������ͬ�ı��:";
	string id = "";
	cin >> id;
	pCompact temp = NULL;
	string na;
	if (IsexistsID(id))
		na = SearchCompact(temp, id)->GetName() + ".txt";
	else
	{
		cout << "\n\t�����ڴ˱��ָ���ĺ�ͬ,����ϸ�˲�...\n";
		return;
	}
	//string na = id + ".txt";
	const char *path = na.c_str();
	system(path);
}
inline void Company::ViewCompactTxt()
{
	cout << "\n�������ͬ�ı��:";
	string id = "";
	cin >> id;
	pCompact temp = NULL;
	string na;
	if (IsexistsID(id))
		na = SearchCompact(temp, id)->GetName() + ".txt";
	else
	{
		Exception_HandlingX(const_cast<char*>("��ǰû�м�¼�ñ��ָ���ĺ�ͬ����,������Ӻ�ͬ���߶�ȡ�ļ�..."));
		return;
	}
	const char *path = na.c_str();
	ifstream read(path, ios::in);
	if (!read)
	{
		Exception_HandlingX(const_cast<char*>("��ǰû�м�¼�ñ��ָ���ĺ�ͬ����,������Ӻ�ͬ���߶�ȡ�ļ�..."));
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
	cout << "\n\nPS:�������ݾ���Դ��������Դ,���κ���ҵ��;...\n";
	cout << "\n\n\t�밴���������..." << endl;
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
