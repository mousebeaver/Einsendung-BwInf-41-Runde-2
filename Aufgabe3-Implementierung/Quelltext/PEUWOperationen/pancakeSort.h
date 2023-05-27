#ifndef PANCAKESORT_H_INCLUDED
#define PANCAKESORT_H_INCLUDED

//Headerdatei fuer das Berechnen von A(S) fuer einen Stapel S

#include "definitions.h"
#include "pancakeStacks.h"

void readInput(int& pcounter, vs& pancakes, ofstream& fout) //Einlesen der Eingabe
{
    cout<<"Name der Eingabedatei: ";
    string finPath;
    cin>>finPath;
    cout<<"Name der Ausgabedatei: ";
    string foutPath;
    cin>>foutPath;

    freopen(finPath.c_str(), "r", stdin);
    fout.open(foutPath, ios_base::out);

    cin>>pcounter;
    pancakes.assign(pcounter, 0);
    for(int i = 0; i < pcounter; i++)
    {
        cin>>pancakes[i];
    }
}

void outputPEUWSequence(ofstream& fout, unordered_map<ll, vector<pil>>& backtracking, vs pancakes, vs start, vector<ll>& faculty)
{
    //Ausgabe von benoetigten Wendeoperationen
    vector<vs> output(0); //Reihenfolge der erreichten Stapel
    vs rstart = start;
    simplify(rstart); //Stapel, bei dem das backtracking endet

    //Hinzufuegen der letzten beiden Stapel:
    output.push_back({});
    output.push_back(pancakes);

    //Hinzufuegen der weiteren Stapel (in simplifizierter Form):
    while(pancakes != rstart)
    {
        //Schliessen auf den vorhergehenden Stapel:
        pil previous = backtracking[stackIndex(pancakes, faculty)][pancakes.size()-1];
        pancakes = explicitStack(previous.first, previous.second, faculty);
        output.push_back(pancakes);
    }
    reverse(output.begin(), output.end());

    //Ausgabe:
    fout<<"Fuer das Sortieren des Stapels "<<stackOutput(start)<<" werden mindestens "<<output.size()-1<<" PEUW-Operationen benoetigt.\n";
    fout<<"Es ist also A"<<stackOutput(start)<<" = "<<output.size()-1<<".\n";
    fout<<"Folgende Operationen sortieren den Stapel mit der minimalen Anzahl an Schritten:\n";
    //Durchgehen aller Schritte
    pancakes = start;
    output.erase(output.begin());
    while(output.size())
    {
        int range = 0; //Anzahl der Pfannkuchen, auf die die naechste Operation angewandt wird

        vs test = pancakes; //Zum Durchfuehren von PEUW-Operationen
        test.erase(test.begin());
        outputSimplify(test);

        while(test != output[0])
        {
            range++;
            //Ausfuehren der naechsten PEUW-Operation:
            test = pancakes;
            test.erase(test.begin());
            reverse(test.begin(), test.begin()+range);
            outputSimplify(test);
        }
        output.erase(output.begin());

        fout<<"\nNach Anwenden der PEUW-Operation auf die ersten "<<range+1<<" Pfannkuchen:\n";
        pancakes.erase(pancakes.begin());
        reverse(pancakes.begin(), pancakes.begin()+range); //Durchfuehren einer PEUW-Operation OHNE Simplifizieren
        fout<<stackOutput(pancakes)<<endl;
    }
    fout<<"\nDamit ist der Pfannkuchenstapel sortiert.\n";
    fout.close();
    exit(0);
}

void evalA(vector<ll>& faculty) //Die Funktion fuer das Berechnend der Funktion A(S)
{
    int pcounter; //Anzahl der zu sortierenden Pfannkuchen
    vs pancakes; //Der zu sortierende Pfannkuchenstapel
    ofstream fout; //Ausgabedatei

    readInput(pcounter, pancakes, fout); //Einlesen der Eingabe

    if(sorted(pancakes))
    {
        //Es muessen keine Operationen mehr durchgefuehrt werden!
        fout<<"Der Pfannkuchenstapel "<<stackOutput(pancakes)<<" ist schon sortiert.\n";
        fout<<"Fuer diesen Stapel S ist entsprechend A(S) = 0\n";
        fout.close();
        exit(0);
    }

    unordered_map<ll, vector<pil>> backtracking; //Der Stapel mit (j+1) Pfannkuchen und Index i kann vom Stapel in backtracking[i][j] (Anzahl, Index) durch eine PEUW-Operation erreicht werden
    queue<pil> q; //Warteschlange fuer Pfannkuchenstapel. (i, j) := Stapel mit i Pfannkuchen und Index j
    q.push({pcounter, stackIndex(pancakes, faculty)});

    while(true) //Berechnung durchfuehren bis zum Erreichen einer Loesung
    {
        //Aktuell zu untersuchender Stapel
        vs current = explicitStack(q.front().first, q.front().second, faculty);
        q.pop();
        ll currentIndex = stackIndex(current, faculty);
        for(int i = 0; i < (int) current.size(); i++) //Durchgehen aller moeglichen Positionen fuer eine PEUW-Operation
        {
            vs test = flipOperation(current, i); //Stapel nach einer solchen PEUW-Operation
            if(test.empty())
            {
                //Der Pfannkuchenstapel ist nun sortiert!
                //Eine Loesung wird ausgegeben
                outputPEUWSequence(fout, backtracking, current, pancakes, faculty);
                exit(0);
            }
            //Der Pfannkuchenstapel ist noch nicht sortiert
            ll testIndex = stackIndex(test, faculty);
            //Aktualisieren von backtracking:
            if(backtracking.count(testIndex) == 0)
            {
                backtracking[testIndex].assign(pcounter, {-1, -1});
            }
            if(backtracking[testIndex][test.size()-1] == make_pair(-1, (ll) -1)) //Der Stapel wurde noch nicht untersucht
            {
                backtracking[testIndex][test.size()-1] = {current.size(), currentIndex};
                q.push({test.size(), testIndex});
            }
        }
    }
}

#endif // PANCAKESORT_H_INCLUDED
