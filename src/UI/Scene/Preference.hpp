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
#include "../../System/ScoreManager.hpp"

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
        
    public:
        PrefBox() = default;
        PrefBox(const PrefItem _item, const String& content, const RectF& _entity);
        void draw(const Vec2& moveWidth, bool isSelected) const;
        void draw(const Vec2& moveWidth, bool isSelected, double currentValue) const;
        void draw(const Vec2& moveWidth, bool isSelected, score::Difficulty currentDiff) const;
        PrefItem getEntry() const;
    };

    class Preference : public MyApp::Scene {
    private:
        const Point boxSize;
        const RectF defaultEntity;
        size_t currentItem;
        Array<PrefBox> prefItem;
        bool adjustment;
        
    public:
        Preference(const InitData& init);
        void update() override;
        void draw() const override;
    };
    
}

#endif /* Preference_hpp */
