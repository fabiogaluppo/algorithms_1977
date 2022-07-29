//Sample by Fabio Galuppo - http://member.acm.org/~fabiogaluppo - fabiogaluppo@acm.org
//July 2022
//This file is licensed to you under the MIT license

//cl /Fo.\obj\ /std:c++17 /EHsc /O2 connect_the_dots.cpp /link /out:connect_the_dots.exe

#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

#include <cstdint>
#include <vector>
#include <array>
#include <limits>
#include <algorithm>
#include <iostream>

using dots_type = std::vector<std::array<std::int32_t, 2>>;

class ConnectTheDots : public olc::PixelGameEngine
{
    int step = 10;
    std::size_t draw_dots = 2;
    dots_type dots;

public:
    ConnectTheDots(dots_type&& dots, const char* app_name = nullptr) 
        : dots(dots)
    {
        sAppName = app_name ? app_name : "Connect The Dots";
    }

public:
    bool OnUserCreate() override
    {
        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override
    {
        if (GetKey(olc::Key::ESCAPE).bReleased)
            return false;
        if (GetKey(olc::Key::UP).bReleased)
            step = std::min(step + 1, 50);
        else if (GetKey(olc::Key::DOWN).bReleased)
            step = std::max(step - 1, 1);
        else if (GetKey(olc::Key::SPACE).bReleased)
            draw_dots = dots.size();
        else if (GetKey(olc::Key::BACK).bReleased)
            draw_dots = 2;

        Clear(olc::WHITE);
        
        for (int i = 1, j = i - 1; i != std::min(draw_dots, dots.size()); ++i, ++j)
        {
            DrawLine(dots[i][0], dots[i][1], dots[j][0], dots[j][1], olc::BLUE);
        }
        if (draw_dots < dots.size())
            draw_dots += step;
        
        return true;
    }
};

int main(int argc, const char** argv)
{
    dots_type dots; 
    std::int32_t xMin = std::numeric_limits<std::int32_t>::max(), yMin = xMin,
                 xMax = std::numeric_limits<std::int32_t>::min(), yMax = xMax;
    std::int32_t x, y;
    while (std::cin >> x >> y)
    {
        xMin = std::min(x, xMin); xMax = std::max(x, xMax);
        yMin = std::min(y, yMin); yMax = std::max(y, yMax);
        dots.emplace_back(std::array<std::int32_t, 2>({ x, y }));
    }
    
    const std::int32_t WIDTH = 640;
    const std::int32_t HEIGHT = 640;
    
    for (auto& dot : dots)
    {
        dot[0] = WIDTH - static_cast<std::int32_t>(WIDTH * (dot[0] / static_cast<double>(xMax)));
        dot[1] = HEIGHT - static_cast<std::int32_t>(HEIGHT * (dot[1] / static_cast<double>(yMax)));        
    }
    
    std::reverse(dots.begin(), dots.end());
    dots.emplace_back(dots[0]);

    ConnectTheDots app(std::move(dots), argc > 1 ? argv[1] : nullptr);
    if (app.Construct(WIDTH, HEIGHT, 1, 1))
        app.Start();

    return 0;
}
