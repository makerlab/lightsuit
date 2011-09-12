
float xmag, ymag = 0;
float newXmag, newYmag = 0; 
int nvertices = 0;
int nfaces = 0;
float[] vertices = null;
int[] faces = null;

//////////////////////////////////////////////////////////////////////////////////////////////
// test data
///////////////////////////////////////////////////////////////////////////////////////////////

float[] cube_vertices = {

  1, 0, 0,   -1, -1,  -1,
  1, 0, 0,    1, -1,  -1,
  1, 0, 0,    1,  1,  -1,
  0, 0, 1,   -1,  1,  -1,

  0, 0, 1,   -1, -1,  1,
  0, 0, 1,    1, -1,  1,
  0, 0, 1,    1,  1,  1,
  0, 0, 1,   -1,  1,  1,

};

int[] cube_faces = {
  0,1,2,3,
  4,5,6,7,
  0,1,5,4,
};

//////////////////////////////////////////////////////////////////////////////////////////////
// load data
///////////////////////////////////////////////////////////////////////////////////////////////

void load() {
  String[] lines = loadStrings("/Users/ahook/anselm_body.obj");
  vertices = new float[lines.length*6]; // basically overestimate
  faces = new int[lines.length*4];
  nvertices = 0;
  nfaces = 0;
  for(int i = 0; i < lines.length; i++ ) {
    String[] pieces = split(lines[i], ' ');
    if (pieces.length == 4) {
      vertices[nvertices] = 0; nvertices++;
      vertices[nvertices] = 0; nvertices++;
      vertices[nvertices] = 1; nvertices++;
      vertices[nvertices] = float(pieces[1]); nvertices++;
      vertices[nvertices] = float(pieces[2]) - 10; nvertices++;
      vertices[nvertices] = float(pieces[3]); nvertices++;
    }
    if(pieces.length == 5) {
      faces[nfaces] = int(pieces[1])-1; nfaces++;
      faces[nfaces] = int(pieces[2])-1; nfaces++;
      faces[nfaces] = int(pieces[3])-1; nfaces++;
      faces[nfaces] = int(pieces[4])-1; nfaces++;
    }
  }
  nvertices = nvertices / 6;
  nfaces = nfaces/4;
  println("Loaded nvertices="+nvertices+" nfaces="+nfaces);
}

void scavenge_vertices() {
  // it turned out i had zero duplicate verts - so this is not needed
  int scavenged = 0;
  for(int j = 0; j < faces.length;j++) {
    
    // get at next original vertex cited by quad
    int v = faces[j];
    float x = vertices[v*6+3];
    float y = vertices[v*6+4];
    float z = vertices[v*6+5];

    // look for an earlier vertex that is similar and switch to it if so
    for(int v2 = 0; v2 < v; v2++) {
      float r = vertices[v2*6+0];
      float g = vertices[v2*6+1];
      float b = vertices[v2*6+2];
      float x2 = vertices[v2*6+3];
      float y2 = vertices[v2*6+4];
      float z2 = vertices[v2*6+5];
      if(x2 == x && y2 == y && z2 == z) {
        faces[j] = v2;
        scavenged++;
        break;
      }
    }
  }
  println("scavenged " + scavenged);
}

//////////////////////////////////////////////////////////////////////////////////////////////
// generate a human body shape
///////////////////////////////////////////////////////////////////////////////////////////////

void span(float x1, float y1, float z1,
          float r1, float g1, float b1,
          float x2, float y2, float z2,
          float r2, float g2, float b2,
          float steps)
{
  for(float i = 0; i < steps; i++) {
    float r = (r2-r1)*i/steps + r1;
    float g = (g2-g1)*i/steps + g1;
    float b = (b2-b1)*i/steps + b1;
    float x = (x2-x1)*i/steps + x1;
    float y = (y2-y1)*i/steps + y1;
    float z = (z2-z1)*i/steps + z1;
    vertices[nvertices] = r; nvertices++;
    vertices[nvertices] = g; nvertices++;
    vertices[nvertices] = b; nvertices++;
    vertices[nvertices] = x; nvertices++;
    vertices[nvertices] = y; nvertices++;
    vertices[nvertices] = z; nvertices++;
  }
}

