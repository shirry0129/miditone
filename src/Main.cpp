
# include <Siv3D.hpp> // OpenSiv3D v0.2.8
#include"Lane.h"

void Main() {
    Window::Resize(1920, 1080);
    Window::SetTitle(U"MusicGame");
    Graphics::SetBackground(Palette::Yellow);

    Lane::create();
    
    while (System::Update()) {
        Lane::getInstance().draw();
    }
}
