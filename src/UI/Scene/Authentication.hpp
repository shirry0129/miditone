//
//  Authentication.hpp
//  empty
//
//  Created by Shimizu Yuta on 2019/10/16.
//

#ifndef Authentication_hpp
#define Authentication_hpp

#include "Common.hpp"
#include "../../System/MiditoneClient.hpp"

namespace ui {

    class Authentication : public MyApp::Scene{
    private:
        Webcam cam;
        QRDecoder decoder;
        QRContent userId;
        Image camImage;
        DynamicTexture camTexture;
        Array<Rect> instructionBox;
        
        bool authed, isFailed;
        const INIData settings;
        
    public:
        Authentication(const InitData& init);
        void update() override;
        void draw() const override;
    };

}

#endif /* Authentication_hpp */
