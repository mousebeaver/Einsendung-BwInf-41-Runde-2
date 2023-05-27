#define vs vector<short>
#include <bits/stdc++.h>
using namespace std;

void readInput(int& pcounter, vs& pancakes) //Einlesen der Eingabe
{
    cout<<"Name der Eingabedatei: ";
    string finPath;
    cin>>finPath;
    freopen(finPath.c_str(), "r", stdin);

    cin>>pcounter;
    pancakes.assign(pcounter, 0);
    for(int i = 0; i < pcounter; i++)
    {
        cin>>pancakes[i];
    }
}

int main()
{
    int pcounter; //Anzahl der zu sortierenden Pfannkuchen
    vs pancakes; //Der zu sortierende Pfannkuchenstapel

    readInput(pcounter, pancakes); //Einlesen der Eingabe

    int postfix = pcounter-1; //Start des laengsten sortierten Postfix

    while(postfix > 0 && pancakes[postfix] > pancakes[postfix-1]) //postfix so weit wie moeglich verringern
    {
        postfix--;
    }

    int operations = postfix; //Anzahl der benoetigten Operationen
    cout<<"Der Pfannkuchenstapel kann durch "<<operations<<" PE-Operationen sortiert werden."<<endl;

    return 0;
}
