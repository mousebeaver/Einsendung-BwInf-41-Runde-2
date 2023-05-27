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
        ll volume(); //Liefert das Volumen des Quaders zurueck
        void cutSlice(ll k); //Schneidet k-t-groesste Scheibe ab
        void addSlice(pll slice); //Fuegt die Scheibe wieder hinzu
        pll sliceK(ll k); //Liefert k-t-groesste Scheibe zurueck
};

#endif // CUBOID_H
