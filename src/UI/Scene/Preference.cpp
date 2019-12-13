//
//  Preference.cpp
//  MusicGame
//
//  Created by Shimizu Yuta on 2018/10/08.
//

#include "Preference.hpp"

namespace ui{
    
    PrefBox::PrefBox(const PrefItem _item, const String& content, const RectF& _entity, const Texture& _boxTex):
    IBox(_entity, _boxTex),
    m_content(content),
    m_item(_item){}
    
    void PrefBox::draw(const Vec2& moveWidth, bool isSelected) const {
        if (moveWidth.x == 0) {
            entity.movedBy(moveWidth)(design).draw();
        }else{
            entity.movedBy(moveWidth).scaled(0.8)(design).draw();
        }
    }
    
    void PrefBox::draw(const s3d::Vec2 &moveWidth, bool isSelected, double currentValue) const {
        double scale = 0.8;
        String value = ToString(currentValue);
        
        draw(moveWidth, isSelected);
        
        if (moveWidth.x == 0) {
            scale = 1;
            if (isSelected) {
                value.push_front(U'<');
                value.push_back(U'>');
            }
        }
        
        Transformer2D t(Mat3x2::Scale(scale, entity.center() + moveWidth));
        FontAsset(U"50_bold")(m_content).drawAt(entity.center() + moveWidth + Vec2(0, 150), gameinfo::defaultFontColor);
        FontAsset(U"50")(value).drawAt(entity.center() + moveWidth + Vec2(0, -100), gameinfo::defaultFontColor);
    }
    
    void PrefBox::draw(const s3d::Vec2 &moveWidth, bool isSelected, score::Difficulty currentDiff) const {
        String diff;
        Color diffColor;
        double scale = 0.8;
        
        switch (currentDiff) {
            case score::Difficulty::EASY:
                diff = U"EASY";
                diffColor = gameinfo::easy;
                break;
            case score::Difficulty::NORMAL:
                diff = U"NORMAL";
                diffColor = gameinfo::normal;
                break;
            case score::Difficulty::HARD:
                diff = U"HARD";
                diffColor = gameinfo::hard;
                break;
        }
        
        draw(moveWidth, isSelected);
        
        if (moveWidth.x == 0) {
            scale = 1;
            if (isSelected) {
                diff.push_front(U'<');
                diff.push_back(U'>');
            }
        }
        
        Transformer2D t(Mat3x2::Scale(scale, entity.center() + moveWidth));
        FontAsset(U"50_bold")(m_content).drawAt(entity.center() + moveWidth + Vec2(0, 150), gameinfo::defaultFontColor);
        FontAsset(U"50")(diff).drawAt(entity.center() + moveWidth + Vec2(0, -100), diffColor);
    }
    
    ui::PrefItem PrefBox::getEntry() const {
        return m_item;
    }
    
    
    
    
    
    Preference::Preference(const InitData &init):
    IScene(init),
    boxSize(400, 600),
    currentItem(0),
    adjustment(false),
    example(getData().currentMusic->musicPath, Arg::loop = true),
    defaultEntity(Arg::center(::gameinfo::originalScreenCenter), boxSize){
        prefItem.emplace_back(PrefItem::GAMESTART, U"GAMESTART", defaultEntity, TextureAsset(U"gameStart"));
        prefItem.emplace_back(PrefItem::DIFFICULTY, U"Difficulty", defaultEntity, TextureAsset(U"boxTemplate"));
        prefItem.emplace_back(PrefItem::SPEED, U"Speed", defaultEntity, TextureAsset(U"boxTemplate"));
        prefItem.emplace_back(PrefItem::DECISIONVOL, U"SE Volume", defaultEntity, TextureAsset(U"boxTemplate"));
        
        for (auto i : step(4)) {
            instructionBox.emplace_back(325.5 + 355 * i, 880, 200);
        }
        
        example.setLoop(
                        Arg::loopBegin = static_cast<SecondsF>(getData().currentMusic->songInfo.chorusBegSec()),
                        Arg::loopEnd   = static_cast<SecondsF>(getData().currentMusic->songInfo.chorusEndSec())
                        );
        example.setPosSec(getData().currentMusic->songInfo.chorusBegSec());
        example.play();
        
        getRanking();
        
        getData().selectTimer.start();
    }
    
