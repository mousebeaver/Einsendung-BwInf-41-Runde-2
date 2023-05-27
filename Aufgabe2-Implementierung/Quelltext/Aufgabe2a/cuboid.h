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
        pll cutBiggestSlice(set<pll>& slices); //Schneidet groesste Seitenflaeche ab, die auch schon in der Liste ist. Liefert abgeschnitte Flaeche oder NOPAIR zurueck
        ll longestEdge(); //Liefert laengste Seitenlaenge zurueck
        ll largestArea(); //Liefert maximalen Flaecheninhalt einer Seitenflaeche zurueck
};

#endif // CUBOID_H
