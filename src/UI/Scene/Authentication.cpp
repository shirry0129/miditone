//
//  Authentication.cpp
//  empty
//
//  Created by Shimizu Yuta on 2019/10/16.
//

#include "Authentication.hpp"

namespace ui{
    
    Authentication::Authentication(const InitData& init):
    IScene(init),
    cam(0),
    authed(false),
    isFailed(false)
    {
        const INIData authData(U"../settings.ini");
        
        getData().client.destination(authData[U"auth.host"].narrow(), authData[U"auth.port"].narrow());
        getData().client.token(authData[U"auth.token"].narrow());
        
        cam.setResolution(DisplayResolution::SVGA_800x600);
        if (!cam.start()) {
            throw Error(U"webcam not available.");
        }
        
        for (const auto i : step(4)) {
            instructionBox.emplace_back(325.5 + 355 * i, 880, 200);
        }
    }
    
    void Authentication::update(){
        if (cam.hasNewFrame()) {
            cam.getFrame(camImage);
            camTexture.fillIfNotBusy(camImage);
            
            if (decoder.decode(camImage, userId)) {
                authed = true;
                cam.stop();
                
                const auto& result = getData().client.get_user(userId.text.narrow());
                
                if (result) {
                    const auto& response = result.success_value();
                    getData().userName = Unicode::Widen(response.parsed_body().name);
                } else {
                    isFailed = true;
                    const auto& error = result.failed_value();
                    Logger << U"error      : " << Unicode::Widen(error.body());
                }
            }
        }
        
        if ((!authed && gameinfo::back.down()) || (authed && gameinfo::decide.down())) {
            changeScene(SceneName::MUSICSELECT, gameinfo::fadeTime);
        }
    }

    void Authentication::draw() const{
        TextureAsset(U"login").drawAt(gameinfo::originalScreenCenter);
        
        if (camTexture && !authed) {
            camTexture.drawAt(gameinfo::originalScreenCenter);
        }
        
        if (authed) {
            if (isFailed) {
                const String errorMsg = U"Network Error. Continue as Guest.";
                Rect(Arg::center = gameinfo::originalScreenCenter, gameinfo::originalResolution.x, 100).draw(Color(Palette::Darkslategray, 200));
                FontAsset(U"trackFont")(errorMsg).draw(Arg::center = gameinfo::originalScreenCenter, gameinfo::defaultFontColor);
            } else {
                FontAsset(U"countDown")(U"Welcome!").draw(Arg::center = gameinfo::originalScreenCenter - Vec2(0, 240), gameinfo::infoFontColor);
                Rect(Arg::center = gameinfo::originalScreenCenter, gameinfo::originalResolution.x, FontAsset(U"countDown").height() * 1.75).draw(Color(Palette::Darkslategray, 200));
                FontAsset(U"countDown")(getData().userName).draw(Arg::center = gameinfo::originalScreenCenter, gameinfo::defaultFontColor);
                FontAsset(U"prefFont")(U"Continue with this data?").draw(Arg::center = gameinfo::originalScreenCenter + Vec2(0, 350), gameinfo::infoFontColor);
            }
        }
        
        for (const auto [i, rect] : Indexed(instructionBox)) {
            rect(TextureAsset(U"instBack")).draw();
            switch (i) {
                case 2:
                    if (authed) {
                        FontAsset(U"infoFont")(U"OK").drawAt(rect.center() + Vec2(0, 25), gameinfo::infoFontColor);
                    }
                    break;
                case 3:
                    if (!authed) {
                        FontAsset(U"infoFont")(U"Guest").drawAt(rect.center() + Vec2(0, 25), gameinfo::infoFontColor);
                    }
                    break;
                default:
                    break;
            }
        }
    }

}
