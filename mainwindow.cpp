#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "parser.h"
#include "trie.h"
#include "title_trie.h"
#include <QDebug>
#include <QMap>
#include <QAction>
#include <QMultiMap>
#include <QList>
#include <QCheckBox>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QMessageBox>
#include <QDir>
#include "graph.h"
parser my_parser;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QString filename ="E://dblp.xml";
    my_parser.setFilename(filename);//设置文件名称
    my_parser.begin_parse();//开始解析该文件

  //以下所有icon都存放在电脑的E盘中
    ui->pButton->setIcon(QIcon("E://author_serch.png"));//搜索作者这一按钮的icon
    ui->pButton->setIconSize(QSize(100, 100));//设置大小
    ui->pButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);//设置位置

    ui->tButton->setIcon(QIcon("E://title_serch.png"));//搜索题目这一按钮的icon
    ui->tButton->setIconSize(QSize(100, 100));
    ui->tButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    ui->titlefuzzy->setIcon(QIcon("E://fuzzysearch.png"));//搜索部分匹配搜索这一按钮的icon
    ui->titlefuzzy->setIconSize(QSize(100, 100));//设置大小
    ui->titlefuzzy->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);//设置位置

    ui->TTbutton->setIcon(QIcon("E://top.png"));//搜索作者这一按钮的icon
    ui->TTbutton->setIconSize(QSize(80, 80));//设置大小

    ui->statusbar->showMessage("Welcome!The file has already been parsed.") ;//解析完成后.底部状态栏改变
    connect(ui->titlefuzzy,&QToolButton::clicked,this,&MainWindow::fuzzy_search);//槽函数


}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_tButton_clicked()//按照题目名称搜索
{

    ui->tableWidget->clearContents();//清空表格内容

    QString title_get;
    title_get = ui->lineEdit->text();//获取目标名称

    int work = 1;

    QMap<int,parser::String_data>::iterator it;//迭代器
      for ( it = my_parser.title_index.begin(); it != my_parser.title_index.end(); ++it )
      {
              if(it.value().s_data == title_get)//如果查找到相应文献
              {


                  int uy = it.key();//获取QMap的key值
                  parser::String_data_else s_else("","",0);
                  auto itelse = my_parser.else_index.constFind(uy);
                  auto ityear = my_parser.year_index.constFind(uy);

                  QList<parser::String_data> list_au = my_parser.author_index.values(uy);//将（key，values）键值对中values值存入QList结构之中

                  QString str = "";
                  for ( int i=0; i!=list_au.size(); ++i )
                  {
                      str=str + list_au.at(i).s_data + ", ";
                  }

                  s_else = itelse.value();//获取文献的其他信息
                  QString d = s_else.m_date;//获取文献其他信息中的mdate
                  QString k = s_else.m_key;//获取文献其他信息中的key

                   QString ye = ityear.value();//获取文献发表年份

                  ui->tableWidget->setRowCount(work);//第n行
                  ui->tableWidget->setItem(work-1, 0, new QTableWidgetItem(it.value().s_data));//对应位置输出文章题目
                  ui->tableWidget->setItem(work-1, 1, new QTableWidgetItem(ye));//第二列输出文章发表年份
                  ui->tableWidget->setItem(work-1, 2, new QTableWidgetItem(str));//第三列输出作者
                  ui->tableWidget->setItem(work-1, 3, new QTableWidgetItem(d));//第四列打印mdate
                  ui->tableWidget->setItem(work-1, 4, new QTableWidgetItem(k));//第五列打印输出独有的key

                  work++;
              }
      }
      if (work == 1)//若不存在对应文献,则弹出警告弹窗
      {
          QMessageBox::critical(this, tr("Warning"), tr("The title you are querying does not exist"));

      }

}



