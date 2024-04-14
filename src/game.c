
#define SEAT_COUNT 3
static f32 SEAT_POSITIONS[SEAT_COUNT] = {
	74.0f,
	119.0f,
	165.0f,
};

static f32 SEAT_START_X[SEAT_COUNT] = {
	47.0f,
	95.0f,
	140.0f,
};

static f32 SEAT_END_X[SEAT_COUNT] = {
	94.0f,
	139.0f,
	184.0f,
};

#define HAND_TRAIL_LENGTH 6
const Color HAND_TRAIL_COLORS[HAND_TRAIL_LENGTH] = {
	{ 0, 0, 0, 0 },
	{ 255, 82, 119, 255 },
	{ 0, 0, 0, 0 },
	{ 200, 212, 93, 255 },
	{ 0, 0, 0, 0 },
	{ 146, 232, 192, 255 },
};

#define TABLETOP_Y 70

typedef enum FoodEntryID {
	FOOD_ENTRY_ID_ROAST_CHICKEN,
	FOOD_ENTRY_ID_BURNT_ROAST_CHICKEN,
	FOOD_ENTRY_ID_COUNT,
} FoodEntryID;

typedef struct FoodEntry {
	FoodEntryID entryID;
	AssetTexture texture;
} FoodEntry;

const FoodEntry FOOD_ENTRIES[FOOD_ENTRY_ID_COUNT] = {
	[FOOD_ENTRY_ID_ROAST_CHICKEN] = {
		.entryID = FOOD_ENTRY_ID_ROAST_CHICKEN,
		.texture = ASSET_TEXTURE_RCHICKEN0
	},
	[FOOD_ENTRY_ID_BURNT_ROAST_CHICKEN] = {
		.entryID = FOOD_ENTRY_ID_BURNT_ROAST_CHICKEN,
		.texture = ASSET_TEXTURE_BRCHICKEN0
	},
};

typedef struct Food {
	FoodEntryID entryID;
	Vector2 position;
} Food;

static AssetTexture CUSTOMER_TEXTURES[] = {
	ASSET_TEXTURE_DWARF0,
	ASSET_TEXTURE_OGRE0,
	ASSET_TEXTURE_LADY0,
};

typedef enum CustomerState {
	CUSTOMER_STATE_INIT,
	CUSTOMER_STATE_WALKING_TO_SEAT,
	CUSTOMER_STATE_GETTING_ON_SEAT,
	CUSTOMER_STATE_LOOKING_AT_MENU,
	CUSTOMER_STATE_WAITING_FOR_MEAL,
	CUSTOMER_STATE_EATING_MEAL,
	CUSTOMER_STATE_GETTING_OFF_SEAT,
	CUSTOMER_STATE_WALKING_TO_EXIT,
	CUSTOMER_STATE_DEINIT,
} CustomerState;

typedef struct Customer {
	CustomerState state;
	Texture2D texture;
	
	f64 stateStartTime;
	f64 stateEndTime;
	
	Vector2 currentPosition;
	
	FoodEntryID desiredFoodID;
	
	u8 seat;
} Customer;

f32 CustomerGetTimeForState(CustomerState state) {
	switch (state) {
		case CUSTOMER_STATE_WALKING_TO_EXIT:
		case CUSTOMER_STATE_WALKING_TO_SEAT: {
			return 2.0f;
		} break;
			
		case CUSTOMER_STATE_GETTING_ON_SEAT:
		case CUSTOMER_STATE_GETTING_OFF_SEAT: {
			return 1.0f;
		} break;
			
		case CUSTOMER_STATE_LOOKING_AT_MENU: {
			return 3.0f;
		} break;
			
		case CUSTOMER_STATE_WAITING_FOR_MEAL:
		case CUSTOMER_STATE_EATING_MEAL: {
			return 1.0f;
		} break;
	
		case CUSTOMER_STATE_INIT:
		case CUSTOMER_STATE_DEINIT: {
			return 0.0f;
		} break;
	}
	return 0.0f;
}

