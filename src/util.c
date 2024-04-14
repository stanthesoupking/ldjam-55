
static inline void DrawTextureCenteredV(Texture2D texture, Vector2 position, Color tint) {
	DrawTextureV(texture, Vector2Subtract(position, (Vector2) { (f32)(texture.width >> 1), (f32)(texture.height >> 1) }), tint);
}

static inline void DrawTextDefault(const char* text, Vector2 position, Color tint) {
	const Font font = AssetsGetFont();
	DrawTextEx(font, text, position, (f32)font.baseSize, 0.0f, tint);
}

static inline bool RangeIntersects(f32 start0, f32 end0, f32 start1, f32 end1) {
	return (start0 <= end1) && (end0 >= start1);
}
