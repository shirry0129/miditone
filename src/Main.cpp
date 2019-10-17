
# include <Siv3D.hpp> // OpenSiv3D v0.4.0
#include "GameInfo.hpp"
#include "UI/Scene/Common.hpp"
#include "UI/Scene/Title.hpp"
#include "UI/Scene/MusicSelect.hpp"
#include "UI/Scene/Preference.hpp"
#include "UI/Scene/Play.hpp"
#include "UI/Scene/Result.hpp"
#include "UI/Scene/TotalResult.hpp"
#include "UI/Scene/GameOver.hpp"

void Main() {

	Size screenSize;
	auto monitors = System::EnumerateActiveMonitors();
	for (const auto & m : monitors) {
		if (m.isPrimary)
			screenSize = m.displayRect.size;
	}

	const auto ratio = Float2(screenSize) / Float2(gameinfo::originalResolution);
	const auto renderRatio = Min(ratio.x, ratio.y);
	
	const Size windowSize {
		static_cast<int32>(gameinfo::originalResolution.x * renderRatio),
		static_cast<int32>(gameinfo::originalResolution.y * renderRatio)
	};
	
	Window::Resize(windowSize);
	//Graphics::SetFullScreen(true, {1920, 1080});
    Window::SetTitle(gameinfo::title);
    Scene::SetBackground(gameinfo::backGroundColor);
    System::SetTerminationTriggers(UserAction::CloseButtonClicked);
    
    FontAsset::Register(U"countDown", 100, Typeface::Bold);
    FontAsset::Register(U"songTitle", 50, Typeface::Bold);
    FontAsset::Register(U"musicInfo", 30);
    FontAsset::Register(U"trackFont", 80, Typeface::Bold);
    FontAsset::Register(U"prefFont", 50, Typeface::Bold);
    FontAsset::Register(U"infoFont", 50);
    FontAsset::Register(U"songInfo", 30, Typeface::Bold);
    FontAsset::Register(U"artistInfo", 15);
    FontAsset::Register(U"diffInfo", 45, Typeface::Bold);
    FontAsset::Register(U"scoreFont", 50, Typeface::Bold);
    FontAsset::Register(U"countFont", 15);
    FontAsset::Register(U"effectFont", 30, Typeface::Bold);
    FontAsset::Register(U"comboFont", 200);
    FontAsset::Register(U"resultScore", 100, Typeface::Bold);
    FontAsset::Register(U"decisionFont", 70);
    FontAsset::Register(U"totalTrackNum", 30);
    FontAsset::Register(U"totalSong", 45, Typeface::Bold);
    FontAsset::Register(U"totalArtist", 30);
    
#if defined(SIV3D_TARGET_MACOS)
    TextureAsset::Register(U"gameStart", Resource(U"resource/gamestart.png"), TextureDesc::Mipped);
    TextureAsset::Register(U"boxTemplate", Resource(U"resource/box.png"), TextureDesc::Mipped);
    TextureAsset::Register(U"title", Resource(U"resource/title.jpg"));
    TextureAsset::Register(U"select", Resource(U"resource/select.jpg"));
    TextureAsset::Register(U"preference", Resource(U"resource/preference.jpg"));
    TextureAsset::Register(U"play", Resource(U"resource/play.jpg"));
    TextureAsset::Register(U"result", Resource(U"resource/result.jpg"));
    TextureAsset::Register(U"totalResult", Resource(U"resource/totalresult.jpg"));
    TextureAsset::Register(U"track", Resource(U"resource/track.png"));
    TextureAsset::Register(U"score", Resource(U"resource/score_box.png"), TextureDesc::Mipped);
    TextureAsset::Register(U"song", Resource(U"resource/song_box_long.png"), TextureDesc::Mipped);
    TextureAsset::Register(U"gameover", Resource(U"resource/end.png"));
    TextureAsset::Register(U"hitNote", Resource(U"resource/hitnote.png"));
    TextureAsset::Register(U"arrow", Resource(U"resource/ya.png"));
    TextureAsset::Register(U"return", Resource(U"resource/turn.png"));
    TextureAsset::Register(U"instBack", Resource(U"resource/back.png"));
    TextureAsset::Register(U"resultDecision", Resource(U"resource/result_box.png"));
    TextureAsset::Register(U"resultCombo", Resource(U"resource/max_combo_.png"));
#elif defined(SIV3D_TARGET_WINDOWS)
    TextureAsset::Register(U"gameStart", U"resource/gamestart.png", TextureDesc::Mipped);
    TextureAsset::Register(U"boxTemplate", U"resource/box.png", TextureDesc::Mipped);
    TextureAsset::Register(U"title", U"resource/title.jpg");
    TextureAsset::Register(U"select", U"resource/select.jpg");
    TextureAsset::Register(U"preference", U"resource/preference.jpg");
    TextureAsset::Register(U"play", U"resource/play.jpg");
    TextureAsset::Register(U"result", U"resource/result.jpg");
    TextureAsset::Register(U"totalResult", U"resource/totalresult.jpg");
    TextureAsset::Register(U"track", U"resource/track.png");
    TextureAsset::Register(U"score", U"resource/score_box.png", TextureDesc::Mipped);
    TextureAsset::Register(U"song", U"resource/song_box_long.png", TextureDesc::Mipped);
    TextureAsset::Register(U"gameover", U"resource/end.png");
    TextureAsset::Register(U"hitNote", U"resource/hitnote.png");
    TextureAsset::Register(U"arrow", U"resource/ya.png");
    TextureAsset::Register(U"return", U"resource/turn.png");
    TextureAsset::Register(U"instBack", U"resource/back.png");
    TextureAsset::Register(U"resultDecision", U"resource/result_box.png");
    TextureAsset::Register(U"resultCombo", U"resource/max_combo_.png");
#endif
    
    const INIData settings(U"../settings.ini");
    gameinfo::totalTrack = Parse<int32>(settings[U"play.tracks"]);
    
    ui::MyApp sceneManager;
    sceneManager
        .add<ui::Title>(ui::SceneName::TITLE)
        .add<ui::MusicSelect>(ui::SceneName::MUSICSELECT)
        .add<ui::Preference>(ui::SceneName::PREFERENCE)
        .add<ui::Play>(ui::SceneName::PLAY)
        .add<ui::Result>(ui::SceneName::RESULT)
        .add<ui::TotalResult>(ui::SceneName::TOTALRESULT)
        .add<ui::GameOver>(ui::SceneName::GAMEOVER)
        .setFadeColor(gameinfo::fadeColor);
	
    try {
        while (System::Update()) {
            Cursor::RequestStyle(CursorStyle::Hidden);
            
            const Transformer2D scaler(Mat3x2::Scale(renderRatio));
        
            if(!sceneManager.update()){
                break;
            }
        }
    } catch (std::exception e) {
        auto s = e.what();
        std::ofstream ofs("../error.txt");
        ofs << s;
        ofs.close();
    }
}
