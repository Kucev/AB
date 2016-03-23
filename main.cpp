#include <cstdlib>
#include <fstream>
#include <string>
#include <algorithm>
#include <cctype>
#include <vector>
#include <iostream>
#include <dir.h>
#include <cstdio>
#include <windows.h>


using namespace std;

class ABRecord {
public:
	string last_name;
    string first_name;
    string middle_name;
    string phone_number;
    string email;
};

class ABTreeItem {
public:
    vector<ABRecord> m_records;
    string list_name;
    string file_name;
	ABTreeItem(string l_name, string f_name) {
        list_name = l_name;
        file_name = f_name;
	}
};

class AddressBook {
    vector<ABTreeItem> m_treeitems;
public:
    bool Load(string dirName);
    bool find_list(string s);
    void new_list();
    void show_lists();
    void show_contacts();
    void find_contact();
    void delete_list();
    void rename_list();
    void make_list_copy();
    void edit_contact();
    void new_contact();
    void make_contact_copy();
    void delete_contact();
    void move_contact();
    void f();
private:
	ABTreeItem* m_rootItem;
};

const char *config_name = "config.txt";
const char *dir_name = "C:\\test";

bool comp(const string &a, const string &b)
{
    if (a.length() != b.length())
        return false;
    for (int i = 0; i < a.length(); i++)
    {
        if (toupper(a[i]) != toupper(b[i]))
            return false;
    }
    return true;
}

bool AddressBook::Load(string dirName)
{
    string cm = "dir /B " + dirName + " > content.txt";
    system(cm.c_str());
    fstream fin("content.txt", fstream::in);
    vector < string > files;
    while (!fin.eof())
    {
        string s;
        getline(fin, s);
        if (s.length() < 1)
            break;
        files.push_back(s);
    }
    fin.close();
    vector < string > :: iterator it;
    it = find(files.begin(), files.end(), string(config_name));

    if (it == files.end())
    {
        cout << "missing configfile" << endl;
        return false;
    }
    files.erase(it);
    string s = dirName + string("\\") + config_name;
    fstream fin2(s.c_str(), fstream::in);
    vector < string > group_names;
    vector < string > group_files;
    while (!fin2.eof())
    {
        string s;
        getline(fin2, s);
        if (s.length() < 1)
            break;
        group_names.push_back(s.substr(0, s.find('-')));
        group_files.push_back(s.substr(s.find('-') + 1));
        ABTreeItem  ab (s.substr(0, s.find('-')), dirName + string("\\") + s.substr(s.find('-') + 1));
        m_treeitems.push_back(ab);
    }
    fin2.close();
    for (int i = 0; i < group_names.size(); i++)
    {
        for (int j = i + 1; j < group_names.size(); j++)
        {
            if (comp(group_names[i], group_names[j]))
            {
                cout << "!!!" << endl;
                return false;
            }
        }
    }
    bool flag = false;
    for (int i = 0; i < group_files.size(); i++)
    {
        bool find = false;
        for (int j = 0; j < files.size(); j++)
        {
            if (comp(group_files[i], files[j]))
                find = true;
        }
        if (!find)
        {
            if(!flag){
                cout<<"files not found in directory:"<<endl;
                flag = true;
            }
            cerr << group_files[i] << endl;
            for (int t = 0; t < m_treeitems.size(); t++)
            {
                if (comp(m_treeitems[t].list_name, group_names[i]))
                {
                    m_treeitems.erase(m_treeitems.begin() + t);
                    break;
                }
            }
        }
    }
    flag = false;
    for (int i = 0; i < files.size(); i++)
    {
        bool find = false;
        for (int j = 0; j < group_files.size(); j++)
        {
            if (comp(group_files[j], files[i]))
                find = true;
        }
        if (!find)
        {
            if(!flag){
                cout<<"files not found in config file:"<<endl;
                flag = true;
            }
            cerr << files[i] << endl;
        }
    }
    for (int i = 0; i < m_treeitems.size(); i++)
    {
        string str;
        fstream fin3(m_treeitems[i].file_name.c_str(), fstream::in);
        while (!fin3.eof())
        {
            getline(fin3, str);
            ABRecord abr;
            abr.last_name = str.substr(0, str.find(','));
            str.erase(0, str.find(',') + 1);
            abr.first_name = str.substr(0, str.find(','));
            str.erase(0, str.find(',') + 1);
            abr.middle_name = str.substr(0, str.find(','));
            str.erase(0, str.find(',') + 1);
            abr.phone_number = str.substr(0, str.find(','));
            str.erase(0, str.find(',') + 1);
            abr.email = str;
            m_treeitems[i].m_records.push_back(abr);
        }
        fin3.close();
        cout << m_treeitems[i].list_name << endl;
    }
    return true;
}

