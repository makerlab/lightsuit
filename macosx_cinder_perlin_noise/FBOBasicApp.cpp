#include "cinder/Cinder.h"
// select an appropriate base class for our App based on whether we're on iOS or not
#if defined( CINDER_COCOA_TOUCH )
	#include "cinder/app/AppCocoaTouch.h"
	typedef ci::app::AppCocoaTouch AppBase;
#else
	#include "cinder/app/AppBasic.h"
	typedef ci::app::AppBasic AppBase;
#endif

#include "cinder/Camera.h"
#include "cinder/gl/Fbo.h"
#include "cinder/Perlin.h"
#include "cinder/Rand.h"
#include "cinder/Utilities.h"
#include "cinder/params/Params.h"

#include "simplexnoise.h"
#include "simplextextures.h"


using namespace ci;
using namespace ci::app;
using namespace std;


float fDeltaT = 0;
float fLastTime = 0;
float fTime = 0;
float fBaseSpeed = .0001;
float fSpeed = fBaseSpeed;
float iNoiseType = 0;


float r,g,b;
int cleared = 0;



class FBOBasicApp : public AppBase {
  public:
	virtual void	setup();
	virtual void	update();
	virtual void	draw();
	void			renderSceneToFbo();
	gl::Fbo				mFbo;
	Matrix44f			mTorusRotation;
	static const int	FBO_WIDTH = 256, FBO_HEIGHT = 256;
  
    params::InterfaceGl		mParams;
    int scale = 0.0042 * 10000.0f;
    int persistence = 0.03 * 100.0f;
    int octaves = 1;
    int zoom = 3;
    int speed = 100;
};





void FBOBasicApp::setup() {
	gl::Fbo::Format format;
//	format.setSamples( 4 ); // uncomment this to enable 4x antialiasing
	mFbo = gl::Fbo( FBO_WIDTH, FBO_HEIGHT, format );
	gl::disableDepthRead();
	gl::disableDepthWrite();
	mTorusRotation.setToIdentity();
    
	mParams = params::InterfaceGl( "Parameters", Vec2i( 200, 200 ) );
	mParams.addParam( "Persistence", &persistence, "keyIncr=c" );
	mParams.addParam( "Scale", &scale, "keyIncr== keyDecr=-" );
	mParams.addParam( "Octaves", &octaves, "" );
	mParams.addParam( "Zoom", &zoom, "" );
	mParams.addParam( "Speed", &speed, "" );

}




void FBOBasicApp::update() {
        
	// Rotate the torus by .06 radians around an arbitrary axis
	mTorusRotation.rotate( Vec3f( 0.16666f, 0.333333f, 0.666666f ).normalized(), 0.06f );

	
    // this will restore the old framebuffer binding when we leave this function
	// on non-OpenGL ES platforms, you can just call mFbo.unbindFramebuffer() at the end of the function
	// but this will restore the "screen" FBO on OpenGL ES, and does the right thing on both platforms
	gl::SaveFramebufferBinding bindingSaver;
	
	// bind the framebuffer - now everything we draw will go there
	mFbo.bindFramebuffer();

	// setup the viewport to match the dimensions of the FBO
	gl::setViewport( mFbo.getBounds() );

	// setup our camera to render the torus scene
	CameraPersp cam( mFbo.getWidth(), mFbo.getHeight(), 60.0f );
	cam.setPerspective( 60, mFbo.getAspectRatio(), 1, 1000 );
	cam.lookAt( Vec3f( 2.8f, 1.8f, -2.8f ), Vec3f::zero() );
	gl::setMatrices( cam );

	// set the modelview matrix to reflect our current rotation
	gl::multModelView( mTorusRotation );
	
	// clear out the FBO with blue
    if(!cleared) {
        gl::clear( Color( 0.25, 0.5f, 1.0f ) );
        cleared = 1;
    }

	// render an orange torus, with no textures
	glDisable( GL_TEXTURE_2D );
    r = r + 0.01f; if(r>1)r=0.5f;
    g = g - 0.02f; if(g<0.5f)g=1;
    b = b + 0.03f; if(b>1)b=0.5f;
	gl::color( Color( r,g,b ) );
	gl::drawTorus( 1.4f, 0.3f, 32, 64 );
    

//	gl::drawColorCube( Vec3f::zero(), Vec3f( 2.2f, 2.2f, 2.2f ) );

}



#include <stdlib.h>
#include <stdio.h>

#define MIN3(x,y,z)  ((y) <= (z) ? \
((x) <= (y) ? (x) : (y)) \
: \
((x) <= (z) ? (x) : (z)))

#define MAX3(x,y,z)  ((y) >= (z) ? \
((x) >= (y) ? (x) : (y)) \
: \
((x) >= (z) ? (x) : (z)))

struct rgb_color {
    double r, g, b;    /* Channel intensities between 0.0 and 1.0 */
};

