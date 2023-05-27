#include "cuboid.h"

cuboid::cuboid(ll w, ll b, ll h)
{
    x = w;
    y = b;
    z = h;
    orderEdges();
}

void cuboid::orderEdges()
{
    //Sortiert die Seitenlaengen aufsteigend durch maximal drei Vertauschungen
    if(x > y)
    {
        swap(x, y);
    }
    if(y > z)
    {
        swap(y, z);
    }
    if(x > y)
    {
        swap(x, y);
    }
}

pll cuboid::cutBiggestSlice(set<pll>& slices)
{
    if(slices.count({y, z}) > 0)
    {
        x--;
        orderEdges();
        return {y, z};
    }
    if(slices.count({x, z}) > 0)
    {
        y--;
        orderEdges();
        return {x, z};
    }
    if(slices.count({x, y}) > 0)
    {
        z--;
        orderEdges();
        return {x, y};
    }
    return NOPAIR;
}

ll cuboid::longestEdge()
{
    return z;
}

ll cuboid::largestArea()
{
    return y*z;
}
