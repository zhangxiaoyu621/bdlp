#include "trie.h"
#include <QTextCodec>
#include <QFile>
#include <QTextStream>
#include <QString>
#include <QDataStream>

trie::node* trie::root = nullptr;

void trie::buidTree(QString qstr_authorname)
{

    char* s;
    QByteArray ba = qstr_authorname.toLatin1();
    s=ba.data();
    trie::node* p = trie::root;
    trie::node* tmp = nullptr;
    int l = strlen(s);

    for (int i = 0; i < l; i++)
    {


        if (s[i] != '.'&& s[i] != ' ')
        {
            if(int(s[i])>=97&&int(s[i])<=122)
            {
                if (p->next[int(s[i] - 'a')+26] == nullptr)
                {
                    tmp = new trie::node;
                    p->next[int(s[i] - 'a')+26] = tmp;
                }
                p = p->next[int(s[i] - 'a')+26];

            }
            else if(int(s[i])>=65&&int(s[i])<=90)
            {
                if (p->next[int(s[i] - 'A')] == nullptr)
                {
                    tmp = new trie::node;
                    p->next[int(s[i] - 'A')] = tmp;
                }
                p = p->next[int(s[i] - 'A')];
            }
            else if(int(s[i])>=48&&int(s[i])<=57)
            {
                if (p->next[int(s[i])+6] == nullptr)
                {
                    tmp = new trie::node;
                    p->next[int(s[i])+6] = tmp;
                }
                p = p->next[int(s[i])+6];
            }



        }
        else if (s[i] == '.')
        {
            if (p->next[52] == nullptr)
            {
                tmp = new trie::node;
                p->next[52] = tmp;
            }
            p = p->next[52];
        }
        else if (s[i] == ' ')
        {
            if (p->next[53] == nullptr)
            {
                tmp = new trie::node;
                p->next[53] = tmp;
            }
            p = p->next[53];
        }
        else if(s[i] == '&')
        {
            if (p->next[64] == nullptr)
            {
                tmp = new trie::node;
                p->next[64] = tmp;
            }
            p = p->next[64];
        }
        else if(s[i] == ';')
        {
            if (p->next[65] == nullptr)
            {
                tmp = new trie::node;
                p->next[65] = tmp;
            }
            p = p->next[65];
        }
        else if(s[i]=='-')
        {
            if (p->next[66] == nullptr)
            {
                tmp = new trie::node;
                p->next[66] = tmp;
            }
            p = p->next[66];
        }
    }

    p->cnt++;


}
void trie::findTree(QString &str,trie::node*p)
{


   for(int i=0;i<=66;i++)
   {

       if(p->next[i] != nullptr)
       {
           str= str+ch[i];

           if(p->next[i]->cnt>0)
           {
               new_int a;
               a.id = p->next[i]->cnt;
               amap.insert(a,str);

           }

           findTree(str,p->next[i]);

           str = str.left(str.size() - 1);
       }

   }
   return;

}

void trie::del(trie::node*_root)
{
    for(int i=0;i<=66;i++)
    {

        if(_root->next[i] != nullptr)
        {

            del(_root->next[i]);
            delete _root->next[i];

        }
        else if(_root->next[i] == nullptr)
        {

            delete _root->next[i];

        }


    }
    return;
}

