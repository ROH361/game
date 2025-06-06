#include "pch.h"
#include "square.h"

NAMESPACE_BEGIN(CryptoPP)

const byte Square::Enc::Se[256] = {
177, 206, 195, 149,  90, 173, 231,   2,  77,  68, 251, 145,  12, 135, 161,  80,
203, 103,  84, 221,  70, 143, 225,  78, 240, 253, 252, 235, 249, 196,  26, 110,
 94, 245, 204, 141,  28,  86,  67, 254,   7,  97, 248, 117,  89, 255,   3,  34,
138, 209,  19, 238, 136,   0,  14,  52,  21, 128, 148, 227, 237, 181,  83,  35,
 75,  71,  23, 167, 144,  53, 171, 216, 184, 223,  79,  87, 154, 146, 219,  27,
 60, 200, 153,   4, 142, 224, 215, 125, 133, 187,  64,  44,  58,  69, 241,  66,
101,  32,  65,  24, 114,  37, 147, 112,  54,   5, 242,  11, 163, 121, 236,   8,
 39,  49,  50, 182, 124, 176,  10, 115,  91, 123, 183, 129, 210,  13, 106,  38,
158,  88, 156, 131, 116, 179, 172,  48, 122, 105, 119,  15, 174,  33, 222, 208,
 46, 151,  16, 164, 152, 168, 212, 104,  45,  98,  41, 109,  22,  73, 118, 199,
232, 193, 150,  55, 229, 202, 244, 233,  99,  18, 194, 166,  20, 188, 211,  40,
175,  47, 230,  36,  82, 198, 160,   9, 189, 140, 207,  93,  17,  95,   1, 197,
159,  61, 162, 155, 201,  59, 190,  81,  25,  31,  63,  92, 178, 239,  74, 205,
191, 186, 111, 100, 217, 243,  62, 180, 170, 220, 213,   6, 192, 126, 246, 102,
108, 132, 113,  56, 185,  29, 127, 157,  72, 139,  42, 218, 165,  51, 130,  57,
214, 120, 134, 250, 228,  43, 169,  30, 137,  96, 107, 234,  85,  76, 247, 226,
};

const byte Square::Dec::Sd[256] = {
 53, 190,   7,  46,  83, 105, 219,  40, 111, 183, 118, 107,  12, 125,  54, 139,
146, 188, 169,  50, 172,  56, 156,  66,  99, 200,  30,  79,  36, 229, 247, 201,
 97, 141,  47,  63, 179, 101, 127, 112, 175, 154, 234, 245,  91, 152, 144, 177,
135, 113, 114, 237,  55,  69, 104, 163, 227, 239,  92, 197,  80, 193, 214, 202,
 90,  98,  95,  38,   9,  93,  20,  65, 232, 157, 206,  64, 253,   8,  23,  74,
 15, 199, 180,  62,  18, 252,  37,  75, 129,  44,   4, 120, 203, 187,  32, 189,
249,  41, 153, 168, 211,  96, 223,  17, 151, 137, 126, 250, 224, 155,  31, 210,
103, 226, 100, 119, 132,  43, 158, 138, 241, 109, 136, 121, 116,  87, 221, 230,
 57, 123, 238, 131, 225,  88, 242,  13,  52, 248,  48, 233, 185,  35,  84,  21,
 68,  11,  77, 102,  58,   3, 162, 145, 148,  82,  76, 195, 130, 231, 128, 192,
182,  14, 194, 108, 147, 236, 171,  67, 149, 246, 216,  70, 134,   5, 140, 176,
117,   0, 204, 133, 215,  61, 115, 122,  72, 228, 209,  89, 173, 184, 198, 208,
220, 161, 170,   2,  29, 191, 181, 159,  81, 196, 165,  16,  34, 207,   1, 186,
143,  49, 124, 174, 150, 218, 240,  86,  71, 212, 235,  78, 217,  19, 142,  73,
 85,  22, 255,  59, 244, 164, 178,   6, 160, 167, 251,  27, 110,  60,  51, 205,
 24,  94, 106, 213, 166,  33, 222, 254,  42,  28, 243,  10,  26,  25,  39,  45,
};

