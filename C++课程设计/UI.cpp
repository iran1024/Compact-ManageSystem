#include "ranUI.h"
#include "Declaration.h"

int main()
{
	system("title ��ҵ��ͬ����ϵͳ");
	system("color 74");
	srand((unsigned int)time(NULL));
	VerifyIdentiy();
	mciSendString("open ./res/CUCCI.mp3 alias 1", 0, 0, 0);
	mciSendString("play 1", 0, 0, 0);
	PageLoad();			// ����ҳ��
	Company c;
	c.CreateList();
	typedef void(Company::*p_Fun[FUNCTIONPOINTER])();
	p_Fun pFun = { &Company::ManageCompany,&Company::ManageCompact,&Company::Show_All_CompactData ,&Company::FileOperate ,
		&Company::SortCompact };// ���庯��ָ������
	while (1)
	{
		Mouse(&c, pFun);
	}
	_getch();
	return 0;
}