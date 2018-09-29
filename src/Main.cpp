
# include <Siv3D.hpp> // OpenSiv3D v0.2.8
#include"UI/Lane.hpp"
#include"UI/Note.hpp"
#include "System/ScoreManager.hpp"
#include "UI/Score.hpp"

void Main() {
    Window::Resize(1920, 1080);
    Window::SetTitle(U"MusicGame");
    Graphics::SetBackground(Palette::Yellow);

    ui::LaneBG::create();
    
    score::ScoreManager testFile("../resource/438_0905.txt", score::Difficulty::EASY);
    
    ui::Score testScore(testFile.getNotes(), 1);
    
    Stopwatch count(true);
    
    while (System::Update()) {
        ui::LaneBG::getInstance().draw();
        
        testScore.update(count.sF());
        testScore.draw();
        
    }
}
