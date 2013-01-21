#include <set>
#include <sstream>
#include <cvt/io/DC1394Camera.h>
#include <cvt/util/Time.h>
#include <cvt/util/Exception.h>

#include <cvt/gui/Application.h>
#include <cvt/gui/Window.h>
#include <cvt/gui/WidgetLayout.h>
#include <cvt/gui/Moveable.h>
#include <cvt/gui/Button.h>
#include <cvt/gui/ImageView.h>
#include <cvt/gui/BasicTimer.h>
#include <cvt/gui/TimeoutHandler.h>

using namespace cvt;

class MultiCamApp : public TimeoutHandler
{
	public:
		MultiCamApp( std::vector<Camera*>& cams ) :
			TimeoutHandler(),
			_cams( cams ),
			_frames( 0.0f ),
			_dump( false ),
			_dumpIter( 0 ),
			_window( "Multicam App" ),
			_quitButton( "Quit" ),
			_saveButton( "Save" )
	{
		_timer.reset();

		_window.setSize( 800, 600 );
		_window.setVisible( true );
		_window.setMinimumSize( 320, 240 );

		Delegate<void ()> dquit( &Application::exit );
		_quitButton.clicked.add( dquit );

		WidgetLayout wl;
		wl.setAnchoredRight( 10, 50 );
		wl.setAnchoredBottom( 10, 20 );
		_window.addWidget( &_quitButton, wl );
		
		Delegate<void ()> save( this, &MultiCamApp::setDump );
		_saveButton.clicked.add( save );
		wl.setAnchoredBottom( 40, 20 );
		_window.addWidget( &_saveButton, wl );

		_timerId = Application::registerTimer( 20, this );

		String movTitle;
		for( size_t i = 0; i < _cams.size(); i++ ){
			_views.push_back( new ImageView() );
			_moveables.push_back( new Moveable( _views.back() ) );
			movTitle.sprintf( "Cam %d: %s", i, _cams[ i ]->identifier().c_str() );
			_moveables.back()->setTitle( movTitle );
			_moveables.back()->setSize( 320, 240 );
			_window.addWidget( _moveables.back() );
		}
	}

		~MultiCamApp()
		{
			Application::unregisterTimer( _timerId );
			for( size_t i = 0; i < _cams.size(); i++ ){
				_cams[ i ]->stopCapture();
				delete _moveables[ i ];
				delete _views[ i ];
				delete _cams[ i ];
			}
		}

		void onTimeout()
		{
			for( size_t i = 0; i < _cams.size(); i++ ){
				_cams[ i ]->nextFrame( 10 );
			}

			for( size_t i = 0; i < _cams.size(); i++ ){
				_views[ i ]->setImage( _cams[ i ]->frame() );
			}
			
			_frames++;
			if( _timer.elapsedSeconds() > 2.0f ) {
				char buf[ 200 ];
				sprintf( buf,"Multicam App FPS: %.2f", _frames / _timer.elapsedSeconds() );
				_window.setTitle( buf );
				_frames = 0;
				_timer.reset();
			}

			if( _dump ){
				_dumpIter++;

				String name;
				Image img;

				for( size_t i = 0; i < _cams.size(); i++ ){
					name.sprintf( "camera_%s_image_%03d.png", _cams[ i ]->identifier().c_str(), _dumpIter );
					_cams[ i ]->frame().convert( img, IFormat::RGBA_UINT8 );
					img.save( name );
				}
				std::cout << "dumped" << std::endl;
				_dump = false;
			}
		}

		void setDump(){ _dump = true; }

	private:
		std::vector<Camera*>&		_cams;
		Time						_timer;
		float						_frames;
		bool						_dump;
		size_t						_dumpIter;

		Window						_window;
		Button						_quitButton;
		Button						_saveButton;
		std::vector<Moveable*>		_moveables;
		std::vector<ImageView*>		_views;

		uint32_t					_timerId;

};


#define PTG_PIO_DIRECTION ( 0x11F8 )
int main( int argc, char* argv[] )
{
	uint32_t offset = 0x1110; 
	int numCams = DC1394Camera::count();

	if( argc == 2 ){
		std::stringstream ss;
		ss << std::hex << argv[ 1 ];
		ss >> offset;
	}

	std::cout << "Overall number of Cameras: " << numCams << std::endl;
	if( numCams == 0 ){
		std::cout << "No DC1394 Camera found" << std::endl;
		return 1;
	}

	CameraInfo cinfo;
	DC1394Camera::cameraInfo( 0, cinfo );
	DC1394Camera cam( 0, cinfo.bestMatchingMode( IFormat::GRAY_UINT8, 640, 480, 30 ) );
	cam.startCapture();

	std::vector<Camera*> cameras;
	cameras.push_back( &cam );

	try {
		static const uint64_t BASE = 0xF00000;
		//uint64_t csr = 0xf011f0 + 0x;
		std::cout << "Requesting register: 0x" << std::hex << offset << std::endl;
		uint32_t val = cam.getRegister( BASE + offset );
		std::cout << "Value: 0x" << std::hex << val << std::endl;;
		//		MultiCamApp camTimeOut( cameras );
//		Application::run();

	} catch( cvt::Exception e ) {
		std::cout << e.what() << std::endl;
	}

	return 0;
}
