#ifndef TITLE_TRIE_H
#define TITLE_TRIE_H
#include<QString>
#include <QDebug>
#include <QSet>
#include <QMultiMap>

class title_trie
{
public:
    title_trie();
public:
    char ch[45];
    struct node//节点结构定义
    {
        node* next[45];//子节点数量
        long long int cnt = 0;
        node()//初始化
        {
            cnt = 0;
            for(int i=0;i<45;i++)
            {
                next[i] = nullptr;
            };
        }
    };
    struct new_int//倒序排序整数的new_int类型
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
    QMultiMap<new_int,QString> words;//题目
public:
    void buidTree(QString qstr_titlename);//建树函数
    void del(title_trie::node*_root);//删除树的函数
    void findTree(QString &str,title_trie::node*p);//查找遍历

};

#endif // TITLE_TRIE_H
