
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
//    Graphics::SetTargetFrameRateHz(120);
//    Graphics::SetFullScreen(true, {1280, 720});

    ui::LaneBG::create();
    
    score::ScoreManager testFile("../resource/427_1001.txt", score::Difficulty::HARD);
    double measureLength = testFile.getBar()[1].time.sec;

    ui::Score testScore(testFile.getNotes(), 1.5);

    const Audio testAudio(U"../resource/larva.mp3");

//    testAudio.play();
    
//    Stopwatch time(true);
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
