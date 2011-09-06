
#define NCOLORS  300      // leds max
#define WIDTH     22      // size of 2d world of leds
#define HEIGHT    44      // size of 2d world of leds
#define NON       -1

//////////////////////////////////////////////////////////////////////////////////////////////
// utils
//////////////////////////////////////////////////////////////////////////////////////////////

#include <SPI.h>

//////////////////////////////////////////////////////////////////////////////////////////////
// color eye balance
//////////////////////////////////////////////////////////////////////////////////////////////

// http://blog.makezine.com/archive/2011/08/arduino-cookbook-excerpt-large-tables-of-data-in-program-memory.html
/*
#include <avr/pgmspace.h>  // needed for PROGMEM

// table of exponential values
// generated for values of i from 0 to 255 -> x=round( pow( 2.0, i/32.0) - 1);

const byte color_normalize[]PROGMEM = {
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,
   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   2,   2,   2,   2,   2,
   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   3,   3,   3,   3,   3,   3,
   3,   3,   3,   3,   3,   3,   4,   4,   4,   4,   4,   4,   4,   4,   4,   5,
   5,   5,   5,   5,   5,   5,   5,   6,   6,   6,   6,   6,   6,   6,   7,   7,
   7,   7,   7,   8,   8,   8,   8,   8,   9,   9,   9,   9,   9,  10,  10,  10,
  10,  11,  11,  11,  11,  12,  12,  12,  12,  13,  13,  13,  14,  14,  14,  15,
  15,  15,  16,  16,  16,  17,  17,  18,  18,  18,  19,  19,  20,  20,  21,  21,
  22,  22,  23,  23,  24,  24,  25,  25,  26,  26,  27,  28,  28,  29,  30,  30,
  31,  32,  32,  33,  34,  35,  35,  36,  37,  38,  39,  40,  40,  41,  42,  43,
  44,  45,  46,  47,  48,  49,  51,  52,  53,  54,  55,  56,  58,  59,  60,  62,
  63,  64,  66,  67,  69,  70,  72,  73,  75,  77,  78,  80,  82,  84,  86,  88,
  90,  91,  94,  96,  98, 100, 102, 104, 107, 109, 111, 114, 116, 119, 122, 124,
 127, 130, 133, 136, 139, 142, 145, 148, 151, 155, 158, 161, 165, 169, 172, 176,
 180, 184, 188, 192, 196, 201, 205, 210, 214, 219, 224, 229, 234, 239, 244, 250
};
*/

const int firewalk[] = {
 0,0,70, 4,0,65, 8,0,61, 13,0,56, 17,0,52, 21,0,48, 26,0,43, 30,0,39,
35,0,35, 39,0,30, 43,0,26, 48,0,21, 52,0,17, 56,0,13, 61,0,8, 65,0,4,
70,0,0, 70,4,0, 70,8,0, 70,13,0, 70,17,0, 70,21,0, 70,26,0, 70,30,0,
70,35,0, 70,39,0, 70,43,0, 70,48,0, 70,52,0, 70,56,0, 70,61,0, 70,65,0,
70,70,0, 70,70,4, 70,70,8, 70,70,13, 70,70,17, 70,70,21, 70,70,26, 70,70,30,
70,70,35, 70,70,39, 70,70,43, 70,70,48, 70,70,52, 70,70,56, 70,70,61, 70,70,65,
70,70,70, 70,70,70, 70,70,70, 70,70,70, 70,70,70, 70,70,70, 70,70,70, 70,70,70,
70,70,70, 70,70,70, 70,70,70, 70,70,70, 70,70,70, 70,70,70, 70,70,70, 70,70,70
};

const int falling[] = {
70,0,70, 65,4,70, 61,8,70, 56,13,70, 52,17,70, 48,21,70, 43,26,70, 39,30,70,
35,35,70, 30,39,70, 26,43,70, 21,48,70, 17,52,70, 13,56,70, 8,61,70, 4,65,70,
0,70,70, 4,70,65, 8,70,61, 13,70,56, 17,70,52, 21,70,48, 26,70,43, 30,70,39,
35,70,35, 39,70,30, 43,70,26, 48,70,21, 52,70,17, 56,70,13, 61,70,8, 65,70,4,
70,70,0, 65,70,0, 61,70,0, 56,70,0, 52,70,0, 48,70,0, 43,70,0, 39,70,0,
35,70,0, 30,70,0, 26,70,0, 21,70,0, 17,70,0, 13,70,0, 8,70,0, 4,70,0,
0,70,0, 4,65,0, 8,61,0, 13,56,0, 17,52,0, 21,48,0, 26,43,0, 30,39,0,
35,35,0, 39,30,0, 43,26,0, 48,21,0, 52,17,0, 56,13,0, 61,8,0, 65,4,0
};


