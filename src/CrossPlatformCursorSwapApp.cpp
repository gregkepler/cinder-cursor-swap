#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#if defined( CINDER_MAC )
#include "Cocoa/Cocoa.h"
#endif

using namespace ci;
using namespace ci::app;
using namespace std;

struct Box {
	Box( Rectf bounds, Color color )
		: mBounds( bounds ), mColor( color )
	{};
	
	Rectf mBounds;
	Color mColor;
	
};

class CrossPlatformCursorSwapApp : public AppNative {
  public:
	enum CURSOR {
		DEFAULT,
		HAND
	};
	
	void setup();
	void mouseMove( MouseEvent event );
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
	void changeCursor( CURSOR type );
	
	bool mHit;
	Box *mBox;
	
#if defined( CINDER_MSW )
	HCURSOR mArrowCursor;
	HCURSOR mHandCursor;
#endif
};

void CrossPlatformCursorSwapApp::setup()
{
	mBox = new Box( Rectf( -100, -100, 100, 100 ), Color( 1, 0, 0 ) );
	mHit = false;

#if defined( CINDER_MSW )
	mArrowCursor = ::LoadCursor( NULL, IDC_ARROW );
	mHandCursor = ::LoadCursor( NULL, IDC_HAND );
#endif
}

void CrossPlatformCursorSwapApp::mouseMove( MouseEvent event )
{
	if( event.getPos().x > getWindowCenter().x + mBox->mBounds.x1 &&
		event.getPos().x < getWindowCenter().x + mBox->mBounds.x2 &&
	    event.getPos().y > getWindowCenter().y + mBox->mBounds.y1 &&
		event.getPos().y < getWindowCenter().y + mBox->mBounds.y2 )
	{
		if( !mHit ) {
			mHit = true;
			console() << "HIT" << endl;
			changeCursor( HAND );
		}
	} else {
		if( mHit ) {
			mHit = false;
			console() << "NO HIT" << endl;
			changeCursor( DEFAULT );
		}
	}
}

void CrossPlatformCursorSwapApp::changeCursor( CURSOR type )
{
#if defined( CINDER_MAC )
	switch ( type ) {
		case DEFAULT:	[[NSCursor arrowCursor] set];			break;
		case HAND:		[[NSCursor pointingHandCursor] set];	break;
		default:		[[NSCursor arrowCursor] set];			break;
	}
#elif defined( CINDER_MSW )
	switch ( type ) {
		case DEFAULT:	::SetCursor( mArrowCursor );	break;
		case HAND:		::SetCursor( mHandCursor );		break;
		default:		::SetCursor( mArrowCursor );	break;
	}
#endif
}

void CrossPlatformCursorSwapApp::mouseDown( MouseEvent event )
{
}

void CrossPlatformCursorSwapApp::update()
{
}

void CrossPlatformCursorSwapApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) );
	
	gl::pushMatrices();
	gl::translate( getWindowCenter() );
	gl::color( mBox->mColor );
	gl::drawSolidRect( mBox->mBounds );
	gl::popMatrices();
}

CINDER_APP_NATIVE( CrossPlatformCursorSwapApp, RendererGl )
