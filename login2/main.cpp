#include <iostream>
#include <fstream>
#include <windows.h>
#include <math.h>
#include <cstdlib>
#include <conio.h>
#include <sstream>
using namespace std;

class bank
{
public:
    systemLogowania()
    {
        dostep = 0;
    }

    void adminLogin()
    {
        cout << "Podaj PIN: " << endl;
        cin >> PIN;
        if(PIN == 0000)
        {
            cout << "PIN poprawny!";
            system("cls");
            panelAdmina();
        }
        else
        {
            cout << "PIN niepoprawny! Sprobuj jeszcze raz.";
            Sleep(1000);
            system("cls");
            adminLogin();
        }
    }

    void panelAdmina()
    {
        for(;;)
        {
            cout << "------------------------------------------------------------------------" << endl;
            cout << "                               PANEL ADMINISTRATORA                        " << endl;
            cout << "------------------------------------------------------------------------" << endl;
            cout << "1.Usun konto." << endl;
            cout << "2.Dodaj konto."<< endl;
            cout << "3.Wroc do menu glownego."<< endl;
            cout << "------------------------------------------------------------------------" << endl;
            cout << endl;

            wybor = getch();
            switch(wybor)
            {
            case '1':
            {
                system("cls");
                Sleep(300);
                usunKonto();
            }
            break;

            case '2':
            {
                system("cls");
                Sleep(300);
                rejestracja();
            }
            break;

            case '3':
            {
                system("cls");
                Sleep(300);
                ekranStartowy();
            }
            break;
            default:
                cout << "Takiej opcji nie ma w menu!";
            }
            getchar();
            system("cls");
        }
    }

    void usunKonto()
    {
        int id;
        cout << "Podaj id konta do usuniecia: ";
        cin >> id;

        usunLinie("loginy.txt", id);
        usunLinie("hasla.txt", id);
        usunLinie("stany.txt", id);
        usunLinie("typy.txt", id);
    }
    void usunLinie(const char *nazwa, int n)
    {
        ifstream is(nazwa);
        ofstream temp;
        temp.open("temp.txt", ofstream::out);

        char c;
        int line_no = 1;
        while (is.get(c))
        {
            if (c == '\n')
                line_no++;

            if (line_no != n)
                ofs << c;
        }
        temp.close();
        is.close();

        remove(nazwa);
        rename("temp.txt", nazwa);
    }

    void login()
    {
        cout << "Witamy w naszym banku!" << endl;
        cout << "Podaj login: ";                                                    // logowanie
        cin >> log;

        int loginID = checkFile(log, "loginy.txt");
        if(loginID != 0)
        {
            cout << "Podaj haslo: ";
            cin >> haslo;

            int hasloID = checkFile(haslo, "hasla.txt");
            if(loginID == hasloID)
            {
                cout << "Logowanie przebieglo pomyslnie";
                Sleep(1500);
                system("cls");
            }
            else
            {
                cout << "Logowanie sie nie powiodlo! Sprobuj ponowanie!";
                Sleep(1500);
                system("cls");
                login();
            }
        }
        else
        {
            cout << "Logowanie sie nie powiodlo! Sprobuj ponowanie!";
            Sleep(1500);
            system("cls");
            login();
        }
    }

    void rejestracja()
    {
        string login, haslo;
        cout << "Prosze podac login: ";
        cin >> login;
        if(checkFile(login, "loginy.txt") != 0)
        {
            cout << "Ten login jest juz zajety!" << endl;
        }
        else
        {
            cout << "Prosze podac haslo: ";
            cin >> haslo;
        }
        int id = 1 + getLastID();
        saveFile(login, "loginy.txt", id);
        saveFile(haslo, "hasla.txt", id);

        cout << "Wybierz typ konta (O-Oszczednosciowe/S-standard): " << endl;
        cin >> typ;
        saveFile(typ, "typy.txt", id);

        cout << "Wprowadz kwote do wplaty(Dla O - min. 500zl, dla S - min.1000zl.\n";
        cin >> stanS;
        saveFileBezSz(stanS, "stany.txt", id);

    }

    int getLastID()
    {
        fstream file;
        file.open("loginy.txt", ios:: in);
        file.seekg(0, ios::end);

        if(file.tellg() == -1)
            return 0;

        string x;

        for(int i = -1; x.find("#") == string::npos; i--)
        {
            file.seekg(i, ios::end);
            file >> x;
        }

        file.close();
        x.erase(0, 4);

        int id;
        istringstream(x) >> id;

        return id;
    }

