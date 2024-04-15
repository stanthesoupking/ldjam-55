
#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 144

void UpdateDrawFrame(void)
{
	GameUpdate();
	GameRender();
}

i32 main(i32 argc, const char *argv[]) {
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(240 * 4, 144 * 4, "ldjam55");
	InitAudioDevice();
	
	AssetsInit();
	GameInit();
	 
#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(UpdateDrawFrame, 0, 1);
#else
	SetTargetFPS(60);
	while (!WindowShouldClose()) {
		UpdateDrawFrame();
	}
#endif
		
	GameDeinit();
	AssetsDeinit();
	CloseWindow();
	return 0;
}
