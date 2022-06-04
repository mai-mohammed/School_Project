// School_project.cpp : This file contains the 'main' function. Program execution begins and ends there.
#include <iostream>
#include <fstream>
#include <string.h>
#include <climits>
#include <sstream>
#include <exception>
using namespace std;

class student
{

    const static string student_intro;

protected:
    long StNo;
    int CsNo;
    char StName[30];
    double grades[100];
    

public:
    const static char FILE_NAME[];
    student() {}
    ~student()
    {
    }
    void initialize();
    long get_stno() { return StNo; }
    void set_stno(int s) { StNo = s; }
    char *get_stname() { return StName; }
    double average();
    double get_grade(int i);
    void Get_data();
    friend bool operator<(student s1, student s2);
    string get_student_intro() { return student_intro; }
};

const char student::FILE_NAME[] = "StudentFile.dat\0";
const string student::student_intro = "12010";

double SectionAv(student *ar, int n);

class PostGrad : public student
{
    const static string student_intro;
    char supervisor[30];

public:
    const static char FILE_NAME[];
    PostGrad() {}
    ~PostGrad() {}
    char *get_super() { return supervisor; }
    void initialize();
    void Get_data();
    friend bool operator<(PostGrad s1, PostGrad s2);
    string get_student_intro() { return student_intro; }
};

const char PostGrad::FILE_NAME[] = "PostGrad.dat\0";
const string PostGrad::student_intro = "1304";

template <class T>
class section
{
    T **sec;
    int MaxSize;
    int size;
    int binary_search(long stno);
    void expand();
    void shrink();

public:
    section()
    {
        sec = new T *[20];
        MaxSize = 20;
        size = 0;
    }
    void StAdd(T *s);
    void StDelete(long stno);
    void StList();
    void StRetrieve(long stno);
    void Update(int stno);
    void operator+(T *s);
    void operator-(long stno);
    template <class U>
    friend int transfer(long stno, section<U> &c1, section<U> &c2);
    void sort();
};

int main()
{

    section<PostGrad> cs100;
    int choice;
    do
    {
        cout << "\n\n\tEnter 1 to add a new student \n\n";
        cout << "\tEnter 2 to delete a student \n\n";
        cout << "\tEnter 3 to some information about a student \n\n";
        cout << "\tEnter 4 to get a list of all students \n\n";
        cout << "\tEnter 5 to update student information\n\n";
        cout << "\tEnter 6 to Exit\n";
        cin >> choice;
        while (cin.fail())
        {
            cin.clear();
            cin.ignore(INT_MAX, '\n');
            cout << "\t\n*** Enter a numerical values *** \n";
            cin >> choice;
        }
        system("cls");
        switch (choice)
        {
        case 1:
        {
            PostGrad *s = new PostGrad();
            s->initialize();
            cs100 + s;
            system("cls");
            break;
        }
        case 2:
        {
            long sn;
            cout << "\nEnter student's number : \n";
            cin >> sn;
            cs100 - sn;
            system("cls");
            break;
        }
        case 3:
        {
            long sn;
            cout << "\nEnter student number : \n";
            cin >> sn;
            cs100.StRetrieve(sn);
            system("cls");
            break;
        }
        case 4:
        {
            cs100.sort();
            cs100.StList();
            system("cls");
            break;
        }
        case 5:
        {
            long sn;
            cout << "\nEnter student number : \n";
            cin >> sn;
            cs100.Update(sn);
            system("cls");
            break;
        }
        default:
        {
            cout << "\nInvalid choice ... Try again \n";
            system("cls");
            break;
        }
        }
    } while (choice != 6);
    return 0;
}

template <class T>
int section<T>::binary_search(long stno)
{
    int lo = 0;
    int hi = size - 1;
    while (lo <= hi)
    {
        int mid = (lo + hi) / 2;
        long r = sec[mid]->get_stno();
        if (r == stno)
        {
            return mid;
        }
        else if (stno > r)
        {
            lo = mid + 1;
        }
        else
        {
            hi = mid - 1;
        }
    }
    return -1;
}