void limb(float x,float y,float xstep) {
  // limbs start on outer limb, down 10, back up 10, then on the inner portion do 5 down
  // then on the back side do 5 back up, then down 10 then up 10 again; ending near where started.
  span(x,y+0, 1, 1,1,0,  x,y+10, 1,  1,0,0,  10 ); x-= xstep;
  span(x,y+9, 1, 1,0,0,  x,y-1, 1,   1,1,0,  10 ); x-= xstep;
  span(x,y+0, 1, 1,1,0,  x,y+5, 1,   1,0,0,   5 ); x+= xstep; 
  span(x,y+4,-1, 1,1,1,  x,y-1,-1,   1,0,0,   5 ); x+= xstep;
  span(x,y+0,-1, 1,0,1,  x,y+10,-1,  1,1,0,  10 ); x+= xstep;
  span(x,y+9,-1, 1,1,1,  x,y-1,-1,   1,0,0,  10 );
}

void chest() {
  // the chest is 100 lights - a 10 x 10 pattern
  float x = 0;
  float y = -10;
  float z = 1;
  float xstep = 0;
  for(float i = 0; i < 10 ; i++) {
    span(-5,y+i, z, 0,0.5,i/10,  5,y+i, z,  0,0,i/10,  10 );
  }
}

void back() {
  // the back is 100 lights - a 10 x 10 pattern
  float x = 0;
  float y = -10;
  float z = -1;
  float xstep = 0;
  for(float i = 0; i < 10 ; i++) {
    span(-5,y+i, z, 0.5,0.5,i/10,  5,y+i, z,  0.5,0,i/10,  10 );
  }
}

void body() {

  // we have 400 lights in use
  nfaces = 0;
  nvertices = 400*6;
  vertices = new float[nvertices];

  nfaces = 0;
  nvertices = 0;  

  limb(-10,  0,-1); // left leg
  limb( 10,  0, 1); // right leg
  limb(-10,-10,-1); // left arm
  limb( 10,-10, 1); // right arm
  chest();
  back();

  // #5 make front torso

  nvertices = nvertices / 6;
  println("num verts is " + nvertices );
}

//////////////////////////////////////////////////////////////////////////////////////////////
// animate
///////////////////////////////////////////////////////////////////////////////////////////////

// test logic

void randomize_vertex_colors() {
  for(int i = 0; i < nvertices; i++) {
    int v = i * 6;
    float r = vertices[v+0];
    float g = vertices[v+1];
    float b = vertices[v+2];
    float x = vertices[v+3];
    float y = vertices[v+4];
    float z = vertices[v+5];

    vertices[v+0] = random(1);
    vertices[v+1] = random(1);
    vertices[v+2] = random(1);
  }
}


//////////////////////////////////////////////////////////////////////////////////////////////
// print
///////////////////////////////////////////////////////////////////////////////////////////////

void draw_faces() {
  beginShape(QUADS);
  // the faces is just a huge list of vertices... visit each and add to opengl
  for(int i = 0;i < nfaces * 4;i++) {
    int v = faces[i] * 6;
    float r = vertices[v+0];
    float g = vertices[v+1];
    float b = vertices[v+2];
    float a = 0.8;
    float x = vertices[v+3];
    float y = vertices[v+4];
    float z = vertices[v+5];
    fill(r,g,b,a);
    vertex(x,y,z);
  }
  endShape();
}

void draw_spheres() {
  sphereDetail(5);

  for(int i = 0;i < nvertices;i++) {
    int v = i * 6;
    float r = vertices[v+0];
    float g = vertices[v+1];
    float b = vertices[v+2];
    float x = vertices[v+3];
    float y = vertices[v+4];
    float z = vertices[v+5];    
    pushMatrix();
    translate(x,y,z);
    scale(0.1,0.1,0.1);
    stroke(r,g,b,0.5);
    fill(r,g,b,0.5);
    sphere(1);
    popMatrix();
  }  
}

void setup() { 
  size(640, 640, P3D); 
  noStroke(); 
  colorMode(RGB, 1); 
  // load();
  // scavenge_vertices();
  body();
} 


