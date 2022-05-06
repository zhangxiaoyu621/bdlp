#ifndef PARSER_H
#define PARSER_H
#include <QXmlStreamReader>
#include <QDebug>
#include <QMultiMap>
#include <QMap>
#include <QList>
class parser
{
public:
    parser();//构造
    void parse_main();//解析器功能实现主体
    void setFilename(QString s);//设置文件名
    QString Filename();//返回文件名
    void index_clear();
//    void setPos(long long int l, long long int r);//设置左右边界值（暂未用到）
    void begin_parse();//开始解析

public:
    qint64 length;//file.size();

    //title和autor都是用这个struct存放
    struct String_data
    {

        int index;
        QString s_data;
        String_data(QString s, int c)
        {
            s_data = s;
            index = c;
        }
        String_data& operator=(const String_data& cls);
    };

    //mdate和key共同存放在同一个struct里
    struct String_data_else
    {

        int index;//索引值
        QString m_date, m_key;//mdate和key的value
        String_data_else(QString s, QString s1,int c)
        {

            m_date = s;
            m_key = s1;
            index = c;
        }
        String_data_else& operator=(const String_data_else& cls);//重载“=”
    };
    bool author_judge(const char* s, long long int pos);//判断是否已经遍历到了“author”
    bool title_judge(const char* s, long long int pos);//同上，同时key（）值加1，即索引值（index）加1
    bool else_judge(long long int &pos);//判断文件的char是否读到了“mdate”和“key”
    bool year_judge(const char* s, long long int pos);
    String_data read_author(long long int pos,int count);//实现把author的值放到qmultimap里
    String_data read_title(long long int pos,int count);//实现把title的值放到qmap里
    QString read_year(long long int pos,int count);
    String_data_else read_else(long long int pos, int count);//实现把mdate和key放到qmap里
    String_data read_editor(long long int pos,int count);
    QMultiMap<int, parser::String_data> author_index;//作者为multimap，考虑到一个key对应多个作者
    QMap<int, parser::String_data> title_index;//题目
    QMap<int, String_data_else>else_index;//mdate和key
    QMap<int, QString>year_index;//mdate和key
    //注意：同一篇文章的题目、作者、else（mdate、key）的key（）是一样的int类型，一样的int值

private:
    long long int _l, _r;
    QString filename_xml;
    static uchar* file_data;//将整个dblp.xml文件以动态数组的形式存储、遍历。

};

#endif // PARSER_H



