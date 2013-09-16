
void setup() {
 size(256,256); 
 smooth(); 
 background(0);
 noStroke();
 fill(226);

 make_colortable();
}

int[] colors = new int[64*3];
int[] colors2 = new int[64*3];
int[] colors3 = new int[64*3];
int[] colors4 = new int[64*3];
int[] spectrum = { 30,30,70, 30,70,0, 70,70,0, 0,70,0, 70,0,0 };
int[] spectrum2 =  { 0,0,70,  70,0,0,  70,70,0, 70,70,70, 70,70,70 };
int[] spectrum3 = { 0,0,255, 255,0,0, 255,255,0, 255,255,255, 255,255,255 };
int[] spectrum4 = { 0,0,255, 255,0,0, 255,255,0, 255,255,255, 255,255,255 };

int counter = 0;
float r=0,g=0,b=0;
float r1=0,g1=0,b1=0;
float r2=0,g2=0,b2=0;

void interpolate(float x, float y) {
  r = r1*x + r2*y;
  g = g1*x + g2*y;
  b = b1*x + b2*y;
}

void make_colortable() {

 for(int i = 0; i < 64; i++) {
   
   // get colors to work on
   int c = i>>4;
   r1 = spectrum[c*3+0];
   g1 = spectrum[c*3+1];
   b1 = spectrum[c*3+2];
   r2 = spectrum[c*3+3];
   g2 = spectrum[c*3+4];
   b2 = spectrum[c*3+5];
   
   // how close are we to first color?
   float x = (16.0-(i&15))/16.0;

   // how close are we to second color?
   float y = (i&15)/16.0;

   // spectrum contribution as percentage of closeness to that item
   interpolate(x,y);

   colors[i*3+0] = (int)r;
   colors[i*3+1] = (int)g;
   colors[i*3+2] = (int)b;

 }

 for(int i = 0; i < 8 ; i++ ) {
   for(int j = 0; j < 8 ; j++ ) {
     r = colors[(i*8+j)*3+0];
     g = colors[(i*8+j)*3+1];
     b = colors[(i*8+j)*3+2];
     print( (int)r + "," + (int)g + "," + (int)b  );
     if(j<7) print(", ");
   }
   print("\n");
 }

}

void draw() {
 
 counter = counter + 1;
 
 // try to draw a pattern from dark blue to light yellow
 // animated it based on distance from center
/*
 for(int i = 0 ; i < 64 ; i++ ) {
   int r = colors[ i*3+0 ];
   int g = colors[ i*3+1 ];
   int b = colors[ i*3+2 ];
   fill(r,g,b);
   color(r,g,b);
   rect(i*4,0,i*4+4,256);
 }
*/

 for(int j = 0 ; j < 256 ; j++ ) {
 for(int i = 0 ; i < 256 ; i++ ) {
   int d;
   if(i<128) {
     d = abs(i-128+counter) & 63;
   } else {
     d = abs((255-i)-128+counter) & 63;
   }
   
   int r = colors[ d*3+0 ];
   int g = colors[ d*3+1 ];
   int b = colors[ d*3+2 ];
   
   fill(r,g,b);
   color(r,g,b);
   rect(i,j,i,j);
 }
 }

  
}
