#include "title_trie.h"

title_trie::node* title_trie::root = nullptr;//初始化静态变量根节点为空
title_trie::title_trie()//在ch中设置字符与序号对应关系
{
    for(int i=0;i<26;i++)
    {
        ch[i] = char(i+97);
    }
    ch[26] = '$';
    ch[27] = '%';
    ch[28] = char(39);
    ch[29] = '*';
    ch[30] = '+';
    ch[31] = '-';
    ch[32] = '/';
    ch[44] = '^';
    ch[43] = '=';
    for(int i=33;i<=42;i++)
    {
        ch[i] = char(i+15);
    }
}
void title_trie::buidTree(QString qstr_titlename)
{

    char* s;
    QByteArray ba = qstr_titlename.toLatin1();//类型转换
    s=ba.data();
    title_trie::node* p = title_trie::root;
    title_trie::node* tmp = nullptr;

    int l = strlen(s);//当前单词长度
    for (int i = 0; i < l; i++)
    {
        if(int(s[i])>64&&int(s[i])<=90)//大写字母转换为小写
        {
            s[i] = char(s[i]+32);
        }
        if(s[i]-'a'>=0 && 'z'-s[i]>=0)
        {
            if (p->next[int(s[i] - 'a')] == nullptr)//如果该字符对应节点为空
            {
                tmp = new title_trie::node;
                p->next[int(s[i] - 'a')] = tmp;
            }
            p = p->next[int(s[i] - 'a')];//不为空则跳转至该节点
        }
        else if(s[i] - '0' >= 0 && '9' - s[i] >=0)
        {
            if (p->next[int(s[i])-15] == nullptr)
            {
                tmp = new title_trie::node;
                p->next[int(s[i])-15] = tmp;
            }
            p = p->next[int(s[i])-15];
        }
        else if(s[i]=='$')
        {
            if (p->next[26] == nullptr)
            {
                tmp = new title_trie::node;
                p->next[26] = tmp;
            }
            p = p->next[26];
        }
        else if(s[i]=='%')
        {
            if (p->next[27] == nullptr)
            {
                tmp = new title_trie::node;
                p->next[27] = tmp;
            }
            p = p->next[27];
        }
        else if(s[i]==char(39))
        {
            if (p->next[28] == nullptr)
            {
                tmp = new title_trie::node;
                p->next[28] = tmp;
            }
            p = p->next[28];
        }
        else if(s[i]=='*')
        {
            if (p->next[29] == nullptr)
            {
                tmp = new title_trie::node;
                p->next[29] = tmp;
            }
            p = p->next[29];
        }
        else if(s[i]=='+')
        {
            if (p->next[30] == nullptr)
            {
                tmp = new title_trie::node;
                p->next[30] = tmp;
            }
            p = p->next[30];
        }
        else if(s[i]=='-')
        {
            if (p->next[31] == nullptr)
            {
                tmp = new title_trie::node;
                p->next[31] = tmp;
            }
            p = p->next[31];
        }
        else if(s[i]=='=')
        {
            if (p->next[43] == nullptr)
            {
                tmp = new title_trie::node;
                p->next[43] = tmp;
            }
            p = p->next[43];
        }
        else if(s[i]=='/')
        {
            if (p->next[32] == nullptr)
            {
                tmp = new title_trie::node;
                p->next[32] = tmp;
            }
            p = p->next[32];
        }
        else if(s[i]=='^')
        {
            if (p->next[44] == nullptr)
            {
                tmp = new title_trie::node;
                p->next[44] = tmp;
            }
            p = p->next[44];
        }
    }

    p->cnt++;


}
void title_trie::findTree(QString &str,title_trie::node*p)
{
    for(int i=0;i<=44;i++)//对孩子节点进行遍历
    {
        if(p->next[i] != nullptr)//如果非空，即不为叶子结点
        {
            str= str+ch[i];

            if(p->next[i]->cnt>0)
            {
                new_int a;//定义新整数类型
                a.id = p->next[i]->cnt;
                words.insert(a,str);

            }
            findTree(str,p->next[i]);//递归遍历思想
            str = str.left(str.size() - 1);
        }
    }
    return;
}
void title_trie::del(title_trie::node*_root)
{
    for(int i=0;i<=44;i++)
    {

        if(_root->next[i] != nullptr)
        {

            del(_root->next[i]);//递归遍历各节点
            delete _root->next[i];

        }
        else if(_root->next[i] == nullptr)
        {

            delete _root->next[i];

        }
    }
    return;
}
