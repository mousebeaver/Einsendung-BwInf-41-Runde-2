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

ll cuboid::volume()
{
    return x*y*z;
}

pll cuboid::sliceK(ll k)
{
    if(k == 1)
    {
        return {y, z};
    }
    if(k == 2)
    {
        return  {x, z};
    }
    //k == 3
    return {x, y};
}

void cuboid::addSlice(pll slice)
{
    if(slice == sliceK(1))
    {
        x++;
    }
    else if(slice == sliceK(2))
    {
        y++;
    }
    else if(slice == sliceK(3))
    {
        z++;
    }
    orderEdges();
}

void cuboid::cutSlice(ll k)
{
    if(k == 1)
    {
        x--;
    }
    else if(k == 2)
    {
        y--;
    }
    else if(k == 3)
    {
        z--;
    }
    orderEdges();
}