Vector2 CustomerGetPositionForState(Customer* customer, CustomerState state) {
	const f32 floorY = 92.0f;
	const f32 seatX = SEAT_POSITIONS[customer->seat];
	const f32 seatY = 80.0f;
	switch (state) {
		case CUSTOMER_STATE_INIT: {
			return (Vector2) { -20.0f, floorY };
		} break;
			
		case CUSTOMER_STATE_WALKING_TO_SEAT: {
			return (Vector2) { seatX, floorY };
		} break;
			
		case CUSTOMER_STATE_GETTING_ON_SEAT:
		case CUSTOMER_STATE_LOOKING_AT_MENU:
		case CUSTOMER_STATE_WAITING_FOR_MEAL:
		case CUSTOMER_STATE_EATING_MEAL: {
			return (Vector2) { seatX, seatY };
		} break;
			
		case CUSTOMER_STATE_GETTING_OFF_SEAT: {
			return (Vector2) { seatX, floorY };
		} break;
			
		case CUSTOMER_STATE_WALKING_TO_EXIT:
		case CUSTOMER_STATE_DEINIT: {
			return (Vector2) { -20.0f, floorY };
		} break;
	}
	return Vector2Zero();
}

#define SUMMON_MAX_TEXTURE_COUNT 16
typedef struct Summon {
	AssetTexture textures[SUMMON_MAX_TEXTURE_COUNT];
	u32 textureCount;
	f64 animationStartTime;
	f32 animationSpeed;
	Vector2 position;
	f32 speed;
	bool cooking;
	f32 cookAmount;
	f32 cookDoneDuration;
	f32 cookBurntDuration;
	FoodEntryID onCookDoneProducesFoodID;
	FoodEntryID onCookBurntProducesFoodID;
	bool dead;
} Summon;

typedef enum SummonBookAnimationStateMode {
	SUMMON_BOOK_ANIMATION_STATE_MODE_ANIMATE_IN,
	SUMMON_BOOK_ANIMATION_STATE_MODE_ANIMATE_OUT,
} SummonBookAnimationStateMode;

typedef struct SummonBookAnimationState {
	SummonBookAnimationStateMode mode;
	f32 animationStartTime;
	f32 animationEndTime;
} SummonBookAnimationState;

typedef struct Game {
	RenderTexture2D targetTex;
	
	// customers/seats
	Customer* customers;
	u8* freeSeats;
	
	// summons
	Summon* summons;
	
	// foods
	Food* foods;
	
	// hand
	Vector2 handPosition;
	Vector2 synthesizedHandPosition;
	Vector2 handVelocity;
	Vector2 handPositionHistory[HAND_TRAIL_LENGTH];
	u32 handPositionHistoryHead;
	
	f64 handShakeTime;
	Vector2 handShakeVector;
	
	SummonBook summonBook;
	
	bool summonBookToggle;
	
	// 0 = hidden, 1 = shown
	f32 summonBookAmount;
	
	bool hoveringSummonBook;
	
	bool shootingFire;
	f32 shootingFireStartX;
	f32 shootingFireEndX;
	
	u32 gold;
} Game;

static Game game;

void GameTrySpawnCustomer(void) {
	if (arrlen(game.freeSeats) == 0) {
		return;
	}
	
	const i32 freeSeatIndex = GetRandomValue(0, (i32)arrlen(game.freeSeats) - 1);
	const u8 seat = game.freeSeats[freeSeatIndex];
	arrdel(game.freeSeats, freeSeatIndex);
	
	const u32 customerTextureIndex = GetRandomValue(0, (sizeof(CUSTOMER_TEXTURES) / sizeof(AssetTexture)) - 1);
	
	Customer customer = {
		.state = CUSTOMER_STATE_WALKING_TO_SEAT,
		.seat = seat,
		.stateStartTime = GetTime(),
		.stateEndTime = GetTime() + CustomerGetTimeForState(CUSTOMER_STATE_WALKING_TO_SEAT),
		.texture = AssetsGetTexture(CUSTOMER_TEXTURES[customerTextureIndex]),
		.desiredFoodID = FOOD_ENTRY_ID_ROAST_CHICKEN,
	};
	arrpush(game.customers, customer);
}

