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
    beatSound(Resource(U"resource/forSystem/beats.mp3")),
    hitSound(Resource(U"resource/forSystem/hitSound.wav")),
    combo(0),
    point(0),
    isDrawable(false){
        ClearPrint();
        LaneBG::create();
        
        m_file.create(getData().scoreFile.toUTF32(), static_cast<score::Difficulty>(getData().currentDiff));
        
        if(m_file.getLastError().isError()){
            Print << U"譜面読み込み失敗:" << m_file.getLastError().getMessage();
            Print << U"エラー箇所:" << m_file.getReader().getLastError().getMessage() <<  U" 行数:" << m_file.getReader().getCurrentLine();
        }else{
            getData().resultSongInfo.push_back(m_file.getHeader());
            m_score.setFromFile(m_file, getData().speed / 10);
            pointEachNote = static_cast<double>(gameinfo::maxPoint) / static_cast<double>(m_file.getNumofNotes() + m_file.getNumofHolds());
            judger.create(score::numofLanes, m_file.getNotes());
            measureLength = m_file.getBar().at(1).time.sec;
            delay = measureLength * 2 + m_score.getWakeUpTime();
            beatLength = 60 / m_file.getTempo(0);
            time.addEvent(U"Beat1", SecondsF(beatLength + measureLength));
            time.addEvent(U"Beat2", SecondsF(beatLength * 2 + measureLength));
            time.addEvent(U"Beat3", SecondsF(beatLength * 3 + measureLength));
            time.addEvent(U"Beat4", SecondsF(beatLength * 4 + measureLength));
            time.addEvent(U"Draw", SecondsF(delay - m_score.getWakeUpTime()));
            time.addEvent(U"Start", SecondsF(delay));
            time.addEvent(U"End", SecondsF(m_song.lengthSec() + delay + measureLength));
        }
        
        Image buf;
        writeShineImage(buf);
        shine = Texture(buf);
        
        m_song.setVolume(0.5);
        
        time.start();
    }
    
    void Play::update() {
        time.update();
        
        if (time.onTriggered(U"Draw")) {
            isDrawable = true;
        }
        
        if (time.onTriggered(U"Start")) {
            m_song.play();
        }
        
        results =
        judger.input(0, KeyD.pressed())
        .input(1, KeyF.pressed())
        .input(2, KeyJ.pressed())
        .input(3, KeyK.pressed())
        .judge(time.sF() - delay);
        
        for (const auto &r : results) {
            Point effectPos(leftEnd + (interval * r->lane) + (interval / 2), laneEnd);
            float remainSec = 60 / m_file.getTempo(time.sF() - delay);
            bool shineEffect = true;
            
            if (r->type == score::NoteType::HOLD) {
                shineEffect = false;
            }
            
            switch (r->result.getJudge()) {
                case musicgame::JudgeState::BEST:
                    decision.criticalCount++;
                    point += pointEachNote;
                    decisionEffect.add<CriticalHitEffect>(shine, effectPos, remainSec, shineEffect);
                    break;
                case musicgame::JudgeState::BETTER:
                    decision.correctCount++;
                    point += pointEachNote * 0.8;
                    decisionEffect.add<CorrectHitEffect>(shine, effectPos, remainSec, shineEffect);
                    break;
                case musicgame::JudgeState::GOOD:
                case musicgame::JudgeState::NOTBAD:
                    decision.niceCount++;
                    point += pointEachNote * 0.6;
                    decisionEffect.add<NiceHitEffect>(shine, effectPos, remainSec, shineEffect);
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
            }
            
            if(r->result.getHoldState() != musicgame::JudgeState::HOLDCONTINUE) {
                m_score.deleteJudgedNote(r->lane, r->indexInLane);
            }
            
        }
        
        for (auto l : step(4)) {
            Point effectPos(leftEnd + (interval * l) + (interval / 2), laneEnd);
            float remainSec = 60 / m_file.getTempo(time.sF() - delay);
            
            if (judger.getJudgingHoldNote(l)) {
                decisionEffect.add<CriticalHitEffect>(shine, effectPos, remainSec, false);
            }
        }
        
        m_score.update(time.sF() - delay);
        
        if (time.onTriggered(U"End")) {
            getData().decisionCount.push_back(decision);
            getData().resultScore.push_back(point);
            changeScene(SceneName::RESULT, 2000);
        };
    }
    
    void Play::draw() const {
        LaneBG::getInstance().draw();
        
        if (time.onTriggered(U"Beat1") || time.onTriggered(U"Beat2") || time.onTriggered(U"Beat3") || time.onTriggered(U"Beat4")) {
            beatSound.playOneShot(0.8);
        }
        
        for (const auto &r : results) {
            Print << U"Lane: " << r->lane
            << U"   " << r->result.getJudgeMsg();
            
            if (!r->result.isMiss()) {
                hitSound.playOneShot(getData().decisionVolume / 10);
            }
        }
        
        if(isDrawable){
            m_score.draw();
        }
        
        FontAsset(U"comboFont")(combo).drawAt(Window::Width()/2, 700);
        
        decisionEffect.update();
    }

}
