//
//  BaranceBoardPlay.cpp
//  empty
//
//  Created by Shimizu Yuta on 2019/10/20.
//

#ifdef MIDITONE_WIIBALANCEBOARD

#include "BalanceBoardPlay.hpp"
#include "../../GameInfo.hpp"
#include "../JudgeEffect.hpp"
#include "../JudgeStrEffect.hpp"
#include "../../System/JudgeFunc.hpp"

using namespace ui;

BalanceBoardPlay::BalanceBoardPlay(const InitData& init) :
IScene(init),
m_song(getData().currentMusic->musicPath),
#if defined(SIV3D_TARGET_MACOS)
beatSound(Resource(U"resource/forSystem/beats.mp3")),
hitSound(Resource(U"resource/forSystem/hitSound.wav")),
#elif defined(SIV3D_TARGET_WINDOWS)
beatSound(U"resource/forSystem/beats.mp3"),
hitSound(U"resource/forSystem/hitSound.wav"),
#endif
combo(0),
point(0),
maxWidth(270),
isDrawable(false),
decisionLine(gameinfo::originalScreenCenter, lineRadius),
laneAngle(4),
effectAngle(4){
    musicgame::TimingJudge::beg_judge_func_t beg;
    musicgame::TimingJudge::end_judge_func_t end;
    musicgame::TimingJudge::missed_judge_func_t miss;
    getData().trackCount++;
    
    laneAngle.at(0) = Math::Pi;
    laneAngle.at(1) = Math::HalfPi;
    laneAngle.at(2) = Math::HalfPi * 3;
    laneAngle.at(3) = 0;

    effectAngle.at(0) = 90_deg;
    effectAngle.at(1) = 0;
    effectAngle.at(2) = 180_deg;
    effectAngle.at(3) = -90_deg;
    
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
    
    m_file.create(getData().currentMusic->scorePath, static_cast<score::Difficulty>(getData().currentDiff));
    
    if(m_file.getLastError().isError()){
        Logger << U"譜面読み込み失敗:" << m_file.getLastError().getMessage();
        Logger << U"エラー箇所:" << m_file.getReader().getLastError().getMessage() << U"行数:" << m_file.getReader().getCurrentLine();
    } else {
        getData().resultSongInfo.emplace_back(*getData().currentMusic, m_file.getHeader().difficulty);
        m_score.setFromFile(m_file, getData().speed / 10, laneAngle);
        pointEachNote = static_cast<double>(gameinfo::maxPoint) /  static_cast<double>(m_file.getNumofNotes() + m_file.getNumofHolds());
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
        albumArt = Texture(getData().currentMusic->jacketPath);
    }
    
    Image buf;
    writeShineImage(buf);
    shine = Texture(buf);
    
    m_song.setVolume(0.5);
    
    time.start();
}

void BalanceBoardPlay::update(){
    time.update();
    
    if (time.onTriggered(U"Draw")) {
        isDrawable = true;
    }
    
    if (time.onTriggered(U"Start")) {
        m_song.play();
    }
    
    results = judger.input(0, gameinfo::balanceBoard.top_left().pressed())
    .input(1, gameinfo::balanceBoard.bottom_left().pressed())
    .input(2, gameinfo::balanceBoard.top_right().pressed())
    .input(3, gameinfo::balanceBoard.bottom_right().pressed())
    .judge(time.sF() - delay);
    
    double currentTime = time.sF() - delay;
    float remainSec = 60 / m_file.getTempo(currentTime < 0 ? 0 : currentTime);
    
    for (const auto &r : results) {
        const Vec2 offset(Math::Cos(laneAngle.at(r->lane)) * decisionLine.r, Math::Sin(laneAngle.at(r->lane)) * decisionLine.r);
        Vec2 effectPos(gameinfo::originalScreenCenter + offset);
        Vec2 effectStrPos(gameinfo::originalScreenCenter + offset - Vec2(0, 100));
        const bool shineEffect = (r->type == score::NoteType::HOLD) ? false : true;
        
        switch (r->result.getJudge()) {
            case musicgame::JudgeState::BEST:
                decision.criticalCount++;
                point += pointEachNote;
                decisionEffect.add<CriticalHitEffect>(shine, effectPos.asPoint(), remainSec, shineEffect);
                decisionEffect.add<CriticalStrEffect>(FontAsset(U"30_bold"), effectStrPos, remainSec, effectAngle.at(r->lane));
                break;
            case musicgame::JudgeState::BETTER:
                decision.correctCount++;
                point += pointEachNote * 0.8;
                decisionEffect.add<CorrectHitEffect>(shine, effectPos.asPoint(), remainSec, shineEffect);
                decisionEffect.add<CorrectStrEffect>(FontAsset(U"30_bold"), effectStrPos, remainSec, effectAngle.at(r->lane));
                break;
            case musicgame::JudgeState::GOOD:
            case musicgame::JudgeState::NOTBAD:
                decision.niceCount++;
                point += pointEachNote * 0.6;
                decisionEffect.add<NiceHitEffect>(shine, effectPos.asPoint(), remainSec, shineEffect);
                decisionEffect.add<NiceStrEffect>(FontAsset(U"30_bold"), effectStrPos, remainSec, effectAngle.at(r->lane));
                break;
            default:
                break;
        }
        
        if (r->result.isMiss()) {
            combo = 0;
            decision.missCount++;
            m_score.deleteJudgedNote(r->lane, r->indexInLane);
            decisionEffect.add<MissStrEffect>(FontAsset(U"30_bold"), effectStrPos, remainSec, effectAngle.at(r->lane));
        } else {
            combo++;
            if (combo >= decision.combo) {
                decision.combo++;
            }
        }
        
        if (r->result.getHoldState() == musicgame::JudgeState::HOLDCONTINUE) {
            m_score.adjustJudgingNote(r->lane, r->indexInLane);
        } else {
            m_score.deleteJudgedNote(r->lane, r->indexInLane);
        }
        
        if (r->index == m_file.getNumofNotes() - 1) {
            if (decision.criticalCount == m_file.getNumofNotes() + m_file.getNumofHolds()) {
                point = gameinfo::maxPoint;
            }
        }
    }
    
    for (const auto& l : step(4)) {
        const Vec2 offset(Math::Cos(laneAngle.at(l)) * decisionLine.r, Math::Sin(laneAngle.at(l)) * decisionLine.r);
        Vec2 effectPos(gameinfo::originalScreenCenter + offset);
        Vec2 effectStrPos(gameinfo::originalScreenCenter + offset - Vec2(0, 100));
        
        if (judger.getJudgingHoldNote(l)) {
            if (Scene::FrameCount() % 10 == 0) {
                decisionEffect.add<CriticalHitEffect>(shine, effectPos.asPoint(), remainSec, false);
                decisionEffect.add<CriticalStrEffect>(FontAsset(U"30_bold"), effectStrPos, remainSec, effectAngle.at(l));
            }
        }
    }
    
    m_score.update(time.sF() - delay);
    
    if (time.onTriggered(U"End") || KeyQ.down()) {
        getData().decisionCount.push_back(decision);
        getData().resultScore.push_back(point);
        changeScene(SceneName::RESULT, gameinfo::fadeTime);
    }
}

