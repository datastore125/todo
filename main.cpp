#include <iostream>
#include <fstream>
#define _WIN32_WINNT 0x0500
#include <windows.h>
#include <ctime>
#include <sstream>


using namespace std;

// pointer la structura tm care retine data actuala (<ctime>)
tm *ltm ;


// clasa care retine data
class data{
    int zi,luna,an;
public:
    data(int z, int l,int a): zi(z),luna(l),an(a){}
    int getzi(){return zi;}
    int getluna(){return luna;}
    int getan(){return an;}
};


// clasa din care va fi alcatuit fiecare element al listei
class toDo{
    static int total;   // static int pentru a retine numarul de "toDo"-uri
    int id;     //id-ul fiecarui toDo
    string mesaj;
public:
    static int getTotal(){return total;}
    static void tZ(){total=0;}
    int getId(){return id;}
    string getMesaj(){return mesaj;}
    void bifareMesaj(){char c=219;mesaj=mesaj + "-" + c;}

    // constructori care initializeaza data
    toDo(string s){mesaj=s;id=++total;}
    toDo(){mesaj="";id=++total;};

};

int toDo::total=0;


//  lista care retine elementele toDo
class lista : public toDo{
public:
    lista* next;
    lista* prev;
    lista(string s): toDo(s){}
    friend void adaugare(string s);
}*prim,*ultim;


// funtie care adauga element nou in linsta
void adaugare(string s){
    lista *n=new lista(s);
    if(!prim){
        prim=ultim=n;
        prim->prev=ultim->prev=prim->next=ultim->next=NULL;
    }else{
        lista *p=prim;
        while(p->next){
            p=p->next;
        }
        p->next=n;
        n->prev=ultim;
        n->next=NULL;
        ultim=n;
    }
}


// functie care afiseaza toate elementele listei actuale
void afisare(){
    lista* p=prim;
    while(p){
        cout<<p->getId()<<": "<<p->getMesaj()<<"\n";
        p=p->next;
    }
}

// functie care rescrie in fisier elementele listei actuale
void scriere(string s){
    ofstream f(s.c_str());
    int a=toDo::getTotal();
    f<<a<<"\n";
    lista *p=prim;
    while(p){
        f<<p->getId()<<"\n"<<p->getMesaj()<<"\n";
        p=p->next;
    }
    f.close();
}

// functie de citire din fisier ... citeste si adauga fiecare mesaj intr-o lista ... are
void citire(string s){
    ifstream f(s.c_str());
    string d;
    getline(f,d);

    while(getline(f,d)){
        getline(f,d);
        adaugare(d);
    }
    f.close();
}


//functie de "bifare"... apeleaza bifareMesaj din clasa toDo
int bifare(int n){
    int ok=0;
    lista *p=prim;
    while(p){
        if(p->getId()==n){
            p->bifareMesaj();
            ok=1;
            break;
        }
        p=p->next;
    }
    return ok;
}

//stergerea listei
void stergerel(){
    while(prim!=ultim){
        lista *p=prim;
        prim=prim->next;
        delete p;
    }
    delete prim;
    prim=ultim=NULL;
    toDo::tZ();
}

void SetWindow(int Width, int Height)
{
    _COORD coord;
    coord.X = Width;
    coord.Y = Height;

    _SMALL_RECT Rect;
    Rect.Top = 0;
    Rect.Left = 0;
    Rect.Bottom = Height - 1;
    Rect.Right = Width - 1;

    HANDLE Handle = GetStdHandle(STD_OUTPUT_HANDLE);      // Get Handle
    SetConsoleScreenBufferSize(Handle, coord);            // Set Buffer Size
    SetConsoleWindowInfo(Handle, TRUE, &Rect);            // Set Window Size
}



int main()
{
    SetWindow(53,30);

    HWND consoleWindow = GetConsoleWindow();

	SetWindowPos( consoleWindow, 0, 870, 0, 0, 0, SWP_NOSIZE | SWP_NOZORDER );

    time_t now = time(0);
    ltm = localtime(&now);


    stringstream ss;
    int op=1;
    int n,c=0;
    string t;
    citire("toDo.td");
    afisare();
    data d(ltm->tm_mday,1+ltm->tm_mon,ltm->tm_year+1900);
    ss<<"("<<d.getzi()<<"/"<<d.getluna()<<"/"<<d.getan()<<")";
    string dat,da;
    ss>>dat;
    da=dat;
    while(op){
        cout<<"\n\n0.iesire\n1.adaugare\n2.bifare\n3.afisare\n4.golire lista\n\n";
        cin>>op;
        switch(op){
            case 0:break;
            case 1: cout<<"\n MESAJ:\n"; getline(cin,t); getline(cin,t);dat+=" "+t ;adaugare(dat);dat=da;    scriere("toDo.td"); break;
            case 2:  while(!c){ cout<<"\n NR. MESAJ:\n"; cin>>n; c=bifare(n);} break;
            case 3: afisare(); break;
            case 4: cout<<"Sunteti sigur?\n1.DA   2.NU\n"; cin>>n; if(n==1){stergerel();} break;
            default: break;
        }
    }

    return 0;
}
