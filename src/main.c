
#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 144

i32 main(i32 argc, const char *argv[]) {
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(240 * 4, 144 * 4, "ldjam55");
	InitAudioDevice();
	SetTargetFPS(60);
	
	AssetsInit();
	GameInit();
	
	while (!WindowShouldClose()) {
		GameUpdate();
		GameRender();
	}
		
	GameDeinit();
	AssetsDeinit();
	CloseWindow();
	return 0;
}
