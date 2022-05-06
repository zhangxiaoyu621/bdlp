#include "parser.h"
#include <QTextCodec>
#include <QFile>
#include <QTextStream>
#include <QString>
#include <QDataStream>
#include <QRegExp>
#include <QMap>
#include <QHash>
#include <QDate>
#include <QMultiMap>
#include <regex>
#include <QList>
bool parser::author_judge(const char* s, long long int pos)//判断当前读入的是否为author标签
{
    long long int x = pos;
    while (*s != 0)
    {
        if (*s != (char)file_data[x]) return false;
        ++s;
        ++x;
        //if (x >= right) return false;
    }
    return true;
}

bool parser::title_judge(const char* s, long long int pos)//判断当前读入的是否为title标签
{
    long long int x = pos;
    while (*s != 0)
    {
        if (*s != (char)file_data[x]) return false;
        ++s;
        ++x;

    }
    return true;
}

bool parser::year_judge(const char* s, long long int pos)//判断当前读入的是否为year标签
{
    long long int x = pos;
    while (*s != 0)
    {
        if (*s != (char)file_data[x]) return false;
        ++s;
        ++x;

    }
    return true;
}

bool parser::else_judge(long long int &pos)//判断当前读入的是否为标识文献类型的六大标签之一
{
    std::string s_ary[7] = { "<article","<book","<inproceedings","<proceedings","<incollection","<phdthesis","<mastwesthesis" };
    long long int x = pos;
    for (int y = 0; y < 7; y++)
    {
        const char* s = s_ary[y].c_str();//转换

        while (*s != 0)
        {

            if (*s != (char)file_data[x]) goto here;
            ++s;
            ++x;

        }
        pos = x;
        return true;
        here:
        x = pos;
    }
    return false;

}

parser::String_data& parser::String_data::operator = (const String_data& clk)//重载String_data结构的等号运算符,便于后续操作
{
    this->s_data = clk.s_data;//数据赋值
    this->index = clk.index;//索引值赋值
    return *this;
}

parser::String_data_else& parser::String_data_else::operator = (const String_data_else& clk)//重载String_data_else的等号运算符
{
    this->m_date = clk.m_date;
    this->m_key = clk.m_key;
    this->index = clk.index;
    return *this;
}

parser::parser()
{

}

uchar* parser::file_data = nullptr;//初始化为指向空值

parser::String_data parser::read_author(long long int pos, int count)//读取author标签内的作者名称
{

    long long int i = pos;int _pos = 0;
    char* ka;
    ka = new char[2000];//动态数组存放姓名
    while (1)
    {
        if ((char)file_data[i] == '>')
        {
            while (1)
            {
                if ((char)file_data[i + 1] == '<')
                {
                    QString _authorname="";
                    for(int p=0;p<_pos;p++)
                    {
                        _authorname = _authorname + ka[p];
                    }
                    QString searchText = "\r\n";//正则表达式.若读入字符为这二者转义字符,则需要转换为空格
                    QString replaceText = " ";//替换符号为空格
                    QString authorname =_authorname.replace(QRegExp(searchText), replaceText);
                    //qDebug()<<authorname;
                    delete [] ka;//删除

                    return String_data(authorname, count);
                }
                ka[_pos] = (char)file_data[i + 1];
                i++;
                _pos++;
            }

        }
        i++;
    }

}

parser::String_data parser::read_editor(long long int pos, int count)
{

    long long int i = pos;int _pos = 0;
    char* ke;
    ke = new char[200];
    while (1)
    {
        if ((char)file_data[i] == '>')
        {
            while (1)
            {
                if ((char)file_data[i + 1] == '<')
                {
                    QString _authorname="";
                    for(int p=0;p<_pos;p++)
                    {
                        _authorname = _authorname + ke[p];
                    }
                    QString searchText = "\r\n";
                    QString replaceText = " ";
                    QString authorname =_authorname.replace(QRegExp(searchText), replaceText);
                    //qDebug()<<authorname;
                    delete [] ke;

                    return String_data(authorname, count);
                }
                ke[_pos] = (char)file_data[i + 1];
                i++;
                _pos++;
            }

        }
        i++;
    }

}

parser::String_data parser::read_title(long long int pos, int count)
{
    long long int i = pos;int _pos = 0;
    char* kt;
    kt = new char[12000];
    while (1)
    {
        if ((char)file_data[i] == '>')
        {
            while (1)
            {
                if ((char)file_data[i + 1] == '<'&&(char)file_data[i + 2] == '/'&&(char)file_data[i + 3] == 't'&&(char)file_data[i + 4] == 'i')
                {
                    QString _titlename="";
                    for(int p=0;p<_pos;p++)
                    {
                        _titlename = _titlename + kt[p];
                    }
                    QString searchText = "\r\n";
                    QString replaceText = " ";
                    QString titlename =_titlename.replace(QRegExp(searchText), replaceText);
                    //qDebug()<<authorname;
                    delete [] kt;

                    return String_data(titlename, count);
                }
                kt[_pos] = (char)file_data[i + 1];
                i++;
                _pos++;
            }

        }
        i++;
    }
}