void GameInit(void) {
	game = (Game) { 0 };
	game.targetTex = LoadRenderTexture(SCREEN_WIDTH, SCREEN_HEIGHT);
	for (u8 seatIndex = 0; seatIndex < SEAT_COUNT; seatIndex++) {
		arrpush(game.freeSeats, seatIndex);
	}
	SummonBookInit(&game.summonBook);
	PlayMusicStream(AssetsGetMusic());
	GameTrySpawnCustomer();
}
void GameDeinit(void) {
	
}

// MARK: Game Update

void GameUpdate(void) {
	const f64 time = GetTime();
	const f32 dt = GetFrameTime();
	
	UpdateMusicStream(AssetsGetMusic());
	
	// update hand
	Vector2 handVector = { 0 };
	if (IsKeyDown(KEY_A)) {
		handVector.x -= 1.0f;
	}
	if (IsKeyDown(KEY_D)) {
		handVector.x += 1.0f;
	}
	if (IsKeyDown(KEY_W)) {
		handVector.y -= 1.0f;
	}
	if (IsKeyDown(KEY_S)) {
		handVector.y += 1.0f;
	}
	handVector.x += GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_X);
	handVector.y += GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_Y);
	f32 handVectorLength = Vector2Length(handVector);
	if (handVectorLength > 1.0f) {
		handVector = Vector2Scale(handVector, 1.0f / handVectorLength);
	}

	// TODO: X on controller
	if (IsKeyPressed(KEY_X) || IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_DOWN)) {
		// activate hovered item
		if (game.hoveringSummonBook) {
			PlaySound(AssetsGetSound(ASSET_SOUND_WOOSH));
			game.summonBookToggle = !game.summonBookToggle;
		}
	}
	
	// TODO: Circle on controller
	if (IsKeyPressed(KEY_C) || IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT)) {
		if (game.summonBookToggle) {
			PlaySound(AssetsGetSound(ASSET_SOUND_WOOSH));
			game.summonBookToggle = false;
		}
	}
	
	// TODO: Square on controller
	if ((IsKeyDown(KEY_V) || IsGamepadButtonDown(0, GAMEPAD_BUTTON_RIGHT_FACE_LEFT)) && (!game.summonBookToggle)) {
		Sound burnSound = AssetsGetSound(ASSET_SOUND_BURNING);
		if (!IsSoundPlaying(burnSound)) {
			PlaySound(burnSound);
		}
		
		game.shootingFire = true;
		const f32 shakeFrequency = 0.02f;
		if ((time - game.handShakeTime) > shakeFrequency) {
			game.handShakeVector = (Vector2) { (f32)GetRandomValue(-1, 1), (f32)GetRandomValue(-1, 1) };
			game.handShakeTime = time;
		}
		
		const f32 shootingFireRadius = 6.0f;
		game.shootingFireStartX = game.synthesizedHandPosition.x - shootingFireRadius;
		game.shootingFireEndX = game.synthesizedHandPosition.x + shootingFireRadius;
	} else {
		game.shootingFire = false;
		game.handShakeVector = Vector2Zero();
		StopSound(AssetsGetSound(ASSET_SOUND_BURNING));
	}

	if (Vector2LengthSqr(handVector) > 0.0f) {
		Vector2 handAcceleration = Vector2Scale(handVector, dt * 1500.0f);
		game.handVelocity = Vector2Add(game.handVelocity, handAcceleration);
	}
	game.handPosition = Vector2Add(Vector2Add(game.handPosition, Vector2Scale(game.handVelocity, dt)), (Vector2) { 0, (f32)sin(GetTime() * 4.0) * 0.25f });
	game.synthesizedHandPosition = Vector2Add(game.handPosition, game.handShakeVector);
	game.handVelocity = Vector2Scale(game.handVelocity, powf(0.001f, dt));
	game.handPositionHistoryHead = (game.handPositionHistoryHead + 1) % HAND_TRAIL_LENGTH;
	game.handPositionHistory[game.handPositionHistoryHead] = game.synthesizedHandPosition;
	
	// spawn a customer every ~200 frames
	if (GetRandomValue(0, 200) == 0) {
		GameTrySpawnCustomer();
	}
	
	// update customers
	for (u32 customerIdx = 0; customerIdx < arrlen(game.customers); customerIdx++) {
		Customer* customer = &game.customers[customerIdx];
		
		i32 interestedFoodIndex = -1;
		for (u32 foodIdx = 0; foodIdx < arrlen(game.foods); foodIdx++) {
			Food* food = &game.foods[foodIdx];
			if (food->entryID == customer->desiredFoodID) {
				if ((food->position.x >= SEAT_START_X[customer->seat]) && (food->position.x <= SEAT_END_X[customer->seat])) {
					interestedFoodIndex = foodIdx;
					break;
				}
			}
		}
		
		switch (customer->state) {
			case CUSTOMER_STATE_INIT:
			case CUSTOMER_STATE_WALKING_TO_SEAT:
			case CUSTOMER_STATE_GETTING_ON_SEAT:
			case CUSTOMER_STATE_LOOKING_AT_MENU:
			case CUSTOMER_STATE_GETTING_OFF_SEAT:
			case CUSTOMER_STATE_WALKING_TO_EXIT:
			case CUSTOMER_STATE_DEINIT: {
				if (time > customer->stateEndTime) {
					// advance state
					customer->state++;
					customer->stateStartTime = time;
					customer->stateEndTime = time + CustomerGetTimeForState(customer->state);
					
					// seat can be made available now
					if (customer->state == CUSTOMER_STATE_WALKING_TO_EXIT) {
						arrpush(game.freeSeats, customer->seat);
					}
				}
			} break;
				
			case CUSTOMER_STATE_WAITING_FOR_MEAL: {
				// check if meal is ready
				if (interestedFoodIndex != -1) {
					customer->state = CUSTOMER_STATE_EATING_MEAL;
					customer->stateStartTime = time;
					customer->stateEndTime = time + CustomerGetTimeForState(customer->state);
				}
			} break;
				
			case CUSTOMER_STATE_EATING_MEAL: {
				if (time > customer->stateEndTime) {
					PlaySound(AssetsGetSound(ASSET_SOUND_CRUNCH));
					PlaySound(AssetsGetSound(ASSET_SOUND_COIN0 + GetRandomValue(0, 2)));
					
					game.gold += 10;
					
					if (interestedFoodIndex != -1) {
						arrdel(game.foods, interestedFoodIndex);
					}
					customer->state = CUSTOMER_STATE_GETTING_OFF_SEAT;
					customer->stateStartTime = time;
					customer->stateEndTime = time + CustomerGetTimeForState(customer->state);
				}
			} break;
		}
		
		
		Vector2 positionA = CustomerGetPositionForState(customer, customer->state - 1);
		Vector2 positionB = CustomerGetPositionForState(customer, customer->state);
		f32 animationAmount = (f32)((time - customer->stateStartTime) / (customer->stateEndTime - customer->stateStartTime));
		customer->currentPosition = Vector2Lerp(positionA, positionB, EaseInOutQuart(animationAmount));
	}
	
	// remove customers
	{
		bool removed = true;
		while (removed) {
			removed = false;
			for (u32 customerIdx = 0; customerIdx < arrlen(game.customers); customerIdx++) {
				Customer* customer = &game.customers[customerIdx];
				if (customer->state == CUSTOMER_STATE_DEINIT) {
					arrdel(game.customers, customerIdx);
					removed = true;
					break;
				}
			}
		}
	}
	
	// update summons
	for (u32 summonIdx = 0; summonIdx < arrlen(game.summons); summonIdx++) {
		Summon* summon = &game.summons[summonIdx];
		const Texture2D summonTexture = AssetsGetTexture(summon->textures[0]);
		
		const f32 summonStartX = summon->position.x - ((f32)summonTexture.width/2.0f);
		const f32 summonEndX = summon->position.x + ((f32)summonTexture.width/2.0f);
		
		bool wasCooking = summon->cooking;
		summon->cooking = (game.shootingFire && RangeIntersects(game.shootingFireStartX, game.shootingFireEndX, summonStartX, summonEndX) && (summon->cookAmount < summon->cookBurntDuration + 1.0f));
		
		summon->position.x -= summon->speed * dt;
		
		if (summon->cooking) {
			summon->cookAmount += dt;
		} else {
			summon->cookAmount -= dt;
		}
		summon->cookAmount = ld_max(0.0f, summon->cookAmount);
		
		if ((wasCooking && !summon->cooking) && (summon->cookAmount >= summon->cookDoneDuration)) {
			if (summon->cookAmount >= summon->cookBurntDuration) {
				// produce burnt item
				const Food food = { .position = (Vector2) { summon->position.x, summon->position.y + 1 }, .entryID = summon->onCookBurntProducesFoodID };
				arrpush(game.foods, food);
			} else {
				// produce food item
				const Food food = { .position = (Vector2) { summon->position.x, summon->position.y + 1 }, .entryID = summon->onCookDoneProducesFoodID };
				arrpush(game.foods, food);
			}
			summon->dead = true;
			PlaySound(AssetsGetSound(ASSET_SOUND_CHICKEN_DEAD));
		}
	}
	
	// remove summons
	{
		bool removed = true;
		while (removed) {
			removed = false;
			for (u32 summonIdx = 0; summonIdx < arrlen(game.summons); summonIdx++) {
				Summon* summon = &game.summons[summonIdx];
				if (summon->dead) {
					arrdel(game.summons, summonIdx);
					removed = true;
					break;
				}
			}
		}
	}
	
	// update summon book
	{
		const f32 summonBookAnimationDuration = 0.5f;
		if (game.summonBookToggle) {
			game.summonBookAmount += (dt / summonBookAnimationDuration);
		} else {
			game.summonBookAmount -= (dt / summonBookAnimationDuration);
		}
		game.summonBookAmount = Clamp(game.summonBookAmount, 0.0f, 1.0f);
		
		Vector2 bookStartPosition = { (f32)(SCREEN_WIDTH/2), (f32)(SCREEN_HEIGHT + (AssetsGetTexture(ASSET_TEXTURE_SUMMON_BOOK).height / 2)) };
		Vector2 bookEndPosition = { (f32)(SCREEN_WIDTH/2), (f32)((SCREEN_HEIGHT/2) - 8) };
		Vector2 bookPosition = Vector2Lerp(bookStartPosition, bookEndPosition, EaseInOutQuart(game.summonBookAmount));
		
		SummonBookUpdate(&game.summonBook, game.summonBookToggle, bookPosition, game.handPosition);
	}
	
	game.hoveringSummonBook = (game.handPosition.x > 185.0f) && (game.handPosition.x < 235.0f) && !game.summonBookToggle;
}