template <class T>
void section<T>::StAdd(T *s)
{
    long stno;
    string intro = s->get_student_intro();
    string add = to_string((s->get_stno()));
    intro += add;
    stringstream ss;
    ss << intro;
    ss >> stno;
    s->set_stno(stno);

    int pos = 0;
    if (size + 1 > MaxSize)
    {
        expand();
    }
    if (s->get_stno() > sec[pos]->get_stno() && pos < size)
    {
        pos++;
    }
    for (int i = size - 1; i >= pos; i--)
    {
        sec[i + 1] = sec[i];
    }
    sec[pos] = s;
    size++;
    ofstream oFile;
    oFile.open(T::FILE_NAME, ios::binary | ios::app);
    oFile.write(reinterpret_cast<char *>(s), sizeof(T));
    oFile.close();
    cout << "\n\nStudent record Has Been Created ";

    cin.ignore();
    cin.get();
}

template <class T>
void section<T>::StDelete(long stno)
{
    int i = binary_search(stno);
    bool flag = false;
    T *s = new T();
    ifstream iFile;
    iFile.open(T::FILE_NAME, ios::binary);
    if (!iFile)
    {
        cout << "File could not be opened... Press any Key to exit...";
        cin.ignore();
        cin.get();
        return;
    }

    ofstream oFile;
    oFile.open("Temp.dat", ios::out);
    iFile.seekg(0, ios::beg);
    while (iFile.read(reinterpret_cast<char *>(s), sizeof(T)))
    {
        if (s->get_stno() != stno)
        {
            oFile.write(reinterpret_cast<char *>(s), sizeof(T));
        }
        else
        {
            flag = true;
        }
    }
    oFile.close();
    iFile.close();
    remove(T::FILE_NAME);
    rename("Temp.dat", T::FILE_NAME);

    if (flag == true)
    {
        cout << "\n\n\tRecord Deleted ..";
    }
    else
    {
        cout << "Student Number : " << stno << " Was Not Found .\n";
        cin.ignore();
        cin.get();
        return;
    }

    if (i > -1)
    {
        delete sec[i];
        for (int j = i; j < size - 1; j++)
        {
            sec[j] = sec[j + 1];
        }
        size--;
        if (size < (MaxSize / 2))
        {
            shrink();
        }
    }
    cin.ignore();
    cin.get();
}

template <class T>
void section<T>::StList()
{

    T *s = new T();
    ifstream iFile;
    iFile.open(T::FILE_NAME, ios::binary);
    if (!iFile)
    {
        cout << "File could not be opened !! Press any Key to exit";
        cin.ignore();
        cin.get();
        return;
    }
    cout << "\n\n\n\t\tDISPLAYING ALL RECORDS\n\n";
    while (iFile.read(reinterpret_cast<char *>(s), sizeof(T)))
    {
        s->Get_data();
        cout << "\n\n====================================\n\n";
    }
    iFile.close();
    cin.ignore();
    cin.get();
}

template <class T>
void section<T>::StRetrieve(long stno)
{
    bool flag = false;
    T *s = new T();
    ifstream iFile;
    iFile.open(T::FILE_NAME, ios::binary);
    if (!iFile)
    {
        cout << "File could not be opened... Press any Key to exit";
        cin.ignore();
        cin.get();
        return;
    }

    while (iFile.read(reinterpret_cast<char *>(s), sizeof(T)))
    {
        if (s->get_stno() == stno)
        {
            s->Get_data();
            flag = true;
        }
    }

    if (flag == false)
    {
        cout << "Student Number :" << stno << " Was Not Found .\n";
    }
    cin.ignore();
    cin.get();
}

template <class T>
void section<T>::Update(int stno)
{
    bool flag = false;
    T *s = new T();
    fstream File;
    File.open(T::FILE_NAME, ios::binary | ios::in | ios::out);
    if (!File)
    {
        cout << "File could not be opened. Press any Key to exit...";
        cin.ignore();
        cin.get();
        return;
    }

    while (!File.eof())
    {
        File.read(reinterpret_cast<char *>(s), sizeof(T));
        if (s->get_stno() == stno)
        {
            flag = true;
            s->Get_data();
            cout << "\nEnter new student details:" << endl;
            s->initialize();
            int pos = (-1) * static_cast<int>(sizeof(*s));
            File.seekp(pos, ios::cur);
            File.write(reinterpret_cast<char *>(s), sizeof(T));
            cout << "\n\n\t Record Updated";
        }
    }
    File.close();
    if (flag == false)
    {
        cout << "\n\n Record Not Found ";
    }

    int i = binary_search(stno);
    if (i > -1)
    {
        sec[i] = s;
    }

    cin.ignore();
    cin.get();
}