bool AddressBook::find_list(string s)
{
    for (int i = 0; i < m_treeitems.size(); i++)
    {
        if(comp(m_treeitems[i].list_name, s))
        {
            return true;
        }
    }
    return false;
}

void AddressBook::show_lists()
{
    for (int i = 0; i < m_treeitems.size(); i++)
    {
        cout << m_treeitems[i].list_name << ' ' << m_treeitems[i].file_name << endl;
    }
}

void AddressBook::new_list()
{
    cout << "enter the list name" << endl;
    string s, tmp, tmp1;
    char file_name[7] = "XXXXXX";
    cin >> s;
    while(find_list(s))
    {
        cout << "enter another name" << endl;
        cin >> s;
    }
    mktemp(file_name);
    tmp1.assign(file_name);
    tmp = dir_name;
    tmp += "\\" + tmp1 + ".txt";
    ofstream ofs(tmp.c_str());
    ofs.close();
    ABTreeItem ab(s, tmp);
    m_treeitems.push_back(ab);
}

void AddressBook::show_contacts()
{
    cout << "enter the list name" << endl;
    string s;
    cin >> s;
    while(!find_list(s))
    {
        cout << "enter another name" << endl;
        cin >> s;
    }
    for (int i = 0; i < m_treeitems.size(); i++)
    {
        if(comp(m_treeitems[i].list_name, s))
        {
            for (int j = 0; j < m_treeitems[i].m_records.size(); j++)
            {
                cout << m_treeitems[i].m_records[j].last_name << ' ' << m_treeitems[i].m_records[j].first_name
                    << ' ' << m_treeitems[i].m_records[j].middle_name << ' ' << m_treeitems[i].m_records[j].phone_number
                    << ' ' << m_treeitems[i].m_records[j].email << endl;
            }
        }
    }
}

void AddressBook::find_contact()
{
    cout << "enter the list name" << endl;
    string s, fam, name, otch;
    int f1 = 0, f2 = 0, f3 = 0;
    cin >> s;
    while(!find_list(s))
    {
        cout << "enter another name" << endl;
        cin >> s;
    }
    for (int i = 0; i < m_treeitems.size(); i++)
    {
        if(comp(m_treeitems[i].list_name, s))
        {
            cout << "enter the fam or NULL" << endl;
            cin >> fam;
            if (comp(fam, "NULL"))
            {
                f1 = 1;
            } else
            {
                for (int j = 0; j < m_treeitems[i].m_records.size(); j++)
                {
                    if(comp(m_treeitems[i].m_records[j].last_name, fam))
                    {
                        cout << m_treeitems[i].m_records[j].last_name << ' ' << m_treeitems[i].m_records[j].first_name
                            << ' ' << m_treeitems[i].m_records[j].middle_name << ' ' << m_treeitems[i].m_records[j].phone_number
                            << ' ' << m_treeitems[i].m_records[j].email << endl;
                    }
                }
            }
            cout << "enter the name or NULL" << endl;
            cin >> name;
            if (comp(name, "NULL"))
            {
                f2 = 1;
            } else
            {
                for (int j = 0; j < m_treeitems[i].m_records.size(); j++)
                {
                    if((comp(m_treeitems[i].m_records[j].last_name, fam) || f1) && comp(m_treeitems[i].m_records[j].first_name, name))
                    {
                        cout << m_treeitems[i].m_records[j].last_name << ' ' << m_treeitems[i].m_records[j].first_name
                            << ' ' << m_treeitems[i].m_records[j].middle_name << ' ' << m_treeitems[i].m_records[j].phone_number
                            << ' ' << m_treeitems[i].m_records[j].email << endl;
                    }
                }
            }
            cout << "enter the otch or NULL" << endl;
            cin >> otch;
            if (comp(otch, "NULL"))
            {
                f3 = 1;
            }
            for (int j = 0; j < m_treeitems[i].m_records.size(); j++)
            {
                if((comp(m_treeitems[i].m_records[j].last_name, fam) || f1) && (comp(m_treeitems[i].m_records[j].first_name, name) || f2)
                   && (comp(m_treeitems[i].m_records[j].middle_name, otch) || f3))
                {
                    cout << m_treeitems[i].m_records[j].last_name << ' ' << m_treeitems[i].m_records[j].first_name
                        << ' ' << m_treeitems[i].m_records[j].middle_name << ' ' << m_treeitems[i].m_records[j].phone_number
                        << ' ' << m_treeitems[i].m_records[j].email << endl;
                }
            }
        }
    }
}

