
static inline void DrawTextureCenteredV(Texture2D texture, Vector2 position, Color tint) {
	DrawTextureV(texture, Vector2Subtract(position, (Vector2) { texture.width >> 1, texture.height >> 1 }), tint);
}

static inline void DrawTextDefault(const char* text, Vector2 position, Color tint) {
	const Font font = AssetsGetFont();
	DrawTextEx(font, text, position, font.baseSize, 0.0f, tint);
}
