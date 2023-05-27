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

pll cuboid::sliceK(ll k)
{
    if(k == 1)
    {
        return {x, y};
    }
    if(k == 2)
    {
        return  {x, z};
    }
    //k == 3
    return {y, z};
}

void cuboid::addSlice(ll k)
{
    if(k == 1)
    {
        z++;
    }
    else if(k == 2)
    {
        y++;
    }
    else
    {
        x++;
    }
    orderEdges();
}

void cuboid::cutSlice(pll slice)
{
    if(slice == sliceK(1))
    {
        z--;
    }
    else if(slice == sliceK(2))
    {
        y--;
    }
    else
    {
        x--;
    }
    orderEdges();
}

tlll cuboid::getLengths()
{
    return {x, y, z};
}