void AddressBook::delete_list()
{
    cout << "enter the list name" << endl;
    string s;
    cin >> s;
    while(!find_list(s))
    {
        cout << "enter another name" << endl;
        cin >> s;
    }
    for (int i = 0; i < m_treeitems.size(); i++)
    {
        if(comp(m_treeitems[i].list_name, s))
        {
            remove(m_treeitems[i].file_name.c_str());
            m_treeitems[i].m_records.clear();
            m_treeitems.erase(m_treeitems.begin() + i);
        }
    }
}

void AddressBook::rename_list()
{
    cout << "enter the list name" << endl;
    string s;
    cin >> s;
    while(!find_list(s))
    {
        cout << "enter another name" << endl;
        cin >> s;
    }
    for (int i = 0; i < m_treeitems.size(); i++)
    {
        if(comp(m_treeitems[i].list_name, s))
        {
            cout << "enter the new list name" << endl;
            string s1;
            cin >> s1;
            while(find_list(s1))
            {
                cout << "enter another name" << endl;
                cin >> s1;
            }
            m_treeitems[i].list_name = s1;
        }
    }
}

void AddressBook::make_list_copy()
{
    cout << "enter the list name" << endl;
    string s;
    cin >> s;
    while(!find_list(s))
    {
        cout << "enter another name" << endl;
        cin >> s;
    }
    for (int i = 0; i < m_treeitems.size(); i++)
    {
        if(comp(m_treeitems[i].list_name, s))
        {
            cout << "enter the new list name" << endl;
            string s1;
            cin >> s1;
            while(find_list(s1))
            {
                cout << "enter another name" << endl;
                cin >> s1;
            }
            string tmp, tmp1;
            char file_name[7] = "XXXXXX";
            mktemp(file_name);
            tmp1.assign(file_name);
            tmp = dir_name;
            tmp += "\\" + tmp1 + ".txt";
            //cout << tmp << endl;
            ofstream ofs(tmp.c_str());
            ofs.close();
            ABTreeItem ab(s1, tmp);
            ab = m_treeitems[i];
            ab.file_name = tmp;
            ab.list_name = s1;
            m_treeitems.push_back(ab);
            char *buffer;
            int len = 4096;
            ifstream infile(m_treeitems[i].file_name.c_str());
            ofstream outfile(tmp.c_str());
            buffer = new char[len];
            while (!infile.eof()){
                infile.read(buffer, len);
                if(infile.gcount()) outfile.write(buffer, infile.gcount());
            }
            infile.close();
            outfile.close();
            delete[] buffer;
            break;
        }
    }
}

