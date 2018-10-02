
# include <Siv3D.hpp> // OpenSiv3D v0.2.8
#include"UI/Lane.hpp"
#include"UI/Note.hpp"
#include "System/ScoreManager.hpp"
#include "UI/Score.hpp"
#include "System/EventTimer.hpp"

void Main() {
    Window::Resize(1920, 1080);
    Window::SetTitle(U"MusicGame");
    Graphics::SetBackground(Palette::Yellow);

    ui::LaneBG::create();
    
    auto score = Dialog::OpenFile({{U"譜面データ(*.txt)", {U"txt"}}});
    
    score::ScoreManager testFile(score.value().narrow().c_str(), score::Difficulty::HARD);
    double measureLength = testFile.getBar()[1].time.sec;

    ui::Score testScore(testFile.getNotes(), 0.8);

    const Audio testAudio = Dialog::OpenAudio();

    EventTimer time;
    time.addEvent(U"Start", SecondsF(measureLength));
    
    time.start();
    
    while (System::Update()) {
        time.update();
        ui::LaneBG::getInstance().draw();
        
        if (time.onTriggered(U"Start")) {
            testAudio.play();
        }
        
        testScore.update(time.sF() - measureLength);
        testScore.draw();
    }
}
