#include "definitions.h"
#include "cuboid.h"

void readInput(ll& n, ll& m, set<pll>& slices, map<pll, ll>& counters, map<pll, ll>& countersSafe, ofstream& fout)
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
        if(slices.count({a, b}) == 0)
        {
            slices.insert({a, b});
            counters[{a, b}] = 0;
        }
        counters[{a, b}]++;
    }

    m = slices.size();
    countersSafe = counters;
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
    ll n; //Anzahl der Scheiben
    ll m; //Anzahl der Scheiben ohne Duplikate
    set<pll> slices; //Menge von Scheiben
    map<pll, ll> counters; //Zaehler fuer Scheiben
    map<pll, ll> countersSafe; //Unveraenderlicher Zaehler fuer Scheiben
    //In jedem Paar (a, b) ganzer Zahlen soll dabei b >= a gelten!
    ofstream fout; //Ausgabedatei

    readInput(n, m, slices, counters, countersSafe, fout); //Einlesen der Eingabe


    stack<pll> order; //Reihenfolge der abgeschnittenen Scheiben
    for(pll s : slices) //Scheibe, mit der gestartet wird
    {
        cuboid q(s.first, s.second, 1);
        counters[s]--;
        order.push(s);

        stack<tblp> dfs;
        /*Stapel zum Verwalten der Tiefensuch-Frames (speichert jeweils,
        ob schon die fehlende Scheibe hinzugefuegt wurde,
        in queries die Anzahl der untersuchten Folgemoeglichkeiten
        und zuletzt hinzugefuegte Scheibe)*/
        dfs.push({false, 0, {-1, -1}});
        while(dfs.size())
        {
            bool missing = get<0>(dfs.top());
            ll queries = get<1>(dfs.top());
            pll lastSlice = get<2>(dfs.top());
            dfs.pop();
            if(queries < 6)
            {
                dfs.push({missing, queries+1, lastSlice});
            }

            if((ll) order.size() == n+1) //Es wurden schon ausreichend viele Scheiben hinzugefuegt
            {
                //Ausgabe!
                tlll t = q.getLengths();
                fout<<"Der gesuchte Quader hat die Seitenlaengen "<<get<0>(t)<<", "<<get<1>(t)<<" und "<<get<2>(t)<<".\n";
                fout<<"Die Scheiben muessen dabei in der folgenden Reihenfolge abgeschnitten werden:\n\n";
                while(order.size())
                {
                    pll s = order.top();
                    order.pop();
                    fout<<s.first<<" x "<<s.second;
                    if(countersSafe.count(s) == 0 || countersSafe[s] == 0)
                    {
                        fout<<" (Diese Scheibe hatte Antje schon vorher gegessen)";
                    }
                    else
                    {
                        countersSafe[s]--;
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
            if(queries == 0 && available(s1, slices, counters)) //Kleinste Seitenflaeche muss hinzugefuegt werden
            {
                q.addSlice(1);
                counters[s1]--;
                order.push(s1);
                dfs.push({missing, 0, s1});
            }
            else if(queries == 1 && !missing) //Kleinste Seitenflaeche als die von Antje gegessene
            {
                q.addSlice(1);
                counters[s1]--;
                order.push(s1);
                dfs.push({true, 0, s1});
            }

            if(queries == 2 && available(s2, slices, counters)) //Zweitkleinste Seitenflaeche muss hinzugefuegt werden
            {
                q.addSlice(2);
                counters[s2]--;
                order.push(s2);
                dfs.push({missing, 0, s2});
            }
            else if(queries == 3 && !missing) //Zweitkleinste Seitenflaeche als die von Antje gegessene
            {
                q.addSlice(2);
                counters[s2]--;
                order.push(s2);
                dfs.push({true, 0, s2});
            }

            if(queries == 4 && available(s3, slices, counters)) //Zweitkleinste Seitenflaeche muss hinzugefuegt werden
            {
                q.addSlice(3);
                counters[s3]--;
                order.push(s3);
                dfs.push({missing, 0, s3});
            }
            else if(queries == 5 && !missing) //Zweitkleinste Seitenflaeche als die von Antje gegessene
            {
                q.addSlice(3);
                counters[s3]--;
                order.push(s3);
                dfs.push({true, 0, s3});
            }

            if(queries == 6) //Tiefensuche ueber diese Moeglichkeit ist beendet
            {
                q.cutSlice(lastSlice);
                if(counters.count(lastSlice) > 0)
                {
                    counters[lastSlice]++;
                }
                order.pop();
            }
        }

        //order.pop();
        counters[s]++;
    }

    fout<<"Die angegebene Menge von Kaesescheiben laesst sich zu keinem vollstaendigen Quader zusammensetzen.\n";
    fout.close();

    return 0;
}