void AddressBook::edit_contact()
{
    cout << "enter the list name" << endl;
    string s;
    cin >> s;
    while(!find_list(s))
    {
        cout << "enter another name" << endl;
        cin >> s;
    }
    for (int i = 0; i < m_treeitems.size(); i++)
    {
        if(comp(m_treeitems[i].list_name, s))
        {
            for (int j = 0; j < m_treeitems[i].m_records.size(); j++)
            {
                cout << j + 1 << ' ' << m_treeitems[i].m_records[j].last_name << ' ' << m_treeitems[i].m_records[j].first_name
                    << ' ' << m_treeitems[i].m_records[j].middle_name << ' ' << m_treeitems[i].m_records[j].phone_number
                    << ' ' << m_treeitems[i].m_records[j].email << endl;
            }
            cout << "enter the number" << endl;
            int n;
            cin >> n;
            while(n < 1 || n > m_treeitems[i].m_records.size())
            {
                cout << "enter another number" << endl;
                cin >> n;
            }
            n--;
            ABRecord abr;
            cout << "enter the fam" << endl;
            string fam;
            cin >> fam;
            abr.last_name = fam;
            cout << "enter the name" << endl;
            string name;
            cin >> name;
            abr.first_name = name;
            cout << "enter the otch" << endl;
            string otch;
            cin >> otch;
            abr.middle_name = otch;
            cout << "enter the phone number" << endl;
            string phone;
            cin >> phone;
            abr.phone_number = phone;
            cout << "enter the email" << endl;
            string email;
            cin >> email;
            abr.email = email;
            m_treeitems[i].m_records[n] = abr;
            char file_name[7] = "XXXXXX";
            mktemp(file_name);
            string tmp = dir_name;
            tmp += "\\" + string(file_name) + ".txt";
            ofstream ofs(tmp.c_str());
            for(int j = 0; j < m_treeitems[i].m_records.size() - 1; j++)
            {
                ofs << m_treeitems[i].m_records[j].last_name + ',' + m_treeitems[i].m_records[j].first_name
                    + ',' + m_treeitems[i].m_records[j].middle_name + ',' + m_treeitems[i].m_records[j].phone_number
                    + ',' + m_treeitems[i].m_records[j].email << endl;
            }
            ofs.close();
            remove(m_treeitems[i].file_name.c_str());
            m_treeitems[i].file_name = tmp;
            m_treeitems[i].m_records.erase(m_treeitems[i].m_records.begin() + n);

            break;
        }
    }
}

void AddressBook::new_contact()
{
    cout << "enter the list name" << endl;
    string s;
    cin >> s;
    while(!find_list(s))
    {
        cout << "enter another name" << endl;
        cin >> s;
    }
    for (int i = 0; i < m_treeitems.size(); i++)
    {
        if(comp(m_treeitems[i].list_name, s))
        {
            ABRecord abr;
            cout << "enter the fam" << endl;
            string fam;
            cin >> fam;
            abr.last_name = fam;
            cout << "enter the name" << endl;
            string name;
            cin >> name;
            abr.first_name = name;
            cout << "enter the otch" << endl;
            string otch;
            cin >> otch;
            abr.middle_name = otch;
            cout << "enter the phone number" << endl;
            string phone;
            cin >> phone;
            abr.phone_number = phone;
            cout << "enter the email" << endl;
            string email;
            cin >> email;
            abr.email = email;
            m_treeitems[i].m_records.push_back(abr);
            ofstream of;
            of.open(m_treeitems[i].file_name.c_str(), std::ios_base::app);
            of << fam << ',' << name << ',' << otch << ',' << phone << ',' << email << endl;
            of.close();
        }
    }
}

