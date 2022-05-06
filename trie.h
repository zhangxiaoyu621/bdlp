#ifndef TRIE_H
#define TRIE_H
#include<QString>
#include <QDebug>
#include <QSet>
#include <QMultiMap>
class trie
{
public:
    char ch[67];
    trie()
    {
        for(int i=0;i<=25;i++)
        {
           ch[i] = char(i+65);
        }
        for(int i = 26;i<52;i++)
        {
            ch[i] = char(i+71);
        }
        for(int i=54;i<64;i++)
        {
            ch[i] = char(i-6);
        }
        ch[52] = '.';
        ch[53] = ' ';
        ch[64] = '&';
        ch[65] = ';';
        ch[66] = '-';
    }
    int topten[10];
    struct node//节点定义
    {
        node* next[67];//67个孩子节点

        long long int cnt = 0;//作count统计数目
        int visited = 0;
        node()//初始化节点
        {
            cnt = 0;
            visited = 0;
            for(int i=0;i<=66;i++)//各个孩子节点为空
            {
                next[i] = nullptr;
            };
        }
    };
    struct new_int//新整数,用于将key值倒序排序
    {
        int id;
        QString name;
        //重载操作符
        bool operator < (const new_int& per) const
        {
            return id > per.id; //根据id降序排列
        }
    };
    void ini()//初始化root
    {
        root = new node;
    }
    static node* root;
    QMultiMap<new_int,QString> amap;//题目
public:
    void buidTree(QString qstr_authorname);
    void findTree(QString& str,trie::node*p);
    void del(trie::node*_root);

};
#endif // TRIE_H
