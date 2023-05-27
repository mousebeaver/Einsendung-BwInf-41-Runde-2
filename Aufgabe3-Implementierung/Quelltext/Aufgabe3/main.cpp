//Loesung fuer Aufgabe 3: Pancake Sort
#include "definitions.h"
#include "pancakeStacks.h"
#include "pancakeSort.h"
#include "evalPWUE.h"

int main()
{
    //Vorberechnung von Fakultaeten:
    vector<ll> faculty(21, 1); //Faculty[k] = k!
    for(ll i = 2; i <= 20; i++)
    {
        faculty[i] = i*faculty[i-1];
    }

    cout<<"Nummer der auszufuehrenden Funktionalitaet:\n1 => Sortieren eines gegebenen Stapels\n2 => Berechnen der Funktion P"<<endl;
    int command;
    cin>>command;
    if(command == 1)
    {
        //Ein einzelner Stapel soll sortiert werden
        evalA(faculty);
    }
    else
    {
        //Die Funktion P soll berechnet werden
        evalP(faculty);
    }

    return 0;
}