Vector2 GameGetMousePosition(void) {
	f32 renderWidth = (f32)GetScreenWidth();
	f32 renderHeight = (f32)GetScreenHeight();
	f32 scaleRatioW = (f32)renderWidth / (f32)SCREEN_WIDTH;
	f32 scaleRatioH = (f32)renderHeight / (f32)SCREEN_HEIGHT;
	f32 scaleRatio = ld_min(scaleRatioW, scaleRatioH);
	Vector2 mouseOffset = { (renderWidth * 0.5f) - (scaleRatio * SCREEN_WIDTH * 0.5f), (renderHeight * 0.5f) - (scaleRatio * SCREEN_HEIGHT * 0.5f) };
	Vector2 originalMousePosition = GetMousePosition();
	return Vector2Scale(Vector2Subtract(originalMousePosition, mouseOffset), 1.0f / scaleRatio);
}

// MARK: Game Render

void GameRender(void) {
	const f64 time = GetTime();

	BeginTextureMode(game.targetTex);
	{
		DrawTexture(AssetsGetTexture(ASSET_TEXTURE_WALL), 0, 0, WHITE);
		
		// draw customers
		for (u32 customerIdx = 0; customerIdx < arrlen(game.customers); customerIdx++) {
			Customer* customer = &game.customers[customerIdx];
			
			Vector2 characterOffset = { floorf((f32)-customer->texture.width * 0.5f), (f32)-(customer->texture.height - 1) };
			DrawTextureV(customer->texture, Vector2Add(customer->currentPosition, characterOffset), WHITE);
			
			if (customer->state == CUSTOMER_STATE_LOOKING_AT_MENU) {
				Vector2 thinkingOffset = { -10.0f, (f32)-(customer->texture.height + 14) };
				DrawTextureV(AssetsGetTexture(ASSET_TEXTURE_THINKING), Vector2Add(customer->currentPosition, thinkingOffset), WHITE);
			}
			if (customer->state == CUSTOMER_STATE_WAITING_FOR_MEAL) {
				Vector2 speechOffset = { 3.0f, (f32)-(customer->texture.height + 9) };
				DrawTextureCenteredV(AssetsGetTexture(ASSET_TEXTURE_SPEECH), Vector2Add(customer->currentPosition, speechOffset), WHITE);
				
				FoodEntry food_entry = FOOD_ENTRIES[customer->desiredFoodID];
				DrawTextureCenteredV(AssetsGetTexture(food_entry.texture), Vector2Add(customer->currentPosition, speechOffset), WHITE);
			}
		}
	
		DrawTexture(AssetsGetTexture(ASSET_TEXTURE_SHRINE0 + fabsf(roundf((f32)sin(GetTime() * 5.0) * 2.0f))), 190, 54, WHITE);
		DrawTexture(AssetsGetTexture(ASSET_TEXTURE_COUNTER), 0, 69, WHITE);
		
		// draw summon book prompt
		if (game.hoveringSummonBook) {
			DrawTextEx(AssetsGetFont(), "summon", (Vector2) { 193, 77 }, (f32)AssetsGetFont().baseSize, 0.0f, BLACK);
			DrawTextEx(AssetsGetFont(), "summon", (Vector2) { 193, 76 }, (f32)AssetsGetFont().baseSize, 0.0f, WHITE);
			DrawTexture(AssetsGetTexture(ASSET_TEXTURE_BUTTON_X), 202, 46, WHITE);
		}
		
		// draw summons
		for (u32 summonIdx = 0; summonIdx < arrlen(game.summons); summonIdx++) {
			Summon* summon = &game.summons[summonIdx];
			u32 frame = (u32)((time - summon->animationStartTime) * summon->animationSpeed) % summon->textureCount;
			Texture2D summonTexture = AssetsGetTexture(summon->textures[frame]);
			DrawTextureV(summonTexture, Vector2Add(summon->position, (Vector2) { (f32)-summonTexture.width/2, (f32)-summonTexture.height }), WHITE);
		}
		
		// draw foods
		for (u32 foodIdx = 0; foodIdx < arrlen(game.foods); foodIdx++) {
			Food* food = &game.foods[foodIdx];
			const FoodEntry* foodEntry = &FOOD_ENTRIES[food->entryID];
			Texture2D foodTexture = AssetsGetTexture(foodEntry->texture);
			DrawTextureV(foodTexture, Vector2Add(food->position, (Vector2) { (f32)-foodTexture.width/2, (f32)-foodTexture.height }), WHITE);
		}
		
		// draw flame
		if (game.shootingFire) {
			DrawRectangle((i32)(game.synthesizedHandPosition.x - 3.0f), (i32)floorf(game.synthesizedHandPosition.y), 6, (i32)floorf(TABLETOP_Y - game.synthesizedHandPosition.y), (Color) { 255, 137, 51, 255 });
			DrawRectangle((i32)(game.synthesizedHandPosition.x - 2.0f), (i32)floorf(game.synthesizedHandPosition.y), 4, (i32)floorf(TABLETOP_Y - game.synthesizedHandPosition.y), (Color) { 255, 238, 131, 255 });
		}
		
		// draw summon cook bars
		for (u32 summonIdx = 0; summonIdx < arrlen(game.summons); summonIdx++) {
			Summon* summon = &game.summons[summonIdx];
			if (summon->cookAmount > 0.0f) {
				const Vector2 meterOffset = (Vector2) { 0.0f, 10.0f };
				const f32 meterWidth = 27.0f;
				const Color cookedColor = { 128, 156, 97, 255 };
				const Color burntColor = { 170, 108, 108, 255 };
				const f32 totalRange = summon->cookBurntDuration + 1.0f;
				
				i32 amountLineX = (i32)roundf(meterOffset.x + ((summon->cookAmount / totalRange) * meterWidth));
				i32 cookedX = (i32)roundf(meterOffset.x + ((summon->cookDoneDuration / totalRange) * meterWidth));
				i32 burntX = (i32)roundf(meterOffset.x + ((summon->cookBurntDuration / totalRange) * meterWidth));
				
				DrawTextureCenteredV(AssetsGetTexture(ASSET_TEXTURE_METER_BOTTOM), Vector2Add(summon->position, meterOffset), WHITE);
				DrawRectangle(summon->position.x + cookedX - (meterWidth / 2.0f), meterOffset.y + summon->position.y - 2, meterWidth - cookedX + 1, 5, cookedColor);
				DrawRectangle(summon->position.x + burntX - (meterWidth / 2.0f), meterOffset.y + summon->position.y - 2, meterWidth - burntX + 1, 5, burntColor);
				DrawLine(summon->position.x + amountLineX - (meterWidth / 2.0f), summon->position.y + meterOffset.y - 2, summon->position.x + amountLineX - (meterWidth / 2.0f), summon->position.y + meterOffset.y + 3, RED);
				DrawTextureCenteredV(AssetsGetTexture(ASSET_TEXTURE_METER_TOP), Vector2Add(summon->position, meterOffset), WHITE);
			}
		}
		
		// draw summon book
		if (game.summonBookAmount > 0.0f) {
			f32 startOpacity = 0.0f;
			f32 endOpacity = 0.4f;
			f32 opacity = Lerp(startOpacity, endOpacity, EaseInOutQuart(game.summonBookAmount));
			DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, ColorAlpha((Color) { 48, 44, 46, 255 }, opacity));
			
			SummonBookDraw(&game.summonBook);
			
			Color actionsTint = game.summonBookToggle ? WHITE : BLANK;
			DrawTextDefault("select", (Vector2) { 36, 128 }, actionsTint);
			DrawTextureCenteredV(AssetsGetTexture(ASSET_TEXTURE_BUTTON_X), (Vector2) { 20, 132 }, actionsTint);
			
			DrawTextDefault("leave", (Vector2) { 200, 128 }, actionsTint);
			DrawTextureCenteredV(AssetsGetTexture(ASSET_TEXTURE_BUTTON_CIRCLE), (Vector2) { 184, 132 }, actionsTint);
		}
		
		// draw gold amount
		if (!game.summonBookToggle) {
			DrawTexture(AssetsGetTexture(ASSET_TEXTURE_GOLD_ICON), 8, 128, WHITE);
			char goldText[32];
			sprintf(goldText, "%u", game.gold);
			DrawTextDefault(goldText, (Vector2) { 19, 128 }, (Color) { 241, 233, 230, 255 });
		}
		
		// draw hand (always on top)
		{
			AssetTexture currentHandTexture = game.shootingFire ? ASSET_TEXTURE_HAND1 : ASSET_TEXTURE_HAND0;
			AssetTexture currentHandTrailTexture = game.shootingFire ? ASSET_TEXTURE_HAND1_TRAIL : ASSET_TEXTURE_HAND0_TRAIL;
			for (u32 handTrailIndex = 0; handTrailIndex < HAND_TRAIL_LENGTH; handTrailIndex++) {
				const Vector2 trailPosition = game.handPositionHistory[(game.handPositionHistoryHead + handTrailIndex) % HAND_TRAIL_LENGTH];
				const f32 trailAlpha = ld_min(Vector2Distance(game.handPosition, trailPosition) / 4.0f, 1.0f);
				Color trailColor = HAND_TRAIL_COLORS[handTrailIndex];
				trailColor.a = (u8)((f32)trailColor.a * trailAlpha);
				DrawTextureCenteredV(AssetsGetTexture(currentHandTrailTexture), game.handPositionHistory[(game.handPositionHistoryHead + handTrailIndex) % HAND_TRAIL_LENGTH], trailColor);
			}
			DrawTextureCenteredV(AssetsGetTexture(currentHandTexture), game.synthesizedHandPosition, WHITE);
		}
	}
	EndTextureMode();
	
	BeginDrawing();
	ClearBackground(BLACK);
	{
		f32 renderWidth = (f32)GetScreenWidth();
		f32 renderHeight = (f32)GetScreenHeight();
		f32 scaleRatioW = (f32)renderWidth / (f32)SCREEN_WIDTH;
		f32 scaleRatioH = (f32)renderHeight / (f32)SCREEN_HEIGHT;
		f32 scaleRatio = ld_min(scaleRatioW, scaleRatioH);
		
		Rectangle srcRec = {
			.x = 0,
			.y = 0,
			.width = SCREEN_WIDTH,
			.height = -SCREEN_HEIGHT,
		};
		Rectangle dstRec = {
			.x = (renderWidth * 0.5f) - (scaleRatio * SCREEN_WIDTH * 0.5f),
			.y = (renderHeight * 0.5f) - (scaleRatio * SCREEN_HEIGHT * 0.5f),
			.width = SCREEN_WIDTH * scaleRatio,
			.height = SCREEN_HEIGHT * scaleRatio,
		};
		Vector2 origin = {
			.x = 0,
			.y = 0,
		};
		DrawTexturePro(game.targetTex.texture, srcRec, dstRec, origin, 0.0f, WHITE);
	}
	EndDrawing();
}