struct hsv_color {
    double hue;        /* Hue degree between 0.0 and 360.0 */
    double sat;        /* Saturation between 0.0 (gray) and 1.0 */
    double val;        /* Value between 0.0 (black) and 1.0 */
};


struct hsv_color rgb_to_hsv(struct rgb_color rgb) {
    struct hsv_color hsv;
    double rgb_min, rgb_max;
    rgb_min = MIN3(rgb.r, rgb.g, rgb.b);
    rgb_max = MAX3(rgb.r, rgb.g, rgb.b);
    hsv.val = rgb_max;
    if (hsv.val == 0) {
        hsv.hue = hsv.sat = 0;
        return hsv;
    }
    /* Normalize value to 1 */
    rgb.r /= hsv.val;
    rgb.g /= hsv.val;
    rgb.b /= hsv.val;
    rgb_min = MIN3(rgb.r, rgb.g, rgb.b);
    rgb_max = MAX3(rgb.r, rgb.g, rgb.b);
    hsv.sat = rgb_max - rgb_min;
    if (hsv.sat == 0) {
        hsv.hue = 0;
        return hsv;
    }
    /* Normalize saturation to 1 */
    rgb.r = (rgb.r - rgb_min)/(rgb_max - rgb_min);
    rgb.g = (rgb.g - rgb_min)/(rgb_max - rgb_min);
    rgb.b = (rgb.b - rgb_min)/(rgb_max - rgb_min);
    rgb_min = MIN3(rgb.r, rgb.g, rgb.b);
    rgb_max = MAX3(rgb.r, rgb.g, rgb.b);
    /* Compute hue */
    if (rgb_max == rgb.r) {
        hsv.hue = 0.0 + 60.0*(rgb.g - rgb.b);
        if (hsv.hue < 0.0) {
            hsv.hue += 360.0;
        }
    } else if (rgb_max == rgb.g) {
        hsv.hue = 120.0 + 60.0*(rgb.b - rgb.r);
    } else /* rgb_max == rgb.b */ {
        hsv.hue = 240.0 + 60.0*(rgb.r - rgb.g);
    }
    return hsv;
}



float hue,  s,  v;  /* hue (0.0 to 360.0, is circular, 0=360) s and v are from 0.0 - 1.0) */
float       nr, ng, nb;          /* rgb values of 0.0 - 1.0      */

void hsv_to_rgb() {
    
    float p1, p2, p3, i, f;
    float xh;
    
    if (hue == 360.0) {
        hue = 0.0;           /* (THIS LOOKS BACKWARDS)       */
    }
    
    xh = hue / 60.;                   /* convert hue to be in 0,6       */
    i = (float)floor((double)xh);    /* i = greatest integer <= h    */
    f = xh - i;                     /* f = fractional part of h     */
    p1 = v * (1 - s);
    p2 = v * (1 - (s * f));
    p3 = v * (1 - (s * (1 - f)));
    
    switch ((int) i)
    {
        case 0:
            nr = v;
            ng = p3;
            nb = p1;
            break;
        case 1:
            nr = p2;
            ng = v;
            nb = p1;
            break;
        case 2:
            nr = p1;
            ng = v;
            nb = p3;
            break;
        case 3:
            nr = p1;
            ng = p2;
            nb = v;
            break;
        case 4:
            nr = p3;
            ng = p1;
            nb = v;
            break;
        case 5:
            nr = v;
            ng = p1;
            nb = p2;
            break;
    }
    return;
}


int hsv2rgb( struct hsv_color *hsv, struct rgb_color *rgb ) {
    /*
     * Purpose:
     * Convert HSV values to RGB values
     * All values are in the range [0.0 .. 1.0]
     */
    float S, H, V, F, M, N, K;
    int   I;
    
    S = hsv->sat;  /* Saturation */
    H = hsv->hue;  /* Hue */
    V = hsv->val;  /* value or brightness */
    
    if ( S < 0.2 ) {
        /*
         * Achromatic case, set level of grey
         */
        rgb->r = 0;
        rgb->g = 0;
        rgb->b = 0;
    } else {
        /*
         * Determine levels of primary colours.
         */
        if (H >= 1.0) {
            H = 0.0;
        } else {
            H = H * 6;
        } /* end if */
        I = (int) H;   /* should be in the range 0..5 */
        F = H - I;     /* fractional part */
        
        M = V * (1 - S);
        N = V * (1 - S * F);
        K = V * (1 - S * (1 - F));
        
        if (I == 0) { rgb->r = V; rgb->g = K; rgb->b = M; }
        if (I == 1) { rgb->r = N; rgb->g = V; rgb->b = M; }
        if (I == 2) { rgb->r = M; rgb->g = V; rgb->b = K; }
        if (I == 3) { rgb->r = M; rgb->g = N; rgb->b = V; }
        if (I == 4) { rgb->r = K; rgb->g = M; rgb->b = V; }
        if (I == 5) { rgb->r = V; rgb->g = M; rgb->b = N; }
    } /* end if */
    
    return 0;
} /* end function hsv2rgb */


