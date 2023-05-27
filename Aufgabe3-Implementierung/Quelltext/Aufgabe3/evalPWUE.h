//Umsetzung der Berechnungen in Teilaufgabe b)
#ifndef EVALPWUE_H_INCLUDED
#define EVALPWUE_H_INCLUDED

#include "definitions.h"
#include "pancakeStacks.h"

void derive(vector<vs>& target, vector<vs>& source, vector<unordered_map<ll, short>>& moves, vector<ll>& faculty)
{
    /*Diese Funktion berechnet alle Pfannkuchenstapel, die durch eine PWUE-Operation nur in Stapel ueberfuehrt werden koennen,
    die entweder in source liegen oder die fuer das Sortieren mehr Operationen brauchen als die Stapel in der Liste
    source. Diese werden dann in target geschoben
    */
    if(source.empty())
    {
        return;
    }
    int pcounter = source[0].size(); //Anzahl der Pfannkuchen in Stapeln in source
    unordered_set<ll> tested; //Schon fuer target untersucht

    for(vs v : source) //Durchgehen aller Pfannkuchenstapel in source
    {
        short vMoves = moves[v.size()-1][stackIndex(v, faculty)]; //A(v)

        //Durchgehen aller Stapel, die durch eine PWUE-Operation in v ueberfuehrt werden koennen
        for(int pos = 0; pos <= pcounter; pos++) //Durchgehen der Position, an der ein neuer Pfannkuchen eingefuegt wird
        {
            for(short val = 1; val <= pcounter+1; val++) //Wert des neuen Pfannkuchens
            {
                vs test = insertCake(pos, val, v); //Zu untersuchender Pfannkuchenstapel
                ll testInd = stackIndex(test, faculty);
                if(tested.count(testInd) != 0) //Der Stapel wurde schon untersucht
                {
                    continue;
                }

                if(moves[test.size()-1].count(testInd) == 0 && !sorted(test))
                {
                    //Der Pfannkuchenstapel ist nicht sortiert und kann daher weiter untersucht werden
                    bool vSort = true; //Fuehrt das schnellstmoegliche Sortieren ueber v?
                    for(int k = 0; k <= pcounter; k++) //Durchgehen aller Positionen fuer die PWUE-Operation
                    {
                        vs flipped = flipOperation(test, k); //Durchfuehren einer PWUE-Operation
                        ll fInd = stackIndex(flipped, faculty);

                        if((sorted(flipped) && !sorted(v)) || (!sorted(flipped) && (moves[flipped.size()-1].count(fInd) == 0 || moves[flipped.size()-1][fInd] < vMoves)))
                        {
                            /*Der Stapel laesst sich durch eine PWUE-Operation in einen Stapel ueberfuehren,
                            der schneller sortierbar ist als v*/
                            vSort = false;
                            break;
                        }
                    }
                    if(vSort)
                    {
                        //Der Stapel muss in target eingefuegt werden
                        moves[test.size()-1][testInd] = vMoves+1;
                        target.push_back(test);
                    }
                }
                tested.insert(testInd);
            }
        }
    }
}

