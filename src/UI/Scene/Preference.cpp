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
                value.push_front(U'◀');
                value.push_back(U'▶');
            }
        }
        
        Transformer2D t(Mat3x2::Scale(scale, entity.center() + moveWidth));
        FontAsset(U"prefFont")(m_content).drawAt(entity.center() + moveWidth + Vec2(0, 150), gameinfo::fontColor);
        FontAsset(U"infoFont")(value).drawAt(entity.center() + moveWidth + Vec2(0, -100), gameinfo::fontColor);
    }
    
    void PrefBox::draw(const s3d::Vec2 &moveWidth, bool isSelected, score::Difficulty currentDiff) const {
        String diff;
        double scale = 0.8;
        
        switch (currentDiff) {
            case score::Difficulty::EASY:
                diff = U"EASY";
                break;
            case score::Difficulty::NORMAL:
                diff = U"NORMAL";
                break;
            case score::Difficulty::HARD:
                diff = U"HARD";
                break;
        }
        
        draw(moveWidth, isSelected);
        
        if (moveWidth.x == 0) {
            scale = 1;
            if (isSelected) {
                diff.push_front(U'◀');
                diff.push_back(U'▶');
            }
        }
        
        Transformer2D t(Mat3x2::Scale(scale, entity.center() + moveWidth));
        FontAsset(U"prefFont")(m_content).drawAt(entity.center() + moveWidth + Vec2(0, 150), gameinfo::fontColor);
        FontAsset(U"infoFont")(diff).drawAt(entity.center() + moveWidth + Vec2(0, -100), gameinfo::fontColor);
    }
    
    ui::PrefItem PrefBox::getEntry() const {
        return m_item;
    }
    
    
    
    
    
    Preference::Preference(const InitData &init):
    IScene(init),
    boxSize(400, 600),
    currentItem(0),
    adjustment(false),
    defaultEntity(Arg::center(Window::Center()), boxSize){
        ClearPrint();
        prefItem.emplace_back(PrefItem::GAMESTART, U"GAMESTART", defaultEntity, TextureAsset(U"gameStart"));
        prefItem.emplace_back(PrefItem::DIFFICULTY, U"Difficulty", defaultEntity, TextureAsset(U"boxTemplate"));
        prefItem.emplace_back(PrefItem::SPEED, U"Speed", defaultEntity, TextureAsset(U"boxTemplate"));
        prefItem.emplace_back(PrefItem::DECISIONVOL, U"SE Volume", defaultEntity, TextureAsset(U"boxTemplate"));
    }
    
    void Preference::update() {
        if (gameinfo::backArrow.down()) {
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
        
        if (gameinfo::goArrow.down()) {
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
                changeScene(SceneName::PLAY, gameinfo::fadeTime);
            }else{
                adjustment = true;
            }
        }
        
        if (gameinfo::back.down()) {
            if (adjustment) {
                adjustment = false;
            }else{
                changeScene(SceneName::MUSICSELECT, gameinfo::fadeTime);
            }
        }
    }
    
    void Preference::draw() const {
        TextureAsset(U"preference").drawAt(Window::Center());
        
        for (auto i : step(prefItem.size())) {
            Vec2 moveWidth(((int)i - (int)currentItem) * defaultEntity.w, 0);
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

}
