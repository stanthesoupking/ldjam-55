
static inline f32 EaseInOutQuart(f32 x) {
	return x < 0.5 ? 8 * x * x * x * x : 1 - powf(-2 * x + 2, 4) / 2;
}

static inline f32 EaseInOutElastic(f32 x) {
	const f32 c5 = (2.0f * PI) / 4.5f;

	return x == 0.0f
	  ? 0.0f
	  : x == 1.0f
	  ? 1.0f
	  : x < 0.5f
	  ? -(powf(2.0f, 20.0f * x - 10.0f) * sinf((20.0f * x - 11.125f) * c5)) / 2.0f
	  : (powf(2.0f, -20.0f * x + 10.0f) * sinf((20.0f * x - 11.125f) * c5)) / 2.0f + 1.0f;
}

static inline f32 EaseInElastic(f32 x) {
	const f32 c4 = (2.0f * PI) / 3.0f;

	return x == 0.0f
	  ? 0.0f
	  : x == 1.0f
	  ? 1.0f
	  : -powf(2.0f, 10.0f * x - 10.0f) * sinf((x * 10.0f - 10.75f) * c4);
}

static inline f32 EaseOutElastic(f32 x) {
	const f32 c4 = (2.0f * PI) / 3.0f;

	return x == 0.0f
	  ? 0.0f
	  : x == 1.0f
	  ? 1.0f
	  : powf(2.0f, -10.0f * x) * sinf((x * 10.0f - 0.75f) * c4) + 1.0f;
}