const word32 Square::Enc::Te[4][256] = {
{
0x97b1b126UL, 0x69cecea7UL, 0x73c3c3b0UL, 0xdf95954aUL,
0xb45a5aeeUL, 0xafadad02UL, 0x3be7e7dcUL, 0x04020206UL,
0x9a4d4dd7UL, 0x884444ccUL, 0x03fbfbf8UL, 0xd7919146UL,
0x180c0c14UL, 0xfb87877cUL, 0xb7a1a116UL, 0xa05050f0UL,
0x63cbcba8UL, 0xce6767a9UL, 0xa85454fcUL, 0x4fdddd92UL,
0x8c4646caUL, 0xeb8f8f64UL, 0x37e1e1d6UL, 0x9c4e4ed2UL,
0x15f0f0e5UL, 0x0ffdfdf2UL, 0x0dfcfcf1UL, 0x23ebebc8UL,
0x07f9f9feUL, 0x7dc4c4b9UL, 0x341a1a2eUL, 0xdc6e6eb2UL,
0xbc5e5ee2UL, 0x1ff5f5eaUL, 0x6dcccca1UL, 0xef8d8d62UL,
0x381c1c24UL, 0xac5656faUL, 0x864343c5UL, 0x09fefef7UL,
0x0e070709UL, 0xc26161a3UL, 0x05f8f8fdUL, 0xea75759fUL,
0xb25959ebUL, 0x0bfffff4UL, 0x06030305UL, 0x44222266UL,
0xe18a8a6bUL, 0x57d1d186UL, 0x26131335UL, 0x29eeeec7UL,
0xe588886dUL, 0x00000000UL, 0x1c0e0e12UL, 0x6834345cUL,
0x2a15153fUL, 0xf5808075UL, 0xdd949449UL, 0x33e3e3d0UL,
0x2fededc2UL, 0x9fb5b52aUL, 0xa65353f5UL, 0x46232365UL,
0x964b4bddUL, 0x8e4747c9UL, 0x2e171739UL, 0xbba7a71cUL,
0xd5909045UL, 0x6a35355fUL, 0xa3abab08UL, 0x45d8d89dUL,
0x85b8b83dUL, 0x4bdfdf94UL, 0x9e4f4fd1UL, 0xae5757f9UL,
0xc19a9a5bUL, 0xd1929243UL, 0x43dbdb98UL, 0x361b1b2dUL,
0x783c3c44UL, 0x65c8c8adUL, 0xc799995eUL, 0x0804040cUL,
0xe98e8e67UL, 0x35e0e0d5UL, 0x5bd7d78cUL, 0xfa7d7d87UL,
0xff85857aUL, 0x83bbbb38UL, 0x804040c0UL, 0x582c2c74UL,
0x743a3a4eUL, 0x8a4545cfUL, 0x17f1f1e6UL, 0x844242c6UL,
0xca6565afUL, 0x40202060UL, 0x824141c3UL, 0x30181828UL,
0xe4727296UL, 0x4a25256fUL, 0xd3939340UL, 0xe0707090UL,
0x6c36365aUL, 0x0a05050fUL, 0x11f2f2e3UL, 0x160b0b1dUL,
0xb3a3a310UL, 0xf279798bUL, 0x2dececc1UL, 0x10080818UL,
0x4e272769UL, 0x62313153UL, 0x64323256UL, 0x99b6b62fUL,
0xf87c7c84UL, 0x95b0b025UL, 0x140a0a1eUL, 0xe6737395UL,
0xb65b5bedUL, 0xf67b7b8dUL, 0x9bb7b72cUL, 0xf7818176UL,
0x51d2d283UL, 0x1a0d0d17UL, 0xd46a6abeUL, 0x4c26266aUL,
0xc99e9e57UL, 0xb05858e8UL, 0xcd9c9c51UL, 0xf3838370UL,
0xe874749cUL, 0x93b3b320UL, 0xadacac01UL, 0x60303050UL,
0xf47a7a8eUL, 0xd26969bbUL, 0xee777799UL, 0x1e0f0f11UL,
0xa9aeae07UL, 0x42212163UL, 0x49dede97UL, 0x55d0d085UL,
0x5c2e2e72UL, 0xdb97974cUL, 0x20101030UL, 0xbda4a419UL,
0xc598985dUL, 0xa5a8a80dUL, 0x5dd4d489UL, 0xd06868b8UL,
0x5a2d2d77UL, 0xc46262a6UL, 0x5229297bUL, 0xda6d6db7UL,
0x2c16163aUL, 0x924949dbUL, 0xec76769aUL, 0x7bc7c7bcUL,
0x25e8e8cdUL, 0x77c1c1b6UL, 0xd996964fUL, 0x6e373759UL,
0x3fe5e5daUL, 0x61cacaabUL, 0x1df4f4e9UL, 0x27e9e9ceUL,
0xc66363a5UL, 0x24121236UL, 0x71c2c2b3UL, 0xb9a6a61fUL,
0x2814143cUL, 0x8dbcbc31UL, 0x53d3d380UL, 0x50282878UL,
0xabafaf04UL, 0x5e2f2f71UL, 0x39e6e6dfUL, 0x4824246cUL,
0xa45252f6UL, 0x79c6c6bfUL, 0xb5a0a015UL, 0x1209091bUL,
0x8fbdbd32UL, 0xed8c8c61UL, 0x6bcfcfa4UL, 0xba5d5de7UL,
0x22111133UL, 0xbe5f5fe1UL, 0x02010103UL, 0x7fc5c5baUL,
0xcb9f9f54UL, 0x7a3d3d47UL, 0xb1a2a213UL, 0xc39b9b58UL,
0x67c9c9aeUL, 0x763b3b4dUL, 0x89bebe37UL, 0xa25151f3UL,
0x3219192bUL, 0x3e1f1f21UL, 0x7e3f3f41UL, 0xb85c5ce4UL,
0x91b2b223UL, 0x2befefc4UL, 0x944a4adeUL, 0x6fcdcda2UL,
0x8bbfbf34UL, 0x81baba3bUL, 0xde6f6fb1UL, 0xc86464acUL,
0x47d9d99eUL, 0x13f3f3e0UL, 0x7c3e3e42UL, 0x9db4b429UL,
0xa1aaaa0bUL, 0x4ddcdc91UL, 0x5fd5d58aUL, 0x0c06060aUL,
0x75c0c0b5UL, 0xfc7e7e82UL, 0x19f6f6efUL, 0xcc6666aaUL,
0xd86c6cb4UL, 0xfd848479UL, 0xe2717193UL, 0x70383848UL,
0x87b9b93eUL, 0x3a1d1d27UL, 0xfe7f7f81UL, 0xcf9d9d52UL,
0x904848d8UL, 0xe38b8b68UL, 0x542a2a7eUL, 0x41dada9bUL,
0xbfa5a51aUL, 0x66333355UL, 0xf1828273UL, 0x7239394bUL,
0x59d6d68fUL, 0xf0787888UL, 0xf986867fUL, 0x01fafafbUL,
0x3de4e4d9UL, 0x562b2b7dUL, 0xa7a9a90eUL, 0x3c1e1e22UL,
0xe789896eUL, 0xc06060a0UL, 0xd66b6bbdUL, 0x21eaeacbUL,
0xaa5555ffUL, 0x984c4cd4UL, 0x1bf7f7ecUL, 0x31e2e2d3UL,
},

{
0x2697b1b1UL, 0xa769ceceUL, 0xb073c3c3UL, 0x4adf9595UL,
0xeeb45a5aUL, 0x02afadadUL, 0xdc3be7e7UL, 0x06040202UL,
0xd79a4d4dUL, 0xcc884444UL, 0xf803fbfbUL, 0x46d79191UL,
0x14180c0cUL, 0x7cfb8787UL, 0x16b7a1a1UL, 0xf0a05050UL,
0xa863cbcbUL, 0xa9ce6767UL, 0xfca85454UL, 0x924fddddUL,
0xca8c4646UL, 0x64eb8f8fUL, 0xd637e1e1UL, 0xd29c4e4eUL,
0xe515f0f0UL, 0xf20ffdfdUL, 0xf10dfcfcUL, 0xc823ebebUL,
0xfe07f9f9UL, 0xb97dc4c4UL, 0x2e341a1aUL, 0xb2dc6e6eUL,
0xe2bc5e5eUL, 0xea1ff5f5UL, 0xa16dccccUL, 0x62ef8d8dUL,
0x24381c1cUL, 0xfaac5656UL, 0xc5864343UL, 0xf709fefeUL,
0x090e0707UL, 0xa3c26161UL, 0xfd05f8f8UL, 0x9fea7575UL,
0xebb25959UL, 0xf40bffffUL, 0x05060303UL, 0x66442222UL,
0x6be18a8aUL, 0x8657d1d1UL, 0x35261313UL, 0xc729eeeeUL,
0x6de58888UL, 0x00000000UL, 0x121c0e0eUL, 0x5c683434UL,
0x3f2a1515UL, 0x75f58080UL, 0x49dd9494UL, 0xd033e3e3UL,
0xc22fededUL, 0x2a9fb5b5UL, 0xf5a65353UL, 0x65462323UL,
0xdd964b4bUL, 0xc98e4747UL, 0x392e1717UL, 0x1cbba7a7UL,
0x45d59090UL, 0x5f6a3535UL, 0x08a3ababUL, 0x9d45d8d8UL,
0x3d85b8b8UL, 0x944bdfdfUL, 0xd19e4f4fUL, 0xf9ae5757UL,
0x5bc19a9aUL, 0x43d19292UL, 0x9843dbdbUL, 0x2d361b1bUL,
0x44783c3cUL, 0xad65c8c8UL, 0x5ec79999UL, 0x0c080404UL,
0x67e98e8eUL, 0xd535e0e0UL, 0x8c5bd7d7UL, 0x87fa7d7dUL,
0x7aff8585UL, 0x3883bbbbUL, 0xc0804040UL, 0x74582c2cUL,
0x4e743a3aUL, 0xcf8a4545UL, 0xe617f1f1UL, 0xc6844242UL,
0xafca6565UL, 0x60402020UL, 0xc3824141UL, 0x28301818UL,
0x96e47272UL, 0x6f4a2525UL, 0x40d39393UL, 0x90e07070UL,
0x5a6c3636UL, 0x0f0a0505UL, 0xe311f2f2UL, 0x1d160b0bUL,
0x10b3a3a3UL, 0x8bf27979UL, 0xc12dececUL, 0x18100808UL,
0x694e2727UL, 0x53623131UL, 0x56643232UL, 0x2f99b6b6UL,
0x84f87c7cUL, 0x2595b0b0UL, 0x1e140a0aUL, 0x95e67373UL,
0xedb65b5bUL, 0x8df67b7bUL, 0x2c9bb7b7UL, 0x76f78181UL,
0x8351d2d2UL, 0x171a0d0dUL, 0xbed46a6aUL, 0x6a4c2626UL,
0x57c99e9eUL, 0xe8b05858UL, 0x51cd9c9cUL, 0x70f38383UL,
0x9ce87474UL, 0x2093b3b3UL, 0x01adacacUL, 0x50603030UL,
0x8ef47a7aUL, 0xbbd26969UL, 0x99ee7777UL, 0x111e0f0fUL,
0x07a9aeaeUL, 0x63422121UL, 0x9749dedeUL, 0x8555d0d0UL,
0x725c2e2eUL, 0x4cdb9797UL, 0x30201010UL, 0x19bda4a4UL,
0x5dc59898UL, 0x0da5a8a8UL, 0x895dd4d4UL, 0xb8d06868UL,
0x775a2d2dUL, 0xa6c46262UL, 0x7b522929UL, 0xb7da6d6dUL,
0x3a2c1616UL, 0xdb924949UL, 0x9aec7676UL, 0xbc7bc7c7UL,
0xcd25e8e8UL, 0xb677c1c1UL, 0x4fd99696UL, 0x596e3737UL,
0xda3fe5e5UL, 0xab61cacaUL, 0xe91df4f4UL, 0xce27e9e9UL,
0xa5c66363UL, 0x36241212UL, 0xb371c2c2UL, 0x1fb9a6a6UL,
0x3c281414UL, 0x318dbcbcUL, 0x8053d3d3UL, 0x78502828UL,
0x04abafafUL, 0x715e2f2fUL, 0xdf39e6e6UL, 0x6c482424UL,
0xf6a45252UL, 0xbf79c6c6UL, 0x15b5a0a0UL, 0x1b120909UL,
0x328fbdbdUL, 0x61ed8c8cUL, 0xa46bcfcfUL, 0xe7ba5d5dUL,
0x33221111UL, 0xe1be5f5fUL, 0x03020101UL, 0xba7fc5c5UL,
0x54cb9f9fUL, 0x477a3d3dUL, 0x13b1a2a2UL, 0x58c39b9bUL,
0xae67c9c9UL, 0x4d763b3bUL, 0x3789bebeUL, 0xf3a25151UL,
0x2b321919UL, 0x213e1f1fUL, 0x417e3f3fUL, 0xe4b85c5cUL,
0x2391b2b2UL, 0xc42befefUL, 0xde944a4aUL, 0xa26fcdcdUL,
0x348bbfbfUL, 0x3b81babaUL, 0xb1de6f6fUL, 0xacc86464UL,
0x9e47d9d9UL, 0xe013f3f3UL, 0x427c3e3eUL, 0x299db4b4UL,
0x0ba1aaaaUL, 0x914ddcdcUL, 0x8a5fd5d5UL, 0x0a0c0606UL,
0xb575c0c0UL, 0x82fc7e7eUL, 0xef19f6f6UL, 0xaacc6666UL,
0xb4d86c6cUL, 0x79fd8484UL, 0x93e27171UL, 0x48703838UL,
0x3e87b9b9UL, 0x273a1d1dUL, 0x81fe7f7fUL, 0x52cf9d9dUL,
0xd8904848UL, 0x68e38b8bUL, 0x7e542a2aUL, 0x9b41dadaUL,
0x1abfa5a5UL, 0x55663333UL, 0x73f18282UL, 0x4b723939UL,
0x8f59d6d6UL, 0x88f07878UL, 0x7ff98686UL, 0xfb01fafaUL,
0xd93de4e4UL, 0x7d562b2bUL, 0x0ea7a9a9UL, 0x223c1e1eUL,
0x6ee78989UL, 0xa0c06060UL, 0xbdd66b6bUL, 0xcb21eaeaUL,
0xffaa5555UL, 0xd4984c4cUL, 0xec1bf7f7UL, 0xd331e2e2UL,
},

{
0xb12697b1UL, 0xcea769ceUL, 0xc3b073c3UL, 0x954adf95UL,
0x5aeeb45aUL, 0xad02afadUL, 0xe7dc3be7UL, 0x02060402UL,
0x4dd79a4dUL, 0x44cc8844UL, 0xfbf803fbUL, 0x9146d791UL,
0x0c14180cUL, 0x877cfb87UL, 0xa116b7a1UL, 0x50f0a050UL,
0xcba863cbUL, 0x67a9ce67UL, 0x54fca854UL, 0xdd924fddUL,
0x46ca8c46UL, 0x8f64eb8fUL, 0xe1d637e1UL, 0x4ed29c4eUL,
0xf0e515f0UL, 0xfdf20ffdUL, 0xfcf10dfcUL, 0xebc823ebUL,
0xf9fe07f9UL, 0xc4b97dc4UL, 0x1a2e341aUL, 0x6eb2dc6eUL,
0x5ee2bc5eUL, 0xf5ea1ff5UL, 0xcca16dccUL, 0x8d62ef8dUL,
0x1c24381cUL, 0x56faac56UL, 0x43c58643UL, 0xfef709feUL,
0x07090e07UL, 0x61a3c261UL, 0xf8fd05f8UL, 0x759fea75UL,
0x59ebb259UL, 0xfff40bffUL, 0x03050603UL, 0x22664422UL,
0x8a6be18aUL, 0xd18657d1UL, 0x13352613UL, 0xeec729eeUL,
0x886de588UL, 0x00000000UL, 0x0e121c0eUL, 0x345c6834UL,
0x153f2a15UL, 0x8075f580UL, 0x9449dd94UL, 0xe3d033e3UL,
0xedc22fedUL, 0xb52a9fb5UL, 0x53f5a653UL, 0x23654623UL,
0x4bdd964bUL, 0x47c98e47UL, 0x17392e17UL, 0xa71cbba7UL,
0x9045d590UL, 0x355f6a35UL, 0xab08a3abUL, 0xd89d45d8UL,
0xb83d85b8UL, 0xdf944bdfUL, 0x4fd19e4fUL, 0x57f9ae57UL,
0x9a5bc19aUL, 0x9243d192UL, 0xdb9843dbUL, 0x1b2d361bUL,
0x3c44783cUL, 0xc8ad65c8UL, 0x995ec799UL, 0x040c0804UL,
0x8e67e98eUL, 0xe0d535e0UL, 0xd78c5bd7UL, 0x7d87fa7dUL,
0x857aff85UL, 0xbb3883bbUL, 0x40c08040UL, 0x2c74582cUL,
0x3a4e743aUL, 0x45cf8a45UL, 0xf1e617f1UL, 0x42c68442UL,
0x65afca65UL, 0x20604020UL, 0x41c38241UL, 0x18283018UL,
0x7296e472UL, 0x256f4a25UL, 0x9340d393UL, 0x7090e070UL,
0x365a6c36UL, 0x050f0a05UL, 0xf2e311f2UL, 0x0b1d160bUL,
0xa310b3a3UL, 0x798bf279UL, 0xecc12decUL, 0x08181008UL,
0x27694e27UL, 0x31536231UL, 0x32566432UL, 0xb62f99b6UL,
0x7c84f87cUL, 0xb02595b0UL, 0x0a1e140aUL, 0x7395e673UL,
0x5bedb65bUL, 0x7b8df67bUL, 0xb72c9bb7UL, 0x8176f781UL,
0xd28351d2UL, 0x0d171a0dUL, 0x6abed46aUL, 0x266a4c26UL,
0x9e57c99eUL, 0x58e8b058UL, 0x9c51cd9cUL, 0x8370f383UL,
0x749ce874UL, 0xb32093b3UL, 0xac01adacUL, 0x30506030UL,
0x7a8ef47aUL, 0x69bbd269UL, 0x7799ee77UL, 0x0f111e0fUL,
0xae07a9aeUL, 0x21634221UL, 0xde9749deUL, 0xd08555d0UL,
0x2e725c2eUL, 0x974cdb97UL, 0x10302010UL, 0xa419bda4UL,
0x985dc598UL, 0xa80da5a8UL, 0xd4895dd4UL, 0x68b8d068UL,
0x2d775a2dUL, 0x62a6c462UL, 0x297b5229UL, 0x6db7da6dUL,
0x163a2c16UL, 0x49db9249UL, 0x769aec76UL, 0xc7bc7bc7UL,
0xe8cd25e8UL, 0xc1b677c1UL, 0x964fd996UL, 0x37596e37UL,
0xe5da3fe5UL, 0xcaab61caUL, 0xf4e91df4UL, 0xe9ce27e9UL,
0x63a5c663UL, 0x12362412UL, 0xc2b371c2UL, 0xa61fb9a6UL,
0x143c2814UL, 0xbc318dbcUL, 0xd38053d3UL, 0x28785028UL,
0xaf04abafUL, 0x2f715e2fUL, 0xe6df39e6UL, 0x246c4824UL,
0x52f6a452UL, 0xc6bf79c6UL, 0xa015b5a0UL, 0x091b1209UL,
0xbd328fbdUL, 0x8c61ed8cUL, 0xcfa46bcfUL, 0x5de7ba5dUL,
0x11332211UL, 0x5fe1be5fUL, 0x01030201UL, 0xc5ba7fc5UL,
0x9f54cb9fUL, 0x3d477a3dUL, 0xa213b1a2UL, 0x9b58c39bUL,
0xc9ae67c9UL, 0x3b4d763bUL, 0xbe3789beUL, 0x51f3a251UL,
0x192b3219UL, 0x1f213e1fUL, 0x3f417e3fUL, 0x5ce4b85cUL,
0xb22391b2UL, 0xefc42befUL, 0x4ade944aUL, 0xcda26fcdUL,
0xbf348bbfUL, 0xba3b81baUL, 0x6fb1de6fUL, 0x64acc864UL,
0xd99e47d9UL, 0xf3e013f3UL, 0x3e427c3eUL, 0xb4299db4UL,
0xaa0ba1aaUL, 0xdc914ddcUL, 0xd58a5fd5UL, 0x060a0c06UL,
0xc0b575c0UL, 0x7e82fc7eUL, 0xf6ef19f6UL, 0x66aacc66UL,
0x6cb4d86cUL, 0x8479fd84UL, 0x7193e271UL, 0x38487038UL,
0xb93e87b9UL, 0x1d273a1dUL, 0x7f81fe7fUL, 0x9d52cf9dUL,
0x48d89048UL, 0x8b68e38bUL, 0x2a7e542aUL, 0xda9b41daUL,
0xa51abfa5UL, 0x33556633UL, 0x8273f182UL, 0x394b7239UL,
0xd68f59d6UL, 0x7888f078UL, 0x867ff986UL, 0xfafb01faUL,
0xe4d93de4UL, 0x2b7d562bUL, 0xa90ea7a9UL, 0x1e223c1eUL,
0x896ee789UL, 0x60a0c060UL, 0x6bbdd66bUL, 0xeacb21eaUL,
0x55ffaa55UL, 0x4cd4984cUL, 0xf7ec1bf7UL, 0xe2d331e2UL,
},

{
0xb1b12697UL, 0xcecea769UL, 0xc3c3b073UL, 0x95954adfUL,
0x5a5aeeb4UL, 0xadad02afUL, 0xe7e7dc3bUL, 0x02020604UL,
0x4d4dd79aUL, 0x4444cc88UL, 0xfbfbf803UL, 0x919146d7UL,
0x0c0c1418UL, 0x87877cfbUL, 0xa1a116b7UL, 0x5050f0a0UL,
0xcbcba863UL, 0x6767a9ceUL, 0x5454fca8UL, 0xdddd924fUL,
0x4646ca8cUL, 0x8f8f64ebUL, 0xe1e1d637UL, 0x4e4ed29cUL,
0xf0f0e515UL, 0xfdfdf20fUL, 0xfcfcf10dUL, 0xebebc823UL,
0xf9f9fe07UL, 0xc4c4b97dUL, 0x1a1a2e34UL, 0x6e6eb2dcUL,
0x5e5ee2bcUL, 0xf5f5ea1fUL, 0xcccca16dUL, 0x8d8d62efUL,
0x1c1c2438UL, 0x5656faacUL, 0x4343c586UL, 0xfefef709UL,
0x0707090eUL, 0x6161a3c2UL, 0xf8f8fd05UL, 0x75759feaUL,
0x5959ebb2UL, 0xfffff40bUL, 0x03030506UL, 0x22226644UL,
0x8a8a6be1UL, 0xd1d18657UL, 0x13133526UL, 0xeeeec729UL,
0x88886de5UL, 0x00000000UL, 0x0e0e121cUL, 0x34345c68UL,
0x15153f2aUL, 0x808075f5UL, 0x949449ddUL, 0xe3e3d033UL,
0xededc22fUL, 0xb5b52a9fUL, 0x5353f5a6UL, 0x23236546UL,
0x4b4bdd96UL, 0x4747c98eUL, 0x1717392eUL, 0xa7a71cbbUL,
0x909045d5UL, 0x35355f6aUL, 0xabab08a3UL, 0xd8d89d45UL,
0xb8b83d85UL, 0xdfdf944bUL, 0x4f4fd19eUL, 0x5757f9aeUL,
0x9a9a5bc1UL, 0x929243d1UL, 0xdbdb9843UL, 0x1b1b2d36UL,
0x3c3c4478UL, 0xc8c8ad65UL, 0x99995ec7UL, 0x04040c08UL,
0x8e8e67e9UL, 0xe0e0d535UL, 0xd7d78c5bUL, 0x7d7d87faUL,
0x85857affUL, 0xbbbb3883UL, 0x4040c080UL, 0x2c2c7458UL,
0x3a3a4e74UL, 0x4545cf8aUL, 0xf1f1e617UL, 0x4242c684UL,
0x6565afcaUL, 0x20206040UL, 0x4141c382UL, 0x18182830UL,
0x727296e4UL, 0x25256f4aUL, 0x939340d3UL, 0x707090e0UL,
0x36365a6cUL, 0x05050f0aUL, 0xf2f2e311UL, 0x0b0b1d16UL,
0xa3a310b3UL, 0x79798bf2UL, 0xececc12dUL, 0x08081810UL,
0x2727694eUL, 0x31315362UL, 0x32325664UL, 0xb6b62f99UL,
0x7c7c84f8UL, 0xb0b02595UL, 0x0a0a1e14UL, 0x737395e6UL,
0x5b5bedb6UL, 0x7b7b8df6UL, 0xb7b72c9bUL, 0x818176f7UL,
0xd2d28351UL, 0x0d0d171aUL, 0x6a6abed4UL, 0x26266a4cUL,
0x9e9e57c9UL, 0x5858e8b0UL, 0x9c9c51cdUL, 0x838370f3UL,
0x74749ce8UL, 0xb3b32093UL, 0xacac01adUL, 0x30305060UL,
0x7a7a8ef4UL, 0x6969bbd2UL, 0x777799eeUL, 0x0f0f111eUL,
0xaeae07a9UL, 0x21216342UL, 0xdede9749UL, 0xd0d08555UL,
0x2e2e725cUL, 0x97974cdbUL, 0x10103020UL, 0xa4a419bdUL,
0x98985dc5UL, 0xa8a80da5UL, 0xd4d4895dUL, 0x6868b8d0UL,
0x2d2d775aUL, 0x6262a6c4UL, 0x29297b52UL, 0x6d6db7daUL,
0x16163a2cUL, 0x4949db92UL, 0x76769aecUL, 0xc7c7bc7bUL,
0xe8e8cd25UL, 0xc1c1b677UL, 0x96964fd9UL, 0x3737596eUL,
0xe5e5da3fUL, 0xcacaab61UL, 0xf4f4e91dUL, 0xe9e9ce27UL,
0x6363a5c6UL, 0x12123624UL, 0xc2c2b371UL, 0xa6a61fb9UL,
0x14143c28UL, 0xbcbc318dUL, 0xd3d38053UL, 0x28287850UL,
0xafaf04abUL, 0x2f2f715eUL, 0xe6e6df39UL, 0x24246c48UL,
0x5252f6a4UL, 0xc6c6bf79UL, 0xa0a015b5UL, 0x09091b12UL,
0xbdbd328fUL, 0x8c8c61edUL, 0xcfcfa46bUL, 0x5d5de7baUL,
0x11113322UL, 0x5f5fe1beUL, 0x01010302UL, 0xc5c5ba7fUL,
0x9f9f54cbUL, 0x3d3d477aUL, 0xa2a213b1UL, 0x9b9b58c3UL,
0xc9c9ae67UL, 0x3b3b4d76UL, 0xbebe3789UL, 0x5151f3a2UL,
0x19192b32UL, 0x1f1f213eUL, 0x3f3f417eUL, 0x5c5ce4b8UL,
0xb2b22391UL, 0xefefc42bUL, 0x4a4ade94UL, 0xcdcda26fUL,
0xbfbf348bUL, 0xbaba3b81UL, 0x6f6fb1deUL, 0x6464acc8UL,
0xd9d99e47UL, 0xf3f3e013UL, 0x3e3e427cUL, 0xb4b4299dUL,
0xaaaa0ba1UL, 0xdcdc914dUL, 0xd5d58a5fUL, 0x06060a0cUL,
0xc0c0b575UL, 0x7e7e82fcUL, 0xf6f6ef19UL, 0x6666aaccUL,
0x6c6cb4d8UL, 0x848479fdUL, 0x717193e2UL, 0x38384870UL,
0xb9b93e87UL, 0x1d1d273aUL, 0x7f7f81feUL, 0x9d9d52cfUL,
0x4848d890UL, 0x8b8b68e3UL, 0x2a2a7e54UL, 0xdada9b41UL,
0xa5a51abfUL, 0x33335566UL, 0x828273f1UL, 0x39394b72UL,
0xd6d68f59UL, 0x787888f0UL, 0x86867ff9UL, 0xfafafb01UL,
0xe4e4d93dUL, 0x2b2b7d56UL, 0xa9a90ea7UL, 0x1e1e223cUL,
0x89896ee7UL, 0x6060a0c0UL, 0x6b6bbdd6UL, 0xeaeacb21UL,
0x5555ffaaUL, 0x4c4cd498UL, 0xf7f7ec1bUL, 0xe2e2d331UL,
}};

