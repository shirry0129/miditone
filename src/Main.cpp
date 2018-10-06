
# include <Siv3D.hpp> // OpenSiv3D v0.2.8
#include"UI/Lane.hpp"
#include"UI/Note.hpp"
#include "UI/Score.hpp"
#include "System/ScoreManager.hpp"
#include "System/EventTimer.hpp"

void Main() {
    Window::Resize(1920, 1080);
    Window::SetTitle(U"MusicGame");
    Graphics::SetBackground(Palette::Yellow);

    ui::LaneBG::create();
    ui::Score testScore;
    double measureLength;
    auto score = Dialog::OpenFile({{U"譜面データ(*.txt)", {U"txt"}}});
    
    score::ScoreManager testFile(score.value().narrow().c_str(), score::Difficulty::HARD);
    
    if(testFile.getLastError().isError()){
        Print << U"譜面読み込み失敗:" << Unicode::Widen(testFile.getLastError().getMessage());
    }else{
        measureLength = testFile.getBar()[1].time.sec;
        
        testScore.setFromFile(testFile.getNotes(), 0.8);
    }

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
