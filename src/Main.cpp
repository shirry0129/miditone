
# include <Siv3D.hpp> // OpenSiv3D v0.2.8
#include <vector>

namespace Lane_t {
    int interval = 390;
    int top = 0;
    int bottom = 1000;
    int leftmost = 180;
    int rightmost = leftmost + interval * 4;
    
    Vec2 laneBegin(1920/2, -180);
}

namespace Window_t {
    int edge = 0;
}

void Main() {
    Window::Resize(1920, 1080);
    Window::SetTitle(U"MusicGame");
    Graphics::SetBackground(Palette::Yellow);
    
    const Line underLine(Vec2(Window_t::edge, Lane_t::bottom), Vec2(Window::Width(), Lane_t::bottom));
    Font score(50);
    
    while (System::Update()) {
        // draw background of lane
        Triangle(Lane_t::laneBegin, Vec2(Lane_t::leftmost, Lane_t::bottom), Vec2(Lane_t::rightmost, Lane_t::bottom)).draw(Color(65, 65, 65));
        
        // draw lane separator
        for (int i = 0; i < 5 ; i++) {
            Line(Lane_t::laneBegin, Vec2(Lane_t::leftmost, Lane_t::bottom) + Vec2(Lane_t::interval * i, 0)).draw((i == 0 || i == 4) ? 8 : 2, Palette::Orange);
        }
        underLine.draw(8, Palette::Orange);
    }
}
