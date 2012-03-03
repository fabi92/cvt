/*
			CVT - Computer Vision Tools Library

	 Copyright (c) 2012, Philipp Heise, Sebastian Klose

	THIS CODE AND INFORMATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
	KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
	IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
	PARTICULAR PURPOSE.
 */
#ifndef CVT_CAMERACALIBRATION_H
#define	CVT_CAMERACALIBRATION_H

#include <cvt/math/Matrix.h>
#include <cvt/math/Vector.h>
#include <cvt/util/Flags.h>
#include <cvt/geom/Rect.h>
#include <cvt/math/Polynomial.h>
#include <cvt/io/xml/XMLElement.h>
#include <cvt/io/xml/XMLText.h>
#include <cvt/io/xml/XMLSerializable.h>

namespace cvt
{

    enum CamCalibFlagTypes {
        INTRINSICS = ( 1 << 0 ),
        EXTRINSICS = ( 1 << 1 ),
        DISTORTION = ( 1 << 2 )
	};

	CVT_ENUM_TO_FLAGS( CamCalibFlagTypes, CameraCalibrationFlags )


    class CameraCalibration : public XMLSerializable
    {
      public:
        CameraCalibration();
        CameraCalibration( const CameraCalibration & other );

        const Matrix3f & intrinsics()           const { return _intrinsics; }
        const Matrix4f & extrinsics()           const { return _extrinsics; }
        const Vector3f & radialDistortion()     const { return _radial; }
        const Vector2f & tangentialDistortion() const { return _tangential; }
        const Matrix4f & projectionMatrix()     const { return _projection; }
        const CameraCalibrationFlags & flags()  const { return _flags; }
		const Vector2f	 center()				const { return Vector2f( _intrinsics[ 0 ][ 2 ], _intrinsics[ 1 ][ 2 ] ); }
		const Vector2f	 focalLength()			const { return Vector2f( _intrinsics[ 0 ][ 0 ], _intrinsics[ 1 ][ 1 ] ); }

        /**
         * set the extrinsics of the camera
         * @param extr Transformation from Camera to World!
         */
        void setExtrinsics( const Matrix4f & extr );

        void setIntrinsics( const Matrix3f & intr );
        void setIntrinsics( float fx, float fy, float cx, float cy, float alpha = 0.0f );

        void setDistortion( const Vector3f & radial, const Vector2f & tangential );
		Vector2f undistortPoint( const Vector2f& in ) const;
		Vector2f inverseUndistortPoint( const Vector2f& in ) const;
		void calcUndistortRects( Rectf& min, Rectf& max, const Rectf& input ) const;

        bool hasExtrinsics() const { return ( _flags & EXTRINSICS ); }
        bool hasIntrinsics() const { return ( _flags & INTRINSICS ); }
        bool hasDistortion() const { return ( _flags & DISTORTION ); }

        // de-/serialization interface
        void	 deserialize( XMLNode* node );
        XMLNode* serialize() const;

      private:
        Matrix3f                _intrinsics;
        Matrix4f                _extrinsics;
        Matrix4f                _projection;

        // Distortion parameters
        Vector3f                _radial;
        Vector2f                _tangential;

        CameraCalibrationFlags  _flags;

        void updateProjectionMatrix();

    };

    inline CameraCalibration::CameraCalibration() :
		XMLSerializable()
    {
        _intrinsics.setIdentity();
        _extrinsics.setIdentity();
        _projection.setIdentity();
        _radial.setZero();
        _tangential.setZero();
    }

    inline CameraCalibration::CameraCalibration( const CameraCalibration & other ) :
		XMLSerializable(),
        _intrinsics( other._intrinsics ),
        _extrinsics( other._extrinsics ),
        _projection( other._projection ),
        _radial( other._radial ),
        _tangential( other._tangential ),
        _flags( other._flags )
    {
    }

