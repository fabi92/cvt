#ifndef CVT_ORB_H
#define CVT_ORB_H

#include <cvt/vision/Feature2D.h>
#include <cvt/gfx/Image.h>
#include <cvt/vision/IntegralImage.h>
#include <vector>

namespace cvt {

	struct ORBFeature : public Feature2Df {        
		uint8_t desc[ 32 ]; // 256 bit vector
	};

	class ORB {
		public:
			ORB( const Image& img, size_t octaves = 3, float scalefactor = 0.5f, uint8_t cornerThreshold = 25 );

			size_t size() const;
			const ORBFeature& operator[]( size_t index ) const;

		private:
			void detect( const Image& img, float scale );
			void centroidAngle( ORBFeature& feature, IntegralImage& iimg  );
			void descriptor( ORBFeature& feature, IntegralImage& iimg );
        
            void detect9( const uint8_t* im, size_t stride, size_t width, size_t height, float scale );
            void makeOffsets( size_t stride );
            bool isDarkerCorner9( const uint8_t * p, const int barrier );
            bool isBrighterCorner9( const uint8_t * p, const int barrier );

			std::vector<ORBFeature> _features;
			static Vector2i _patterns[ 30 ][ 256 ][ 2 ];
        
            // for OFAST
            uint8_t     _threshold;
            size_t      _lastStride;
            int         _pixel[ 16 ];
	};

	inline size_t ORB::size() const
	{
		return _features.size();
	}

	inline const ORBFeature& ORB::operator[]( size_t index ) const
	{
		return _features[ index ];
	}
}

#endif
