#include "definitions.h"
#include "cuboid.h"

void readInput(int& n, int& m, set<pll>& slicesSafe, map<pll, ll>& countersSafe, set<pll>& slicesChange, map<pll, ll>& countersChange, ofstream& fout)
{
    //Funktion zum Einlesen der Eingabe

    //Oeffnen der Ein- und Ausgabedatei:
    cout<<"Name der Eingabedatei: ";
    char input[100];
    cin>>input;
    cout<<"Name der Ausgabedatei: ";
    string output;
    cin>>output;
    freopen(input, "r", stdin);
    fout.open(output, ios_base::out);

    //Einlesen der Scheiben:
    cin>>n;
    for(int i = 0; i < n; i++)
    {
        ll a, b;
        cin>>a>>b;
        if(a > b)
        {
            swap(a, b); //Die Eingabe soll sortiert vorliegen
        }
        if(slicesSafe.count({a, b}) == 0)
        {
            slicesSafe.insert({a, b});
            countersSafe[{a, b}] = 0;
        }
        countersSafe[{a, b}]++;
    }

    m = slicesSafe.size();
    slicesChange = slicesSafe;
    countersChange = countersSafe;
}

void precompute(ll& volume, ll& longestEdge, ll& largestArea, set<pll>& slices, map<pll, ll>& counters)
{
    //Vorberechnung von Volumen des Quaders und Maximalgroessen der Scheiben
    volume = 0;
    longestEdge = 0;
    largestArea = 0;

    for(pll p : slices) //Durchgehen aller Scheiben
    {
        ll area = p.first*p.second; //Flaeche dieser Scheibe
        volume += area*counters[p];
        largestArea = max(largestArea, area);
        longestEdge = max(longestEdge, p.second);
    }
}

int main()
{
    //Variablen zum Speichern der Eingabe:
    int n; //Anzahl der Scheiben
    int m; //Anzahl der Scheiben ohne Duplikate
    set<pll> slicesSafe; //Unveraenderliche Kopie der Menge von Scheiben
    map<pll, ll> countersSafe; //Unveraenderliche Kopie der Zaehler fuer Scheiben
    set<pll> slicesChange; //Veraenderliche Kopie der Menge von Scheiben
    map<pll, ll> countersChange; //Veraenderliche Kopie der Zaehler fuer Scheiben
    //In jedem Paar (a, b) ganzer Zahlen soll dabei b >= a gelten!
    ofstream fout; //Ausgabedatei

    readInput(n, m, slicesSafe, countersSafe, slicesChange, countersChange, fout); //Lesen der Eingabe

    //Variablen zum Speichern noetiger Vorberechnungen:
    ll volume; //Volumen des Quaders
    ll longestEdge; //Groesste Seitenlaenge einer Scheibe
    ll largestArea; //Groesste Flaeche einer Scheibe

    precompute(volume, longestEdge, largestArea, slicesSafe, countersSafe); //Ausfuehren der Vorberechnungen

    //Durchgehen aller unterschiedlicher Kanten:
    for(pll slice : slicesSafe)
    {
        ll area = slice.first*slice.second;
        if(volume%area == 0)
        {
            //Die Flaeche teilt das Volumen, slice kann eine Seitenflaeche sein!
            cuboid q(slice.first, slice.second, volume/area); //Initialisieren des Quaders Q
            vector<pll> deleted(0); //Abgeschnitte Scheiben
            if(q.largestArea() >= largestArea && q.longestEdge() >= longestEdge)
            {
                //Q ist gross genug, der Test wird durchgefuehrt
                while(!slicesChange.empty())
                {
                    //Abschneiden einer weiteren Scheibe:
                    pll cutted = q.cutBiggestSlice(slicesChange);
                    if(cutted == NOPAIR)
                    {
                        //Es konnte keine weitere Scheibe abgeschnitten werden
                        break;
                    }
                    deleted.push_back(cutted); //Hinzufuegen zur Liste der geloeschten Scheiben
                    countersChange[cutted]--; //Entfernen der Scheibe
                    if(countersChange[cutted] == 0)
                    {
                        slicesChange.erase(cutted);
                    }
                }

                if(slicesChange.empty())
                {
                    //Es wurde eine gueltige Zerlegung gefunden
                    //Ausgabe:
                    fout<<"Die angegebene Menge von Scheiben laesst sich zu einem Quader mit den Seitenlaengen "<<slice.first<<", "<<slice.second<<" und "<<volume/area<<" zusammensetzen.\n";
                    fout<<"Dabei muessen die Scheiben in folgender Reihenfolge vom Quader abgeschnitten werden:\n";
                    for(pll p : deleted)
                    {
                        fout<<p.first<<" x "<<p.second<<"\n";
                    }
                    fout.close();
                    return 0;
                }
                else
                {
                    //Es wurde keine gueltige Zerlegung gefunden:
                    int k = deleted.size();
                    if(k*log2(n) <= m)
                    {
                        //Es ist schneller, die Elemente einzeln in die Liste einzufuegen
                        for(pll p : deleted)
                        {
                            if(countersChange[p] == 0)
                            {
                                slicesChange.insert(p);
                            }
                            countersChange[p]++;
                        }
                    }
                    else
                    {
                        //Es ist schneller, die gesamte Liste zu kopieren
                        slicesChange = slicesSafe;
                        countersChange = countersSafe;
                    }
                }
            }
        }
    }

    //Es wurde keine gueltige Zerlegung gefunden!
    fout<<"Die angegebene Menge von Scheiben laesst sich nicht zu einem vollstaendigen Kaesequader zusammensetzen.\n";
    fout.close();
    return 0;
}
