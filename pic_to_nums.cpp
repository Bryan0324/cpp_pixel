#include "pixel.h"
using namespace std;

int main()
{
    photo pic("un.png");
    pixels p = pic.ps;
    p.channel_to_one();
    p.compression(18, 18);
    p.inversion();
    for(auto &i : p)
    {
        for(auto &j : i)cout << j.color[0] << ' ';
        cout << '\n';
    }
}
