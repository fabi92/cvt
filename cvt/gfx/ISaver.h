#ifndef CVT_ISAVER_H
#define CVT_ISAVER_H

#include <cvt/util/Plugin.h>
#include <cvt/util/String.h>
#include <cvt/gfx/Image.h>

namespace cvt {
	class ISaver : public Plugin
	{
		public:
			ISaver() : Plugin( PLUGIN_ISAVER ) {};

			virtual void save( const String& file, const Image& src ) = 0;
			virtual const String& extension( size_t n ) const = 0;
			virtual size_t sizeExtensions() const = 0;
			virtual const String& name() const = 0;
			bool isExtensionSupported( const String& suffix ) const;
	};

	inline bool ISaver::isExtensionSupported( const String& suffix ) const
	{
		for( size_t i = 0, end = sizeExtensions(); i < end; i++ ) {
			if( suffix == extension( i ) )
				return true;
		}
	}
}

#endif