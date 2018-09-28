
# include <Siv3D.hpp> // OpenSiv3D v0.2.8
#include"UI/Lane.hpp"
#include"UI/Note.hpp"

void Main() {
    Window::Resize(1920, 1080);
    Window::SetTitle(U"MusicGame");
    Graphics::SetBackground(Palette::Yellow);

    ui::LaneBG::create();
    
    ui::Note testNote(1, 1);
    ui::Note testHold(2, 1, 2);
    
    Stopwatch count(true);
    
    while (System::Update()) {
        ui::LaneBG::getInstance().draw();
        testNote.update(count.sF(), 1);
        testHold.update(count.sF(), 1);
        testNote.draw();
        testHold.draw();
        
        if(count.s() > 4){
            count.restart();
        }
    }
}