Colorf myhsvToRGB(float hue, float sat, float val ) {
    
    float x = 0.0f, y = 0.0f, z = 0.0f;
    
    if( hue == 1 ) hue = 0;
    else
		hue *= 6;
    
    int i = static_cast<int>( floorf( hue ) );
    float f = hue - i;
    float p = val * ( 1 - sat );
    float q = val* ( 1 - ( sat * f ) );
    float t = val* ( 1 - ( sat * ( 1 - f ) ) );
    
    switch( i ) {
		case 0: x = val; y = t; z = p; break;
		case 1: x = q; y = val; z = p; break;
		case 2: x = p; y = val; z = t; break;
		case 3: x = p; y = q; z = val; break;
		case 4: x = t; y = p; z = val; break;
		case 5: x = val; y = p; z = q; break;
    }
    
    return Colorf( x, y, z );
}

float zz = 0;

void FBOBasicApp::draw() {

	// clear the window to gray
	gl::clear( Color( 0.35f, 0.35f, 0.35f ) );

	// setup our camera to render the cube
	CameraPersp cam( getWindowWidth(), getWindowHeight(), 60.0f );
	cam.setPerspective( 60, getWindowAspectRatio(), 1, 1000 );
	cam.lookAt( Vec3f( 2.6f, 1.6f, -2.6f ), Vec3f::zero() );
	gl::setMatrices( cam );

	// set the viewport to match our window
	gl::setViewport( getWindowBounds() );

	// use the scene we rendered into the FBO as a texture
	glEnable( GL_TEXTURE_2D );
	mFbo.bindTexture();

	// draw a cube textured with the FBO
//	gl::color( Color::white() );
//	gl::drawCube( Vec3f::zero(), Vec3f( 2.2f, 2.2f, 2.2f ) );

	// show the FBO texture in the upper left corner
	gl::setMatricesWindow( getWindowSize() );
//	gl::draw( mFbo.getTexture(0), Rectf( 0, 0, getWindowWidth(), getWindowHeight() ) );
	


//    float zz = ci::app::getElapsedSeconds() * 2;
    zz+= speed / 10000.0f;

    int width = getWindowWidth();
    int height = getWindowHeight();


    glDisable( GL_TEXTURE_2D );

    for(int x = 0; x < width; x+= 10 ) {
        for(int y = 0; y < height ; y+= 10 ) {
            float xx = x * ((float)scale) / 10000.0f;
            float yy = y * ((float)scale) / 10000.0f;

            
 
            if(false) {
                float r = octave_noise_3d(octaves, persistence / 100.0f, 1.0f / zoom, xx, yy, zz);
                float g = octave_noise_3d(octaves, persistence / 100.0f, 1.0f / zoom, xx, yy, zz * 1.3 );
                float b = octave_noise_3d(octaves, persistence / 100.0f, 1.0f / zoom, xx, yy, zz * 1.5 );
                Color c = Color( r,g,b );
                gl::color( c );
            } else {
                hue = sin( octave_noise_3d(octaves, persistence / 100.0f, 1.0f / zoom, xx, yy, zz) );
                s = 0.3; //octave_noise_3d(octaves, persistence / 100.0f, 1.0f / zoom, xx, yy, zz * 1.3 );
                v = octave_noise_3d(octaves, persistence / 100.0f, 1.0f / zoom, xx, yy, zz * 1.5 );

               // hue = hue * 360;  hsv_to_rgb();
               // Color c = Color( nr,ng,nb );
                
                //Color c = Color( CM_HSV, hue,s,v );
     
//                hsv_color hsv; hsv.hue = hue; hsv.sat = s; hsv.val = v;
//                rgb_color rgb;
//                hsv2rgb(&hsv,&rgb);
//                Color c = Color( CM_HSV, rgb.r, rgb.g, rgb.b );
                
                
                Colorf c = myhsvToRGB(hue,s,v);


                gl::color( c );
            }

            gl::drawSolidRect(Rectf(x,y,x+10,y+10));
        }
    }
    
    params::InterfaceGl::draw();

}

/*
 
 
 //////
 Prng.seed = 282;
 P.setRng(Prng);
 P.noiseDetail(3,.5);
 fScl = .0422;
 //////
 

setInterval(function(){
    fDeltaT = millis()-fLastTime
    fLastTime = millis();
},1);


 */

#if defined( CINDER_COCOA_TOUCH )
CINDER_APP_COCOA_TOUCH( FBOBasicApp, RendererGl )
#else
CINDER_APP_BASIC( FBOBasicApp, RendererGl )
#endif