void compute(int i, int j, vector<vector<vector<vs>>>& stacklist, vector<unordered_map<ll, short>>& moves, vector<short>& output, int goal, vector<ll>& faculty)
{
    //Berechnet stacklist[i][j] und output[i] und alle weiteren dafuer noetigen Eintraege von stacklist
    if(j == output[i])
    {
        //In stacklist[i][j] ist nur der sortierte Stapel S mit A(S) = 0
        vs pancakesSorted(i+1);
        iota(pancakesSorted.begin(), pancakesSorted.end(), 1); //Erstellen des sortieren Stapels
        stacklist[i].push_back({{pancakesSorted}}); //Einfuegen dieses Stapels
        moves[pancakesSorted.size()-1][stackIndex(pancakesSorted, faculty)] = 0;
        return;
    }

    //Erzeugen eines neuen Eintrags von stacklist:
    stacklist[i].push_back({});

    int pre = j; //stacklist[i][j] wird aus stacklist[i-1][pre] berechnet
    if(output[i] == output[i-1]) //P(i) = P(i-1), daher muss sich pre veraendern
    {
        pre++;
    }
    while(pre >= (int) stacklist[i-1].size() && (int) stacklist[i-1].size() <= output[i-1])
    {
        //Es fehlen noch notwendige Eintraege von stacklist
        compute(i-1, stacklist[i-1].size(), stacklist, moves, output, goal, faculty);
    }

    derive(stacklist[i][j], stacklist[i-1][pre], moves, faculty);
    if(j == 0) //Es wurden die Stapel S mit A(S) = P(i) untersucht
    {
        output[i] = output[i-1]+1;
    }

    if(stacklist[i][j].empty()) //Es gibt keinen Stapel S mit i Pfannkuchen und A(s) > P(i-1)
    {
        output[i]--;
        if(i == goal)
        {
            //Es muessen keine weiteren Funktionswerte von P(i) berechnet werden
            //Daher muss stacklist[i][0] nicht vollstaendig bekannt sein.
            vs simple = stacklist[i-1][0][0];
            simple.push_back(simple.size()+1); //Einfacher Stapel S mit A(S) = P(i)
            stacklist[i][0].push_back(simple);
            return;
        }
        else
        {
            pre++; //Ein weiterer Eintrag von stacklist muss berechnet werden
            while(pre >= (int) stacklist[i-1].size())
            {
                //Es fehlen noch Eintraege von stacklist, auf denen die Berechnung aufbaut
                compute(i-1, stacklist[i-1].size(), stacklist, moves, output, goal, faculty);
            }
            derive(stacklist[i][j], stacklist[i-1][pre], moves, faculty);
        }
    }
}

void evalP(vector<ll>& faculty) //Berechnung der Funktion P
{
    //Variablen fuer das Speichern der Eingabe:
    int n; //Berechne P(1) bis P(n)
    cout<<"Berechnung von P(k) fuer k = 1, 2, ..., n\n";
    cout<<"Wert von n: ";
    cin>>n;

    cout<<"Name der Ausgabedatei: ";
    string foutPath;
    cin>>foutPath;
    ofstream fout; //Ausgabedatei
    fout.open(foutPath, ios_base::out);

    //Fuer die Berechnung notwendige Datenstrukturen:
    vector<vector<vector<vs>>> stacklist (n, vector<vector<vs>> (0)); //stacklist[i][j] = Liste von Stapeln S mit (i+1) Pfannkuchen und A(S) = P(i+1)-j
    vector<unordered_map<ll, short>> moves(n); //moves[i][j] = A(S) fuer einen Stapel S mit (i+1) Pfannkuchen und Index j
    vector<short> output(n, numeric_limits<short>::max()/2); //output[i] = P(i)

    //Vorgeben der Werte fuer P(1):
    stacklist[0].push_back({{{1}}});
    moves[0][stackIndex({1}, faculty)] = 0;
    output[0] = 0;

    //Berechnen der Funktionswerte von P:
    for(int i = 1; i < n; i++)
    {
        compute(i, 0, stacklist, moves, output, n-1, faculty);
        cout<<"P("<<i+1<<") = "<<output[i]<<endl; //Kontrollausgabe an die Konsole
    }

    //Ausgabe an die Ausgabedatei:
    for(int i = 0; i < n; i++)
    {
        fout<<"Es gilt P("<<i+1<<") = "<<output[i]<<"\n";
        fout<<"Ein beispielhafter Pfannkuchenstapel S mit A(S) = P("<<i+1<<") = "<<output[i]<<" ist:\n";
        vs sampleStack = stacklist[i][0][0];
        //Noetigenfalls Hinzufuegen weiterer Pfannkuchen:
        while((int) sampleStack.size() < i+1)
        {
            sampleStack.push_back(sampleStack.size());
        }
        fout<<stackOutput(sampleStack)<<"\n\n";
    }
    fout.close();
}

#endif // EVALPWUE_H_INCLUDED
