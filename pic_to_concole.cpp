#include <bits/stdc++.h>
#include <windows.h>
#include "pixel.h"
using namespace std;

int main()
{
    photo p("un.png", "  ");
    p.ps.compression(30, 30);
    for(auto &i : p.ps)
    {
        for(auto &j : i)j.print(true);
        cout << '\n';
    }
    
}