void MainWindow::on_pButton_clicked()
{

    if(ui->checkBox->isChecked())//若勾选合作者一栏,则进行合作者搜索
    {
        ui->statusbar->showMessage("Is searching for the co-authors.") ;//改变mainwidget底部状态
        ui->listWidget->clear();//清空

        int i = 0;

        QString author_get;
        author_get = ui->lineEdit->text();//获取查询作者的名称
//        qDebug()<<author_get;
        int work = 0;
        foreach(const int uy,my_parser.author_index.uniqueKeys())//遍历整个QMultiMap查询对应作者
          {

              QString str = "";
              foreach(parser::String_data value,my_parser.author_index.values(uy))
              {
                  if(value.s_data == author_get)//如果当前作者名称与搜寻的作者名称一致
                  {
                       i++;
                       work++;
                       QString temp = QString::number(i, 10);
                       ui->listWidget->insertItem(work,temp);//输出当前文献的序号
                       foreach(parser::String_data value,my_parser.author_index.values(uy))
                       {
                           str=value.s_data;
                           work++;
                           ui->listWidget->insertItem(work,str);//打印合作者
                       }
                       goto here2;

                  }

              }
              here2:;
          }
        if (work == 0)//如果作者不存在,则跳出弹窗进行提醒
        {
            QMessageBox::critical(this, tr("Warning"), tr("The author you are querying does not exist"));

        }
    }
    else
    {
        ui->statusbar->showMessage("Is searching for articles.") ;//改变底部状态兰
        ui->tableWidget->clearContents();//清空原有信息

        QString author_get;
        author_get = ui->lineEdit->text();//获取作者姓名
//        qDebug()<<author_get;
        int work = 1;
        foreach(const int uy,my_parser.author_index.uniqueKeys())//使用foreach函数进行遍历搜索
          {

              QString str = "";
              foreach(parser::String_data value,my_parser.author_index.values(uy))//同一key值下的foreach搜索
              {
                  if(value.s_data == author_get)//如果找到该作者名称
                  {
                       foreach(parser::String_data value,my_parser.author_index.values(uy))//记录该文章的所有作者
                       {
                           str=str + value.s_data + "; ";
                       }
                       parser::String_data_else s_else("","",0);
                       auto itelse = my_parser.else_index.constFind(uy);//获取对应文献的其他信息
                       s_else = itelse.value();
                       QString d = s_else.m_date;//获取mdate
                       QString k = s_else.m_key;//获取key

                       auto ittitle = my_parser.title_index.constFind(uy);
                       QString st = ittitle.value().s_data;//获取该文章题目

                       auto ityear = my_parser.year_index.constFind(uy);
                       QString ye = ityear.value();//获取文献发表年份

                       //在对应的位置进行输出
                       ui->tableWidget->setRowCount(work);
                       ui->tableWidget->setItem(work-1, 0, new QTableWidgetItem(st));
                       ui->tableWidget->setItem(work-1, 1, new QTableWidgetItem(ye));
                       ui->tableWidget->setItem(work-1, 2, new QTableWidgetItem(str));
                       ui->tableWidget->setItem(work-1, 3, new QTableWidgetItem(d));
                       ui->tableWidget->setItem(work-1, 4, new QTableWidgetItem(k));
                       work++;
                       goto here1;

                  }

              }
              here1:;
          }
        if (work == 1)//如果搜不到对应作者.则进行弹窗警示
        {
            QMessageBox::critical(this, tr("Warning"), tr("The author you are querying does not exist"));

        }
    }

}



void MainWindow::on_pushButton_clicked()//贡献度TOP100作者
{
    ui->listWidget_2->clear();//清空

    trie my_trie;//定义一个字典树trie类
    my_trie.ini();//对字典树初始化

    foreach(const int uy,my_parser.author_index.uniqueKeys())//对每一篇文章之中的每一位作者进行遍历
    {

          foreach(parser::String_data value,my_parser.author_index.values(uy))
          {
              QString str = "";
              str=value.s_data;
              my_trie.buidTree(str);//对当前作者的姓名进行字典树的构建

          }
    }
    QString s1 = "";
    my_trie.findTree(s1,trie::root);   //以root为根节点,递归遍历与记录该树的所有数据
    int ii=0;
    ui->statusbar->showMessage("FindTree() is down") ;//记录完成后.底部状态栏改变
    QMultiMap<trie::new_int,QString>::iterator ittop;

   foreach(const trie::new_int uy,my_trie.amap.uniqueKeys())
    {
       QList<QString> myValues = my_trie.amap.values(uy);
       for (int i = 0; i < myValues.size(); ++i)
       {

           if (ii < 100)//若输出作者数小于100,则进行输出
           {
               QString str2 = myValues.at(i) + "   " + QString::number(uy.id);//姓名+所发表的文献数目
               ui->listWidget_2->insertItem(ii, str2);
               ii++;
           }
           else if (ii >= 100)
           {
               goto abc;
           }
       }

    }
    abc:;
    my_trie.del(trie::root);//删除该字典树以释放空间
}