const int colormap[] = {

  // bk  bk   bk   arm  arm  arm  arm  arm  fr   fr   fr    fr   fr   fr   arm  arm  arm  arm  arm  bk   bk   bk ( 22 wide )
  //                         lg   lg   lg   lg   lg   lg    rlg  lg   lg   lg   lg   lg

    NON, NON, NON, NON, NON, NON, NON, NON, NON, 190, NON,  NON, 159, NON, NON, NON, NON, NON, NON, NON, NON, NON, //1
    NON, 210, NON, NON, NON, NON, NON, NON, 191, NON, NON,  NON, NON, 158, NON, NON, NON, NON, NON, NON, NON, 226,

    225, NON, 209, NON, NON, NON, NON, NON, NON, 189, NON,  NON, 160, NON, 250, NON, NON, NON, NON, NON, 241, NON, //2
    NON, NON, NON, NON, NON, NON, NON,  49, 192, NON, 175,  174, NON, 157, NON, 269, NON, 289, NON, 242, NON, 227,

    NON, NON, 208, NON,  10, NON,  30, NON, NON, 188, NON,  NON, 161, NON, 251, NON, 270, NON, 290, NON, NON, NON, //3
    NON, 211, NON,   9, NON,  29, NON,  48, 193, NON, 176,  173, NON, 156, NON, 268, NON, 288, NON, NON, NON, NON,
  
    224, NON, 207, NON,  11, NON,  31, NON, NON, 187, NON,  NON, 162, NON, 252, NON, 271, NON, 291, NON, 240, NON, //4
    NON, 212, NON,   8, NON,  28, NON,  47, 194, NON, 177,  172, NON, 155, NON, 267, NON, 287, NON, 243, NON, 228,

    223, NON, 206, NON,  12, NON,  32, NON, NON, 186, NON,  NON, 163, NON, 253, NON, 272, NON, 292, NON, 239, NON, //5
    NON, 213, NON,   7, NON,  27, NON,  46, 195, NON, 178,  171, NON, 154, NON, 266, NON, 286, NON, 244, NON, 229,

    222, NON, 205, NON,  13, NON,  33, NON, NON, 185, NON,  NON, 164, NON, 254, NON, 273, NON, 293, NON, 238, NON, //6
    NON, 214, NON,   6, NON,  26, NON,  45, 196, NON, 179,  170, NON, 153, NON, 265, NON, 285, NON, NON, NON, 230,

    NON, NON, 204, NON,  14, NON,  34, NON, NON, 184, NON,  NON, 165, NON, 255, NON, 274, NON, 294, 245, NON, NON, //7
    221, NON, NON,   5, NON,  25, NON,  44, 197, NON, 180,  169, NON, 152, NON, 264, NON, 284, NON, NON, 237, NON,

    NON, NON, 203, NON,  15, NON,  35, NON, NON, 183, NON,  NON, 166, NON, 256, NON, 275, NON, 295, NON, NON, 231, //8
    NON, 215, NON,   4, NON,  24, NON,  43, 198, NON, 181,  168, NON, 151, NON, 263, NON, 283, NON, 246, NON, NON,

    220, NON, 202, NON,  16, NON,  36, NON, NON, 182, NON,  NON, 167, NON, 257, NON, 276, NON, 296, NON, 236, NON, //9
    NON, 216, NON,   3, NON,  23, NON,  42, 199, NON, NON,  NON, NON, 150, NON, 262, NON, 282, NON, 247, NON, 232,

    219, NON, 201, NON,  17, NON,  37, NON, NON, NON, NON,  NON, NON, NON, 258, NON, 277, NON, 297, NON, 235, NON, //10
    NON, 217, NON,   2, NON, NON, NON,  41, NON, NON, NON,  NON, NON, NON, NON, 261, NON, 281, NON, 248, NON, 233,

    218, NON, 200, NON,  18, NON,  38, NON, NON, NON, NON,  NON, NON, NON, 259, NON, 278, NON, 298, NON, 234, NON, //11
    NON, NON, NON,   1, NON,  22, NON,  40, NON, NON, NON,  NON, NON, NON, NON, 260, NON, 280, NON, 249, NON, NON,

    NON, NON, NON, NON,  19,  20,  21, NON, NON, NON, NON,  NON, NON, NON, NON, NON, 279, NON, 299, NON, NON, NON, //12
    NON, NON, NON,   0, NON, NON,  39, NON, NON, NON, NON,  NON, NON, NON, NON, NON, NON, NON, NON, NON, NON, NON,

  // bk  bk   bk   arm  arm  arm  arm  arm  fr   fr   fr    fr   fr   fr   arm  arm  arm  arm  arm  bk   bk   bk ( 22 wide )
  //                         lg   lg   lg   lg   lg   lg    rlg  lg   lg   lg   lg   lg

    NON, NON, NON, NON, NON, NON,  82, NON,  50, NON, NON,  NON, NON, 100, NON, 132, NON, NON, NON, NON, NON, NON, //13
    NON, NON, NON, NON, NON, NON, NON,  99, NON,  68, NON,  NON, 118, NON, 149, NON, 131, NON, NON, NON, NON, NON,
    
    NON, NON, NON, NON, NON, NON,  83, NON,  51, NON,  69,  119, NON, 101, NON, 133, NON, NON, NON, NON, NON, NON, //14
    NON, NON, NON, NON, NON,  81, NON,  98, NON,  67, NON,  NON, 117, NON, 148, NON, 130, NON, NON, NON, NON, NON,

    NON, NON, NON, NON, NON, NON,  84, NON,  52, NON,  70,  120, NON, 102, NON, 134, NON, NON, NON, NON, NON, NON, //15
    NON, NON, NON, NON, NON,  80, NON,  97, NON,  66, NON,  NON, 116, NON, 147, NON, 129, NON, NON, NON, NON, NON,

    NON, NON, NON, NON, NON, NON,  85, NON,  53, NON,  71,  121, NON, 103, NON, 135, NON, NON, NON, NON, NON, NON, //16
    NON, NON, NON, NON, NON,  79, NON,  96, NON,  65, NON,  NON, 115, NON, 146, NON, 128, NON, NON, NON, NON, NON,

    NON, NON, NON, NON, NON, NON,  86, NON,  54, NON,  72,  122, NON, 104, NON, 136, NON, NON, NON, NON, NON, NON, //17
    NON, NON, NON, NON, NON,  78, NON,  95, NON,  64, NON,  NON, 114, NON, 145, NON, 127, NON, NON, NON, NON, NON,

    NON, NON, NON, NON, NON, NON,  87, NON,  55, NON,  73,  123, NON, 105, NON, 137, NON, NON, NON, NON, NON, NON, //18
    NON, NON, NON, NON, NON,  77, NON,  94, NON,  63, NON,  NON, 113, NON, 144, NON, 126, NON, NON, NON, NON, NON,

    NON, NON, NON, NON, NON, NON,  88, NON,  56, NON,  74,  124, NON, 106, NON, 138, NON, NON, NON, NON, NON, NON, //19
    NON, NON, NON, NON, NON,  76, NON,  93, NON,  62, NON,  NON, 112, NON, 143, NON, NON, NON, NON, NON, NON, NON,

    NON, NON, NON, NON, NON, NON,  89, NON,  57, NON,  75,  125, NON, 107, NON, 139, NON, NON, NON, NON, NON, NON, //20
    NON, NON, NON, NON, NON, NON, NON,  92, NON,  61, NON,  NON, 111, NON, 142, NON, NON, NON, NON, NON, NON, NON,

    NON, NON, NON, NON, NON, NON,  90, NON,  58, NON, NON,  NON, NON, 108, NON, 140, NON, NON, NON, NON, NON, NON, //21
    NON, NON, NON, NON, NON, NON, NON,  91, NON,  60, NON,  NON, 110, NON, 141, NON, NON, NON, NON, NON, NON, NON,

    NON, NON, NON, NON, NON, NON, NON, NON,  59, NON, NON,  NON, NON, 109, NON, NON, NON, NON, NON, NON, NON, NON, //22
    NON, NON, NON, NON, NON, NON, NON, NON, NON, NON, NON,  NON, NON, NON, NON, NON, NON, NON, NON, NON, NON, NON,

};