    void Preference::update() {
        if (gameinfo::prev.down()) {
            if (adjustment) {
                switch (prefItem.at(currentItem).getEntry()) {
                    case PrefItem::SPEED:
                        if (getData().speed > 1) {
                            getData().speed -= 0.5;
                        }
                        break;
                    case PrefItem::DECISIONVOL:
                        if (getData().decisionVolume > 0) {
                            getData().decisionVolume--;
                        }
                        break;
                    case PrefItem::DIFFICULTY:
                        if (static_cast<score::Difficulty>(getData().currentDiff) > score::Difficulty::EASY) {
                            getData().currentDiff--;
                        }
                        break;
                    default:
                        break;
                }
            } else {
                if (currentItem > 0) {
                    currentItem--;
                }
            }
        }
        
        if (gameinfo::next.down()) {
            if (adjustment) {
                switch (prefItem.at(currentItem).getEntry()) {
                    case PrefItem::SPEED:
                        if (getData().speed < gameinfo::maxSpeed) {
                            getData().speed += 0.5;
                        }
                        break;
                    case PrefItem::DECISIONVOL:
                        if (getData().decisionVolume < 10) {
                            getData().decisionVolume++;
                        }
                        break;
                    case PrefItem::DIFFICULTY:
                        if (static_cast<score::Difficulty>(getData().currentDiff) < score::Difficulty::HARD) {
                            getData().currentDiff++;
                        }
                        break;
                    default:
                        break;
                }
            } else {
                if (currentItem < prefItem.size() - 1) {
                    currentItem++;
                }
            }
        }
        
        if (gameinfo::decide.down()) {
            if (prefItem.at(currentItem).getEntry() == PrefItem::GAMESTART) {
                getData().selectTimer.pause();
                getData().isSelecting = false;
                changeScene(SceneName::PLAY, gameinfo::fadeTime);
            }else{
                adjustment = true;
            }
        }
        
        if (gameinfo::back.down()) {
            if (adjustment) {
                adjustment = false;
                if (prefItem.at(currentItem).getEntry() == PrefItem::DIFFICULTY) {
                    getRanking();
                }
            }else{
                getData().selectTimer.pause();
                changeScene(SceneName::MUSICSELECT, gameinfo::fadeTime);
            }
        }
        
        if (getData().selectTimer.reachedZero()) {
            getData().selectTimer.pause();
            getData().isSelecting = false;
            changeScene(SceneName::PLAY, gameinfo::fadeTime);
        }
    }
    
    void Preference::draw() const {
        TextureAsset(U"preference").drawAt(::gameinfo::originalScreenCenter);
        
        {
            Transformer2D scaler(Mat3x2::Scale(gameinfo::scale, Vec2(0, 0)));
            TextureAsset(U"name").draw(0, 0);
            TextureAsset(U"track").draw(25, 115);
            FontAsset(U"80_bold")(getData().trackCount + 1).drawAt(296, 180, Palette::Darkslategray);
            FontAsset(U"45_bold")(Unicode::Widen(getData().user.name)).draw(Arg::topLeft = Vec2(50, 45), gameinfo::defaultFontColor);
        }        
        FontAsset(U"100_bold")(getData().selectTimer.s()).draw(Arg::topRight(::gameinfo::originalResolution.x - 10, 0), gameinfo::defaultFontColor);
        
        if (!getData().isGuest) {
            drawHighScore({350, 740});
            
        }
        
        drawRanking({1200, 740});
        
        for (auto [i, rect] : Indexed(instructionBox)) {
            rect(TextureAsset(U"instBack")).draw();
            switch (i) {
                case 0:
                    if (adjustment) {
                        FontAsset(U"50")(U"Down").drawAt(rect.center() + Vec2(0, 25), gameinfo::infoFontColor);
                    }else{
                        FontAsset(U"50")(U"Prev").drawAt(rect.center() + Vec2(0, 25), gameinfo::infoFontColor);
                    }
                    break;
                case 1:
                    if (adjustment) {
                        FontAsset(U"50")(U"Up").drawAt(rect.center() + Vec2(0, 25), gameinfo::infoFontColor);
                    }else{
                        FontAsset(U"50")(U"Next").drawAt(rect.center() + Vec2(0, 25), gameinfo::infoFontColor);
                    }
                    break;
                case 2:
                    if (!adjustment) {
                        FontAsset(U"50")(U"Select").drawAt(rect.center() + Vec2(0, 25), gameinfo::infoFontColor);
                    }
                    break;
                case 3:
                    FontAsset(U"50")(U"Back").drawAt(rect.center() + Vec2(0, 25), gameinfo::infoFontColor);
                    break;
                default:
                    break;
            }
        }
        
        for (auto i : step(prefItem.size())) {
            Vec2 moveWidth(((int)i - (int)currentItem) * defaultEntity.w, -50);
            switch (prefItem.at(i).getEntry()) {
                case PrefItem::GAMESTART:
                    prefItem.at(i).draw(moveWidth, adjustment);
                    break;
                case PrefItem::SPEED:
                    prefItem.at(i).draw(moveWidth, adjustment, getData().speed);
                    break;
                case PrefItem::DECISIONVOL:
                    prefItem.at(i).draw(moveWidth, adjustment, getData().decisionVolume);
                    break;
                case PrefItem::DIFFICULTY:
                    prefItem.at(i).draw(moveWidth, adjustment, static_cast<score::Difficulty>(getData().currentDiff));
                    break;
            }
        }
    }