    inline XMLNode* CameraCalibration::serialize() const
    {
        XMLNode * root;

        root = new XMLElement( "CameraCalibration" );

        if( hasExtrinsics() ){
            XMLElement * element = new XMLElement( "Extrinsics" );
            element->addChild( new XMLText( _extrinsics.toString() ) );
            root->addChild( element );
        }

        if( hasIntrinsics() ){
            XMLElement * element = new XMLElement( "Intrinsics" );
            element->addChild( new XMLText( _intrinsics.toString() ) );
            root->addChild( element );
        }

        if( hasDistortion() ){
            XMLElement * element = new XMLElement( "Distortion" );
            XMLElement * rDist = new XMLElement( "Radial" );
            rDist->addChild( new XMLText( _radial.toString() ) );
            element->addChild( rDist );

            XMLElement * tDist = new XMLElement( "Tangential" );
            tDist->addChild( new XMLText( _tangential.toString() ) );
            element->addChild( tDist );

            root->addChild( element );
        }

        return root;
    }

    inline void CameraCalibration::deserialize( XMLNode* node )
    {
        if( node->name() != "CameraCalibration" )
            throw CVTException( "this is not a camera calibration node" );
        XMLNode * n;
        n = node->childByName( "Extrinsics" );
        if( n != NULL ){
            Matrix4f mat = Matrix4f::fromString( n->child( 0 )->value() );
            setExtrinsics( mat );
        }

        n = node->childByName( "Intrinsics" );
        if( n != NULL ){
            Matrix3f intr = Matrix3f::fromString( n->child( 0 )->value() );
            setIntrinsics( intr );
        }

        n = node->childByName( "Distortion" );
        if( n != NULL ){
            XMLNode * child;
            Vector3f radial;
            Vector2f tangential;

			/* to make the compiler happy */
			radial.setZero();
			tangential.setZero();

            for( size_t i = 0; i < n->childSize(); i++ ){
                child = n->child( i );

                if( child->name() == "Radial"){
                    radial = Vector3f::fromString( child->child( 0 )->value() );
                } else if( child->name() == "Tangential" ){
                    tangential = Vector2f::fromString( child->child( 0 )->value() );
                } else {
                    String message( "Unkown child type: " );
                    message += child->name();
                    throw CVTException( message.c_str() );
                }
            }

            setDistortion( radial, tangential );
        }
    }

    inline void CameraCalibration::setExtrinsics( const Matrix4f & extr )
    {
        _flags |= EXTRINSICS;
        _extrinsics = extr;
        updateProjectionMatrix();
    }

    inline void CameraCalibration::setIntrinsics( const Matrix3f & intr )
    {
        _flags |= INTRINSICS;
        _intrinsics = intr;
        updateProjectionMatrix();
    }

    inline void CameraCalibration::setIntrinsics( float fx, float fy, float cx, float cy, float alpha )
    {
        _flags |= INTRINSICS;
        _intrinsics[ 0 ][ 0 ] = fx;
        _intrinsics[ 0 ][ 1 ] = alpha;
        _intrinsics[ 0 ][ 2 ] = cx;
        _intrinsics[ 1 ][ 0 ] = 0.0f;
        _intrinsics[ 1 ][ 1 ] = fy;
        _intrinsics[ 1 ][ 2 ] = cy;
        _intrinsics[ 2 ][ 0 ] = 0.0f;
        _intrinsics[ 2 ][ 1 ] = 0.0f;
        _intrinsics[ 2 ][ 2 ] = 1.0f;
        updateProjectionMatrix();
    }


    inline void CameraCalibration::setDistortion( const Vector3f & radial, const Vector2f & tangential )
    {
        _flags |= DISTORTION;
        _radial = radial;
        _tangential = tangential;
    }

