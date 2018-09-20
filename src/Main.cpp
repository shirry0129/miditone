
# include <Siv3D.hpp> // OpenSiv3D v0.2.8
#include"Lane.hpp"
#include"Note.hpp"

void Main() {
    Window::Resize(1920, 1080);
    Window::SetTitle(U"MusicGame");
    Graphics::SetBackground(Palette::Yellow);

    Lane::LaneBG::create();
    
    Note testNote(1, 5);
    
    float i = 0;
    
    while (System::Update()) {
        Lane::LaneBG::getInstance().draw();
        testNote.update(i, 0.3);
        testNote.draw();
        
        i += 0.01;
    }
}
