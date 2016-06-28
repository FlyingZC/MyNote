#include<stdio.h>
#include<string.h>
#include<malloc.h>
//����ȫ�ֱ���
int col,row;
FILE *source;
/* tokenString ���ַ������͵�ȫ�ֱ�������ŵ�ǰ�Ĵ���*/
char tokenString[100];

int p=0,m=0;

/*�ʷ���Ԫ���ơ�*/
typedef enum{
	ENDF,ERROR,ID,NUM, RNUM,  /*�ļ����������󡢱�ʶ����������С��*/
INT,REAL,MOD,ITOR, ADD,MUL, /*���͡�ʵ�͡����ࡢ����ת�����ӡ���*/
	ASSIGN,SEMI,COMMA,LPAR,RPAR  /*��ֵ���ֺš����š������š�������*/
} Tag;

Tag token;
#define MAXTOKENSIZE 9  /*���ʵ���󳤶�Ϊ9*/
#define MAXCHILDERN 3    /*�﷨�����ӽڵ��������*/

typedef Tag TokenType;   /*�ʷ���Ԫ������*/
typedef Tag NodeKind;   /*�﷨���ڵ������*/

/*�����ֱ��������ַ��ű��еı����볣��*/
typedef enum{
	NULCAT,VARCAT,CONCAT  /*����𡢱���������*/
}Cat;

/*��������*/
typedef enum{
NULTYPE,INTTYPE,REALTYPE,ERRORTYPE/*�����͡����͡�ʵ�͡����ʹ���*/
}Type;
//�ؼ��ֱ�
#define MAXRESERVEWORDS 3  /*������ֻ��3����mod, int, real*/
//����һ������������reserveword[3]
struct {
	char* str;  /*�����ֵĵ���ƴд*/
	TokenType tok;  /*��Ӧ�Ĵʷ���Ԫ����*/
}reserveword[MAXRESERVEWORDS]={
	{"mod",MOD},
	{"int",INT},
	{"real",REAL}
};  /*���������������ֵĵ���ƴд������ʷ���Ԫ����*/



//��ȡ�ַ��ͷ����ַ�
#define BUFSIZE 128  /*��������С*/
static char buf[BUFSIZE]="\0";  /*��β�����ַ���������־����ֹ������Χ*/

/*ȡ��һ���ַ���ÿ�ζ���һ�У������е��ַ�ȡ��֮���ٶ�ȡ��һ��*/
/*row��col��ȫ�ֱ������ڷ����﷨����ʱ������row�����������ڵ��к�*/
//��ȡ����
static char nextch(){
	if(col>=(int)strlen(buf)){  /*�����ǰ���е��ַ���ȡ��*/
		row++;  /*������1*/
		if(fgets(buf,BUFSIZE-1,source)){   /*��ȡ�µ�һ��*/
			col=0;
			return buf[col++];  /*���ظն�ȡ�����еĵ�һ���ַ�*/
		}
		return EOF;  /*�ļ��ѽ���*/
	}
	return buf[col++];  /*�����ǰ���е��ַ���δȡ�꣬�����ȡ����һ���ַ�*/
}

/*����һ���ַ�*/
static void retract(){
	col--;
}
//ʶ������
/*�뱣�����б��е�3�������ֵĵ��ʽ��бȽϣ������3��������֮һ�򷵻���ʷ���Ԫ���ƣ����򷵻�ID*/
TokenType rsfind(char* s){
	int i;
	for(i=0;i<MAXRESERVEWORDS;i++)  /*��3�������ֽ��бȽ�*/
		if(!strcmp(s,reserveword[i].str)){
			return reserveword[i].tok;
		}
		return ID;  /*������Ǳ����֣�����Ϊ�Ǳ�ʶ��������ID*/
}

//�ж��Ƿ�Ϊ�ո�
bool isspace(char c){
	if(c==' ')
		return true;
	else
		return false;
}

//�ж��Ƿ�������
bool isdigit(char c){
	if(c>='0'&&c<='9')
		return true;
	else
		return false;
}
//�ж��Ƿ�����ĸ
bool isalpha(char c)
{
	if((c>='a'&&c<='z')||(c>='A'&&c<='Z'))
		return true;
	else
		return false;

}
//num���������ǣ�ƥ��ʷ���ԪNUM��RNUM����������Ĵ��ظ���tokenString����NUM��RNUM����token
/* tokenString ���ַ������͵�ȫ�ֱ�������ŵ�ǰ�Ĵ���*/
//NUM������RNUMС��
void num(char c){
	tokenString[p++]=c;

	token=NUM;
}

/*ƥ���ʶ�����ں���id�е���rsfind �ж��Ƿ��Ǳ�����*/

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
//��ȡһ�����ʣ���ƥ���Ӧ�Ĵʷ���Ԫ
void tkget(){
	char ch;
	strcpy(tokenString,"\0");	 /* tokenString ���ַ������͵�ȫ�ֱ�������ŵ�ǰ�Ĵ���*/
	while(isspace(ch=nextch()));  /*ȥ���ո�*/
	if(isdigit(ch)){  /*���������*/
		num(ch);  /*Ҫ���Լ�дnum������num���������ǣ�ƥ��ʷ���ԪNUM��RNUM����������Ĵ��ظ���tokenString����NUM��RNUM����token*/
	}
	else if(isalpha(ch)){  /*�������ĸ*/
		id(ch);  /*ƥ���ʶ�����ں���id�е���rsfind �ж��Ƿ��Ǳ�����*/
	}
	else if(ch==EOF){		  /*�ļ�����*/
		token=ENDF;
	}			
	else{
		switch(ch){
			case '+':  /*ʶ����Ӻ�*/
				token=ADD;
				break;
			case '*':  /*ʶ����˺�*/
				token=MUL;
				break;
			case ';':  /*ʶ����ֺ�*/
				token=SEMI;
				break;
			case ',':  /*ʶ�������*/
				token=COMMA;
				break;
			case '(':  /*ʶ���������*/
				token=LPAR;
				break;
			case ')':  /*ʶ���������*/
				token=RPAR;
				break;
			case '=':  /*ʶ���=*/
				token=ASSIGN;
				break;
			default:  /*���ִʷ�����*/
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
