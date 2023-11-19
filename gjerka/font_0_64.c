#include "font.h"

#define IRBF_PAGE_COUNT 1
#define IRBF_BASE 26
#define IRBF_BASE_SCALE 32
#define IRBF_TEX_PAGE_SIZE 256
const static const char* pages[] = { "asset/fonts/1f32_0.bmp" };
static kerning ker32[] = { {65, -1},{0,0} };
static kerning ker49[] = { {49, -2},{0,0} };
static kerning ker65[] = { {32, -1},{84, -2},{86, -2},{87, -1},{89, -2},{0,0} };
static kerning ker70[] = { {44, -3},{46, -3},{65, -1},{0,0} };
static kerning ker76[] = { {32, -1},{84, -2},{86, -2},{87, -2},{89, -2},{121, -1},{0,0} };
static kerning ker80[] = { {44, -3},{46, -3},{65, -2},{0,0} };
static kerning ker84[] = { {44, -3},{45, -1},{46, -3},{58, -3},{65, -2},{97, -3},{99, -3},{101, -3},{105, -1},{111, -3},{114, -1},{115, -3},{117, -1},{119, -1},{121, -1},{0,0} };
static kerning ker86[] = { {44, -2},{45, -1},{46, -2},{58, -1},{65, -2},{97, -2},{101, -1},{111, -1},{114, -1},{117, -1},{121, -1},{0,0} };
static kerning ker87[] = { {97, -1},{65, -1},{46, -1},{44, -1},{0,0} };
static kerning ker89[] = { {118, -1},{117, -1},{113, -2},{112, -2},{111, -2},{105, -1},{101, -2},{97, -2},{65, -2},{58, -1},{46, -3},{45, -2},{44, -3},{0,0} };
static kerning ker114[] = { {46, -1},{44, -1},{0,0} };
static kerning ker118[] = { {46, -2},{44, -2},{0,0} };
static kerning ker119[] = { {46, -1},{44, -1},{0,0} };
static kerning ker121[] = { {46, -2},{44, -2},{0,0} };
static glyph gly32 = { 1, 1, 243, 23, 0, 31, 8, 0, ker32 };
static glyph gly33 = { 3, 20, 250, 69, 2, 6, 7, 0, NULL };
static glyph gly34 = { 8, 7, 27, 135, 1, 6, 10, 0, NULL };
static glyph gly35 = { 16, 20, 144, 71, 0, 6, 15, 0, NULL };
static glyph gly36 = { 16, 24, 17, 28, 0, 4, 15, 0, NULL };
static glyph gly37 = { 24, 20, 66, 50, 0, 6, 25, 0, NULL };
static glyph gly38 = { 18, 20, 0, 76, 0, 6, 19, 0, NULL };
static glyph gly39 = { 3, 7, 249, 107, 1, 6, 5, 0, NULL };
static glyph gly40 = { 8, 27, 20, 0, 1, 5, 9, 0, NULL };
static glyph gly41 = { 8, 27, 29, 0, 0, 5, 9, 0, NULL };
static glyph gly42 = { 11, 11, 15, 135, 0, 6, 11, 0, NULL };
static glyph gly43 = { 15, 15, 197, 111, 1, 9, 16, 0, NULL };
static glyph gly44 = { 3, 8, 245, 107, 2, 22, 8, 0, NULL };
static glyph gly45 = { 8, 3, 99, 130, 1, 17, 9, 0, NULL };
static glyph gly46 = { 3, 4, 68, 131, 2, 22, 8, 0, NULL };
static glyph gly47 = { 9, 21, 10, 54, 0, 5, 8, 0, NULL };
static glyph gly48 = { 14, 20, 153, 92, 1, 6, 15, 0, NULL };
static glyph gly49 = { 13, 20, 197, 90, 2, 6, 15, 0, ker49 };
static glyph gly50 = { 14, 20, 168, 91, 1, 6, 15, 0, NULL };
static glyph gly51 = { 14, 20, 78, 92, 1, 6, 15, 0, NULL };
static glyph gly52 = { 15, 20, 0, 97, 0, 6, 15, 0, NULL };
static glyph gly53 = { 14, 20, 63, 93, 1, 6, 15, 0, NULL };
static glyph gly54 = { 14, 20, 138, 92, 1, 6, 15, 0, NULL };
static glyph gly55 = { 14, 20, 123, 92, 1, 6, 15, 0, NULL };
static glyph gly56 = { 14, 20, 108, 92, 1, 6, 15, 0, NULL };
static glyph gly57 = { 14, 20, 93, 92, 1, 6, 15, 0, NULL };
static glyph gly58 = { 3, 16, 177, 112, 2, 10, 8, 0, NULL };
static glyph gly59 = { 3, 20, 252, 45, 2, 10, 8, 0, NULL };
static glyph gly60 = { 15, 15, 213, 111, 1, 9, 16, 0, NULL };
static glyph gly61 = { 15, 11, 229, 110, 1, 11, 16, 0, NULL };
static glyph gly62 = { 15, 15, 181, 112, 1, 9, 16, 0, NULL };
static glyph gly63 = { 14, 20, 48, 93, 1, 6, 15, 0, NULL };
static glyph gly64 = { 25, 25, 124, 0, 2, 5, 29, 0, NULL };
static glyph gly65 = { 19, 20, 173, 49, 0, 6, 19, 0, ker65 };
static glyph gly66 = { 16, 20, 161, 70, 2, 6, 19, 0, NULL };
static glyph gly67 = { 19, 20, 193, 48, 1, 6, 20, 0, NULL };
static glyph gly68 = { 17, 20, 91, 71, 2, 6, 20, 0, NULL };
static glyph gly69 = { 16, 20, 178, 70, 2, 6, 19, 0, NULL };
static glyph gly70 = { 15, 20, 16, 97, 2, 6, 17, 0, ker70 };
static glyph gly71 = { 19, 20, 133, 50, 1, 6, 22, 0, NULL };
static glyph gly72 = { 16, 20, 195, 69, 2, 6, 20, 0, NULL };
static glyph gly73 = { 3, 20, 246, 69, 2, 6, 8, 0, NULL };
static glyph gly74 = { 12, 20, 211, 90, 0, 6, 14, 0, NULL };
static glyph gly75 = { 17, 20, 37, 72, 2, 6, 19, 0, NULL };
static glyph gly76 = { 13, 20, 183, 91, 2, 6, 15, 0, ker76 };
static glyph gly77 = { 20, 20, 91, 50, 2, 6, 23, 0, NULL };
static glyph gly78 = { 16, 20, 212, 69, 2, 6, 20, 0, NULL };
static glyph gly79 = { 20, 20, 112, 50, 1, 6, 22, 0, NULL };
static glyph gly80 = { 16, 20, 229, 69, 2, 6, 19, 0, ker80 };
static glyph gly81 = { 20, 26, 86, 0, 1, 6, 22, 0, NULL };
static glyph gly82 = { 17, 20, 109, 71, 2, 6, 20, 0, NULL };
static glyph gly83 = { 17, 20, 55, 72, 1, 6, 19, 0, NULL };
static glyph gly84 = { 17, 20, 19, 76, 0, 6, 17, 0, ker84 };
static glyph gly85 = { 17, 20, 73, 71, 2, 6, 20, 0, NULL };
static glyph gly86 = { 19, 20, 213, 48, 0, 6, 19, 0, ker86 };
static glyph gly87 = { 28, 20, 37, 51, 0, 6, 27, 0, ker87 };
static glyph gly88 = { 19, 20, 153, 49, 0, 6, 19, 0, NULL };
static glyph gly89 = { 18, 20, 233, 48, 0, 6, 18, 0, ker89 };
static glyph gly90 = { 16, 20, 127, 71, 0, 6, 17, 0, NULL };
static glyph gly91 = { 7, 27, 38, 0, 1, 5, 8, 0, NULL };
static glyph gly92 = { 9, 21, 0, 54, 0, 5, 8, 0, NULL };
static glyph gly93 = { 7, 27, 46, 0, 0, 5, 8, 0, NULL };
static glyph gly94 = { 14, 11, 0, 135, 0, 6, 13, 0, NULL };
static glyph gly95 = { 17, 3, 72, 131, 0, 29, 15, 0, NULL };
static glyph gly96 = { 7, 5, 44, 135, 1, 4, 9, 0, NULL };
static glyph gly97 = { 15, 16, 22, 118, 1, 10, 15, 0, NULL };
static glyph gly98 = { 13, 21, 179, 26, 1, 5, 15, 0, NULL };
static glyph gly99 = { 13, 16, 99, 113, 1, 10, 14, 0, NULL };
static glyph gly100 = { 13, 21, 207, 26, 1, 5, 15, 0, NULL };
static glyph gly101 = { 14, 16, 54, 114, 1, 10, 15, 0, NULL };
static glyph gly102 = { 8, 21, 20, 53, 0, 5, 8, 0, NULL };
static glyph gly103 = { 13, 22, 137, 26, 1, 10, 15, 0, NULL };
static glyph gly104 = { 13, 21, 221, 26, 1, 5, 15, 0, NULL };
static glyph gly105 = { 3, 21, 29, 53, 1, 5, 6, 0, NULL };
static glyph gly106 = { 6, 27, 54, 0, -1, 5, 6, 0, NULL };
static glyph gly107 = { 13, 21, 193, 26, 1, 5, 14, 0, NULL };
static glyph gly108 = { 3, 21, 33, 53, 1, 5, 6, 0, NULL };
static glyph gly109 = { 21, 16, 233, 90, 1, 10, 24, 0, NULL };
static glyph gly110 = { 13, 16, 141, 113, 1, 10, 15, 0, NULL };
static glyph gly111 = { 14, 16, 84, 113, 1, 10, 15, 0, NULL };
static glyph gly112 = { 13, 22, 123, 27, 1, 10, 15, 0, NULL };
static glyph gly113 = { 13, 22, 109, 27, 1, 10, 15, 0, NULL };
static glyph gly114 = { 8, 16, 168, 112, 1, 10, 9, 0, ker114 };
static glyph gly115 = { 13, 16, 127, 113, 0, 10, 14, 0, NULL };
static glyph gly116 = { 8, 19, 224, 90, 0, 7, 8, 0, NULL };
static glyph gly117 = { 13, 16, 113, 113, 1, 10, 15, 0, NULL };
static glyph gly118 = { 15, 16, 38, 118, 0, 10, 14, 0, ker118 };
static glyph gly119 = { 21, 16, 0, 118, 0, 10, 20, 0, ker119 };
static glyph gly120 = { 14, 16, 69, 114, 0, 10, 14, 0, NULL };
static glyph gly121 = { 14, 22, 80, 27, 0, 10, 13, 0, ker121 };
static glyph gly122 = { 12, 16, 155, 113, 1, 10, 14, 0, NULL };
static glyph gly123 = { 9, 27, 0, 0, 0, 5, 9, 0, NULL };
static glyph gly124 = { 3, 27, 61, 0, 2, 5, 7, 0, NULL };
static glyph gly125 = { 9, 27, 10, 0, 0, 5, 9, 0, NULL };
static glyph gly126 = { 15, 4, 52, 135, 1, 14, 16, 0, NULL };
static glyph gly168 = { 8, 3, 37, 93, 0, 6, 9, 0, NULL };
static glyph gly173 = { 8, 3, 90, 130, 1, 17, 9, 0, NULL };
static glyph gly180 = { 7, 5, 36, 135, 0, 4, 9, 0, NULL };
static glyph gly211 = { 20, 25, 150, 0, 1, 1, 22, 0, NULL };
static glyph gly243 = { 14, 22, 65, 27, 1, 4, 15, 0, NULL };
static glyph gly260 = { 20, 26, 65, 0, 0, 6, 19, 0, NULL };
static glyph gly261 = { 15, 22, 34, 28, 1, 10, 15, 0, NULL };
static glyph gly262 = { 19, 25, 171, 0, 1, 1, 20, 0, NULL };
static glyph gly263 = { 13, 22, 95, 27, 1, 4, 14, 0, NULL };
static glyph gly280 = { 16, 26, 107, 0, 2, 6, 19, 0, NULL };
static glyph gly281 = { 14, 22, 50, 28, 1, 10, 15, 0, NULL };
static glyph gly321 = { 15, 20, 32, 97, 0, 6, 15, 0, NULL };
static glyph gly322 = { 7, 21, 248, 23, 0, 5, 6, 0, NULL };
static glyph gly323 = { 16, 25, 209, 0, 2, 1, 20, 0, NULL };
static glyph gly324 = { 13, 22, 151, 26, 1, 4, 15, 0, NULL };
static glyph gly346 = { 17, 25, 191, 0, 1, 1, 19, 0, NULL };
static glyph gly347 = { 13, 22, 165, 26, 0, 4, 14, 0, NULL };
static glyph gly377 = { 16, 25, 0, 28, 0, 1, 17, 0, NULL };
static glyph gly378 = { 12, 22, 243, 0, 1, 4, 14, 0, NULL };
static glyph gly379 = { 16, 25, 226, 0, 0, 1, 17, 0, NULL };
static glyph gly380 = { 12, 21, 235, 26, 1, 5, 14, 0, NULL };
static struct glyph* ascii_resolver[] = { &gly32,
&gly33,
&gly34,
&gly35,
&gly36,
&gly37,
&gly38,
&gly39,
&gly40,
&gly41,
&gly42,
&gly43,
&gly44,
&gly45,
&gly46,
&gly47,
&gly48,
&gly49,
&gly50,
&gly51,
&gly52,
&gly53,
&gly54,
&gly55,
&gly56,
&gly57,
&gly58,
&gly59,
&gly60,
&gly61,
&gly62,
&gly63,
&gly64,
&gly65,
&gly66,
&gly67,
&gly68,
&gly69,
&gly70,
&gly71,
&gly72,
&gly73,
&gly74,
&gly75,
&gly76,
&gly77,
&gly78,
&gly79,
&gly80,
&gly81,
&gly82,
&gly83,
&gly84,
&gly85,
&gly86,
&gly87,
&gly88,
&gly89,
&gly90,
&gly91,
&gly92,
&gly93,
&gly94,
&gly95,
&gly96,
&gly97,
&gly98,
&gly99,
&gly100,
&gly101,
&gly102,
&gly103,
&gly104,
&gly105,
&gly106,
&gly107,
&gly108,
&gly109,
&gly110,
&gly111,
&gly112,
&gly113,
&gly114,
&gly115,
&gly116,
&gly117,
&gly118,
&gly119,
&gly120,
&gly121,
&gly122,
&gly123,
&gly124,
&gly125,
&gly126 };
glyph* rbf_resolver1f64(int64_t glyphId) {
	if (glyphId >= 32 && glyphId < 127) return ascii_resolver[glyphId - 32]; else return ascii_resolver['?' - 32];
}
int64_t font_register0f64() {
	//id, base px scale, page size, page count, base, pages pathes, glyph resolver
	registerFont(0, 32, IRBF_TEX_PAGE_SIZE, IRBF_PAGE_COUNT, IRBF_BASE, pages, rbf_resolver1f64);
	return 1;
}