void MainWindow::on_TTbutton_clicked()//年度热词
{
    ui->listWidget_2->clear();


    QString year_get;
    year_get = ui->lineEdit->text();//在输入框获取所查找的年份
    title_trie my_titletrie;//定义热词字典树类
    my_titletrie.ini();//热词字典树初始化
    int work = 1;
    QMap<int,parser::String_data>::iterator it;
    for ( it = my_parser.title_index.begin(); it != my_parser.title_index.end(); ++it )
    {

        auto ityear = my_parser.year_index.constFind(it.key());
        QString ye = ityear.value();
        if(ye == year_get)//如果当前文献的年份与欲查询到年份相同.则进行相应的切割\建树操作
        {
            work = 0;
            QStringList list = it.value().s_data.split(" ");//QString字符串分割函数
            for (QStringList::iterator itstr = list.begin();itstr != list.end(); ++itstr)
            {
                QString current = *itstr;
                my_titletrie.buidTree(current); //对当前单词进行解析建树
            }
        }
    }
    if (work == 1)//如果查询年份没有发表的作品,则跳出警示弹框
    {
        QMessageBox::critical(this, tr("Warning"), tr("The year you are querying does not exist"));

    }

    QString s1 = "";
    my_titletrie.findTree(s1,title_trie::root);
    int ii=0;
    if(work!=1)
    {
    foreach(const title_trie::new_int uy,my_titletrie.words.uniqueKeys())
     {
         QList<QString> myValues = my_titletrie.words.values(uy);
         qDebug()<<myValues.size();
         for (int i=0; i<myValues.size(); ++i)
         {

             if(ii<10)
             {
                 QString value = myValues.at(i);
                 //以下为过滤掉无实义单词,包括介词,助词,连接词等常用词汇;同时过滤掉一些固定出现且频次较高的不具有参考意义的单词
                 if(value=="in" ||value=="an"||value=="problem"||value=="than"||value=="more"||value=="under"|| value == "a"||value=="the"||value=="and"|| value=="of"||value=="for"||value=="as"||value=="new"||value=="with"||value=="on"||value=="based"||value=="from"||value=="to"||value=="over"||value=="by"||value=="using"||value=="through"||value=="data")
                 {
                     goto igno;
                 }
                 QString str2 = value +"   " + QString::number(uy.id);
                 ui->listWidget_2->insertItem(ii,str2);
                 ii++;
             }
             else if(ii>=10)
             {
                 goto outio;
             }
             igno:;
         }

     }
    outio:;
    }
    my_titletrie.del(title_trie::root);//释放删除

}

void MainWindow::fuzzy_search()  //部分匹配搜索
{

   ui->tableWidget->clearContents();
    QString title_get;
    title_get = ui->lineEdit->text();
QStringList search=title_get.split(" "); //将关键词分割开
    int work = 0;
    ui->tableWidget->setRowCount(0);
    QMap<int,parser::String_data>::iterator it;

       for(int i=0;i<search.size();i++)
       {
if(i!=0) //从第二个关键词开始，在tableWidget上减去不符合包含每一个关键词的项
{
           for(int t=ui->tableWidget->rowCount()-1;t>=0;t--) //遍历tableWidget的项
            {
               if(!ui->tableWidget->item(t,0)->text().contains(search[i],Qt::CaseInsensitive))
               {
                ui->tableWidget->removeRow(t); //如果不包含本关键词，将其从tableWidget中删除

             }
           }}
           if(i==0)  //第一个关键词时遍历QMap寻找包含第一个关键词的标题和它们的其他信息
           {
            for ( it = my_parser.title_index.begin(); it != my_parser.title_index.end(); ++it)//遍历title.index的QMap
      {

                int uy=it.key();

              if(it.value().s_data.contains(search[i],Qt::CaseInsensitive)) //大小写都可以匹配



{

                  parser::String_data_else s_else("","",0);

                  auto itelse = my_parser.else_index.constFind(uy); //寻找本标题的其他信息
                  QString str = "";
                  QList<parser::String_data> its=my_parser.author_index.values(uy); //寻找本标题的作者信息
                  for ( int j=0; j!=its.size(); ++j )
                  {

                      str=str + its.at(j).s_data + ", ";
                  }

                  s_else = itelse.value();
                  QString d = s_else.m_date;
                  QString k = s_else.m_key;
                  auto ityear = my_parser.year_index.constFind(uy);//寻找本标题的年份信息
                  QString ye = ityear.value();
                 ui->tableWidget->setRowCount(work+1);
                 ui->tableWidget->setItem(work, 0, new QTableWidgetItem(it.value().s_data)); //向tableWidget中添加标题信息
                 ui->tableWidget->setItem(work, 1, new QTableWidgetItem(ye)); //向tableWidget中添加年份信息
                 ui->tableWidget->setItem(work, 2, new QTableWidgetItem(str)); //向tableWidget中添加作者信息
                 ui->tableWidget->setItem(work, 3, new QTableWidgetItem(d)); //向tableWidget中添加日期信息
                 ui->tableWidget->setItem(work, 4, new QTableWidgetItem(k)); //向tableWidget中添加其他信息
                  work++;
              }
      }
            if(work==0) //如果无关键词匹配项，则弹出警告弹窗
                    {
                        QMessageBox::critical(this, tr("Warning"), tr("There is no title matching the keyword"));
                    }
}
}}

extern int count_num[10000]; //graph.cpp中的各阶完全子图个数数组
void MainWindow::on_pushButton_2_clicked() //聚团分析
{
    ui->listWidget_2->clear();
    for(int i=1;i<200;i++)
    {
        ui->listWidget_2->addItem("Rank ");
        ui->listWidget_2->addItem(QString::number(i)); //完全子图阶数
        ui->listWidget_2->addItem(":");
        ui->listWidget_2->addItem(QString::number(count_num[i]));//各阶完全子图个数
    }
}
