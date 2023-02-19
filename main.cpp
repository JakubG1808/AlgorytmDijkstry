#include <iostream>
#include <vector>
#include <fstream>
#include <string>


using namespace std;

struct wierzcholek
{
    int id; /// NUMER WIERZCHOLKA
    int liczba_pol;
    int status;
    int k_dist; /// ODLEGLOSC OD STARTU
    int k_poprzedzajacy;
    int sasiedzi[20][3]; /// LISTA SASIADOW MAX 20 (NUMER WIERZCHOLKA, WAGA)
};

class graf
{
public:
    wierzcholek* wierzcholki;
    graf(string);
};

graf::graf(string nazwaPliku)
{
    fstream plik;
    plik.open(nazwaPliku, std::ofstream::in);
    int rozmiar = 0;
    plik >> rozmiar;

    wierzcholki = new wierzcholek[rozmiar+1];

    for(int i = 1; i < (rozmiar+1); i++)
    {
        wierzcholki[i].id = 0;
        wierzcholki[i].liczba_pol = 0;
        wierzcholki[i].status = 0;
        wierzcholki[i].k_dist = 99999999;
        wierzcholki[i].k_poprzedzajacy = 0;
        for(int j = 0; j < 20; j++)
        {
            wierzcholki[i].sasiedzi[j][0] = 0;
            wierzcholki[i].sasiedzi[j][1] = 0;
        }
    }

    int temp_id = 0;
    string stemp = "";

    while(true) /// PETLA LADUJACA WIERZCHOLKI DO GRAFU
    {
        plik >> stemp;
        if(stemp == "KONIEC") break;
        temp_id = stoi(stemp);

        if (wierzcholki[temp_id].id != temp_id)
        {
            //cout << temp_id << endl; ///TEST
            wierzcholki[temp_id].id = temp_id;
        }

        plik >> wierzcholki[temp_id].sasiedzi[wierzcholki[temp_id].liczba_pol][0];
        plik >> wierzcholki[temp_id].sasiedzi[wierzcholki[temp_id].liczba_pol][1];



        //cout << "dla: " << wierzcholki[temp_id].id << " polaczenie do: " << wierzcholki[temp_id].sasiedzi[wierzcholki[temp_id].liczba_pol][0]; ///TEST
        //cout << " o wadze " << wierzcholki[temp_id].sasiedzi[wierzcholki[temp_id].liczba_pol][1] << endl; ///TEST

        wierzcholki[temp_id].liczba_pol++;
    }
    plik.close();
}

class AlgorytmDijkstry
{
public:
    graf* graf_a;
    AlgorytmDijkstry(graf* graf_c);
    string Podaj_najkr_sciezke(int start, int koniec);
};

AlgorytmDijkstry::AlgorytmDijkstry(graf* graf_c)
{
    graf_a = graf_c;
}

string AlgorytmDijkstry::Podaj_najkr_sciezke(int start, int koniec)
{


    ///WRZUC DO KOLEJKI WEZEL START DODAJ k_dist 0 I k_Poprzedzajacy 0 I k_Act = START

    vector<int> kolejka;
    int k_Act = start;
    graf_a->wierzcholki[k_Act].k_dist = 0;
    int k_najmniejsza = 99999999;


    ///PETLA
    while(k_Act != koniec)
    {
        cout << "==========================" << endl;

        ///DODAJ SASIADOW k_Act (JEZELI STATUS 0)


        k_najmniejsza = 99999999;
        int sasiad_id = 0;
        cout << "srawdzam sasiadow wierzcholka: " << graf_a->wierzcholki[k_Act].id << endl; ///TEST
        for(int i = 0; i < graf_a->wierzcholki[k_Act].liczba_pol; i++)
        {

            sasiad_id = graf_a->wierzcholki[k_Act].sasiedzi[i][0];

            cout << "if(" << graf_a->wierzcholki[sasiad_id].k_dist << " > " << graf_a->wierzcholki[k_Act].sasiedzi[i][1] << " + " << graf_a->wierzcholki[k_Act].k_dist << ") ";


            if(graf_a->wierzcholki[sasiad_id].status < 2 && graf_a->wierzcholki[sasiad_id].k_dist > (graf_a->wierzcholki[k_Act].sasiedzi[i][1] + graf_a->wierzcholki[k_Act].k_dist))
            {

                cout << " podnieniam k_dist" << endl;
                graf_a->wierzcholki[sasiad_id].k_poprzedzajacy = k_Act;

                graf_a->wierzcholki[sasiad_id].k_dist = graf_a->wierzcholki[k_Act].sasiedzi[i][1] + graf_a->wierzcholki[k_Act].k_dist;
                cout << "sasiad: " << sasiad_id << "   | k_pop: " << graf_a->wierzcholki[sasiad_id].k_poprzedzajacy << "   | k_dis: " << graf_a->wierzcholki[sasiad_id].k_dist << endl;

                if(graf_a->wierzcholki[sasiad_id].status == 0)
                {
                    cout << "kolejka push: " << kolejka.size() << " | id: " <<  sasiad_id << endl;
                    kolejka.push_back(sasiad_id);
                    graf_a->wierzcholki[sasiad_id].status = 1;
                }

            }
        }


        ///PETLA PO WPISACH

        for(int id : kolejka)
        {
            ///SPRAWDZAJ I PODMIENIAJ NAJMNIEJSZA WAGE
            cout << "sprawdzenie: id: " << id << " ... " << graf_a->wierzcholki[id].k_dist << " <= " << k_najmniejsza << endl;
            if(graf_a->wierzcholki[id].k_dist <= k_najmniejsza)
            {
                k_najmniejsza = graf_a->wierzcholki[id].k_dist;
                ///USTAW k_Act NA WIERZCHOLEK Z NAJMNIEJASZA WAGA
                k_Act = id;
                cout << "graf_a->wierzcholki[id].id = " << graf_a->wierzcholki[id].id << endl;
            }
        }

        ///USTAW STATUS k_Act NA 1
        graf_a->wierzcholki[k_Act].status = 2;
        ///USUN WIERZCHOLEK k_Act z kolejki
        int i = 0;
        cout<< "szukanie do wywalenia: " << k_Act << endl;
        for (auto& w : kolejka)
        {
            cout << i << " ; " << w << endl;


            if (w == k_Act)
            {
                kolejka.erase(kolejka.begin() + i);
            }
            i++;
        }
        cout << "wygrywa wierzcholek nr: " << k_Act << endl;
    }

    cout << "DONE" << endl;

    string sciezka = to_string(k_Act);
    int step = k_Act;
    while(step != start)
    {
        step = graf_a->wierzcholki[step].k_poprzedzajacy;
        sciezka = to_string(step) + " > " + sciezka;
    }
    sciezka = "sciezka: " + sciezka;
    return sciezka;

}

int main()
{
    graf g1("graf0.txt");
    AlgorytmDijkstry a1(&g1);
    cout << a1.Podaj_najkr_sciezke(1,15);
    return 0;
}
