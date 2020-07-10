#include "ranUI.h"
#include "Declaration.h"

int main()
{
	system("title 企业合同管理系统");
	system("color 74");
	srand((unsigned int)time(NULL));
	VerifyIdentiy();
	mciSendString("open ./res/CUCCI.mp3 alias 1", 0, 0, 0);
	mciSendString("play 1", 0, 0, 0);
	PageLoad();			// 加载页面
	Company c;
	c.CreateList();
	typedef void(Company::*p_Fun[FUNCTIONPOINTER])();
	p_Fun pFun = { &Company::ManageCompany,&Company::ManageCompact,&Company::Show_All_CompactData ,&Company::FileOperate ,
		&Company::SortCompact };// 定义函数指针数组
	while (1)
	{
		Mouse(&c, pFun);
	}
	_getch();
	return 0;
}