void draw()  { 

  // clear the screen
  background(0.5);

  // save all rotation state  
  pushMatrix(); 
 
  // back up the camera
  translate(width/2, height/2, -30); 

  // read the user mouse  
  newXmag = (mouseX-width/2)/float(width) * TWO_PI;
  newYmag = (mouseY-height/2)/float(height) * TWO_PI;
  
  // rotate the view based on the mouse
  float diff = xmag-newXmag;
  if (abs(diff) >  0.01) { xmag -= diff/4.0; }
  diff = ymag-newYmag;
  if (abs(diff) >  0.01) { ymag -= diff/4.0; }  
  rotateX(-ymag); 
  rotateY(-xmag); 

  // scale the world a bit  
  scale(30);

  // draw shape
  draw_spheres();
  
  popMatrix(); 

} 

// my overarching metaphor for manipulating the skin is a series of effects objects that together act on the skin
// these effects are chained both in time and in layers so that the skin ( with latency of its own ) is the bottom effect
// an effect is a visible entity that moves through space over time
// it has a ramp up attack sequence; typically shifting into full brightness
// it has a sustained activity period
// it has a ramp down period
// it may spawn other effects or be chained to them; or even collide with them
// it typically moves around the space over time with inertia
// it tends to be a single self contained visible concept - complicated visuals are multiple effects
// it knows which way is up or down or left or right or front or back and can move in those directions
// it could disappear and re-appear somewhere else if it wants but this may be the province of effect layers
// the skin it is on has a latency of its own; these effects may layered therefore... to a final substrate.
// the kinds of effects i would like to do

abstract class effect {
  int attack;      // time period of the effect warming up
  int sustain;     //
  int decay;
  int time;
  int longevity;
  void behavior() {
  }
}

// effect_update_state_base() {
//
//   - if target position then update impulse to reach target
//   - if gravity then update impulse due to gravity
//
//   - if random walk then set new target periodically
//   - if random then set new position periodically
//   - if chase then set new target position == target if target still exists
//
//   - do update impulse due to energy consumption
//   - do update momentum due to impulse with limits due to friction or absolute limits
//   - do update position due to momentum
//   - do wall reflections and bouncing if any enabled
//   - do friction if any
//
//   - do rotation if any? ( do i need a 2d matrix transforms system? pre or post rotate? )
//   - do scaling if any?
//
//   - per source pixel color targets if any with attack rate { otherwise all pixels are white }
//
//   - age effect over time
//   - switch to attack,sustain,decay,release rules at appropriate time { new impulses, color targets etc }
//   - after n amount of time chain to specified effect if any 
//   - spawn new effects at a rate if any rate and new effects specified; rate can be random.
// }
//
// effect_render_base(x,y,incoming_pixel) {
//      rxy = index into effect location of this pixel
//      xy = pixel location + effect location + rotation and scaling
//      source color = seek to target color if any with attack etc
//      color1 = source rxy color
//      color2 = target xy color
//      color = source + target or other rule...
//      xy = color
//      return outgoing_pixel
//  }
//
//  other effects include:
//   - apply blur, fade, latency, shudder, 3d red blue to source pixels
//   - all pixels are passed down to the next effect in the stack or rendered immediately to a raw surface
//   - skin fade
//   - shudder and blur and 3d (redblue) color etc on entire skin
//
// global_render {
//
//    - for each pixel
//      - for all active effects
//        - read the raw world pixel
//        - ask effect for a pixel for this location; pass it previous stacked pixel if any
//        - 
// }
//
// 
//  high level
//
//  - emitter object
//  - sequence object
//  - how can we stack effects?
//  - latency ( skin )
//  - bar
//  - shape
//  - text
//  - fire feet
//  - firefly
//  - firework
//  - humor and visual imagery; hearts, gay pride, skeleton man, blending into scenery, chameleon, fountains
//
// my_effect: base_effect {
//   setup { set art and rules }
//
// pipeline
//
//  - run effects { now they have new positions }
//  - render all active effects - effects are arranged in a stack with a priority value and an area of coverage
//    each effect pipes all of its pixels to the next level down
//
// effects master scripts
//
// script1 {
//    effect1 [ start time, chain targets, other properties like duration etc ]
//    effect2 [ start time, chain targets, other properties ]
//    ...
//

class supereffect extends effect {
  void behavior() {
    super.behavior();
  }
}