void AddressBook::make_contact_copy()
{
    cout << "enter the list name" << endl;
    string s;
    cin >> s;
    while(!find_list(s))
    {
        cout << "enter another name" << endl;
        cin >> s;
    }
    for (int i = 0; i < m_treeitems.size(); i++)
    {
        if(comp(m_treeitems[i].list_name, s))
        {
            for (int j = 0; j < m_treeitems[i].m_records.size(); j++)
            {
                cout << j + 1 << ' ' << m_treeitems[i].m_records[j].last_name << ' ' << m_treeitems[i].m_records[j].first_name
                    << ' ' << m_treeitems[i].m_records[j].middle_name << ' ' << m_treeitems[i].m_records[j].phone_number
                    << ' ' << m_treeitems[i].m_records[j].email << endl;
            }
            cout << "enter the number" << endl;
            int n;
            cin >> n;
            while(n < 1 || n > m_treeitems[i].m_records.size())
            {
                cout << "enter another number" << endl;
                cin >> n;
            }
            cout << "enter the list name" << endl;
            string s1;
            cin >> s1;
            while(!find_list(s1))
            {
                cout << "enter another name" << endl;
                cin >> s;
            }
            for (int j = 0; j < m_treeitems[i].m_records.size(); j++)
            {
                if(comp(m_treeitems[j].list_name, s1))
                {
                    ABRecord abr;
                    abr = m_treeitems[i].m_records[n - 1];
                    m_treeitems[j].m_records.push_back(abr);
                    ofstream of;
                    of.open(m_treeitems[j].file_name.c_str(), std::ios_base::app);
                    of << abr.last_name << ',' << abr.first_name
                        << ',' << abr.middle_name << ',' << abr.phone_number
                        << ',' << abr.email << endl;
                    of.close();
                    break;
                }
            }
        break;
        }
    }
}

void AddressBook::delete_contact()
{
    cout << "enter the list name" << endl;
    string s;
    cin >> s;
    while(!find_list(s))
    {
        cout << "enter another name" << endl;
        cin >> s;
    }
    for (int i = 0; i < m_treeitems.size(); i++)
    {
        if(comp(m_treeitems[i].list_name, s))
        {
            for (int j = 0; j < m_treeitems[i].m_records.size(); j++)
            {
                cout << j + 1 << ' ' << m_treeitems[i].m_records[j].last_name << ' ' << m_treeitems[i].m_records[j].first_name
                    << ' ' << m_treeitems[i].m_records[j].middle_name << ' ' << m_treeitems[i].m_records[j].phone_number
                    << ' ' << m_treeitems[i].m_records[j].email << endl;
            }
            cout << "enter the number" << endl;
            int n;
            cin >> n;
            while(n < 1 || n > m_treeitems[i].m_records.size())
            {
                cout << "enter another number" << endl;
                cin >> n;
            }
            n--;
            char file_name[7] = "XXXXXX";
            mktemp(file_name);
            string tmp = dir_name;
            tmp += "\\" + string(file_name) + ".txt";
            ofstream ofs(tmp.c_str());
            fstream fin(m_treeitems[i].file_name.c_str(), fstream::in);
            string temp = m_treeitems[i].m_records[n].last_name + ',' + m_treeitems[i].m_records[n].first_name
                    + ',' + m_treeitems[i].m_records[n].middle_name + ',' + m_treeitems[i].m_records[n].phone_number
                    + ',' + m_treeitems[i].m_records[n].email;
            while (!fin.eof())
            {
                string s1;
                getline(fin, s1);
                if(!comp(s1, temp))
                {
                    ofs << s1 << endl;
                }
            }
            ofs.close();
            fin.close();
            remove(m_treeitems[i].file_name.c_str());
            m_treeitems[i].file_name = tmp;
            m_treeitems[i].m_records.erase(m_treeitems[i].m_records.begin() + n);
            break;
        }
    }
}