const word32 Square::Dec::Td[4][256] = {
{
0xe368bc02UL, 0x5585620cUL, 0x2a3f2331UL, 0x61ab13f7UL,
0x98d46d72UL, 0x21cb9a19UL, 0x3c22a461UL, 0x459d3dcdUL,
0x05fdb423UL, 0x2bc4075fUL, 0x9b2c01c0UL, 0x3dd9800fUL,
0x486c5c74UL, 0xf97f7e85UL, 0xf173ab1fUL, 0xb6edde0eUL,
0x283c6bedUL, 0x4997781aUL, 0x9f2a918dUL, 0xc9579f33UL,
0xa907a8aaUL, 0xa50ded7dUL, 0x7c422d8fUL, 0x764db0c9UL,
0x4d91e857UL, 0xcea963ccUL, 0xb4ee96d2UL, 0x3028e1b6UL,
0x0df161b9UL, 0xbd196726UL, 0x419bad80UL, 0xc0a06ec7UL,
0x5183f241UL, 0x92dbf034UL, 0x6fa21efcUL, 0x8f32ce4cUL,
0x13e03373UL, 0x69a7c66dUL, 0xe56d6493UL, 0xbf1a2ffaUL,
0xbb1cbfb7UL, 0x587403b5UL, 0xe76e2c4fUL, 0x5d89b796UL,
0xe89c052aUL, 0x446619a3UL, 0x342e71fbUL, 0x0ff22965UL,
0xfe81827aUL, 0xb11322f1UL, 0xa30835ecUL, 0xcd510f7eUL,
0xff7aa614UL, 0x5c7293f8UL, 0x2fc29712UL, 0xf370e3c3UL,
0x992f491cUL, 0xd1431568UL, 0xc2a3261bUL, 0x88cc32b3UL,
0x8acf7a6fUL, 0xb0e8069fUL, 0x7a47f51eUL, 0xd2bb79daUL,
0xe6950821UL, 0x4398e55cUL, 0xd0b83106UL, 0x11e37bafUL,
0x7e416553UL, 0xccaa2b10UL, 0xd8b4e49cUL, 0x6456a7d4UL,
0xfb7c3659UL, 0x724b2084UL, 0xea9f4df6UL, 0x6a5faadfUL,
0x2dc1dfceUL, 0x70486858UL, 0xcaaff381UL, 0x0605d891UL,
0x5a774b69UL, 0x94de28a5UL, 0x39df1042UL, 0x813bc347UL,
0xfc82caa6UL, 0x23c8d2c5UL, 0x03f86cb2UL, 0x080cd59aUL,
0xdab7ac40UL, 0x7db909e1UL, 0x3824342cUL, 0xcf5247a2UL,
0xdcb274d1UL, 0x63a85b2bUL, 0x35d55595UL, 0x479e7511UL,
0x15e5ebe2UL, 0x4b9430c6UL, 0x4a6f14a8UL, 0x91239c86UL,
0x4c6acc39UL, 0x5f8aff4aUL, 0x0406904dUL, 0xee99ddbbUL,
0x1e1152caUL, 0xaaffc418UL, 0xeb646998UL, 0x07fefcffUL,
0x8b345e01UL, 0x567d0ebeUL, 0xbae79bd9UL, 0x4263c132UL,
0x75b5dc7bUL, 0x97264417UL, 0x67aecb66UL, 0x95250ccbUL,
0xec9a9567UL, 0x57862ad0UL, 0x60503799UL, 0xb8e4d305UL,
0x65ad83baUL, 0x19efae35UL, 0xa4f6c913UL, 0xc15b4aa9UL,
0x873e1bd6UL, 0xa0f0595eUL, 0x18148a5bUL, 0xaf02703bUL,
0xab04e076UL, 0xdd4950bfUL, 0xdf4a1863UL, 0xc6a5b656UL,
0x853d530aUL, 0xfa871237UL, 0x77b694a7UL, 0x4665517fUL,
0xed61b109UL, 0x1bece6e9UL, 0xd5458525UL, 0xf5753b52UL,
0x7fba413dUL, 0x27ce4288UL, 0xb2eb4e43UL, 0xd6bde997UL,
0x527b9ef3UL, 0x62537f45UL, 0x2c3afba0UL, 0x7bbcd170UL,
0xb91ff76bUL, 0x121b171dUL, 0xfd79eec8UL, 0x3a277cf0UL,
0x0c0a45d7UL, 0x96dd6079UL, 0x2233f6abUL, 0xacfa1c89UL,
0xc8acbb5dUL, 0xa10b7d30UL, 0xd4bea14bUL, 0xbee10b94UL,
0x25cd0a54UL, 0x547e4662UL, 0xa2f31182UL, 0x17e6a33eUL,
0x263566e6UL, 0xc3580275UL, 0x83388b9bUL, 0x7844bdc2UL,
0x020348dcUL, 0x4f92a08bUL, 0x2e39b37cUL, 0x4e6984e5UL,
0xf0888f71UL, 0x362d3927UL, 0x9cd2fd3fUL, 0x01fb246eUL,
0x893716ddUL, 0x00000000UL, 0xf68d57e0UL, 0xe293986cUL,
0x744ef815UL, 0x9320d45aUL, 0xad0138e7UL, 0xd3405db4UL,
0x1a17c287UL, 0xb3106a2dUL, 0x5078d62fUL, 0xf48e1f3cUL,
0xa70ea5a1UL, 0x71b34c36UL, 0x9ad725aeUL, 0x5e71db24UL,
0x161d8750UL, 0xef62f9d5UL, 0x8d318690UL, 0x1c121a16UL,
0xa6f581cfUL, 0x5b8c6f07UL, 0x37d61d49UL, 0x6e593a92UL,
0x84c67764UL, 0x86c53fb8UL, 0xd746cdf9UL, 0xe090d0b0UL,
0x29c74f83UL, 0xe49640fdUL, 0x0e090d0bUL, 0x6da15620UL,
0x8ec9ea22UL, 0xdb4c882eUL, 0xf776738eUL, 0xb515b2bcUL,
0x10185fc1UL, 0x322ba96aUL, 0x6ba48eb1UL, 0xaef95455UL,
0x406089eeUL, 0x6655ef08UL, 0xe9672144UL, 0x3e21ecbdUL,
0x2030be77UL, 0xf28bc7adUL, 0x80c0e729UL, 0x141ecf8cUL,
0xbce24348UL, 0xc4a6fe8aUL, 0x31d3c5d8UL, 0xb716fa60UL,
0x5380ba9dUL, 0xd94fc0f2UL, 0x1de93e78UL, 0x24362e3aUL,
0xe16bf4deUL, 0xcb54d7efUL, 0x09f7f1f4UL, 0x82c3aff5UL,
0x0bf4b928UL, 0x9d29d951UL, 0xc75e9238UL, 0xf8845aebUL,
0x90d8b8e8UL, 0xdeb13c0dUL, 0x33d08d04UL, 0x685ce203UL,
0xc55ddae4UL, 0x3bdc589eUL, 0x0a0f9d46UL, 0x3fdac8d3UL,
0x598f27dbUL, 0xa8fc8cc4UL, 0x79bf99acUL, 0x6c5a724eUL,
0x8ccaa2feUL, 0x9ed1b5e3UL, 0x1fea76a4UL, 0x73b004eaUL,
},

{
0x02e368bcUL, 0x0c558562UL, 0x312a3f23UL, 0xf761ab13UL,
0x7298d46dUL, 0x1921cb9aUL, 0x613c22a4UL, 0xcd459d3dUL,
0x2305fdb4UL, 0x5f2bc407UL, 0xc09b2c01UL, 0x0f3dd980UL,
0x74486c5cUL, 0x85f97f7eUL, 0x1ff173abUL, 0x0eb6eddeUL,
0xed283c6bUL, 0x1a499778UL, 0x8d9f2a91UL, 0x33c9579fUL,
0xaaa907a8UL, 0x7da50dedUL, 0x8f7c422dUL, 0xc9764db0UL,
0x574d91e8UL, 0xcccea963UL, 0xd2b4ee96UL, 0xb63028e1UL,
0xb90df161UL, 0x26bd1967UL, 0x80419badUL, 0xc7c0a06eUL,
0x415183f2UL, 0x3492dbf0UL, 0xfc6fa21eUL, 0x4c8f32ceUL,
0x7313e033UL, 0x6d69a7c6UL, 0x93e56d64UL, 0xfabf1a2fUL,
0xb7bb1cbfUL, 0xb5587403UL, 0x4fe76e2cUL, 0x965d89b7UL,
0x2ae89c05UL, 0xa3446619UL, 0xfb342e71UL, 0x650ff229UL,
0x7afe8182UL, 0xf1b11322UL, 0xeca30835UL, 0x7ecd510fUL,
0x14ff7aa6UL, 0xf85c7293UL, 0x122fc297UL, 0xc3f370e3UL,
0x1c992f49UL, 0x68d14315UL, 0x1bc2a326UL, 0xb388cc32UL,
0x6f8acf7aUL, 0x9fb0e806UL, 0x1e7a47f5UL, 0xdad2bb79UL,
0x21e69508UL, 0x5c4398e5UL, 0x06d0b831UL, 0xaf11e37bUL,
0x537e4165UL, 0x10ccaa2bUL, 0x9cd8b4e4UL, 0xd46456a7UL,
0x59fb7c36UL, 0x84724b20UL, 0xf6ea9f4dUL, 0xdf6a5faaUL,
0xce2dc1dfUL, 0x58704868UL, 0x81caaff3UL, 0x910605d8UL,
0x695a774bUL, 0xa594de28UL, 0x4239df10UL, 0x47813bc3UL,
0xa6fc82caUL, 0xc523c8d2UL, 0xb203f86cUL, 0x9a080cd5UL,
0x40dab7acUL, 0xe17db909UL, 0x2c382434UL, 0xa2cf5247UL,
0xd1dcb274UL, 0x2b63a85bUL, 0x9535d555UL, 0x11479e75UL,
0xe215e5ebUL, 0xc64b9430UL, 0xa84a6f14UL, 0x8691239cUL,
0x394c6accUL, 0x4a5f8affUL, 0x4d040690UL, 0xbbee99ddUL,
0xca1e1152UL, 0x18aaffc4UL, 0x98eb6469UL, 0xff07fefcUL,
0x018b345eUL, 0xbe567d0eUL, 0xd9bae79bUL, 0x324263c1UL,
0x7b75b5dcUL, 0x17972644UL, 0x6667aecbUL, 0xcb95250cUL,
0x67ec9a95UL, 0xd057862aUL, 0x99605037UL, 0x05b8e4d3UL,
0xba65ad83UL, 0x3519efaeUL, 0x13a4f6c9UL, 0xa9c15b4aUL,
0xd6873e1bUL, 0x5ea0f059UL, 0x5b18148aUL, 0x3baf0270UL,
0x76ab04e0UL, 0xbfdd4950UL, 0x63df4a18UL, 0x56c6a5b6UL,
0x0a853d53UL, 0x37fa8712UL, 0xa777b694UL, 0x7f466551UL,
0x09ed61b1UL, 0xe91bece6UL, 0x25d54585UL, 0x52f5753bUL,
0x3d7fba41UL, 0x8827ce42UL, 0x43b2eb4eUL, 0x97d6bde9UL,
0xf3527b9eUL, 0x4562537fUL, 0xa02c3afbUL, 0x707bbcd1UL,
0x6bb91ff7UL, 0x1d121b17UL, 0xc8fd79eeUL, 0xf03a277cUL,
0xd70c0a45UL, 0x7996dd60UL, 0xab2233f6UL, 0x89acfa1cUL,
0x5dc8acbbUL, 0x30a10b7dUL, 0x4bd4bea1UL, 0x94bee10bUL,
0x5425cd0aUL, 0x62547e46UL, 0x82a2f311UL, 0x3e17e6a3UL,
0xe6263566UL, 0x75c35802UL, 0x9b83388bUL, 0xc27844bdUL,
0xdc020348UL, 0x8b4f92a0UL, 0x7c2e39b3UL, 0xe54e6984UL,
0x71f0888fUL, 0x27362d39UL, 0x3f9cd2fdUL, 0x6e01fb24UL,
0xdd893716UL, 0x00000000UL, 0xe0f68d57UL, 0x6ce29398UL,
0x15744ef8UL, 0x5a9320d4UL, 0xe7ad0138UL, 0xb4d3405dUL,
0x871a17c2UL, 0x2db3106aUL, 0x2f5078d6UL, 0x3cf48e1fUL,
0xa1a70ea5UL, 0x3671b34cUL, 0xae9ad725UL, 0x245e71dbUL,
0x50161d87UL, 0xd5ef62f9UL, 0x908d3186UL, 0x161c121aUL,
0xcfa6f581UL, 0x075b8c6fUL, 0x4937d61dUL, 0x926e593aUL,
0x6484c677UL, 0xb886c53fUL, 0xf9d746cdUL, 0xb0e090d0UL,
0x8329c74fUL, 0xfde49640UL, 0x0b0e090dUL, 0x206da156UL,
0x228ec9eaUL, 0x2edb4c88UL, 0x8ef77673UL, 0xbcb515b2UL,
0xc110185fUL, 0x6a322ba9UL, 0xb16ba48eUL, 0x55aef954UL,
0xee406089UL, 0x086655efUL, 0x44e96721UL, 0xbd3e21ecUL,
0x772030beUL, 0xadf28bc7UL, 0x2980c0e7UL, 0x8c141ecfUL,
0x48bce243UL, 0x8ac4a6feUL, 0xd831d3c5UL, 0x60b716faUL,
0x9d5380baUL, 0xf2d94fc0UL, 0x781de93eUL, 0x3a24362eUL,
0xdee16bf4UL, 0xefcb54d7UL, 0xf409f7f1UL, 0xf582c3afUL,
0x280bf4b9UL, 0x519d29d9UL, 0x38c75e92UL, 0xebf8845aUL,
0xe890d8b8UL, 0x0ddeb13cUL, 0x0433d08dUL, 0x03685ce2UL,
0xe4c55ddaUL, 0x9e3bdc58UL, 0x460a0f9dUL, 0xd33fdac8UL,
0xdb598f27UL, 0xc4a8fc8cUL, 0xac79bf99UL, 0x4e6c5a72UL,
0xfe8ccaa2UL, 0xe39ed1b5UL, 0xa41fea76UL, 0xea73b004UL,
},

{
0xbc02e368UL, 0x620c5585UL, 0x23312a3fUL, 0x13f761abUL,
0x6d7298d4UL, 0x9a1921cbUL, 0xa4613c22UL, 0x3dcd459dUL,
0xb42305fdUL, 0x075f2bc4UL, 0x01c09b2cUL, 0x800f3dd9UL,
0x5c74486cUL, 0x7e85f97fUL, 0xab1ff173UL, 0xde0eb6edUL,
0x6bed283cUL, 0x781a4997UL, 0x918d9f2aUL, 0x9f33c957UL,
0xa8aaa907UL, 0xed7da50dUL, 0x2d8f7c42UL, 0xb0c9764dUL,
0xe8574d91UL, 0x63cccea9UL, 0x96d2b4eeUL, 0xe1b63028UL,
0x61b90df1UL, 0x6726bd19UL, 0xad80419bUL, 0x6ec7c0a0UL,
0xf2415183UL, 0xf03492dbUL, 0x1efc6fa2UL, 0xce4c8f32UL,
0x337313e0UL, 0xc66d69a7UL, 0x6493e56dUL, 0x2ffabf1aUL,
0xbfb7bb1cUL, 0x03b55874UL, 0x2c4fe76eUL, 0xb7965d89UL,
0x052ae89cUL, 0x19a34466UL, 0x71fb342eUL, 0x29650ff2UL,
0x827afe81UL, 0x22f1b113UL, 0x35eca308UL, 0x0f7ecd51UL,
0xa614ff7aUL, 0x93f85c72UL, 0x97122fc2UL, 0xe3c3f370UL,
0x491c992fUL, 0x1568d143UL, 0x261bc2a3UL, 0x32b388ccUL,
0x7a6f8acfUL, 0x069fb0e8UL, 0xf51e7a47UL, 0x79dad2bbUL,
0x0821e695UL, 0xe55c4398UL, 0x3106d0b8UL, 0x7baf11e3UL,
0x65537e41UL, 0x2b10ccaaUL, 0xe49cd8b4UL, 0xa7d46456UL,
0x3659fb7cUL, 0x2084724bUL, 0x4df6ea9fUL, 0xaadf6a5fUL,
0xdfce2dc1UL, 0x68587048UL, 0xf381caafUL, 0xd8910605UL,
0x4b695a77UL, 0x28a594deUL, 0x104239dfUL, 0xc347813bUL,
0xcaa6fc82UL, 0xd2c523c8UL, 0x6cb203f8UL, 0xd59a080cUL,
0xac40dab7UL, 0x09e17db9UL, 0x342c3824UL, 0x47a2cf52UL,
0x74d1dcb2UL, 0x5b2b63a8UL, 0x559535d5UL, 0x7511479eUL,
0xebe215e5UL, 0x30c64b94UL, 0x14a84a6fUL, 0x9c869123UL,
0xcc394c6aUL, 0xff4a5f8aUL, 0x904d0406UL, 0xddbbee99UL,
0x52ca1e11UL, 0xc418aaffUL, 0x6998eb64UL, 0xfcff07feUL,
0x5e018b34UL, 0x0ebe567dUL, 0x9bd9bae7UL, 0xc1324263UL,
0xdc7b75b5UL, 0x44179726UL, 0xcb6667aeUL, 0x0ccb9525UL,
0x9567ec9aUL, 0x2ad05786UL, 0x37996050UL, 0xd305b8e4UL,
0x83ba65adUL, 0xae3519efUL, 0xc913a4f6UL, 0x4aa9c15bUL,
0x1bd6873eUL, 0x595ea0f0UL, 0x8a5b1814UL, 0x703baf02UL,
0xe076ab04UL, 0x50bfdd49UL, 0x1863df4aUL, 0xb656c6a5UL,
0x530a853dUL, 0x1237fa87UL, 0x94a777b6UL, 0x517f4665UL,
0xb109ed61UL, 0xe6e91becUL, 0x8525d545UL, 0x3b52f575UL,
0x413d7fbaUL, 0x428827ceUL, 0x4e43b2ebUL, 0xe997d6bdUL,
0x9ef3527bUL, 0x7f456253UL, 0xfba02c3aUL, 0xd1707bbcUL,
0xf76bb91fUL, 0x171d121bUL, 0xeec8fd79UL, 0x7cf03a27UL,
0x45d70c0aUL, 0x607996ddUL, 0xf6ab2233UL, 0x1c89acfaUL,
0xbb5dc8acUL, 0x7d30a10bUL, 0xa14bd4beUL, 0x0b94bee1UL,
0x0a5425cdUL, 0x4662547eUL, 0x1182a2f3UL, 0xa33e17e6UL,
0x66e62635UL, 0x0275c358UL, 0x8b9b8338UL, 0xbdc27844UL,
0x48dc0203UL, 0xa08b4f92UL, 0xb37c2e39UL, 0x84e54e69UL,
0x8f71f088UL, 0x3927362dUL, 0xfd3f9cd2UL, 0x246e01fbUL,
0x16dd8937UL, 0x00000000UL, 0x57e0f68dUL, 0x986ce293UL,
0xf815744eUL, 0xd45a9320UL, 0x38e7ad01UL, 0x5db4d340UL,
0xc2871a17UL, 0x6a2db310UL, 0xd62f5078UL, 0x1f3cf48eUL,
0xa5a1a70eUL, 0x4c3671b3UL, 0x25ae9ad7UL, 0xdb245e71UL,
0x8750161dUL, 0xf9d5ef62UL, 0x86908d31UL, 0x1a161c12UL,
0x81cfa6f5UL, 0x6f075b8cUL, 0x1d4937d6UL, 0x3a926e59UL,
0x776484c6UL, 0x3fb886c5UL, 0xcdf9d746UL, 0xd0b0e090UL,
0x4f8329c7UL, 0x40fde496UL, 0x0d0b0e09UL, 0x56206da1UL,
0xea228ec9UL, 0x882edb4cUL, 0x738ef776UL, 0xb2bcb515UL,
0x5fc11018UL, 0xa96a322bUL, 0x8eb16ba4UL, 0x5455aef9UL,
0x89ee4060UL, 0xef086655UL, 0x2144e967UL, 0xecbd3e21UL,
0xbe772030UL, 0xc7adf28bUL, 0xe72980c0UL, 0xcf8c141eUL,
0x4348bce2UL, 0xfe8ac4a6UL, 0xc5d831d3UL, 0xfa60b716UL,
0xba9d5380UL, 0xc0f2d94fUL, 0x3e781de9UL, 0x2e3a2436UL,
0xf4dee16bUL, 0xd7efcb54UL, 0xf1f409f7UL, 0xaff582c3UL,
0xb9280bf4UL, 0xd9519d29UL, 0x9238c75eUL, 0x5aebf884UL,
0xb8e890d8UL, 0x3c0ddeb1UL, 0x8d0433d0UL, 0xe203685cUL,
0xdae4c55dUL, 0x589e3bdcUL, 0x9d460a0fUL, 0xc8d33fdaUL,
0x27db598fUL, 0x8cc4a8fcUL, 0x99ac79bfUL, 0x724e6c5aUL,
0xa2fe8ccaUL, 0xb5e39ed1UL, 0x76a41feaUL, 0x04ea73b0UL,
},

{
0x68bc02e3UL, 0x85620c55UL, 0x3f23312aUL, 0xab13f761UL,
0xd46d7298UL, 0xcb9a1921UL, 0x22a4613cUL, 0x9d3dcd45UL,
0xfdb42305UL, 0xc4075f2bUL, 0x2c01c09bUL, 0xd9800f3dUL,
0x6c5c7448UL, 0x7f7e85f9UL, 0x73ab1ff1UL, 0xedde0eb6UL,
0x3c6bed28UL, 0x97781a49UL, 0x2a918d9fUL, 0x579f33c9UL,
0x07a8aaa9UL, 0x0ded7da5UL, 0x422d8f7cUL, 0x4db0c976UL,
0x91e8574dUL, 0xa963ccceUL, 0xee96d2b4UL, 0x28e1b630UL,
0xf161b90dUL, 0x196726bdUL, 0x9bad8041UL, 0xa06ec7c0UL,
0x83f24151UL, 0xdbf03492UL, 0xa21efc6fUL, 0x32ce4c8fUL,
0xe0337313UL, 0xa7c66d69UL, 0x6d6493e5UL, 0x1a2ffabfUL,
0x1cbfb7bbUL, 0x7403b558UL, 0x6e2c4fe7UL, 0x89b7965dUL,
0x9c052ae8UL, 0x6619a344UL, 0x2e71fb34UL, 0xf229650fUL,
0x81827afeUL, 0x1322f1b1UL, 0x0835eca3UL, 0x510f7ecdUL,
0x7aa614ffUL, 0x7293f85cUL, 0xc297122fUL, 0x70e3c3f3UL,
0x2f491c99UL, 0x431568d1UL, 0xa3261bc2UL, 0xcc32b388UL,
0xcf7a6f8aUL, 0xe8069fb0UL, 0x47f51e7aUL, 0xbb79dad2UL,
0x950821e6UL, 0x98e55c43UL, 0xb83106d0UL, 0xe37baf11UL,
0x4165537eUL, 0xaa2b10ccUL, 0xb4e49cd8UL, 0x56a7d464UL,
0x7c3659fbUL, 0x4b208472UL, 0x9f4df6eaUL, 0x5faadf6aUL,
0xc1dfce2dUL, 0x48685870UL, 0xaff381caUL, 0x05d89106UL,
0x774b695aUL, 0xde28a594UL, 0xdf104239UL, 0x3bc34781UL,
0x82caa6fcUL, 0xc8d2c523UL, 0xf86cb203UL, 0x0cd59a08UL,
0xb7ac40daUL, 0xb909e17dUL, 0x24342c38UL, 0x5247a2cfUL,
0xb274d1dcUL, 0xa85b2b63UL, 0xd5559535UL, 0x9e751147UL,
0xe5ebe215UL, 0x9430c64bUL, 0x6f14a84aUL, 0x239c8691UL,
0x6acc394cUL, 0x8aff4a5fUL, 0x06904d04UL, 0x99ddbbeeUL,
0x1152ca1eUL, 0xffc418aaUL, 0x646998ebUL, 0xfefcff07UL,
0x345e018bUL, 0x7d0ebe56UL, 0xe79bd9baUL, 0x63c13242UL,
0xb5dc7b75UL, 0x26441797UL, 0xaecb6667UL, 0x250ccb95UL,
0x9a9567ecUL, 0x862ad057UL, 0x50379960UL, 0xe4d305b8UL,
0xad83ba65UL, 0xefae3519UL, 0xf6c913a4UL, 0x5b4aa9c1UL,
0x3e1bd687UL, 0xf0595ea0UL, 0x148a5b18UL, 0x02703bafUL,
0x04e076abUL, 0x4950bfddUL, 0x4a1863dfUL, 0xa5b656c6UL,
0x3d530a85UL, 0x871237faUL, 0xb694a777UL, 0x65517f46UL,
0x61b109edUL, 0xece6e91bUL, 0x458525d5UL, 0x753b52f5UL,
0xba413d7fUL, 0xce428827UL, 0xeb4e43b2UL, 0xbde997d6UL,
0x7b9ef352UL, 0x537f4562UL, 0x3afba02cUL, 0xbcd1707bUL,
0x1ff76bb9UL, 0x1b171d12UL, 0x79eec8fdUL, 0x277cf03aUL,
0x0a45d70cUL, 0xdd607996UL, 0x33f6ab22UL, 0xfa1c89acUL,
0xacbb5dc8UL, 0x0b7d30a1UL, 0xbea14bd4UL, 0xe10b94beUL,
0xcd0a5425UL, 0x7e466254UL, 0xf31182a2UL, 0xe6a33e17UL,
0x3566e626UL, 0x580275c3UL, 0x388b9b83UL, 0x44bdc278UL,
0x0348dc02UL, 0x92a08b4fUL, 0x39b37c2eUL, 0x6984e54eUL,
0x888f71f0UL, 0x2d392736UL, 0xd2fd3f9cUL, 0xfb246e01UL,
0x3716dd89UL, 0x00000000UL, 0x8d57e0f6UL, 0x93986ce2UL,
0x4ef81574UL, 0x20d45a93UL, 0x0138e7adUL, 0x405db4d3UL,
0x17c2871aUL, 0x106a2db3UL, 0x78d62f50UL, 0x8e1f3cf4UL,
0x0ea5a1a7UL, 0xb34c3671UL, 0xd725ae9aUL, 0x71db245eUL,
0x1d875016UL, 0x62f9d5efUL, 0x3186908dUL, 0x121a161cUL,
0xf581cfa6UL, 0x8c6f075bUL, 0xd61d4937UL, 0x593a926eUL,
0xc6776484UL, 0xc53fb886UL, 0x46cdf9d7UL, 0x90d0b0e0UL,
0xc74f8329UL, 0x9640fde4UL, 0x090d0b0eUL, 0xa156206dUL,
0xc9ea228eUL, 0x4c882edbUL, 0x76738ef7UL, 0x15b2bcb5UL,
0x185fc110UL, 0x2ba96a32UL, 0xa48eb16bUL, 0xf95455aeUL,
0x6089ee40UL, 0x55ef0866UL, 0x672144e9UL, 0x21ecbd3eUL,
0x30be7720UL, 0x8bc7adf2UL, 0xc0e72980UL, 0x1ecf8c14UL,
0xe24348bcUL, 0xa6fe8ac4UL, 0xd3c5d831UL, 0x16fa60b7UL,
0x80ba9d53UL, 0x4fc0f2d9UL, 0xe93e781dUL, 0x362e3a24UL,
0x6bf4dee1UL, 0x54d7efcbUL, 0xf7f1f409UL, 0xc3aff582UL,
0xf4b9280bUL, 0x29d9519dUL, 0x5e9238c7UL, 0x845aebf8UL,
0xd8b8e890UL, 0xb13c0ddeUL, 0xd08d0433UL, 0x5ce20368UL,
0x5ddae4c5UL, 0xdc589e3bUL, 0x0f9d460aUL, 0xdac8d33fUL,
0x8f27db59UL, 0xfc8cc4a8UL, 0xbf99ac79UL, 0x5a724e6cUL,
0xcaa2fe8cUL, 0xd1b5e39eUL, 0xea76a41fUL, 0xb004ea73UL,
}};

NAMESPACE_END