	inline Vector2f CameraCalibration::undistortPoint( const Vector2f& in ) const
	{
		Vector2f out;
		Vector2f p;
		Vector2f c = Vector2f( _intrinsics[ 0 ][ 2 ], _intrinsics[ 1 ][ 2 ] );
		Vector2f f = Vector2f( _intrinsics[ 0 ][ 0 ], _intrinsics[ 1 ][ 1 ] );
		Vector2f tangential =  _tangential;
		Vector3f radial = _radial;
		p = ( in - c );
		p /= f;
		float r2 = p.lengthSqr();
		float r4 = Math::sqr( r2 );
		float r6 = r2 * r4;
		float poly = ( 1.0f + radial[ 0 ] * r2 + radial[ 1 ] * r4 + radial[ 2 ] * r6 );
		float xy2 = 2.0f * p.x * p.y;
		out.x = f.x * ( p.x * poly + xy2 * tangential[ 0 ] + tangential[ 1 ] * ( r2 + 2.0f * p.x * p.x ) ) + c.x;
		out.y = f.y * ( p.y * poly + xy2 * tangential[ 1 ] + tangential[ 0 ] * ( r2 + 2.0f * p.y * p.y ) ) + c.y;
		return out;
	}

	inline Vector2f CameraCalibration::inverseUndistortPoint( const Vector2f& in ) const
	{
		Vector2f c = Vector2f( _intrinsics[ 0 ][ 2 ], _intrinsics[ 1 ][ 2 ] );
		Vector2f f = Vector2f( _intrinsics[ 0 ][ 0 ], _intrinsics[ 1 ][ 1 ] );
		Vector2f p = ( in - c );
		p /= f;
		Vector2f lambda( 10e5f, 10e5f);

		float r2 = p.lengthSqr();
		float r4 = Math::sqr( r2 );
		float r6 = r2 * r4;

		Polynomialf radial( r6 * _radial[ 2 ], 0.0f, r4 * _radial[ 1 ], 0.0f, r2 * _radial[ 0 ], 0.0f, 1.0f, -1.0f  );
		std::vector<Complexf> roots;

		Polynomialf tangentialx( ( _tangential[ 0 ] * 2.0f * p.y + _tangential[ 1 ] * ( ( p.y * p.y ) / p.x + 3.0f * p.x ) ), 0.0f, 0.0f );
		Polynomialf all = radial + tangentialx;
		all.roots( roots );
		for( size_t i = 0; i < roots.size(); i++ ) {
			if( roots[ i ].im == 0.0f ) {
				if( Math::abs( roots[ i ].re - 1 ) < Math::abs( lambda.x - 1 ) )
					lambda.x = roots[ i ].re;
			}
		}

		Polynomialf tangentialy( ( _tangential[ 1 ] * 2.0f * p.x + _tangential[ 0 ] * ( ( p.x * p.x ) / p.y + 3.0f * p.y ) ), 0.0f, 0.0f );
		all = radial + tangentialy;
		all.roots( roots );
		for( size_t i = 0; i < roots.size(); i++ ) {
			if( roots[ i ].im == 0.0f ) {
				if( Math::abs( roots[ i ].re - 1 ) < Math::abs( lambda.y - 1 ) )
					lambda.y = roots[ i ].re;
			}
		}

#if 0 
		p = in - c;
		p.x *= lambda.x;
		p.y *= lambda.y;
		p += c;
		p = undistortPoint( p );

		std::cout << in << std::endl;
		std::cout << p << std::endl;
		std::cout << p - in << std::endl;
		std::cout << lambda << std::endl;
		std::cout << std::endl;
#endif

		p = in - c;
		p.x *= lambda.x;
		p.y *= lambda.y;
		return p + c;
	}

	struct FixedXinverseUndistort {
		FixedXinverseUndistort( float x, const CameraCalibration* cam ) : _pt( x, 0 ), _cam( cam ) {}

		float operator()( float y )
		{
			_pt.y = y;
			Vector2f ret = _cam->inverseUndistortPoint( _pt );
			return ret.x;
		}

		Vector2f _pt;
		const CameraCalibration* _cam;
	};

	struct FixedYinverseUndistort {
		FixedYinverseUndistort( float y, const CameraCalibration* cam ) : _pt( 0, y ), _cam( cam ) {}

