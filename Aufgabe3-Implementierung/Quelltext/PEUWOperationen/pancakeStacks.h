#ifndef PANCAKESTACKS_H_INCLUDED
#define PANCAKESTACKS_H_INCLUDED

//Enthaelt Funktionen fuer alle Operationen, die auf Pfannkuchenstapel angewendet werden
//Ein Stapel wird dabei als Vektor des Typs vector<short> gespeichert
#include "definitions.h"

string stackOutput(vs& pancakes) //Wandelt einen Pfannkuchenstapel in einen String um
{
    string output = "( ";
    for(short i : pancakes)
    {
        output += to_string(i);
        output += " ";
    }
    output += ")";
    return output;
}

void simplify(vs& pancakes) //Vereinfacht einen Pfannkuchenstapel, in dem genau ein Pfannkuchen fehlt
{
    int pcounter = pancakes.size(); //Anzahl der verbleibenden Pfannkuchen

    //Berechnen des fehlenden Pfannkuchens:
    int sum = 0; //Summe aller enthaltenen Pfannkuchen
    for(short i : pancakes)
    {
        sum += i;
    }
    int missing = ((pcounter+1)*(pcounter+2))/2-sum; //Berechnung des fehlenden Pfannkuchens

    //Vereinfachung durch Verkleinerung von Pfannkuchenzahlen:
    for(int i = 0; i < pcounter; i++) //Durchgehen aller Pfannkuchen
    {
        if(pancakes[i] > missing) //Der Pfannkuchen ist groesser als der fehlende
        {
            pancakes[i]--; //Verkleinern des Pfannkuchens
        }
    }

    //Vereinfachung durch Entfernen von Pfannkuchen an der Unterseite des Stapels
    while(pcounter > 0 && pcounter == pancakes[pcounter-1]) //Der groesste Pfannkuchen liegt ganz unten
    {
        pancakes.pop_back(); //Entfernen des untersten Pfannkuchens
        pcounter--;
    }
}

void outputSimplify(vs& pancakes) //Vereinfacht einen Pfannkuchenstapel, in dem eine unbekannte Anzahl Pfannkuchen fehlt - nur fuer Output benoetigt
{
    int pcounter = pancakes.size();
    int m = *max_element(pancakes.begin(), pancakes.end()); //Groesster Pfannkuchen

    vs pancopy = pancakes; //Pfannkuchen nach Groesse sortiert
    sort(pancopy.begin(), pancopy.end());
    vs dict(m); //dict[i] = Wert, den Pfannkuchen i annehmen muss

    int pre = 0;
    for(int i : pancopy)
    {
        int diff = i-pre; //Verringern des Wertes je nach Luecke zum vorherigen
        dict[i-1] = 1+i-diff;
        pre = dict[i-1];
    }

    //Aktualisieren der Pfannkuchen:
    for(int i = 0; i < pcounter; i++)
    {
        pancakes[i] = dict[pancakes[i]-1];
    }

    //Vereinfachung durch Entfernen von Pfannkuchen an der Unterseite des Stapels
    while(!pancakes.empty() && pancakes[pancakes.size()-1] == (int) pancakes.size())
    {
        pancakes.pop_back();
    }
}

vs flipOperation(vs pancakes, int range) //Umsetzung einer Wende-Und-Ess-Operation
{
    //range gibt die Anzahl der betroffenen Pfannkuchen an
    pancakes.erase(pancakes.begin()); //Aufessen des nun obersten Pfannkuchens
    reverse(pancakes.begin(), pancakes.begin()+range); //Umdrehen der Pfannkuchen
    simplify(pancakes);
    return pancakes;
}

bool sorted(vs& pancakes) //Liefert true zurueck gdw. der uebergebene Stapel von Pfannkuchen korrekt (absteigend) sortiert ist
{
    for(int i = 0; i < (int) pancakes.size()-1; i++)
    {
        if(pancakes[i] > pancakes[i+1])
        {
            return false; //Zwei Pfannkuchen liegen in der falschen Reihenfolge vor
        }
    }
    return true; //Der Stapel ist korrekt sortiert
}

ll stackIndex(vs pancakes, vector<ll>& faculty) //Gibt Nummer dieses Stapels in lexikographischer Sortierung aller Stapel (dieser Groesse!) zurueck
{
    int pcounter = pancakes.size();

    //Berechnen des Indizes:
    ll index = 0;
    for(int i = 0; i < pcounter; i++) //Durchgehen aller Positionen im Stapel
    {
        index += faculty[pcounter-i-1]*((ll) pancakes[i]-1); //Erhohen des Indizes nach Einreihung des Elements unter den noch verfuegbaren
        for(int j = i+1; j < pcounter; j++) //Anpassen der folgenden Zahlen
        {
            if(pancakes[j] > pancakes[i])
            {
                pancakes[j]--;
            }
        }
    }
    return index;
}

vs explicitStack(int pcounter, ll index, vector<ll>& faculty) //Gibt Stapel mit entsprechender Anzahl an Elementen und diesem Index zurueck
{
    vs output(pcounter); //Der zu berechnende Stack
    vs remaining(pcounter); //Noch nicht auf den Stapel gelegte Pfannkuchen
    iota(remaining.begin(), remaining.end(), 1);

    for(int i = 0; i < pcounter; i++)
    {
        //Hinzufuegen des naechsten Pfannkuchens
        int position = index/faculty[pcounter-i-1];
        output[i] = remaining[position];
        remaining.erase(remaining.begin()+position);
        index -= faculty[pcounter-i-1]*position;
    }

    return output;
}

vs insertCake(int pos, short val, vs pancakes)
{
    /*Erstellt durch das Einfuegen des uebergebenen Werts
    an der gegebenen Stelle einen Pfannkuchenstapel S mit
    A(S) = A(pancakes)+1, der sich durch eine PEUW-Operation
    in pancakes ueberfuehren laesst.*/
    for(int i = 0; i < (int) pancakes.size(); i++)
    {
        if(pancakes[i] >= val)
        {
            pancakes[i]++; //Erhoehen von nun zu kleinen Werten
        }
    }
    //Umkehrung einer PEUW-Operation:
    reverse(pancakes.begin(), pancakes.begin()+pos);
    pancakes.insert(pancakes.begin(), val);
    return pancakes;
}

#endif // PANCAKESTACKS_H_INCLUDED