template <class T>
void section<T>::sort()
{
    int minInx;
    for (int i = 0; i < size; i++)
    {
        minInx = i;
        for (int j = i + 1; j < size; j++)
        {
            if (sec[j] < sec[minInx])-
            {
                minInx = j;
            }
        }
        T *temp = sec[i];
        sec[i] = sec[minInx];
        sec[minInx] = temp;
    }
}

template <class T>
void section<T>::operator+(T *s)
{
    StAdd(s);
}

template <class T>
void section<T>::operator-(long stno)
{
    StDelete(stno);
}

template <class T>
void section<T>::expand()
{
    T **p = new T *[2 * MaxSize];
    for (int i = 0; i < size; i++)
    {
        p[i] = sec[i];
    }
    delete[] sec;
    sec = p;
    MaxSize *= 2;
}

template <class T>
void section<T>::shrink()
{
    T **p = new T *[(MaxSize / 2)];
    for (int i = 0; i < size; i++)
    {
        p[i] = sec[i];
    }
    delete[] sec;
    sec = p;
    MaxSize /= 2;
}

template <class U>
int transfer(long stno, section<U> &c1, section<U> &c2)
{
    int i = c1.binary_search(stno);
    U *s = new U();
    if (i > -1)
    {
        s = c1.sec[i];
        c1.StDelete(stno);
        c2.StAdd(s);
    }
    return i;
}

void student::initialize()
{
    cout << "\n\nEnter information of student " << endl;
    cout << "\nStudent's Number : ";
    cin >> StNo;
    while (cin.fail())
    {
        cin.clear();
        cin.ignore(INT_MAX, '\n');
        cout << "\t\n*** Enter a numerical values *** \n";
        cout << "\nStudent's Number : ";
        cin >> StNo;
    }
    cout << "\nStudent's Name : ";
    cin.ignore();
    cin.getline(StName, 50);
    cout << "\nNumber of courses taken by the student : ";
    cin >> CsNo;
    while (cin.fail())
    {
        cin.clear();
        cin.ignore(INT_MAX, '\n');
        cout << "\t\n*** Enter a numerical values *** \n";
        cout << "\nNumber of courses taken by the student : ";
        cin >> CsNo;
    }
    for (int i = 0; i < CsNo; i++)
    {
        cout << "\n\tEnter grade No." << i + 1 << ":";
        cin >> grades[i];
        while (cin.fail())
        {
            cin.clear();
            cin.ignore(INT_MAX, '\n');
            cout << "\t\n*** Enter a numerical values *** \n";
            cout << "\n\tEnter grade No." << i + 1 << ":";
            cin >> grades[i];
        }
    }
}

double student::average()
{
    if (CsNo == 0)
    {
        return 0;
    }
    double sum = 0;
    for (int i = 0; i < CsNo; i++)
    {
        sum += grades[i];
    }
    return sum / CsNo;
}

double student::get_grade(int i)
{
    if (i < CsNo && i > 0)
        return grades[i];
    else
        return -1;
}

void student::Get_data()
{
    cout << "\n------------- Student Information -------------\n";
    cout << "\n\tName : " << get_stname() << endl;
    cout << "\n\tStudent's Number : " << get_stno() << endl;
    cout << "\n\tAverage : " << average() << endl;
}

double SectionAv(student *ar, int n)
{
    double sum = 0;
    for (int i = 0; i < n; i++)
    {
        sum += ar[i].average();
    }
    return sum / n;
}

bool operator<(student s1, student s2)
{
    if (s1.get_stno() < s2.get_stno())
    {
        return true;
    }
    return false;
}

bool operator<(PostGrad s1, PostGrad s2)
{
    if (s1.get_stno() < s2.get_stno())
    {
        return true;
    }
    return false;
}

void PostGrad::initialize()
{
    student::initialize();
    cout << "\nName of supervisor : ";
    cin >> supervisor;
}

void PostGrad::Get_data()
{
    student::Get_data();
    cout << "\n\tSupervisor's name : " << supervisor << endl;
}
