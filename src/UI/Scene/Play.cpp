//
//  Play.cpp
//  MusicGame
//
//  Created by Shimizu Yuta on 2018/10/08.
//

#include "Play.hpp"
#include "../../GameInfo.hpp"
#include "../JudgeEffect.hpp"
#include "../JudgeStrEffect.hpp"
#include "../../System/JudgeFunc.hpp"

namespace ui{

    Play::Play(const InitData& init):
    IScene(init),
    m_song(getData().musicFile),
    beatSound(Resource(U"resource/forSystem/beats.mp3")),
    hitSound(Resource(U"resource/forSystem/hitSound.wav")),
    combo(0),
    point(0),
    maxWidth(270),
    isDrawable(false){
        musicgame::TimingJudge::beg_judge_func_t beg;
        musicgame::TimingJudge::end_judge_func_t end;
        musicgame::TimingJudge::missed_judge_func_t miss;
        getData().trackCount++;
        LaneBG::create();
        
        switch (getData().currentDiff) {
            case 0:
                beg = musicgame::judgefunc::begJudgeForEasy;
                end = musicgame::judgefunc::endJudgeForEasy;
                miss = musicgame::judgefunc::missedJudgeForEasy;
                break;
            case 1:
                beg = musicgame::judgefunc::begJudgeForNormal;
                end = musicgame::judgefunc::endJudgeForNormal;
                miss = musicgame::judgefunc::missedJudgeForNormal;
                break;
            case 2:
                beg = musicgame::judgefunc::begJudgeForHard;
                end = musicgame::judgefunc::endJudgeForHard;
                miss = musicgame::judgefunc::missedJudgeForHard;
            default:
                break;
        }
        
        m_file.create(getData().scoreFile.toUTF32(), static_cast<score::Difficulty>(getData().currentDiff));
        
        if(m_file.getLastError().isError()){
            Print << U"譜面読み込み失敗:" << m_file.getLastError().getMessage();
            Print << U"エラー箇所:" << m_file.getReader().getLastError().getMessage() <<  U" 行数:" << m_file.getReader().getCurrentLine();
        }else{
            getData().resultSongInfo.push_back(m_file.getHeader());
            m_score.setFromFile(m_file, getData().speed / 10);
            pointEachNote = static_cast<double>(gameinfo::maxPoint) / static_cast<double>(m_file.getNumofNotes() + m_file.getNumofHolds());
            judger.create(score::numofLanes, m_file.getNotes(), beg, end, miss);
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
            albumArt = Texture(U"../Score/albumArt/{}.png"_fmt(m_file.getHeader().id));
        }
        
        Image buf;
        writeShineImage(buf);
        shine = Texture(buf);
        
        m_song.setVolume(0.5);
        
        time.start();
    }
    
    void Play::compressedDisplay(const s3d::Vec2 &pos, const s3d::Font &assetInfo, const s3d::String &string) const {
        auto _region = assetInfo(string).region();
        
        if (_region.w > maxWidth) {
            Vec2 penPos(pos);
            const double charWidthLate = static_cast<double>(maxWidth) / _region.w;
            for (const auto &c : assetInfo(string)) {
                c.texture.scaled(charWidthLate, 1).draw(penPos + c.offset, Color(U"#002d45"));
                penPos.x += c.xAdvance * charWidthLate;
            }
        }else{
            assetInfo(string).draw(pos, Color(U"#002d45"));
        }
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
                    decisionEffect.add<CriticalStrEffect>(FontAsset(U"effectFont"), effectPos - Vec2(0, 100), remainSec);
                    break;
                case musicgame::JudgeState::BETTER:
                    decision.correctCount++;
                    point += pointEachNote * 0.8;
                    decisionEffect.add<CorrectHitEffect>(shine, effectPos, remainSec, shineEffect);
                    decisionEffect.add<CorrectStrEffect>(FontAsset(U"effectFont"), effectPos - Vec2(0, 100), remainSec);
                    break;
                case musicgame::JudgeState::GOOD:
                case musicgame::JudgeState::NOTBAD:
                    decision.niceCount++;
                    point += pointEachNote * 0.6;
                    decisionEffect.add<NiceHitEffect>(shine, effectPos, remainSec, shineEffect);
                    decisionEffect.add<NiceStrEffect>(FontAsset(U"effectFont"), effectPos - Vec2(0, 100), remainSec);
                    break;
                default:
                    break;
            }
            