parser::String_data_else parser::read_else(long long int pos, int count)
{
    long long int i = pos;
    int _pos1 = 0,_pos2 = 0;
    char* ke; char* ky;
    ke = new char[200];
    ky = new char[400];

    for(i = pos;;i++)
    {
        if((char)file_data[i] == 'm'&&(char)file_data[i+1] == 'd'&&(char)file_data[i+2]== 'a'&&(char)file_data[i+3] == 't'&&(char)file_data[i+4]== 'e')//判断是否读到"mdate"这一字符串
        {
            i = i+4;
            for(;;i++)
            {
                if((char)file_data[i] == '"')//mdate读取开始
                {
                    i++;
                    break;
                }
            }
            break;
        }

    }
    for(;;i++,_pos1++)
    {
        if((char)file_data[i]=='"')//mdate结束
        {
            break;
        }
        ke[_pos1] = (char)file_data[i];
    }

    for(;;i++)
    {
        if((char)file_data[i] == 'k'&&(char)file_data[i+1] == 'e'&&(char)file_data[i+2]== 'y')//判断是否读到key
        {
            i = i+2;
            for(;;i++)
            {
                if((char)file_data[i] == '"')//key值读取开始
                {
                    i++;

                    break;
                }
            }
            break;
        }

    }
    for(;;i++,_pos2++)
    {
        if((char)file_data[i]=='"')//key读取结束
        {
            break;
        }
        ky[_pos2] = (char)file_data[i];
    }
    QString _mdatename="";
    QString _keyname = "";
    for(int p=0;p<_pos1;p++)
    {
        _mdatename = _mdatename + ke[p];
    }
    for(int p=0;p<_pos2;p++)
    {
        _keyname = _keyname + ky[p];
    }
    QString searchText = "\r\n";//正则替换,具体同上
    QString replaceText = " ";
    QString mdatename =_mdatename.replace(QRegExp(searchText), replaceText);
    QString keyname =_keyname.replace(QRegExp(searchText), replaceText);
//    qDebug()<<mdatename<<"+"<<keyname;
    delete [] ky;delete [] ke;

    return String_data_else( mdatename,keyname, count);
}

QString parser::read_year(long long pos, int count)
{
    long long int i = pos;int _pos = 0;
    char* ky;
    ky = new char[20];
    while (1)
    {
        if ((char)file_data[i] == '>')
        {
            while (1)
            {
                if ((char)file_data[i + 1] == '<'&&(char)file_data[i + 2] == '/'&&(char)file_data[i + 3] == 'y'&&(char)file_data[i + 4] == 'e')
                {
                    QString _year="";
                    for(int p=0;p<_pos;p++)
                    {
                        _year = _year + ky[p];
                    }
                    QString searchText = "\r\n";
                    QString replaceText = "";
                    QString year=_year.replace(QRegExp(searchText), replaceText);
                    //qDebug()<<authorname;
                    delete [] ky;

                    return year;
                }
                ky[_pos] = (char)file_data[i + 1];
                i++;
                _pos++;
            }

        }
        i++;
    }
}

void parser::setFilename(QString s)//设置文件名称
{
    filename_xml = s;
}

QString parser::Filename()//返回文件名称
{
    return filename_xml;
}

void parser::index_clear()
{
    QFile("author.dat").remove();
    QFile("title.dat").remove();
    QFile("else.dat").remove();

}

void parser::begin_parse()//开始解析文件
{
    parse_main();
}

void parser::parse_main()//解析文件的函数主体
{

    //QFile 以只读方式打开文件
    QFile file(filename_xml);
    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "Can't open the file!" << endl;
    }



    qint64 len = file.size();//返回无符号整数类型,文件的大小
    qDebug()<<file.size();

    file_data = file.map(0, len);//内存映射
    if (file_data != nullptr)
    {
        qDebug()<<"文件映射成功";
    }


    length = len;

    String_data new_s(0, len);
    String_data_else new_else("", "", 1);


    static int count = 1;
    long long int i = 0;


    while (i < length)//没有atEnd
    {


        if ((char)file_data[i] == '<')
        {


            if (author_judge("author", i + 1))//进行作者标签判断
            {

                String_data author = read_author(i + 1, count);//读取作者
                author_index.insert(author.index,author);

            }
            else if (author_judge("editor", i + 1))//进行编辑者标签判断
            {

                String_data author = read_editor(i + 1, count);//读取编辑者
                author_index.insert(author.index,author);

            }
            else if (title_judge("<title", i ))//进行题目的标签判断
            {
                String_data title = read_title(i + 1, count);//读取题目
                title_index.insert(title.index,title);
            }
            else if (else_judge(i))//进行其他信息的相关判断和六大类型之一的判断
            {
                String_data_else s_else = read_else(i + 1, count);

                     else_index.insert(s_else.index, s_else);
                     if(count%100000==0)//便于开发者跟进解析进度
                {
                    qDebug()<<count;
                }
                count++;

            }
            else if (year_judge("<year",i))//年份判断
            {
                QString year = read_year(i + 1, count-1);
                year_index.insert(count-1,year);

            }
            if(i==len-5)
            {
                qDebug() << "XML.file is parsed successfully!" << endl;

                break;

            }
        }
        ++i;
        if(i==len-5)
        {
            qDebug() << "XML.file is parsed successfully!" << endl;

            break;

        }
    }
    file.unmap(file_data);//删除动态内存

}
