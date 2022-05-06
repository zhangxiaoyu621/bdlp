#include "graph.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
int count_num[10000];// count_num[i]代表整个图i阶完全子图的个数
int path[10000000];// dfs记录路径的数组，路径中所有点可构成完全子图

graph::graph(parser &my_parser)
{
    // 建立“作者名-编号”的映射
    int index = 1;
    foreach(const int i, my_parser.author_index.uniqueKeys())
    {
        foreach(const parser::String_data names, my_parser.author_index.values(i))
        {
            if(!author_node.contains(names.s_data))
            {
                author_node.insert(names.s_data, index++);
            }
        }
    }
    qDebug()<<"author_node done";

    // 建图
    foreach(const int i, my_parser.author_index.uniqueKeys())
    {
        foreach(const parser::String_data names, my_parser.author_index.values(i))
        {
            foreach(const parser::String_data name, my_parser.author_index.values(i))
            {
                if(author_node.value(names.s_data)<author_node.value(name.s_data) && !cooperation.contains(author_node.value(names.s_data), author_node.value(name.s_data)))
                {
                    cooperation.insert(author_node.value(names.s_data), author_node.value(name.s_data));
                }
            }
        }
        if(i%100000==0) qDebug()<<i; // 建图进度条
    }
    qDebug()<<"cooperation done";

    for(int i=1; i<=43; i++) // 每一个点作为起点dfs
    {
        dfs(i,1);
        //qDebug()<<"从第"<<i<<"个点开始的dfs";
    }
    for(int i=0;i<50;i++) // 输出结果（暂时前50阶）
    {qDebug()<<"Rank"<<i<<":"<<count_num[i];
    }
}

void graph::dfs(int x,int deep)
{
    // 当前被搜索点x，层深deep
    count_num[deep]++;
    path[deep]=x;

    //if(x%10000==0) qDebug()<<"某个点开始的第"<<x<<"个";

    foreach(int i, graph::cooperation.values(x))
    {
        bool flag=1;
        for(int j=1; j<deep; j++) // 检查当前被搜索点与路径中所有点是否都有边，有则加入路径，没有则不可能与路径中的点构成完全子图
        {
            if(!graph::cooperation.values(path[j]).contains(i))
            {
                flag=0;
                break;
            }
        }
        if(flag)
        {
            dfs(i, deep+1); // dfs下一层
        }
    }
}
