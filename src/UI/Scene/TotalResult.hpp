//
//  TotalResult.hpp
//  MusicGame
//
//  Created by Shimizu Yuta on 2018/10/13.
//

#ifndef TotalResult_hpp
#define TotalResult_hpp

#include "Common.hpp"

namespace ui {
    
    class TotalResult : public MyApp::Scene {
    private:
        Array<Texture> albumArt;
        Array<Rect> instructionBox;
        size_t maxWidth;
        Timer countDown;
        
        void compressedDisplay(const s3d::Vec2 &centerPos, const s3d::Font &assetInfo, const s3d::String &string) const;
        void drawTrack(const Vec2& pos, size_t track) const;
    public:
        TotalResult(const InitData& init);
        void update() override;
        void draw() const override;
    };
    
}

#endif /* TotalResult_hpp */