    void Preference::getRanking() {
        ranking.clear();
        std::string difficulty;
        
        switch (getData().currentDiff) {
            case 0:
                difficulty = "easy";
                break;
                
            case 1:
                difficulty = "normal";
                break;
                
            case 2:
                difficulty = "hard";
                break;
                
            default:
                break;
        }
        
#if defined(MIDITONE_WIIBALANCEBOARD)
        const auto result = getData().client.get_board_score_ranking(getData().currentMusic->songInfo.id(), difficulty).first();
#else
        const auto result = getData().client.get_button_score_ranking(getData().currentMusic->songInfo.id(), difficulty).first();
#endif
                        
        const auto& body = result.success_value().parsed_body();
        std::copy(body.begin(), body.end(), std::back_inserter(ranking));
    }

    void Preference::drawRanking(Vec2 tlPos) const {
        Rect(tlPos.asPoint(), 500, 200).draw(ColorF(Palette::Dimgray, 0.7)).drawFrame(2, 3, Palette::Gold);
        FontAsset(U"30_bold")(U"Ranking").draw(tlPos + Vec2(10, 10), gameinfo::defaultFontColor);
        for (const auto& i : step(3)) {
            String name, score;
            FontAsset(U"30_bold")(i + 1).draw(tlPos + Vec2(10, 60 + 45 * i));
            try {
                name = Unicode::Widen(ranking.at(i).user.name);
                score = U"{:0>7}"_fmt(ranking.at(i).score.points.value_or(0));
            } catch (std::exception e) {
                name = U"---";
                score = U"---";
            }
            FontAsset(U"30_bold")(name).draw(tlPos + Vec2(40, 60 + 45 * i));
            FontAsset(U"30_bold")(score).draw(tlPos + Vec2(340, 60 + 45 * i));
        }
    }

    void Preference::drawHighScore(Vec2 tlPos) const {
        Rect(tlPos.asPoint(), 350, 200).draw(ColorF(Palette::Dimgray, 0.7)).drawFrame(2, 3, Palette::Gold);
        FontAsset(U"30_bold")(U"HighScore").draw(tlPos + Vec2(10, 10), gameinfo::defaultFontColor);
        FontAsset(U"30_bold")(U"EASY").draw(tlPos + Vec2(10, 60), gameinfo::easy);
        FontAsset(U"30_bold")(U"{:0>7}"_fmt(getData().currentMusic->highScore.easy.point)).draw(tlPos + Vec2(200, 60));
        FontAsset(U"30_bold")(U"NORMAL").draw(tlPos + Vec2(10, 105), gameinfo::normal);
        FontAsset(U"30_bold")(U"{:0>7}"_fmt(getData().currentMusic->highScore.normal.point)).draw(tlPos + Vec2(200, 105));
        FontAsset(U"30_bold")(U"HARD").draw(tlPos + Vec2(10, 150), gameinfo::hard);
        FontAsset(U"30_bold")(U"{:0>7}"_fmt(getData().currentMusic->highScore.hard.point)).draw(tlPos + Vec2(200, 150));
    }

}