//////////////////////////////////////////////////////////////////////////////////////////////
// state
//////////////////////////////////////////////////////////////////////////////////////////////

typedef struct {
  byte r,g,b,a;
  byte r2,g2,b2,a2;
} Color;

Color colors[NCOLORS];            // colors / leds

/////////////////////////////////////////////////////////////////////////////////////////
// effects and effects control
/////////////////////////////////////////////////////////////////////////////////////////

byte effect_target_color_r = 0;
byte effect_target_color_g = 0;
byte effect_target_color_b = 0;
byte effect_color_r        = 255;
byte effect_color_g        = 0;
byte effect_color_b        = 0;
byte effect_color_r2       = 0;
byte effect_color_g2       = 0;
byte effect_color_b2       = 255;
byte effect_x              = 0;
byte effect_y              = 0;
byte effect_z              = 0;

// Effects are VERY limited

#define NEFFECTS 8

#define EFFECT_NOTHING       0
#define EFFECT_FIREWALK      1
#define EFFECT_FALLING       2
#define EFFECT_ROLLING       3
#define EFFECT_RANDOM        4
#define EFFECT_FOLLOW        5

//#define EFFECT_PINWHEEL      6
//#define EFFECT_EXPLOSION     7
//#define EFFECT_FADE          9

typedef struct {
  float x,y;     // where it is
  float rx,ry;
  int a,s,d,v;   // attack sustain decay velocity
  int t;         // type see above defines
  byte r,g,b;    // a color
} Effect;

