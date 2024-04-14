
#define SUMMON_BOOK_DEBUG_HOVER_RECTS 0

typedef void (*SummonEntryFunc)(void);

typedef struct SummonBookEntry {
	AssetTexture graphicTexture;
	Vector2 graphicOffset;
	Rectangle hoverRect;
	SummonEntryFunc summonFunc;
	i32 price;
} SummonBookEntry;

#define SUMMON_BOOK_ENTRY_COUNT 1
static SummonBookEntry SUMMON_BOOK_ENTRIES[SUMMON_BOOK_ENTRY_COUNT] = {
	{
		.graphicTexture = ASSET_TEXTURE_CHICKEN_GRAPHIC,
		.graphicOffset = { 8, 10 },
		.hoverRect = { 2, 4, 78, 28 },
		.summonFunc = GameSummonChicken,
		.price = 0,
	}
};



typedef struct SummonBook {
	bool entryAvailable[SUMMON_BOOK_ENTRY_COUNT];
	bool entryHover[SUMMON_BOOK_ENTRY_COUNT];
	Vector2 bookPosition;
} SummonBook;

void SummonBookInit(SummonBook* book) {
	*book = (SummonBook) {
		.entryAvailable = {
			[0] = true,
		},
	};
}

Rectangle SummonBookGetEntryScreenHoverRect(SummonBook* book, u32 entryIndex) {
	const Texture2D summonBookTexture = AssetsGetTexture(ASSET_TEXTURE_SUMMON_BOOK);
	const Vector2 summonBookOffset = (Vector2) { (f32) -(summonBookTexture.width >> 1), (f32) -(summonBookTexture.height >> 1)};
	const Rectangle bookHover = SUMMON_BOOK_ENTRIES[entryIndex].hoverRect;
	return (Rectangle) {
		.x = bookHover.x + summonBookOffset.x + book->bookPosition.x,
		.y = bookHover.y + summonBookOffset.y + book->bookPosition.y,
		.width = bookHover.width,
		.height = bookHover.height,
	};
}

void SummonBookUpdate(SummonBook* book, bool active, Vector2 bookPosition, Vector2 handPosition) {
	book->bookPosition = bookPosition;
	
	// update hover
	memset(book->entryHover, 0, sizeof(book->entryHover));
	for (u32 entryIndex = 0; entryIndex < SUMMON_BOOK_ENTRY_COUNT; entryIndex++) {
		SummonBookEntry entry = SUMMON_BOOK_ENTRIES[entryIndex];
		Rectangle hoverRect = SummonBookGetEntryScreenHoverRect(book, entryIndex);
		if ((handPosition.x > hoverRect.x) && (handPosition.y > hoverRect.y) && (handPosition.x < hoverRect.x + hoverRect.width) && (handPosition.y < hoverRect.y + hoverRect.height)) {
			book->entryHover[entryIndex] = true;
			
			if (IsKeyPressed(KEY_X) || IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_DOWN)) {
				entry.summonFunc();
			}
			break;
		}
	}
}

void SummonBookDraw(SummonBook* book) {
	const Color inkColor = { 106, 90, 85, 255 };
	
	Texture2D summonBookTexture = AssetsGetTexture(ASSET_TEXTURE_SUMMON_BOOK);
	const Vector2 summonBookOffset = (Vector2) { (f32)-(summonBookTexture.width >> 1), (f32)-(summonBookTexture.height >> 1) };
	DrawTextureV(summonBookTexture, Vector2Add(book->bookPosition, summonBookOffset), WHITE);
	
	for (u32 entryIndex = 0; entryIndex < SUMMON_BOOK_ENTRY_COUNT; entryIndex++) {
		SummonBookEntry entry = SUMMON_BOOK_ENTRIES[entryIndex];
		Color graphicColor = book->entryAvailable[entryIndex] ? inkColor : ColorAlpha(inkColor, 0.1f);
		
		if (book->entryHover[entryIndex]) {
			graphicColor = (Color) { 227, 30, 30, 255 };
		}
		
		DrawTextureV(AssetsGetTexture(entry.graphicTexture), Vector2Add(Vector2Add(book->bookPosition, summonBookOffset), entry.graphicOffset), graphicColor);
		
#if SUMMON_BOOK_DEBUG_HOVER_RECTS
		Rectangle hoverRect = SummonBookGetEntryScreenHoverRect(book, entryIndex);
		DrawRectangleRec(hoverRect, ColorAlpha(RED, 0.5f));
#endif
	}
}
