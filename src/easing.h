
static inline f32 EaseInOutQuart(f32 x) {
	return x < 0.5 ? 8 * x * x * x * x : 1 - powf(-2 * x + 2, 4) / 2;
}

static inline f32 EaseInOutElastic(f32 x) {
	const f32 c5 = (2 * PI) / 4.5;

	return x == 0.0f
	  ? 0
	  : x == 1.0f
	  ? 1
	  : x < 0.5
	  ? -(powf(2, 20 * x - 10) * sinf((20 * x - 11.125) * c5)) / 2
	  : (powf(2, -20 * x + 10) * sinf((20 * x - 11.125) * c5)) / 2 + 1;
}

static inline f32 EaseInElastic(f32 x) {
	const f32 c4 = (2 * PI) / 3;

	return x == 0.0f
	  ? 0
	  : x == 1.0f
	  ? 1
	  : -powf(2, 10 * x - 10) * sinf((x * 10 - 10.75) * c4);
}

static inline f32 EaseOutElastic(f32 x) {
	const f32 c4 = (2 * PI) / 3;

	return x == 0.0f
	  ? 0
	  : x == 1.0f
	  ? 1
	  : powf(2, -10 * x) * sinf((x * 10 - 0.75) * c4) + 1;
}
