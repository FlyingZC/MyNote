#include<stdio.h>
#include<string.h>
#include<malloc.h>
//定义全局变量
int col,row;
FILE *source;
/* tokenString 是字符串类型的全局变量，存放当前的词素*/
char tokenString[100];

int p=0,m=0;

/*词法单元名称、*/
typedef enum{
	ENDF,ERROR,ID,NUM, RNUM,  /*文件结束、错误、标识符、整数、小数*/
INT,REAL,MOD,ITOR, ADD,MUL, /*整型、实型、求余、类型转换、加、乘*/
	ASSIGN,SEMI,COMMA,LPAR,RPAR  /*赋值、分号、逗号、左括号、右括号*/
} Tag;

Tag token;
#define MAXTOKENSIZE 9  /*单词的最大长度为9*/
#define MAXCHILDERN 3    /*语法树中子节点的最大个数*/

typedef Tag TokenType;   /*词法单元的种类*/
typedef Tag NodeKind;   /*语法树节点的种类*/

/*单词种别，用以区分符号表中的变量与常量*/
typedef enum{
	NULCAT,VARCAT,CONCAT  /*空类别、变量、常量*/
}Cat;

/*数据类型*/
typedef enum{
NULTYPE,INTTYPE,REALTYPE,ERRORTYPE/*空类型、整型、实型、类型错误*/
}Type;
//关键字表
#define MAXRESERVEWORDS 3  /*保留字只有3个：mod, int, real*/
//定义一个保留字数组reserveword[3]
struct {
	char* str;  /*保留字的单词拼写*/
	TokenType tok;  /*对应的词法单元名称*/
}reserveword[MAXRESERVEWORDS]={
	{"mod",MOD},
	{"int",INT},
	{"real",REAL}
};  /*定义了三个保留字的单词拼写，及其词法单元名称*/



//读取字符和反读字符
#define BUFSIZE 128  /*缓冲区大小*/
static char buf[BUFSIZE]="\0";  /*结尾加上字符串结束标志，防止超出范围*/

/*取下一个字符，每次读出一行，该行中的字符取完之后再读取下一行*/
/*row和col是全局变量，在发现语法错误时，可由row给出错误所在的行号*/
//读取函数
static char nextch(){
	if(col>=(int)strlen(buf)){  /*如果当前行中的字符已取完*/
		row++;  /*行数加1*/
		if(fgets(buf,BUFSIZE-1,source)){   /*读取新的一行*/
			col=0;
			return buf[col++];  /*返回刚读取的行中的第一个字符*/
		}
		return EOF;  /*文件已结束*/
	}
	return buf[col++];  /*如果当前行中的字符还未取完，则从中取出下一个字符*/
}

/*回退一个字符*/
static void retract(){
	col--;
}
//识别保留字
/*与保留字列表中的3个保留字的单词进行比较，如果是3个保留字之一则返回其词法单元名称，否则返回ID*/
TokenType rsfind(char* s){
	int i;
	for(i=0;i<MAXRESERVEWORDS;i++)  /*与3个保留字进行比较*/
		if(!strcmp(s,reserveword[i].str)){
			return reserveword[i].tok;
		}
		return ID;  /*如果不是保留字，则认为是标识符，返回ID*/
}

//判断是否为空格
bool isspace(char c){
	if(c==' ')
		return true;
	else
		return false;
}

//判断是否是数字
bool isdigit(char c){
	if(c>='0'&&c<='9')
		return true;
	else
		return false;
}
//判断是否是字母
bool isalpha(char c)
{
	if((c>='a'&&c<='z')||(c>='A'&&c<='Z'))
		return true;
	else
		return false;

}
//num函数功能是：匹配词法单元NUM或RNUM，将分离出的词素赋给tokenString，将NUM或RNUM赋给token
/* tokenString 是字符串类型的全局变量，存放当前的词素*/
//NUM整数、RNUM小数
void num(char c){
	tokenString[p++]=c;

	token=NUM;
}

/*匹配标识符，在函数id中调用rsfind 判断是否是保留字*/

void id(char c){
	char c2,c3,c4;
	char s1[3],s2[4];
	char *ps1=s1,*ps2=s2;
	if(c=='m')
	{
		c2=nextch();
		c3=nextch();
		s1[0]=c;
		s1[1]=c2;
		s1[2]=c3;
		retract();
		retract();
	}
	else if(c=='i')
	{
		c2=nextch();
		c3=nextch();
		s1[0]=c;
		s1[1]=c2;
		s1[2]=c3;
		retract();
		retract();
	}
	else if(c=='r')
	{
		c2=nextch();
		c3=nextch();
		c4=nextch();
		s2[0]=c;
		s2[1]=c2;
		s2[2]=c3;
		s2[3]=c4;
		retract();
		retract();
		retract();
	}
	TokenType jieGuo1=rsfind(ps1);
	TokenType jieGuo2=rsfind(ps2);
	//if()
//	tokenString[p++]=c;

}
//读取一个单词，并匹配对应的词法单元
void tkget(){
	char ch;
	strcpy(tokenString,"\0");	 /* tokenString 是字符串类型的全局变量，存放当前的词素*/
	while(isspace(ch=nextch()));  /*去除空格*/
	if(isdigit(ch)){  /*如果是数字*/
		num(ch);  /*要求自己写num函数，num函数功能是：匹配词法单元NUM或RNUM，将分离出的词素赋给tokenString，将NUM或RNUM赋给token*/
	}
	else if(isalpha(ch)){  /*如果是字母*/
		id(ch);  /*匹配标识符，在函数id中调用rsfind 判断是否是保留字*/
	}
	else if(ch==EOF){		  /*文件结束*/
		token=ENDF;
	}			
	else{
		switch(ch){
			case '+':  /*识别出加号*/
				token=ADD;
				break;
			case '*':  /*识别出乘号*/
				token=MUL;
				break;
			case ';':  /*识别出分号*/
				token=SEMI;
				break;
			case ',':  /*识别出逗号*/
				token=COMMA;
				break;
			case '(':  /*识别出左括号*/
				token=LPAR;
				break;
			case ')':  /*识别出右括号*/
				token=RPAR;
				break;
			case '=':  /*识别出=*/
				token=ASSIGN;
				break;
			default:  /*发现词法错误*/
				token=ERROR;
				break;
		}
	}
}


int main()
{	
	FILE *fp;
	fp=fopen("file a","r");
	tkget();
	return 0;
}