    int checkFile(string proba, const char* doc)
    {
        string linia;
        fstream file;

        string aktualnyZnak;
        long long szZnak;                                     // zaszyfrowany znak

        file.open(doc, ios:: in);

        while(1)
        {
            file >> aktualnyZnak;
            if(aktualnyZnak.find("#ID:") != string::npos)
            {
                if(proba == linia)
                {
                    file.close();
                    aktualnyZnak.erase(0, 4);
                    int id;
                    istringstream(aktualnyZnak) >> id;
                    return id;
                }
                else
                {
                    linia.erase(linia.begin(), linia.end());
                }
            }
            else
            {
                istringstream(aktualnyZnak) >> szZnak;
                linia += (char)odszyfrowanie(szZnak);
                aktualnyZnak = "";
            }

            if(file.peek() == EOF)
            {
                file.close();
                return 0;
            }
        }
    }

    void saveFile(string linia, const char* nazwa, const int& id)
    {
        fstream file;
        file.open(nazwa, ios::app);
        file.seekg(0, ios::end);

        if(file.tellg() != 0)
            file << "\n";

        for(int i=0; i<linia.length(); i++)
        {
            file << szyfrowanie(linia[i]);
            file << " ";
        }
        file << "#ID:" << id;
        file.seekg(0, ios:: beg);
        file.close();
    }

    void saveFileBezSz(string linia, const char* nazwa, const int& id)
    {
        fstream file;
        file.open(nazwa, ios::app);
        file.seekg(0, ios::end);

        if(file.tellg() != 0)
            file << "\n";

        for(int i=0; i<linia.length(); i++)
        {
            file << linia[i];
            file << " ";
        }

        file << "#ID:" << id;
        file.seekg(0, ios:: beg);

        file.close();
    }

    double zamiana(string str)
    {
        stringstream ss;
        ss << str;
        ss >> stan;
        return stan;
    }

    long long szyfrowanie(int litera)
    {
        return powf(litera, 5) * 3 - 7;
    }

    int odszyfrowanie(long long litera)
    {
        return powf((litera +7) / 3, 1/5.f);
    }

    void ekranStartowy()
    {
        for(;;)
        {
            cout << "------------------------------------------------------------------------" << endl;
            cout << "                           Witamy w banku Essa                              " << endl;
            cout << "------------------------------------------------------------------------" << endl;
            cout << "1.Zaloguj sie." << endl;
            cout << "2.Zarejestruj sie." << endl;
            cout << "3.Panel administratora."<< endl;
            cout << "------------------------------------------------------------------------" << endl;
            cout << endl;

            wybor = getch();
            switch(wybor)
            {
            case '1':
            {
                system("cls");
                login();
                Sleep(400);
                menuGlowne();
            }
            break;

            case '2':
            {
                system("cls");
                rejestracja();
            }
            break;

            case '3':
            {
                system("cls");
                adminLogin();
                Sleep(400);
            }

            default:
                cout << "Takiej opcji nie ma w menu!";
            }
            getchar();
            system("cls");
        }
    }
    void menuGlowne()
    {
        for(;;)
        {

            cout << "------------------------------------------------------------------------" << endl;
            cout << "Witamy w naszym banku. Logowanie przebieglo pomyslnie!" << endl;
            cout << "------------------------------------------------------------------------" << endl;
            cout << "1.Wplata." << endl;
            cout << "2.Wyplata." << endl;
            cout << "3.Szczegoly konta." << endl;
            cout << "4.Zakonczenie sesji." << endl;
            cout << "------------------------------------------------------------------------" << endl;
            cout << endl;

            wybor = getch();
            switch(wybor)
            {
            case '1':
            {
                wplaty();
            }
            break;
            case '2':
            {
                wyplaty();
            }
            break;

            case '3':
            {
                info();
            }
            break;

            case '4':
            {
                cout << "Milego dnia!";
                exit(0);
            }
            break;

            default:
                cout << "Takiej opcji nie ma w menu!";

            }
            getchar();
            system("cls");
        }
    }
    void wplaty()
    {
        cout << "Podaj kwote do wplaty: ";
        cin >> kwota;
        stan += kwota;
    }

    void wyplaty()
    {
        cout << "Podaj kwote do wyplaty: ";
        cin >> kwota;
        if(kwota>stan)
        {
            cout << "Nie posiadasz tylu srodkow na koncie" << endl;
        }
        stan-= kwota;
    }

    void info()
    {
        system("cls");
        cout<<"\nNazwa wlasciciela: " <<log;
        cout<<"\nTyp konta: "<<typ;
        cout<<"\nStan konta: "<<stan;
    }



private:
    double kwota;
    double stan;
    string stanS;
    string log;
    string haslo;
    bool dostep;
    int wplata, wyplata;
    char wybor;
    string typ;
    int PIN=0000;
};

int main()
{
    bank otworz;
    otworz.ekranStartowy();
    cin.get();
}