void GameSummonChicken(void) {
	const Summon summon = {
		.textures = { ASSET_TEXTURE_CHICKEN0, ASSET_TEXTURE_CHICKEN0, ASSET_TEXTURE_CHICKEN0, ASSET_TEXTURE_CHICKEN0, ASSET_TEXTURE_CHICKEN2, ASSET_TEXTURE_CHICKEN0, ASSET_TEXTURE_CHICKEN0, ASSET_TEXTURE_CHICKEN0, ASSET_TEXTURE_CHICKEN1, ASSET_TEXTURE_CHICKEN0, ASSET_TEXTURE_CHICKEN1 },
		.textureCount = 11,
		.animationStartTime = GetTime(),
		.animationSpeed = 6.0f,
		.speed = 16.0f,
		.position = (Vector2) { 208, 69 },
		.cookAmount = 0.0f,
		.cookDoneDuration = 1.5f,
		.cookBurntDuration = 2.25f,
		.onCookDoneProducesFoodID = FOOD_ENTRY_ID_ROAST_CHICKEN,
		.onCookBurntProducesFoodID = FOOD_ENTRY_ID_BURNT_ROAST_CHICKEN,
	};
	arrpush(game.summons, summon);
	PlaySound(AssetsGetSound(ASSET_SOUND_CHICKEN_SUMMON));
	game.summonBookToggle = false;
}