void BalanceBoardPlay::draw() const {
    TextureAsset(U"play").drawAt(gameinfo::originalScreenCenter);
    TextureAsset(U"track").draw(0, 0, Color(100, 201, 235));
    FontAsset(U"80_bold")(getData().trackCount).drawAt(273, 66, Color(U"#1e3333"));
    drawSongInfo({-10, 100});
    drawScore({gameinfo::originalResolution.x, 0});
    
    decisionLine.draw(Color(Palette::Darkslategray, 200)).drawFrame(0, 4, Color(Palette::Lightskyblue));
    
    if (time.onTriggered(U"Beat1") || time.onTriggered(U"Beat2") ||
        time.onTriggered(U"Beat3") || time.onTriggered(U"Beat4")) {
        beatSound.playOneShot();
    }
    
    for (const auto &r : results) {
        if (!r->result.isMiss()) {
            hitSound.playOneShot(getData().decisionVolume / 10);
        }
    }
    
    {
        Transformer2D rotate(Mat3x2::Rotate(45_deg, gameinfo::originalScreenCenter));
        
        if (isDrawable) {
            m_score.draw();
        }
    
        decisionEffect.update();
    }
    
    FontAsset(U"200")(combo).drawAt(gameinfo::originalScreenCenter);
}

void BalanceBoardPlay::drawSongInfo(const s3d::Vec2 &tlPos) const {
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
    albumArt.resized(192).drawAt(tlPos + Vec2(143, 156));
    FontAsset(U"45_bold")(diff).draw(tlPos + Vec2(248, 65), diffColor);
    compressedDisplay(tlPos + Vec2(256, 132), FontAsset(U"30_bold"), m_file.getHeader().title);
    compressedDisplay(tlPos + Vec2(256, 209), FontAsset(U"15"), m_file.getHeader().artist);
}

void BalanceBoardPlay::drawScore(const s3d::Vec2 &trPos) const {
    TextureAsset(U"score").draw(Arg::topRight(trPos));
    FontAsset(U"45_bold")(Unicode::Widen(getData().user.name)).draw(Arg::topLeft = trPos - Vec2(541, -44), gameinfo::defaultFontColor);
    FontAsset(U"50_bold")(U"{:0>7.0f}"_fmt(point)).draw(Arg::leftCenter = trPos - Vec2(288, -157), Color(U"#c4effd"));
    FontAsset(U"15")(decision.criticalCount).draw(Arg::leftCenter = trPos - Vec2(442, -221), Color(U"#c4effd"));
    FontAsset(U"15")(decision.correctCount).draw(Arg::leftCenter = trPos - Vec2(286, -221), Color(U"#c4effd"));
    FontAsset(U"15")(decision.niceCount).draw(Arg::leftCenter = trPos - Vec2(166, -221), Color(U"#c4effd"));
    FontAsset(U"15")(decision.missCount).draw(Arg::leftCenter = trPos - Vec2(62, -221), Color(U"#c4effd"));
}

void BalanceBoardPlay::compressedDisplay(const s3d::Vec2 &pos, const s3d::Font &assetInfo, const s3d::String &string) const {
    auto _region = assetInfo(string).region();
    
    if (_region.w > maxWidth) {
        Vec2 penPos(pos);
        const double charWidthLate = static_cast<double>(maxWidth) / _region.w;
        for (const auto &c : assetInfo(string)) {
            c.texture.scaled(charWidthLate, 1).draw(penPos + c.offset, Color(U"#002d45"));
            penPos.x += c.xAdvance * charWidthLate;
        }
    } else {
        assetInfo(string).draw(pos, Color(U"#002d45"));
    }
}


#endif
