
# include <Siv3D.hpp> // OpenSiv3D v0.2.8
#include"UI/Lane.hpp"
#include"UI/Note.hpp"
#include "UI/Score.hpp"
#include "System/ScoreManager.hpp"
#include "System/EventTimer.hpp"
#include "System/TimingJudge.hpp"

void Main() {
    Window::Resize(1920, 1080);
    Window::SetTitle(U"MusicGame");
    Graphics::SetBackground(Palette::Yellow);

    ui::LaneBG::create();
    ui::Score testScore;
    double measureLength = 0.0;
    auto score = Dialog::OpenFile({{U"譜面データ(*.txt)", {U"txt"}}});
    
    if(!score){
        score = Optional<String>(U"not a path");
    }
    
    const Audio testAudio = Dialog::OpenAudio();

    EventTimer time;
    measureLength = 3;//testFile.getBar()[1].time.sec;
    time.addEvent(U"Start", SecondsF(measureLength));
    
    const Array<String> difficulty = {U"EASY", U"NORMAL", U"HARD"};
    size_t index = 1;
    double speed = 5;
    bool selectDiff = true;
    
    musicgame::TimingJudge judger;
    size_t combo = 0;
    Font comboCount(100);
    
    while (System::Update()) {
        
        SimpleGUI::RadioButtons(index, difficulty, Vec2(100,100), unspecified, selectDiff);
        SimpleGUI::Slider(U"スピード: {:.1f}"_fmt(speed), speed, 1, 10, Vec2(100, 250), 200, 120, selectDiff);
        if(SimpleGUI::Button(U"決定", Vec2(100, 300), unspecified, selectDiff)){
            selectDiff = false;
            
            score::ScoreManager testFile(score.value().narrow().c_str(), static_cast<score::Difficulty>(index));
            
            if(testFile.getLastError().isError()){
                Print << U"譜面読み込み失敗:" << Unicode::Widen(testFile.getLastError().getMessage());
                Print << U"エラー箇所:" << Unicode::Widen(testFile.getReader().getLastError().getMessage()) <<  U" 行数:" << testFile.getReader().getCurrentLine();
                Print << U"ファイルパス:" << score.value();
            }else{
                testScore.setFromFile(testFile.getNotes(), speed/10);
                judger.create(score::numofLanes, testFile.getNotes());
            }
            
            time.start();
        }
        
        time.update();
        ui::LaneBG::getInstance().draw();
        
        if (time.onTriggered(U"Start")) {
            testAudio.play();
        }
        
        musicgame::judge_results_t results =
        judger.input(0, KeyD.pressed())
        .input(1, KeyF.pressed())
        .input(2, KeyJ.pressed())
        .input(3, KeyK.pressed())
        .judge(time.sF() - measureLength);
        
        comboCount(combo).drawAt(Window::Width() / 2, 400);
        
        for(const auto &r : results){
            Logger << U"lane: " << r->lane
            << U"   " << Unicode::Widen(r->result.getMessage())
            << U"   error: " << r->error << U"s";
            
            Print << U"Lane: " << r->lane
            << U"   " << Unicode::Widen(r->result.getJudgeMsg());
            
            if(r->result.isMiss()){
                combo = 0;
            }else{
                combo++;
            }
        }
        
        testScore.update(time.sF() - measureLength);
        testScore.draw();
    }
}
