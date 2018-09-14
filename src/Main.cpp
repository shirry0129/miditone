
# include <Siv3D.hpp> // OpenSiv3D v0.2.8

namespace Lane_t {
    Vec2 interval(390, 0);
    Vec2 laneBegin(1920/2, -180);
    Vec2 leftEnd(180, 1000);
    Vec2 rightEnd = leftEnd + interval * 4;
    
    Vec2 outerGuide = Line(laneBegin, leftEnd).vector();
    Vec2 insideGuide = Line(laneBegin, leftEnd + interval).vector();
    
    const Line underLine(0,1000,1920,1000);
}

namespace Window_t {
    int edge = 0;
}

void Main() {
    Window::Resize(1920, 1080);
    Window::SetTitle(U"MusicGame");
    Graphics::SetBackground(Palette::Yellow);
    
    Font score(50);
    
    int y;
    
    while (System::Update()) {
        // draw background of lane
        Triangle(Lane_t::laneBegin, Lane_t::leftEnd, Lane_t::rightEnd).draw(Color(65, 65, 65));
        
        // draw lane separator
        for (auto i:step(5)) {
            Line(Lane_t::laneBegin, Lane_t::leftEnd + Lane_t::interval * i).draw((i == 0 || i == 4) ? 8 : 2, Palette::Orange);
        }
        Lane_t::underLine.draw(8, Palette::Orange);
        
    }
}
