#ifndef CUBOID_H
#define CUBOID_H

#include "definitions.h"

class cuboid //Klasse fuer die Quader
{
    private:
        ll x, y, z; //Seitenlaengen

    public:
        cuboid(ll w, ll b, ll h); //Konstruktor, nimmt Seitenlaengen entgegen
        void orderEdges(); //Vertausche Seitenlaengen zu x <= y <= z
        pll sliceK(ll k); //Liefert Seitenflaeche zurueck, die im k-ten Fall des Algorithmus abgeschnitten werden kann
        void addSlice(ll k); //Fuegt k-te Seitenflaeche hinzu
        void cutSlice(pll slice); //Schneidet Scheibe ab (die auch wirklich einer Seitenflaeche entsprechen muss)
        tlll getLengths(); //Liefert die Seitenlaengen des Quaders zurueck
};

#endif // CUBOID_H