Effect effects[NEFFECTS];

void effects_reset() {
  for(int i = 0; i < NEFFECTS; i++) {
    effects[i].x = 0;
    effects[i].y = 0;
    effects[i].rx = 0;
    effects[i].ry = 0;
    effects[i].a = 0;
    effects[i].s = 0;
    effects[i].d = 0;
    effects[i].v = -1;
    effects[i].t = i;
    effects[i].r = 255;
    effects[i].g = 255;
    effects[i].b = 255;
  }
}

void effect_start(int fx,byte r, byte g, byte b) {
        effects[fx].rx = 0.3;
        effects[fx].ry = 0.3;
        effects[fx].a = 32000;
        effects[fx].s = 0;
        effects[fx].d = 0;
        effects[fx].v = -1;
        effects[fx].t = fx;
        effects[fx].r = r;
        effects[fx].g = g;
        effects[fx].b = b;
}

#define fx_raise(a,b) if(b) { a = a + b < 100 ? a+b : 100; }

byte sinewave = 0;
float sinecounter = 0;
int counter = 0;

#define RANDOMWALK(r)   r = r + random(2) -2 ;
#define LIMIT(r) r = r < 100 ? r : 100;

void effects_do() {
  
  // a counter
  counter++;

  // a sine wave in the 0 to 256 space
  sinewave = sin( sinecounter )  * 128.0 + 128.0; sinecounter = sinecounter + 0.01; if(sinecounter > 6.28 ) sinecounter = 0;
  
  // visit all effects and update their timers and stuff

  // update effects state
  for(int i = 0; i < NEFFECTS ; i++) {

    // effect done?
    if(effects[i].a <= 0) continue;

    // age effect
    effects[i].a = effects[i].a + effects[i].v;

    // default mover
    effects[i].x = effects[i].x + effects[i].rx; if(effects[i].x >= WIDTH ) effects[i].x = 0;
    effects[i].y = effects[i].y + effects[i].ry; if(effects[i].y >= HEIGHT ) effects[i].y = 0;

    // custom details
    switch(effects[i].t) {
      case EFFECT_FIREWALK:
        break;
      case EFFECT_FALLING:
        RANDOMWALK(effects[i].r);
        RANDOMWALK(effects[i].g);
        RANDOMWALK(effects[i].b);
        LIMIT(effects[i].r);
        LIMIT(effects[i].g);
        LIMIT(effects[i].b);
        break;
      default:
        break;
    }

  }

  //////////////////////////////////////////////////////////////////////
  // update all spatial effects by visiting spatially mapped suit positions
  //////////////////////////////////////////////////////////////////////
  int d;

  for(int cy = 0; cy < HEIGHT; cy++) {

    for(int cx = 0; cx < WIDTH; cx++ ) {

      // fetch sequence color from spatial table - or skip if no color here
      int c = colormap[(int)(cx+cy*WIDTH)];
      if(c==NON) continue;

      // visit effect on this sequence color
      for(int i = 0; i < NEFFECTS ; i++) {
        if(effects[i].a <= 0) continue;
        switch(effects[i].t) {
          case EFFECT_FIREWALK:
            if(cx < WIDTH/2 && cx == (int)effects[i].x) {
              if(cx<WIDTH/2) {
                d = abs(counter+(cx+cy/10)) & 63;
              } else {
                d = abs(counter+WIDTH/2-(cx+cy/10)) & 63;
              }
              fx_raise(colors[c].r,firewalk[d*3+0]);
              fx_raise(colors[c].g,firewalk[d*3+1]);
              fx_raise(colors[c].b,firewalk[d*3+2]);
            }
            if(cx >= WIDTH/2 && (WIDTH/2-cx) == (int)effects[i].x) {
              if(cx<WIDTH/2) {
                d = abs(counter+(cx+cy/10)) & 63;
              } else {
                d = abs(counter+WIDTH/2-(cx+cy/10)) & 63;
              }
              fx_raise(colors[c].r,firewalk[d*3+0]);
              fx_raise(colors[c].g,firewalk[d*3+1]);
              fx_raise(colors[c].b,firewalk[d*3+2]);
            }
            break;
          case EFFECT_FALLING:
            // if spatial y position matches effect y then light up that row
            if(cy == (int)(effects[i].y)) {
              fx_raise(colors[c].r,effects[i].r);
              fx_raise(colors[c].g,effects[i].g);
              fx_raise(colors[c].b,effects[i].b);
            }
            break;
          case EFFECT_ROLLING:
            // if spatial x position matches effect x then light up that row
            if(cx == (int)(effects[i].x)) {
              fx_raise(colors[c].r,effects[i].r);
              fx_raise(colors[c].g,effects[i].g);
              fx_raise(colors[c].b,effects[i].b);
            }
            break;
          case EFFECT_RANDOM: break; // not here - elsewhere
          case EFFECT_FOLLOW:
            break;
          case EFFECT_NOTHING:
          default:
            break;
        }
      }
    }
  }

  //////////////////////////////////////////////////////////////////////
  // update all non spatial effects as per their own desired approach
  //////////////////////////////////////////////////////////////////////

  // randomly introduce colors all over the surface
  if(effects[EFFECT_RANDOM].a > 0 ) {
    int c = random(NCOLORS);
    colors[c].r = random(40) + 20;
    colors[c].g = random(40) + 20;
    colors[c].b = random(40) + 20;
  }

  // randomly introduce colors all over the surface
  if(effects[EFFECT_FOLLOW].a > 0 ) {
    int dial = analogRead(A8) / 3; if(dial> 255) dial = 255;
    colors[dial].r = 70;
    colors[dial].g = 0;
    colors[dial].b = 70;
  }
}

