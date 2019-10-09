//
//  Preference.hpp
//  MusicGame
//
//  Created by Shimizu Yuta on 2018/10/08.
//

#ifndef Preference_hpp
#define Preference_hpp

#include "Common.hpp"
#include "../../GameInfo.hpp"
#include "../../System/SystemScore.hpp"

namespace ui{
    
    enum class PrefItem {
        GAMESTART,
        SPEED,
        DECISIONVOL,
        DIFFICULTY
    };
    
    class PrefBox : public IBox {
    private:
        PrefItem m_item;
        String m_content;
    public:
//        PrefBox() = default;
        PrefBox(const PrefItem _item, const String& content, const RectF& _entity, const Texture& _boxTex);
        void draw(const Vec2& moveWidth, bool isSelected) const;
        void draw(const Vec2& moveWidth, bool isSelected, double currentValue) const;
        void draw(const Vec2& moveWidth, bool isSelected, score::Difficulty currentDiff) const;
        PrefItem getEntry() const;
    };
    
    class Preference : public MyApp::Scene {
    private:
        const Vec2 boxSize;
        const RectF defaultEntity;
        Audio example;
        size_t currentItem;
        Array<PrefBox> prefItem;
        Array<Rect> instructionBox;
        Timer countDown;
        bool adjustment;
        double chobeg, choend;
    public:
        Preference(const InitData& init);
        void update() override;
        void draw() const override;
    };
    
}

#endif /* Preference_hpp */
