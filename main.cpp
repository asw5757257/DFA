#include <iostream>
#include <algorithm>
#include <vector>
#include <stack>
#include <string.h>
#include <string>
#define MAX 100
#define TRUE 1;
#define FALSE 0;
using namespace std;
struct Node
{
    Node(int id, char input, int nextId)
    {
        this->id = id;
        this->input = input;
        this->nextId = nextId;
    }
    int id;
    char input;
    int nextId;
};
class Myclass
{	public:
		Myclass()
		{
			str="";
			Id=0;
		}
		Myclass(string s)
		{
			str=s;
		}	
		void Printf();
		void strToNFA();
		string str;
		int Id;
		vector<Node*> map;

};




string CHANGE;//转义字符集合 
int Rp=0;
struct edge//边 
{
	char preNode;//头节点 
	char nextNode;//尾节点 
	char exchar;//转义字符 
};
struct newJI
{
	string stJ;//得到的闭包集合 
	bool mark;
};
struct Relation//化简后子集之间的联系 
{
	string pre;
	char ttchar;
	string next;
	int po[2];
	int size;
};
void Myclass::strToNFA()
{
	stack<int> Start;
	stack<int> End;
	stack<char> Str; 
	Str.push('$');
	int Id=0;
	Start.push(Id++);
	End.push(Id++);
	for(int i=0;i<str.size();i++)
	{
		char ch=str[i];
		cout<<ch;
		switch(ch)
		{
			case '(':
				{
					End.push(Id++);//cout<<Id;
					Str.push('(');
					break;
				}
			case')':
				{	int next = End.top();
                	int pre = Start.top();
                	map.push_back(new Node(pre, '-', next));
					ch=Str.top();
					while(ch!='(')
					{
						int next=Start.top();
						Start.pop();
						int pre=Start.top();
						if(ch!='#')
						{
							map.push_back(new Node(pre,ch,next));
						}
						
						Str.pop();
						ch=Str.top();
					}
					Str.pop();
					Str.push('#');
					int top=End.top();
					Start.push(top);
					End.pop();
					break;
				}
			case'*':
				{
					int next=Start.top();//cout<<next;
					Start.pop();
					int pre=Start.top();
					
					map.push_back(new Node(pre,'-',next));
					map.push_back(new Node(next,'-',pre));
					Start.push(next);
					break;
					
				 } 
			case'|':
				{
					int next=End.top();
					int pre=Start.top();
					map.push_back(new Node(pre,'-',next));
					ch=Str.top();
					while(ch!='('&&ch!='$')
					{
						int next=Start.top();
						Start.pop();
						int pre=Start.top();
						if(ch!='#')
						
							map.push_back(new Node(pre,ch,next));
						
						Str.pop();
						ch=Str.top();
					}
					break;
				}
			default:
				{
					Str.push(ch);
					Start.push(Id++);
					break;
				}
		}
		
	}
	char ch=Str.top();
    while(ch!='$')
    {
        int next = Start.top();
        Start.pop();
        int pre = Start.top();
        if(ch!='#')
            map.push_back(new Node(pre,ch,next));
        Str.pop();
        ch=Str.top();
    }
}
void Myclass::Printf()
{
	cout<<"NFA:"<<endl;
	for(int i=0;i<map.size();i++)
	{
		cout<<map[i]->id<<"-"<<map[i]->input<<"->"<<map[i]->nextId<<endl;
	}
}


void eps_closure(char c,string &a,edge b[],int N)
{	
	for(int i=0;i<N;i++)
	{
		if(c==b[i].preNode&&b[i].exchar=='-')
		{
				if(a.find(b[i].nextNode)>a.length())
				{
					a+=b[i].nextNode;//cout<<a<<endl;
					eps_closure(b[i].nextNode,a,b,N);
				}
		}
	}
	
}

