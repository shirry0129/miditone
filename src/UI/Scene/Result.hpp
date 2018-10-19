//
//  Result.hpp
//  MusicGame
//
//  Created by Shimizu Yuta on 2018/10/08.
//

#ifndef Result_hpp
#define Result_hpp

#include "Common.hpp"

namespace ui{
    
    class Result : public MyApp::Scene{
    private:
        Array<Rect> instructionBox;
        Texture albumArt;
        size_t maxWidth;
        void drawDecision(const Vec2& pos) const;
        void drawSongInfo(const Vec2& pos) const;
        
        void compressedDisplay(const s3d::Vec2 &centerPos, const s3d::Font &assetInfo, const s3d::String &string) const;
    public:
        Result(const InitData& init);
        void update() override;
        void draw() const override;
    };
    
}

#endif /* Result_hpp */
