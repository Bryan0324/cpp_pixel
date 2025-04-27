#include<bits/stdc++.h>
#include <windows.h>
using namespace std;
#define STB_IMAGE_IMPLEMENTATION
#include "./stb-master/stb_image.h"
typedef unsigned long long ull;



HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

void SetColor(int textColor = 7, int bgColor = 0)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole,
                            (bgColor << 4) | textColor);
}

vector<int> &operator+=(vector<int> &a, vector<int> &b)
{
    for(size_t i = 0; i < a.size(); i++)a[i] += b[i];
    return a;
}
vector<int> &operator/=(vector<int> &a, int b)
{
    for(auto &i : a)i /= b;
    return a;
}

struct pixel
{
    int channel = 1, bg_color = 0;
    vector<int> color;
    string sprite = "  ";
    pixel(vector<int> c = {0}, string spr = "  ", int bgc = 0)
    {
        sprite = spr;
        channel = c.size();
        bg_color = bgc;
        for (size_t i = 0; i < c.size(); i++)color.push_back(c[i]);
        return;
    }
    pixel channel_to_one()
    {
        switch (channel)
        {
        case 1:
            return *this;

        case 2:
            return pixel({(color[0]+color[1])/2});

        case 3:
        case 4:
            return pixel({(color[0]*19595 + color[1]*38469 +color[2]*7472) >>16});

        default:
            return pixel({color[0]});
        }
    }
    pixel inversion()
    {
        pixel p = *this;
        for(auto &i : p.color)
        {
            i = 255-i;
        }
        return p;
    }
    void print(bool reverse = false)
    {
        pixel c = channel_to_one();
        if(!reverse)SetColor(c.color[0], bg_color);
        else SetColor(255-c.color[0], bg_color);
        cout << sprite;
        SetColor();
        return;
    }
};

class pixels : public vector<vector<pixel>>
{
    public:
        pixels(vector<vector<pixel>> a = {})
        {
            this->resize(a.size(), {});
            for(size_t i = 0; i < a.size(); i++)
            {
                for (size_t j = 0; j < a[0].size(); j++)
                {
                    (*this)[i].push_back(a[i][j]);
                }
            }
            return;
        }
        void compression(size_t h, size_t w)
        {
            vector<vector<pixel>> n(h);
            for (int i = 0; i < h; i++)
            {
                for (int j = 0; j < w; j++)
                {
                    n[i].push_back((*this)[i*(this->size()/h)][j*(this[0].size()/w)]);
                    n[i].back().color = vector<int>(n[i].back().color.size(), 0);
                    int stepx = (this->size()/h), stepy = ((*this)[0].size()/w);
                    for (size_t x = i*stepx; x < this->size() && x < (i+1)*stepx; x++)
                    {
                        for (size_t y = j*stepy; y < (*this)[0].size() && y < (j+1)*stepy; y++)
                        {
                            n[i].back().color += (*this)[x][y].color;
                        }
                    }
                    n[i].back().color /= (min((int)(*this)[0].size(), (j+1)*stepy) - j*stepy)*(min((int)(*this).size(), (i+1)*stepx) - i*stepx);
                }
            }
            (*this) = pixels(n);
        }
        void channel_to_one()
        {
            for(auto &i : *this)
            {
                for(auto &j : i)
                {
                    j = j.channel_to_one();
                }
            }
            return;
        }
        void inversion()
        {
            for(auto &i : *this)
            {
                for(auto &j : i)
                {
                    j = j.inversion();
                }
            }
            return;
        }
};



class photo
{
    public:
        string filename;
        int x = 0, y = 0, channel = 3;
        stbi_uc* _file = nullptr;
        pixels ps;
        string sprite = "  ";
        photo(string s, string spr = "  ", int change_channel = 0)
        {
            sprite = spr;
            filename = s;
            _file = stbi_load(filename.c_str(), &x, &y, &channel, change_channel);
            if(change_channel != 0)channel = change_channel;
            for(int py = 0; py < y; py++)
            {
                ps.push_back({});
                for(int px = 0; px < x; px++)
                {
                    vector<int> tmp;
                    for(int c = 0; c < channel; c++)
                    {
                        tmp.push_back((int)*_file);
                        _file++;
                    }
                    ps.back().push_back(pixel(tmp, spr));
                }
            }
            return;
        }

};