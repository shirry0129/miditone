
# include <Siv3D.hpp> // OpenSiv3D v0.2.8
#include"Lane.hpp"
#include"Note.hpp"

void Main() {
    Window::Resize(1920, 1080);
    Window::SetTitle(U"MusicGame");
    Graphics::SetBackground(Palette::Yellow);

    Lane::LaneBG::create();
    
    HitNote testNote(1, 1);
    HoldNote testHold(2, 1, 2);
    
    Stopwatch count(true);
    
    while (System::Update()) {
        Lane::LaneBG::getInstance().draw();
        testNote.update(count.sF(), 1);
        testHold.update(count.sF(), 1);
        testNote.draw();
        testHold.draw();
        
        if(count.s() > 4){
            count.restart();
        }
    }
}