/////////////////////////////////////////////////////////////////////////////////////////
// dial stuff
/////////////////////////////////////////////////////////////////////////////////////////

#define DIAL_NOTHING    0
#define DIAL_STOP       1
#define DIAL_FIREWALK   2
#define DIAL_MOVEX      3
#define DIAL_MOVEY      4
#define DIAL_ROLLING    5
#define DIAL_FALLING    6
#define DIAL_RANDOM     7
#define DIAL_FOLLOW     8

byte dial_topic_pick = 0;
byte dial_topic      = 0;
byte dial_effect     = 0;
int latcher = 0;

int dx[] = { 1,18,22,38,19,20,0 };

void effects_topic_flash(byte r, byte g, byte b) {
  for(int i = 0; i < 7; i++) {
    if(r>255)r=255;
    if(g>255)g=255;
    if(b>255)b=255;
    int j = dx[i];
    colors[j].r = r;
    colors[j].g = g;
    colors[j].b = b;
  }
}

void dial_do() {

  // read a hardware dial that the human user can rotate through topics; our only form of external input to the suit - looks like it is about < 1000
  int dial = analogRead(A8);

  // if dial is to left at any time then we enter a topic guide mode where we pick topics
  if(dial > 30 && dial < 80 ) {
    dial_topic_pick = 1;
    dial_topic = 0;
    latcher = 0;
  }

  // if we successfully entered topic selection then bring up a menu of topics to roll through
  if(dial_topic_pick) {
    int topic = dial/80;
    switch(topic) {
      case DIAL_NOTHING:    effects_topic_flash(50,50,50); break;
      case DIAL_STOP:       effects_topic_flash(0,50,50);  break;
      case DIAL_FIREWALK:   effects_topic_flash(50,50,0);  break;
      case DIAL_MOVEX:      effects_topic_flash(0,0,50);   break;
      case DIAL_MOVEY:      effects_topic_flash(50,50,0);  break;
      case DIAL_ROLLING:    effects_topic_flash(00,0,50);  break;
      case DIAL_FALLING:    effects_topic_flash(50,0,0);   break;
      case DIAL_RANDOM:     effects_topic_flash(0,50,0);   break;
      case DIAL_FOLLOW:     effects_topic_flash(50,50,50); break;
      default:              return; // just show nothing up here.
    }
    // once we are in a topic long enough then go to it ... leaving top level topic picking
    if(dial_topic == topic) latcher++; else latcher = 0;
    dial_topic = topic;
    if(latcher>100) {
      dial_topic_pick = 0;
      latcher = 0;
    }

    return;
  }
  
  // deal with current dial topic if any
  switch(dial_topic) {
    default:
    case DIAL_NOTHING:
      break;
    case DIAL_STOP:
      dial_topic = 0;
      effects_reset();
      break;
    case DIAL_FIREWALK:
      effect_start(EFFECT_FIREWALK,0,40,0); dial_topic = 0; break;
    case DIAL_ROLLING:
      effect_start(EFFECT_ROLLING,0,0,30); dial_topic = 0; break;
    case DIAL_FALLING:
      effect_start(EFFECT_FALLING,30,0,0); dial_topic = 0; break;
    case DIAL_RANDOM:
      effect_start(EFFECT_RANDOM,0,0,0); dial_topic = 0; break;
    case DIAL_FOLLOW:
      effect_start(EFFECT_FOLLOW,0,40,0); dial_topic = 0; break;
  }
} 

