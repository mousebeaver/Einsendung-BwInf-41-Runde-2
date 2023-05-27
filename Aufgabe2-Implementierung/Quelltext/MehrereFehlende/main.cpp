#include "definitions.h"
#include "cuboid.h"

void readInput(int& n, int& m, set<pll>& slices, map<pll, ll>& counters, map<pll, ll>& countersSafe, ofstream& fout, cuboid& q, ll& sliceVolume)
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
    sliceVolume = 0;
    cin>>n;
    for(int i = 0; i < n; i++)
    {
        ll a, b;
        cin>>a>>b;
        if(a > b)
        {
            swap(a, b); //Die Eingabe soll sortiert vorliegen
        }
        if(slices.count({a, b}) == 0)
        {
            slices.insert({a, b});
            counters[{a, b}] = 0;
        }
        counters[{a, b}]++;
        sliceVolume += a*b;
    }

    m = slices.size();
    countersSafe = counters;

    //Einlesen des Quaders
    ll x, y, z;
    cin>>x>>y>>z;
    cuboid preq(x, y, z);
    q = preq;
}

bool available(pll slice, set<pll>& slices, map<pll, ll>& counters)
{
    //Kann die uebergebene Scheibe noch verwendet werden?
    if(slices.count(slice) > 0 && counters[slice] > 0)
    {
        return true;
    }
    return false;
}

int main()
{
    //Variablen zum Speichern der Eingabe:
    int n; //Anzahl der Scheiben
    int m; //Anzahl der Scheiben ohne Duplikate
    set<pll> slices; //Veraenderliche Kopie der Menge von Scheiben
    map<pll, ll> counters; //Veraenderliche Kopie der Zaehler fuer Scheiben
    map<pll, ll> countersSafe; //Unveraenderliche Kopie der Zaehler fuer Scheiben
    //In jedem Paar (a, b) ganzer Zahlen soll dabei b >= a gelten!
    ofstream fout; //Ausgabedatei
    cuboid q(0, 0, 0); //Der Quader q
    ll sliceVolume; //Volumen aller verfuegbaren Scheiben

    readInput(n, m, slices, counters, countersSafe, fout, q, sliceVolume); //Lesen der Eingabe

    stack<pll> order; //Reihenfolge der abgeschnittenen Scheiben
    stack<tlpb> dfs;
    /*Stapel zum Verwalten der Tiefensuch-Frames, verwaltet die Anzahl
    der Folgemoeglichkeiten und die zuletzt hinzugefuegte Scheibe.
    Ausserdem wird angegeben, ob diese letzte Scheibe schon von Antje gegessen wurde*/
    dfs.push({0, NOPAIR, true}); //Zu Beginn wurde noch keine Scheibe hinzugefuegt
    order.push(NOPAIR);

    while(dfs.size())
    {
        ll queries = get<0>(dfs.top());
        pll lastSlice = get<1>(dfs.top());
        bool eaten = get<2>(dfs.top());
        dfs.pop();
        if(queries < 6)
        {
            dfs.push({queries+1, lastSlice, eaten});
        }

        if(q.volume() == 0)
        {
            //Der Quader wurde vollstaendig zerlegt => Ausgabe!
            fout<<"Die Scheiben werden in der folgenden Reihenfolge vom Quader abgeschnitten:\n\n";
            pll s = order.top();
            order.pop();
            vector<pll> output(0);
            while(s != NOPAIR) //Reihenfolge umkehren
            {
                output.push_back(s);
                s = order.top();
                order.pop();
            }
            reverse(output.begin(), output.end());

            for(ll i = 0; i < (ll) output.size(); i++)
            {
                s = output[i];
                fout<<s.first<<" x "<<s.second;
                if(countersSafe.count(s) > 0 && countersSafe[s] > 0)
                {
                    countersSafe[s]--;
                }
                else
                {
                    fout<<" (Diese Scheibe hatte Antje schon aufgegessen)";
                }
                fout<<"\n";
            }

            fout.close();
            exit(0);
        }

        //Seitenflaechen des Quaders:
        pll s1 = q.sliceK(1);
        pll s2 = q.sliceK(2);
        pll s3 = q.sliceK(3);

        //Versucht nacheinander, Scheiben abzuschneiden (in der Reihenfolge des Algorithmus)
        if(queries == 0 && available(s1, slices, counters)) //Groesste Seitenflaeche muss abgeschnitten werden
        {
            q.cutSlice(1);
            counters[s1]--;
            sliceVolume -= s1.first*s1.second;
            order.push(s1);
            dfs.push({0, s1, false});
        }
        else if(queries == 1 && s1.first*s1.second <= q.volume()-sliceVolume) //Groesste Seitenflaeche als von Antje gegessen
        {
            q.cutSlice(1);
            order.push(s1);
            dfs.push({0, s1, true});
        }

        if(queries == 2 && available(s2, slices, counters)) //Zweitgroesste Seitenflaeche muss abgeschnitten werden
        {
            q.cutSlice(2);
            counters[s2]--;
            sliceVolume -= s2.first*s2.second;
            order.push(s2);
            dfs.push({0, s2, false});
        }
        else if(queries == 3 && s2.first*s2.second <= q.volume()-sliceVolume) //Zweitgroesste Seitenflaeche als von Antje gegessen
        {
            q.cutSlice(2);
            order.push(s2);
            dfs.push({0, s2, true});
        }

        if(queries == 4 && available(s3, slices, counters)) //Drittgroesste Seitenflaeche muss abgeschnitten werden
        {
            q.cutSlice(3);
            counters[s3]--;
            sliceVolume -= s3.first*s3.second;
            order.push(s3);
            dfs.push({0, s3, false});
        }
        else if(queries == 5 && s3.first*s3.second <= q.volume()-sliceVolume) //Drittgroesste Seitenflaeche als von Antje gegessen
        {
            q.cutSlice(3);
            order.push(s3);
            dfs.push({0, s3, true});
        }

        if(queries == 6) //Tiefensuche ueber diese Moeglichkeit ist beendet
        {
            q.addSlice(lastSlice);
            if(!eaten)
            {
                counters[lastSlice]++;
                sliceVolume += lastSlice.first*lastSlice.second;
            }
            order.pop();
        }
    }

    //Es wurde keine gueltige Zerlegung gefunden!
    fout<<"Die angegebene Menge von Scheiben laesst sich nicht zu einem vollstaendigen Kaesequader zusammensetzen.\n";
    fout.close();
    return 0;
}