void AddressBook::move_contact()
{
    cout << "enter the list name" << endl;
    string s;
    cin >> s;
    while(!find_list(s))
    {
        cout << "enter another name" << endl;
        cin >> s;
    }
    for (int i = 0; i < m_treeitems.size(); i++)
    {
        if(comp(m_treeitems[i].list_name, s))
        {
            for (int j = 0; j < m_treeitems[i].m_records.size(); j++)
            {
                cout << j + 1 << ' ' << m_treeitems[i].m_records[j].last_name << ' ' << m_treeitems[i].m_records[j].first_name
                    << ' ' << m_treeitems[i].m_records[j].middle_name << ' ' << m_treeitems[i].m_records[j].phone_number
                    << ' ' << m_treeitems[i].m_records[j].email << endl;
            }
            cout << "enter the number" << endl;
            int n;
            cin >> n;
            while(n < 1 || n > m_treeitems[i].m_records.size())
            {
                cout << "enter another number" << endl;
                cin >> n;
            }
            cout << "enter the list name" << endl;
            string s1;
            cin >> s1;
            while(!find_list(s1))
            {
                cout << "enter another name" << endl;
                cin >> s;
            }
            for (int j = 0; j < m_treeitems[i].m_records.size(); j++)
            {
                if(comp(m_treeitems[j].list_name, s1))
                {
                    ABRecord abr;
                    abr = m_treeitems[i].m_records[n - 1];
                    m_treeitems[j].m_records.push_back(abr);
                    ofstream of;
                    of.open(m_treeitems[j].file_name.c_str(), std::ios_base::app);
                    of << abr.last_name << ',' << abr.first_name
                        << ',' << abr.middle_name << ',' << abr.phone_number
                        << ',' << abr.email << endl;
                    of.close();
                    break;
                }
            }
            n--;
            char file_name[7] = "XXXXXX";
            mktemp(file_name);
            string tmp = dir_name;
            tmp += "\\" + string(file_name) + ".txt";
            ofstream ofs(tmp.c_str());
            fstream fin(m_treeitems[i].file_name.c_str(), fstream::in);
            string temp = m_treeitems[i].m_records[n].last_name + ',' + m_treeitems[i].m_records[n].first_name
                    + ',' + m_treeitems[i].m_records[n].middle_name + ',' + m_treeitems[i].m_records[n].phone_number
                    + ',' + m_treeitems[i].m_records[n].email;
            while (!fin.eof())
            {
                string s1;
                getline(fin, s1);
                if(!comp(s1, temp))
                {
                    ofs << s1 << endl;
                }
            }
            ofs.close();
            fin.close();
            remove(m_treeitems[i].file_name.c_str());
            m_treeitems[i].file_name = tmp;
            m_treeitems[i].m_records.erase(m_treeitems[i].m_records.begin() + n);
            break;
        }
    }
}

void AddressBook::f()
{
    string tmp = dir_name;
    tmp += "\\";
    tmp += config_name;
    ofstream ofs(tmp.c_str());
    for(int i = 0; i < m_treeitems.size(); i++)
    {
        ofs << m_treeitems[i].list_name << '-' << m_treeitems[i].file_name.substr(m_treeitems[i].file_name.find_last_of('\\') + 1) << endl;
    }
}

int main()
{
    AddressBook AB;
    string s;
    if(!AB.Load(dir_name)) return 0;
    while (1)
    {
        cout << endl;
        cout << "enter the command" << endl;
        cin >> s;
        if(comp(s, "exit"))
        {
            AB.f();
            break;
        }
        if(comp(s, "new_list"))
        {
            AB.new_list();
            continue;
        }
        if(comp(s, "show_lists"))
        {
            AB.show_lists();
            continue;
        }
        if(comp(s, "show_contacts"))
        {
            AB.show_contacts();
            continue;
        }
        if(comp(s, "find_contact"))
        {
            AB.find_contact();
            continue;
        }
        if(comp(s, "delete_list"))
        {
            AB.delete_list();
            continue;
        }
        if(comp(s, "rename_list"))
        {
            AB.rename_list();
            continue;
        }
        if(comp(s, "make_list_copy"))
        {
            AB.make_list_copy();
            continue;
        }
        if(comp(s, "edit_contact"))
        {
            AB.edit_contact();
            continue;
        }
        if(comp(s, "new_contact"))
        {
            AB.new_contact();
            continue;
        }
        if(comp(s, "make_contact_copy"))
        {
            AB.make_contact_copy();
            continue;
        }
        if(comp(s, "delete_contact"))
        {
            AB.delete_contact();
            continue;
        }
        if(comp(s, "move_contact"))
        {
            AB.move_contact();
            continue;
        }
        if(comp(s, "help"))
        {
            cout << "exit" << endl <<
                "new_list" << endl <<
                "show_lists" << endl <<
                "find_contact" << endl <<
                "delete_list" << endl <<
                "rename_list" << endl <<
                "make_list_copy" << endl <<
                "edit_contact" << endl <<
                "new_contact" << endl <<
                "make_contact_copy" << endl <<
                "delete_contact" << endl <<
                "move_contact" << endl;
            continue;
        }
        cout << "unknown command" << endl;
    }
    return 0;
}