void move(char c,edge e[],int N,newJI a,string &b) //e为所有边的集合，然后就能从一个转换字符得到全部的，比如2得到bd，而不会第一个2得到b，第二个2得到d
{	
    for (int i=0;i<a.stJ.length();i++) //对每个状态集合中的状态取出
    {
        for (int j=0;j<N;j++)
        {
            if (e[j].preNode==a.stJ[i]&&e[j].exchar==c)//头节点相同 转义字符相同 
            {
                if(b.find(e[j].nextNode)>b.length())
                {
                	b+=e[j].nextNode;
				}
            }
        }
    }
}
int ZIJI(newJI T0,int LEN,edge e[],int N,string CHANGE,newJI C[],Relation RE[],int Rp)
{	 
	int sum=1;C[0].stJ=T0.stJ;int idx=0;int pos=0;
	while(sum>=0)
	{
	
		C[idx].mark=TRUE;
		//cout<<C[idx].stJ<<endl;
		for(int i=0;i<LEN;i++)//LEN等于转义字符数量 
		{	string a;
			//cout<<"C[pos]"<<C[pos].stJ<<endl;
			move(CHANGE[i],e,N,C[idx],a);
			int m=a.length();//cout<<a<<" ";cout<<endl;
			for(int j=0;j<m;j++)//求求得的move集合的闭包 
			{
				eps_closure(a[j],a,e,N);
			}
		//	cout<<a<<" ";cout<<endl;
			
			sort(a.begin(), a.end());//对字符串排序 
				int k=0;
				for(;k<pos+1;k++)
				{
					int panduan=a.compare(C[k].stJ);
					if(panduan==0)
					{
					RE[Rp].pre=C[idx].stJ;
					RE[Rp].next=C[k].stJ;
					RE[Rp].ttchar=CHANGE[i];
					RE[Rp].po[0]=idx;
					RE[Rp].po[1]=k;
					Rp+=1;
					sum-=1;
					break;
					}
				}
				if(k==pos+1)
				{	//cout<<" yes ";
					C[pos+1].stJ=a;
					//cout<<"a:"<<C[pos+1].stJ<<" ";
					C[pos+1].mark=FALSE;
					//RE[pos].pre=C[pos].stJ;
					RE[Rp].pre=C[idx].stJ;
					RE[Rp].next=C[pos+1].stJ;
					RE[Rp].ttchar=CHANGE[i];
					RE[Rp].po[0]=idx;
					RE[Rp].po[1]=pos+1;
					Rp+=1;
					pos+=1;
					sum+=1;
				}
		}
		idx+=1;
		
		
	}
	return Rp;
}
int main()
{
	string str;
	stack<int> s;
	cout<<"请输入一个正则表达式"<<endl;
	//cin>>str;
	str="(a|b)*abb";//cout<<str.size();
	for(int i=0;i<str.size();i++)
	{
		if(str[i]>='a'&&str[i]<='z'||str[i]>='A'&&str[i]<='Z')
		{	//cout<<str[i];
			//cout<<str.find(str[i]);
			if(CHANGE.find(str[i])>CHANGE.size())
			{
				CHANGE+=str[i];
			}
		}
	}
	Myclass myclass(str);
	myclass.strToNFA();
	myclass.Printf();
	cout<<endl;
	edge e[100];
	string m;
	newJI T0;
	//string CHANGE="ab";
	int N=myclass.map.size();
	int LEN=CHANGE.size();
	//cout<<CHANGE<<LEN;
	newJI C[100];
	Relation RE[100];
	for(int i=0;i<myclass.map.size();i++)
	{
		e[i].preNode=char(myclass.map[i]->id+48);
		e[i].exchar=myclass.map[i]->input;
		e[i].nextNode=char(myclass.map[i]->nextId+48);
	}
	int first;
	for(int i=0;i<N;i++)
	{
		if(e[i].preNode=='0')
		{
			first=i;
			break;
		}
	}
	cout<<endl;
	T0.stJ+=e[first].preNode;//cout<<T0.stJ<<" i"; 
	eps_closure('0',m,e,N);
	T0.stJ='0';
	T0.stJ+=m;
	int pos=ZIJI(T0,LEN,e,N,CHANGE,C,RE,Rp);
	cout<<endl;
	cout<<"DFA"<<endl;
	for(int i=0;i<pos;i++)
	{
	cout<<RE[i].po[0]<<"-";
	cout<<RE[i].ttchar<<"->";
	cout<<RE[i].po[1]<<" ";
	cout<<endl;
	}
	return 0;
 } 