/////////////////////////////////////////////////////////////////////////////////////////
// run
/////////////////////////////////////////////////////////////////////////////////////////

void loop() {

  // turn the light on and off so we can see latency
  // digitalWrite(13, HIGH);  
  
  // apply effects to the color list array in memory
  effects_do();

  // do ui
  dial_do();
  
  // mark begin write
  SPI.transfer(0x00);
  SPI.transfer(0x00);
  SPI.transfer(0x00);
  SPI.transfer(0x00);

  // visit every color
  float energy = 0;
  for(int i = 0; i < NCOLORS ; i++ ) {

    // read all color components
    byte r = colors[i].r;
    byte g = colors[i].g;
    byte b = colors[i].b;

#if 0
    // even out energy distribution
    if(0) {
      // slightly slightly amplify the energy of things further down the line if we've used up a lot already
      energy = energy + r+r+r+r + g+g+g+g + b+b+b+b;                  // have we used a lot?
      r = r + energy/(i*768); if(r>255) r = 255;    // pour a bit of that forward 
      g = g + energy/(i*768); if(g>255) g = 255;
      b = b + energy/(i*768); if(b>255) b = 255;
    }
#endif

    // write to suit
    byte f = ~(((r&0xC0)>>6) | ((g&0xC0)>>4) | ((b&0xC0)>>2));
    SPI.transfer(f);
    SPI.transfer(b);
    SPI.transfer(g);
    SPI.transfer(r);

    //////////////////////////////////////////////////////////////////////
    // apply effects that are associated with each light sequentially
    // this effect is hardcoded on - i removed it from the effects list
    //////////////////////////////////////////////////////////////////////

    // apply a global fader effect after the light is out to the suit
    if( r > 0 ) { colors[i].r = r>5?(r-5):0; }
    if( g > 0 ) { colors[i].g = g>5?(g-5):0; }
    if( b > 0 ) { colors[i].b = b>5?(b-5):0; }

  }

  // end write ( this may not be needed )
  SPI.transfer(0x00);
  SPI.transfer(0x00);
  SPI.transfer(0x00);
  SPI.transfer(0x00);

  // digitalWrite(13, LOW);   
  // delay(0) - don't bother sleeping because we are not running very fast
}

void setup() {
  SPI.begin();
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE0);
  SPI.setClockDivider(SPI_CLOCK_DIV2);
  pinMode(13, OUTPUT);
  effects_reset();


      effect_start(EFFECT_ROLLING,0,0,50); 
      //effect_start(EFFECT_FALLING,0,0,50); 

}

