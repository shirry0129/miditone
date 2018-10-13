//
//  Play.cpp
//  MusicGame
//
//  Created by Shimizu Yuta on 2018/10/08.
//

#include "Play.hpp"
#include "../../GameInfo.hpp"

namespace ui{

    Play::Play(const InitData& init):
    IScene(init),
    m_song(Dialog::OpenAudio()),
    comboCount(100),
    hitSound(Resource(U"resource/hitSound.mp3")){
        ClearPrint();
        LaneBG::create();
        
        auto scoreFile = Dialog::OpenFile(gameinfo::scoreData);
        
        if(!scoreFile){
            scoreFile = Optional<String>(U"not a path");
        }
        
        m_file.create(scoreFile.value().narrow().c_str(), static_cast<score::Difficulty>(getData().currentDiff));
        
        if(m_file.getLastError().isError()){
            Print << U"譜面読み込み失敗:" << Unicode::Widen(m_file.getLastError().getMessage());
            Print << U"エラー箇所:" << Unicode::Widen(m_file.getReader().getLastError().getMessage()) <<  U" 行数:" << m_file.getReader().getCurrentLine();
            Print << U"ファイルパス:" << scoreFile.value();
        }else{
            m_score.setFromFile(m_file.getNotes(), getData().speed / 10);
            judger.create(score::numofLanes, m_file.getNotes());
            measureLength = m_file.getBar().at(1).time.sec;
            time.addEvent(U"Start", SecondsF(measureLength));
            time.addEvent(U"End", SecondsF(m_song.lengthSec() + measureLength));
        }
        
        hitSound.setVolume(getData().decisionVolume / 100);
        
        time.start();
    }
    
    void Play::update() {
        time.update();
        if (time.onTriggered(U"Start")) {
            m_song.play();
        }
        
        results =
        judger.input(0, KeyD.pressed())
        .input(1, KeyF.pressed())
        .input(2, KeyJ.pressed())
        .input(3, KeyK.pressed())
        .judge(time.sF() - measureLength);
        
        for (const auto &r : results) {
            if (r->result.isMiss()) {
                combo = 0;
                m_score.deleteJudgedNote(r->lane, r->indexInLane);
            }else if (r->result.getHoldState() != musicgame::JudgeState::HOLDCONTINUE){
                combo++;
                m_score.deleteJudgedNote(r->lane, r->indexInLane);
            }
        }
        
        m_score.update(time.sF() - measureLength);
        
        if (time.onTriggered(U"End")) {
            changeScene(SceneName::RESULT, 2000);
        };
    }
    
    void Play::draw() const {
        //learPrint();
        LaneBG::getInstance().draw();
        
        comboCount(combo).drawAt(Window::Width()/2, 800);
        
        for (const auto &r : results) {
            Logger << U"lane: " << r->lane
            << U"   " << Unicode::Widen(r->result.getMessage())
            << U"   error: " << r->error << U"s";
            
            Print << U"Lane: " << r->lane
            << U"   " << Unicode::Widen(r->result.getJudgeMsg());
            
            if (!r->result.isMiss() && r->result.getHoldState() != musicgame::JudgeState::HOLDFINISHED) {
                hitSound.playOneShot();
            }
        }
        
        if(m_song.isPlaying()){
            m_score.draw();
        }
    }

}