            if (r->result.isMiss()) {
                combo = 0;
                decision.missCount++;
                m_score.deleteJudgedNote(r->lane, r->indexInLane);
                decisionEffect.add<MissStrEffect>(FontAsset(U"effectFont"), effectPos - Vec2(0, 100), remainSec);
            }else{
                combo++;
                if (combo >= decision.combo) {
                    decision.combo++;
                }
            }
            
            if(r->result.getHoldState() == musicgame::JudgeState::HOLDCONTINUE) {
                m_score.adjustJudgingNote(r->lane, r->indexInLane);
            }else{
                m_score.deleteJudgedNote(r->lane, r->indexInLane);
            }
            
            if (r->index == m_file.getNumofNotes()  - 1) {
                if (decision.criticalCount == m_file.getNumofNotes() + m_file.getNumofHolds()) {
                    point = gameinfo::maxPoint;
                }
            }
            
        }
        
        for (auto l : step(4)) {
            Point effectPos(leftEnd + (interval * l) + (interval / 2), laneEnd);
            float remainSec = 60 / m_file.getTempo(time.sF() - delay);
            
            if (judger.getJudgingHoldNote(l)) {
                decisionEffect.add<CriticalHitEffect>(shine, effectPos, remainSec, false);
                if (System::FrameCount() % 8 == 0) {
                    decisionEffect.add<CriticalStrEffect>(FontAsset(U"effectFont"), effectPos - Vec2(0, 100), remainSec);
                }
            }
        }
        
        m_score.update(time.sF() - delay);
        
        if (time.onTriggered(U"End")) {
            getData().decisionCount.push_back(decision);
            getData().resultScore.push_back(point);
            changeScene(SceneName::RESULT, gameinfo::fadeTime);
        };
    }
    
    void Play::draw() const {
        TextureAsset(U"play").drawAt(::gameinfo::originalScreenCenter);
        TextureAsset(U"track").draw(0, 0, Color(100, 201, 235));
        FontAsset(U"trackFont")(getData().trackCount).drawAt(273, 66, Color(U"#1e3333"));
        drawSongInfo({0, 130});
        drawScore({::gameinfo::originalResolution.x, 0});
        
        LaneBG::getInstance().draw();
        
        if (time.onTriggered(U"Beat1") || time.onTriggered(U"Beat2") || time.onTriggered(U"Beat3") || time.onTriggered(U"Beat4")) {
            beatSound.playOneShot();
        }
        
        for (const auto &r : results) {
            if (!r->result.isMiss()) {
                hitSound.playOneShot(getData().decisionVolume / 10);
            }
        }
        
        if(isDrawable){
            m_score.draw();
        }
        
        FontAsset(U"comboFont")(combo).drawAt(::gameinfo::originalResolution.x/2, 700);
        
        decisionEffect.update();
    }
    
    void Play::drawSongInfo(const s3d::Vec2 &tlPos) const {
        String diff;
        Color diffColor;
        
        switch (getData().currentDiff) {
            case 0:
                diff = U"EASY";
                diffColor = gameinfo::easy;
                break;
            case 1:
                diff = U"NORMAL";
                diffColor = gameinfo::normal;
                break;
            case 2:
                diff = U"HARD";
                diffColor = gameinfo::hard;
                break;
        }
        
        TextureAsset(U"song").draw(tlPos);
        albumArt.resized(192).draw(tlPos + Vec2(47, 60));
        FontAsset(U"diffInfo")(diff).draw(tlPos + Vec2(248, 65), diffColor);
        compressedDisplay(tlPos + Vec2(256, 132), FontAsset(U"songInfo"), m_file.getHeader().title);
        compressedDisplay(tlPos + Vec2(256, 209), FontAsset(U"artistInfo"), m_file.getHeader().artist);
    }
    
    void Play::drawScore(const s3d::Vec2 &trPos) const {
        Transformer2D t(Mat3x2::Scale(1.1, trPos));
        TextureAsset(U"score").draw(Arg::topRight(trPos));
        FontAsset(U"scoreFont")(U"{:0>7.0f}"_fmt(point)).draw(trPos - Vec2(307, -61), Color(U"#c4effd"));
        FontAsset(U"countFont")(decision.criticalCount).draw(trPos - Vec2(459, -151), Color(U"#c4effd"));
        FontAsset(U"countFont")(decision.correctCount).draw(trPos - Vec2(302, -151), Color(U"#c4effd"));
        FontAsset(U"countFont")(decision.niceCount).draw(trPos - Vec2(185, -151), Color(U"#c4effd"));
        FontAsset(U"countFont")(decision.missCount).draw(trPos - Vec2(81, -151), Color(U"#c4effd"));
    }
    

}
