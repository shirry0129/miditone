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
    m_song(getData().musicFile),
    hitSound(Resource(U"resource/forSystem/hitSound.wav")),
    combo(0),
    point(0){
        ClearPrint();
        LaneBG::create();
        
        m_file.create(getData().scoreFile.toUTF32(), static_cast<score::Difficulty>(getData().currentDiff));
        
        if(m_file.getLastError().isError()){
            Print << U"譜面読み込み失敗:" << m_file.getLastError().getMessage();
            Print << U"エラー箇所:" << m_file.getReader().getLastError().getMessage() <<  U" 行数:" << m_file.getReader().getCurrentLine();
        }else{
            getData().resultSongInfo.push_back(m_file.getHeader());
            m_score.setFromFile(m_file.getNotes(), getData().speed / 10);
            pointEachNote = static_cast<double>(gameinfo::maxPoint) / static_cast<double>(m_file.getNumofNotes() + m_file.getNumofHolds());
            judger.create(score::numofLanes, m_file.getNotes());
            measureLength = m_file.getBar().at(1).time.sec;
            time.addEvent(U"Start", SecondsF(measureLength));
            time.addEvent(U"End", SecondsF(m_song.lengthSec() + measureLength));
        }
        
        Image buf;
        writeShineImage(buf);
        shine = Texture(buf);
        
        time.start();
    }
    
    void Play::update() {
        time.update();
        if (time.onTriggered(U"Start")) {
            m_song.play();
        }
        
        results =
        judger//.input(0, KeyD.pressed())
//        .input(1, KeyF.pressed())
//        .input(2, KeyJ.pressed())
//        .input(3, KeyK.pressed())
        .inputAuto(time.sF() - measureLength)
        .judge(time.sF() - measureLength);
        
        for (const auto &r : results) {
            Point effectPos(leftEnd + (interval * r->lane) + (interval / 2), laneEnd);
            float remainSec = 60 / m_file.getTempo(time.sF() - measureLength);
            
            switch (r->result.getJudge()) {
                case musicgame::JudgeState::BEST:
                    decision.criticalCount++;
                    point += pointEachNote;
                    decisionEffect.add<CriticalHitEffect>(shine, effectPos, remainSec);
                    break;
                case musicgame::JudgeState::BETTER:
                    decision.correctCount++;
                    point += pointEachNote * 0.8;
                    decisionEffect.add<CorrectHitEffect>(shine, effectPos, remainSec);
                    break;
                case musicgame::JudgeState::GOOD:
                case musicgame::JudgeState::NOTBAD:
                    decision.niceCount++;
                    point += pointEachNote * 0.6;
                    decisionEffect.add<NiceHitEffect>(shine, effectPos, remainSec);
                    break;
                default:
                    break;
            }
            
            if (r->result.isMiss()) {
                combo = 0;
                decision.missCount++;
                m_score.deleteJudgedNote(r->lane, r->indexInLane);
            }else{
                combo++;
                if (combo >= decision.combo) {
                    decision.combo++;
                }
                m_score.deleteJudgedNote(r->lane, r->indexInLane);
            }
        }
        
        m_score.update(time.sF() - measureLength);
        
        if (time.onTriggered(U"End")) {
            getData().decisionCount.push_back(decision);
            getData().resultScore.push_back(point);
            changeScene(SceneName::RESULT, 2000);
        };
    }
    
    void Play::draw() const {
        LaneBG::getInstance().draw();
        
        FontAsset(U"comboFont")(combo).drawAt(Window::Width()/2, 800);
        
        for (const auto &r : results) {
            Logger << U"lane: " << r->lane
            << U"   " << r->result.getMessage()
            << U"   error: " << r->error << U"s";
            
            Print << U"Lane: " << r->lane
            << U"   " << r->result.getJudgeMsg();
            
            if (!r->result.isMiss()) {
                hitSound.playOneShot(getData().decisionVolume / 10);
            }
        }
        
        if(m_song.isPlaying()){
            m_score.draw();
        }
        
        decisionEffect.update();
    }

}