		float operator()( float x )
		{
			_pt.x = x;
			Vector2f ret = _cam->inverseUndistortPoint( _pt );
			return ret.y;
		}

		Vector2f _pt;
		const CameraCalibration* _cam;
	};

	inline void CameraCalibration::calcUndistortRects( Rectf& min, Rectf& max, const Rectf& input ) const
	{
		Vector2f pt;
		float x1min, x2min, y1min, y2min;

		max = input;

		pt = inverseUndistortPoint( Vector2f( input.x, input.y ) );
		max.join( pt );

		pt = inverseUndistortPoint( Vector2f( input.x + input.width, input.y ) );
		max.join( pt );

		pt = inverseUndistortPoint( Vector2f( input.x + input.width, input.y + input.height ) );
		max.join( pt );

		pt = inverseUndistortPoint( Vector2f( input.x, input.y + input.height ) );
		max.join( pt );

		x1min = max.x;
		x2min = max.x + max.width;
		y1min = max.y;
		y2min = max.y + max.height;

		FixedXinverseUndistort left( input.x, this );
		FixedXinverseUndistort right( input.x + input.width, this );
		FixedYinverseUndistort top( input.y, this );
		FixedYinverseUndistort bottom( input.y + input.height, this );
		float tmp;

		if( _radial[ 0 ] < 0 ) { // barrel distortion
			tmp = Math::lineSearchMaxGolden( input.y, input.y + input.height - 1, left );
			pt = inverseUndistortPoint( Vector2f( input.x, tmp ) );
			x1min = Math::max( x1min, pt.x );
			tmp = Math::lineSearchMinGolden( input.y, input.y + input.height - 1, right );
			pt = inverseUndistortPoint( Vector2f(input.x + input.width, tmp ) );
			x2min = Math::min( x2min, pt.x );
			tmp = Math::lineSearchMaxGolden( input.x, input.x + input.width - 1, top );
			pt = inverseUndistortPoint( Vector2f( tmp, input.y ) );
			y1min = Math::max( y1min, pt.y );
			tmp = Math::lineSearchMinGolden( input.x, input.x + input.width - 1, bottom );
			pt = inverseUndistortPoint( Vector2f( tmp, input.y + input.height ) );
			y2min = Math::min( y2min, pt.y );
		} else { // pincushion
			tmp = Math::lineSearchMinGolden( input.y, input.y + input.height - 1, left );
			pt = inverseUndistortPoint( Vector2f( input.x, tmp ) );
			max.join( pt );
			tmp = Math::lineSearchMaxGolden( input.y, input.y + input.height - 1, right );
			pt = inverseUndistortPoint( Vector2f(input.x + input.width, tmp ) );
			max.join( pt );
			tmp = Math::lineSearchMinGolden( input.x, input.x + input.width - 1, top );
			pt = inverseUndistortPoint( Vector2f( tmp, input.y ) );
			max.join( pt );
			tmp = Math::lineSearchMaxGolden( input.x, input.x + input.width - 1, bottom );
			pt = inverseUndistortPoint( Vector2f( tmp, input.y + input.height ) );
			max.join( pt );
		}

		min.x = x1min;
		min.y = y1min;
		min.width = x2min - x1min;
		min.height = y2min - y1min;
	}

    inline void CameraCalibration::updateProjectionMatrix()
    {
        // if intrinsics are set, this will copy it to projection,
        // if not, it will be I_4x4!
        for( size_t i = 0; i < 3; i++ )
            for( size_t k = 0; k < 3; k++ )
                _projection[ i ][ k ] = _intrinsics[ i ][ k ];
        _projection[ 0 ][ 3 ] = _projection[ 1 ][ 3 ] = _projection[ 2 ][ 3 ] = 0.0f;
        _projection[ 3 ][ 0 ] = _projection[ 3 ][ 1 ] = _projection[ 3 ][ 2 ] = 0.0f;
        _projection[ 3 ][ 3 ] = 1.0f;


        if( hasExtrinsics() )
            _projection *= _extrinsics;
    }
}

#endif

