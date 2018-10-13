//
//  Preference.cpp
//  MusicGame
//
//  Created by Shimizu Yuta on 2018/10/08.
//

#include "Preference.hpp"

namespace ui{
    
    PrefBox::PrefBox(const PrefItem _item, const String& content, const RectF& _entity):
    IBox(content, _entity),
    m_item(_item){}
    
    void PrefBox::draw(const Vec2& moveWidth, bool isSelected) const {
        if (moveWidth.x == 0) {
            entity.movedBy(moveWidth).draw(Palette::Green).drawFrame(4, isSelected ? Palette::Red : Palette::Darkgreen);
        }else{
            entity.movedBy(moveWidth).scaled(0.8).draw(Palette::Green).drawFrame(4, Palette::Darkgreen);
        }
        FontAsset(U"BoxFont")(m_content).drawAt(entity.center() + moveWidth + Vec2(0, 150));
    }
    
    void PrefBox::draw(const s3d::Vec2 &moveWidth, bool isSelected, double currentValue) const {
        draw(moveWidth, isSelected);
        FontAsset(U"infoFont")(currentValue).drawAt(entity.center() + moveWidth + Vec2(0, -100));
    }
    
    void PrefBox::draw(const s3d::Vec2 &moveWidth, bool isSelected, score::Difficulty currentDiff) const {
        String diff;
        
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
        FontAsset(U"infoFont")(diff).drawAt(entity.center() + moveWidth + Vec2(0, -100));
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
        prefItem.emplace_back(PrefItem::GAMESTART, U"GAMESTART", defaultEntity);
        prefItem.emplace_back(PrefItem::DIFFICULTY, U"Difficulty", defaultEntity);
        prefItem.emplace_back(PrefItem::SPEED, U"Speed", defaultEntity);
        prefItem.emplace_back(PrefItem::DECISIONVOL, U"SE Volume", defaultEntity);
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
                        if (getData().decisionVolume < 100) {
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
                changeScene(SceneName::PLAY, 2000);
            }else{
                adjustment = true;
            }
        }
        
        if (gameinfo::back.down()) {
            if (adjustment) {
                adjustment = false;
            }
        }
    }
    
    void Preference::draw() const {
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
