
# include <Siv3D.hpp> // OpenSiv3D v0.2.8
#include"Lane.hpp"
#include"Notes.hpp"

void Main() {
    Window::Resize(1920, 1080);
    Window::SetTitle(U"MusicGame");
    Graphics::SetBackground(Palette::Yellow);

    Lane::LaneBG::create();
    
    Notes testNote(1,0);
    
    while (System::Update()) {
        Lane::LaneBG::getInstance().draw();
        testNote.update(0);
        testNote.draw();
    }
}
