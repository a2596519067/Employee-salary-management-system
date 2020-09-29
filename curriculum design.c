/************************************
	name 工资管理系统
	date 2020/6/30
	auth 唐顺帆 119583010220
*************************************/
/*------Local Structures and Typedefs ---
	读取职工工资数据函数：read()	
	保存职工工资数据函数：write()	
	查询职工工资数据函数：find()	
	浏览职工工资数据函数：list()	
	修改职工工资数据函数：modify()	
	删除职工工资数据函数：del()	
	添加职工工资数据函数：add()	
	计算个人所得税函数：grsds()	
	系统主函数：main()
-----------------------------------------*/

#include<stdlib.h>
#include<float.h> 
#include<ctype.h> 
#include<errno.h>
#include<limits.h> 
#include<locale.h>
#include<math.h>
#include<setjmp.h> 
#include<string.h>
#include<stdio.h>
#include<iostream>
using namespace std;
#pragma warning(disable:4996)
typedef struct einfo
{
	char num[10];	//工号
	char name[10];	//姓名
	float powag;	//岗位工资
	float pawag;	//薪级工资
	float poallo;	//职务津贴
	float merpay;	//绩效工资
	float wagpay;	//应发工资
	float tax;		//个人所得税
	float netsal;	//实发工资
}einfo;
typedef struct DulNode
{
	einfo data;
	DulNode* prior, * next;
}DulNode, * DuLinkList;
void read(DuLinkList &l)	//读取
{
	char ch;
	FILE* f;
	f = fopen("wages.dat", "ab+");
	einfo e;
	DuLinkList p,t;
	t = l;
	ch = fgetc(f);
	rewind(f);
	if (ch != EOF)
	{
		while (feof(f) == 0)
		{
			fread(&e, sizeof(einfo), 1, f);
			p = new DulNode;
			p->data = e;
			p->next = t->next;
			t->next = p;
			p->prior = t;
			t = p;
		}
		p->prior->next = NULL;
		free(p);
	}
}
void write(DuLinkList l)		//保存
{
	FILE* f;
	f = fopen("wages.dat", "wb+");
	einfo *e;
	DuLinkList p;
	p = l->next;
	e = new einfo;
	while (p != NULL)
	{
		*e = p->data;
		fwrite(e, sizeof(einfo), 1, f);
		p = p->next;
	}
}
float grsds(float sum)		//个人所得税
{
	float t;
	if (sum > 100000)
		t = 29625.0 + (sum - 100000) * 0.45;
	else if (sum > 80000)
		t = 21700.0 + (sum - 80000) * 0.4;
	else if (sum > 60000)
		t = 14625.0 + (sum - 60000) * 0.35;
	else if (sum > 40000)
		t = 8625.0 + (sum - 40000) * 0.3;
	else if (sum > 20000)
		t = 3625.0 + (sum - 20000) * 0.25;
	else if (sum > 5000)
		t = 625.0 + (sum - 5000) * 0.2;
	else if (sum > 2000)
		t = 175.0 + (sum - 2000) * 0.15;
	else if (sum > 500)
		t = 25.0 + (sum - 500) * 0.1;
	else
		t = sum * 0.05;
	return t;
}
void InitDulList(DuLinkList &l)		//初始化双向链表
{
	l = new DulNode;
	l->prior = NULL;
	l->next = NULL;
}
void scanf_einfo(einfo& e)		//输入员工信息
{
	printf("工号：  姓名： 岗位工资：  薪级工资：  职务津贴：  绩效工资： \n");
	scanf("%s%s%f%f%f%f", e.num, e.name, &e.powag, &e.pawag, &e.poallo, &e.merpay);
	e.wagpay = e.powag + e.pawag + e.poallo + e.merpay;
	e.tax = grsds(e.wagpay);
	e.netsal = e.wagpay - e.tax;
}
void find(DuLinkList &l)		//查找
{
	char a[10];
	DuLinkList p;
	einfo e;
	p = l->next;
	printf("请输入查询职工工号：\n");
	scanf("%s", a);
	while (p!=NULL)
	{
		e = p->data;
		if (strcmp(a, e.num) == 0)
		{
			printf("%s %s %f %f %f %f %f %f %f\n", e.num, e.name, e.powag, e.pawag, e.poallo, e.merpay, e.wagpay, e.tax, e.netsal);
			break;
		}
		p = p->next;
	}
	if (p == NULL)
		printf("未找到该职工！\n");
}
void modify(DuLinkList &l)		//修改
{
	char a[10];
	DuLinkList p;
	einfo e;
	p = l;
	printf("请输入修改职工工号：\n");
	scanf("%s", a);
	while (p != NULL)
	{
		e = p->data;
		if (strcmp(a, p->data.num) == 0)
		{
			scanf_einfo(p->data);
			break;
		}
		p = p->next;
	}
}
void add(DuLinkList &l)		//添加
{
	char e[10];
	DuLinkList p;
	p = new DulNode;
	if (l->next == NULL)
	{
		p->next = l->next;
		l->next = p;
		scanf_einfo(p->data);
	}
	else
	{
		p->prior = l;
		p->next = l->next;
		l->next->prior = p;
		l->next = p;
		printf("请输入添加职工信息：\n");
		scanf_einfo(p->data);
	}
}
void del(DuLinkList &l)		//删除
{
	char a[10];
	printf("请输入您要删除的员工工号：\n");
	scanf("%s", a);
	DuLinkList p;
	einfo e;
	p = l;
	while (p != NULL)
	{
		e = p->data;
		if (strcmp(a, e.num) == 0)
		{
			printf("已删除：\n%s %s %f %f %f %f %f %f %f\n", e.num, e.name, e.powag, e.pawag, e.poallo, e.merpay, e.wagpay, e.tax, e.netsal);
			break;
		}
		p = p->next;
	}
	if (p == NULL)
	{
		printf("未找到该职工！\n");
		return;
	}
	if (p->next != NULL)
	{
		p->next->prior = p->prior;
		p->prior->next = p->next;
	}
	else
	{
		p->prior->next = NULL;
	}
	free(p);
}
void list(DuLinkList l)		//浏览
{
	einfo e;
	DuLinkList p;
	p = l->next;
	if (l->next == NULL)
	{
		printf("文件为空！\n");
		return;
	}
	while (p != NULL)
	{
		e = p->data;
		printf("%s %s %f %f %f %f %f %f %f\n",e.num,  e.name,  e.powag, e.pawag, e.poallo, e.merpay,e.wagpay, e.tax,e.netsal);
		p = p->next;
	}
}
int main()		//主函数
{
	DuLinkList l;
	InitDulList(l);
	int a;
	printf("职工工资管理系统\n\n1.查询  2.修改\n3.添加  4.删除\n5.保存  6.浏览\n7.退出唐顺帆 119583010220\n请输入您要进行的操作：\n");
	read(l);
	scanf("%d", &a);
	while (a != 7)
	{
		switch (a)
		{
		case 1:find(l); break;
		case 2:modify(l); break;
		case 3:add(l); break;
		case 4:del(l); break;
		case 5:write(l); break;
		case 6:list(l); break;
		case 7:exit(0); break;
		}
		printf("请输入您要进行的操作：\n");
			scanf("%d", &a);
	}
}