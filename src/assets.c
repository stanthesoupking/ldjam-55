
typedef struct Assets {
	Texture2D textures[ASSET_TEXTURE_COUNT];
	Font font;
} Assets;

static Assets assets;

void AssetsInit(void) {
	assets = (Assets) {};
	
	const char* appPath = GetApplicationDirectory();
	char bufPath[256];
	
	sprintf(bufPath, "%sassets/wall.png", appPath);
	assets.textures[ASSET_TEXTURE_WALL] = LoadTexture(bufPath);
	
	sprintf(bufPath, "%sassets/counter.png", appPath);
	assets.textures[ASSET_TEXTURE_COUNTER] = LoadTexture(bufPath);
	
	sprintf(bufPath, "%sassets/dwarf0.png", appPath);
	assets.textures[ASSET_TEXTURE_DWARF0] = LoadTexture(bufPath);
	
	sprintf(bufPath, "%sassets/ogre0.png", appPath);
	assets.textures[ASSET_TEXTURE_OGRE0] = LoadTexture(bufPath);
	
	sprintf(bufPath, "%sassets/lady0.png", appPath);
	assets.textures[ASSET_TEXTURE_LADY0] = LoadTexture(bufPath);
	
	sprintf(bufPath, "%sassets/thinking.png", appPath);
	assets.textures[ASSET_TEXTURE_THINKING] = LoadTexture(bufPath);
	
	sprintf(bufPath, "%sassets/speech.png", appPath);
	assets.textures[ASSET_TEXTURE_SPEECH] = LoadTexture(bufPath);
	
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
	
	sprintf(bufPath, "%sassets/buttonX.png", appPath);
	assets.textures[ASSET_TEXTURE_BUTTON_X] = LoadTexture(bufPath);
	
	sprintf(bufPath, "%sassets/buttonO.png", appPath);
	assets.textures[ASSET_TEXTURE_BUTTON_CIRCLE] = LoadTexture(bufPath);
	
	sprintf(bufPath, "%sassets/creep.bdf", appPath);
	assets.font = LoadFont(bufPath);
}

void AssetsDeinit(void) {

}

Texture2D AssetsGetTexture(AssetTexture texture) {
	return assets.textures[texture];
}

Font AssetsGetFont(void) {
	return assets.font;
}
