
typedef struct Assets {
	Texture2D textures[ASSET_TEXTURE_COUNT];
	Sound sounds[ASSET_SOUND_COUNT];
	Music musics[ASSET_MUSIC_COUNT];
	Font font;
} Assets;

static Assets assets;

void AssetsInit(void) {
	assets = (Assets) { 0 };
	
	const char* appPath = GetApplicationDirectory();
	char* bufPath = malloc(2048);
	
	sprintf(bufPath, "%sassets/wall.png", appPath);
	assets.textures[ASSET_TEXTURE_WALL] = LoadTexture(bufPath);
	
	sprintf(bufPath, "%sassets/counter.png", appPath);
	assets.textures[ASSET_TEXTURE_COUNTER] = LoadTexture(bufPath);
	
	sprintf(bufPath, "%sassets/dwarf0.png", appPath);
	assets.textures[ASSET_TEXTURE_DWARF0] = LoadTexture(bufPath);
	
	sprintf(bufPath, "%sassets/dwarf1.png", appPath);
	assets.textures[ASSET_TEXTURE_DWARF1] = LoadTexture(bufPath);
	
	sprintf(bufPath, "%sassets/ogre0.png", appPath);
	assets.textures[ASSET_TEXTURE_OGRE0] = LoadTexture(bufPath);
	
	sprintf(bufPath, "%sassets/ogre1.png", appPath);
	assets.textures[ASSET_TEXTURE_OGRE1] = LoadTexture(bufPath);
	
	sprintf(bufPath, "%sassets/lady0.png", appPath);
	assets.textures[ASSET_TEXTURE_LADY0] = LoadTexture(bufPath);
	
	sprintf(bufPath, "%sassets/lady1.png", appPath);
	assets.textures[ASSET_TEXTURE_LADY1] = LoadTexture(bufPath);
	
	sprintf(bufPath, "%sassets/creepyman0.png", appPath);
	assets.textures[ASSET_TEXTURE_CREEPYMAN0] = LoadTexture(bufPath);
	
	sprintf(bufPath, "%sassets/thinking.png", appPath);
	assets.textures[ASSET_TEXTURE_THINKING] = LoadTexture(bufPath);
	
	sprintf(bufPath, "%sassets/speech.png", appPath);
	assets.textures[ASSET_TEXTURE_SPEECH] = LoadTexture(bufPath);
	
	sprintf(bufPath, "%sassets/speechIntro0.png", appPath);
	assets.textures[ASSET_TEXTURE_SPEECH_INTRO0] = LoadTexture(bufPath);
	
	sprintf(bufPath, "%sassets/hand0.png", appPath);
	assets.textures[ASSET_TEXTURE_HAND0] = LoadTexture(bufPath);
	
	sprintf(bufPath, "%sassets/hand1.png", appPath);
	assets.textures[ASSET_TEXTURE_HAND1] = LoadTexture(bufPath);
	
	sprintf(bufPath, "%sassets/hand0Trail.png", appPath);
	assets.textures[ASSET_TEXTURE_HAND0_TRAIL] = LoadTexture(bufPath);
	
	sprintf(bufPath, "%sassets/hand1Trail.png", appPath);
	assets.textures[ASSET_TEXTURE_HAND1_TRAIL] = LoadTexture(bufPath);
	
	sprintf(bufPath, "%sassets/shrine0.png", appPath);
	assets.textures[ASSET_TEXTURE_SHRINE0] = LoadTexture(bufPath);
	
	sprintf(bufPath, "%sassets/shrine1.png", appPath);
	assets.textures[ASSET_TEXTURE_SHRINE1] = LoadTexture(bufPath);
	
	sprintf(bufPath, "%sassets/shrine2.png", appPath);
	assets.textures[ASSET_TEXTURE_SHRINE2] = LoadTexture(bufPath);
	
	sprintf(bufPath, "%sassets/summonBook.png", appPath);
	assets.textures[ASSET_TEXTURE_SUMMON_BOOK] = LoadTexture(bufPath);
	
	sprintf(bufPath, "%sassets/chickenGraphic.png", appPath);
	assets.textures[ASSET_TEXTURE_CHICKEN_GRAPHIC] = LoadTexture(bufPath);
	
	sprintf(bufPath, "%sassets/sheepGraphic.png", appPath);
	assets.textures[ASSET_TEXTURE_SHEEP_GRAPHIC] = LoadTexture(bufPath);
	
	sprintf(bufPath, "%sassets/chicken0.png", appPath);
	assets.textures[ASSET_TEXTURE_CHICKEN0] = LoadTexture(bufPath);
	
	sprintf(bufPath, "%sassets/chicken1.png", appPath);
	assets.textures[ASSET_TEXTURE_CHICKEN1] = LoadTexture(bufPath);
	
	sprintf(bufPath, "%sassets/chicken2.png", appPath);
	assets.textures[ASSET_TEXTURE_CHICKEN2] = LoadTexture(bufPath);
	
	sprintf(bufPath, "%sassets/rchicken0.png", appPath);
	assets.textures[ASSET_TEXTURE_RCHICKEN0] = LoadTexture(bufPath);
	
	sprintf(bufPath, "%sassets/brchicken0.png", appPath);
	assets.textures[ASSET_TEXTURE_BRCHICKEN0] = LoadTexture(bufPath);
	
	sprintf(bufPath, "%sassets/sheep0.png", appPath);
	assets.textures[ASSET_TEXTURE_SHEEP0] = LoadTexture(bufPath);
	
	sprintf(bufPath, "%sassets/rlamb.png", appPath);
	assets.textures[ASSET_TEXTURE_RLAMB] = LoadTexture(bufPath);
	
	sprintf(bufPath, "%sassets/brlamb.png", appPath);
	assets.textures[ASSET_TEXTURE_BRLAMB] = LoadTexture(bufPath);
	
	sprintf(bufPath, "%sassets/meterBottom.png", appPath);
	assets.textures[ASSET_TEXTURE_METER_BOTTOM] = LoadTexture(bufPath);
	
	sprintf(bufPath, "%sassets/meterTop.png", appPath);
	assets.textures[ASSET_TEXTURE_METER_TOP] = LoadTexture(bufPath);
	
	sprintf(bufPath, "%sassets/buttonX.png", appPath);
	assets.textures[ASSET_TEXTURE_BUTTON_X] = LoadTexture(bufPath);
	
	sprintf(bufPath, "%sassets/buttonO.png", appPath);
	assets.textures[ASSET_TEXTURE_BUTTON_CIRCLE] = LoadTexture(bufPath);
	
	sprintf(bufPath, "%sassets/goldIcon.png", appPath);
	assets.textures[ASSET_TEXTURE_GOLD_ICON] = LoadTexture(bufPath);
	
	sprintf(bufPath, "%sassets/creepIntro.png", appPath);
	assets.textures[ASSET_TEXTURE_CREEP_INTRO] = LoadTexture(bufPath);
	
	sprintf(bufPath, "%sassets/chickenSummon.mp3", appPath);
	assets.sounds[ASSET_SOUND_CHICKEN_SUMMON] = LoadSound(bufPath);
	
	sprintf(bufPath, "%sassets/chickenDead.mp3", appPath);
	assets.sounds[ASSET_SOUND_CHICKEN_DEAD] = LoadSound(bufPath);
	
	sprintf(bufPath, "%sassets/burning.mp3", appPath);
	assets.sounds[ASSET_SOUND_BURNING] = LoadSound(bufPath);
	SetSoundVolume(assets.sounds[ASSET_SOUND_BURNING], 0.25f);
	
	sprintf(bufPath, "%sassets/woosh.mp3", appPath);
	assets.sounds[ASSET_SOUND_WOOSH] = LoadSound(bufPath);
	SetSoundVolume(assets.sounds[ASSET_SOUND_WOOSH], 0.25f);
	
	sprintf(bufPath, "%sassets/crunch.mp3", appPath);
	assets.sounds[ASSET_SOUND_CRUNCH] = LoadSound(bufPath);
	
	sprintf(bufPath, "%sassets/coin0.mp3", appPath);
	assets.sounds[ASSET_SOUND_COIN0] = LoadSound(bufPath);
	
	sprintf(bufPath, "%sassets/coin1.mp3", appPath);
	assets.sounds[ASSET_SOUND_COIN1] = LoadSound(bufPath);
	
	sprintf(bufPath, "%sassets/coin2.mp3", appPath);
	assets.sounds[ASSET_SOUND_COIN2] = LoadSound(bufPath);
	
	sprintf(bufPath, "%sassets/sheepSummon.mp3", appPath);
	assets.sounds[ASSET_SOUND_SHEEP_SUMMON] = LoadSound(bufPath);
	SetSoundVolume(assets.sounds[ASSET_SOUND_SHEEP_SUMMON], 0.21f);
	
	sprintf(bufPath, "%sassets/sheepDead.mp3", appPath);
	assets.sounds[ASSET_SOUND_SHEEP_DEAD] = LoadSound(bufPath);
	SetSoundVolume(assets.sounds[ASSET_SOUND_SHEEP_DEAD], 0.21f);
	
	sprintf(bufPath, "%sassets/creep.bdf", appPath);
	assets.font = LoadFont(bufPath);
	
	sprintf(bufPath, "%sassets/music.mp3", appPath);
	assets.musics[ASSET_MUSIC_PLAYING] = LoadMusicStream(bufPath);
	SetMusicVolume(assets.musics[ASSET_MUSIC_PLAYING], 0.45f);
	
	sprintf(bufPath, "%sassets/creepyMusic.mp3", appPath);
	assets.musics[ASSET_MUSIC_CREEPY_INTRO] = LoadMusicStream(bufPath);
	SetMusicVolume(assets.musics[ASSET_MUSIC_CREEPY_INTRO], 0.45f);
	
	free(bufPath);
}

void AssetsDeinit(void) {

}

Texture2D AssetsGetTexture(AssetTexture texture) {
	return assets.textures[texture];
}
Sound AssetsGetSound(AssetSound sound) {
	return assets.sounds[sound];
}

Font AssetsGetFont(void) {
	return assets.font;
}

Music AssetsGetMusic(AssetMusic music) {
	return assets.musics[music];
}
