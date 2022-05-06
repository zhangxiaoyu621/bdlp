#include "mainwindow.h"
#include "ui_mainwindow.h"
#ifndef GRAPH_H
#define GRAPH_H
#include "parser.h"
#include <QDebug>
#include <QMultiMap>
#include <QMapIterator>
#include <QListIterator>

class graph
{
public:
    graph(parser &my_marser); // 操作在构造函数完成
    void dfs(int x, int deep); // dfs

    QMap<QString, int> author_node; // key:作者名 value:编号
    QMultiMap<int, int> cooperation; // key:作者编号,values:比作者编号大的合作者
};

#endif // GRAPH_H
