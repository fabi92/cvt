/*
			CVT - Computer Vision Tools Library

 	 Copyright (c) 2012, Philipp Heise, Sebastian Klose

 	THIS CODE AND INFORMATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
 	KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 	IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
 	PARTICULAR PURPOSE.
 */
#ifndef SIMDSSE2_H
#define SIMDSSE2_H

#include <cvt/util/SIMDSSE.h>

namespace cvt {

	class SIMDSSE2 : public SIMDSSE {
		friend class SIMD;

		protected:
			SIMDSSE2() {};

        public:
			using SIMDSSE::Mul;
			virtual void MulValue1fx( Fixed* dst, const Fixed* src, Fixed value, size_t n ) const;			
			virtual void MulAddValue1fx( Fixed* dst, const Fixed* src, Fixed value, size_t n ) const;
			using SIMDSSE::SAD;
			virtual size_t SAD( uint8_t const* src1, uint8_t const* src2, const size_t n ) const;

			/* Add vertical */
			virtual void AddVert_f( float* dst, const float**bufs, size_t numbufs, size_t width ) const;
			virtual void AddVert_f_to_u8( uint8_t* dst, const float**bufs, size_t numbufs, size_t width ) const;
			virtual void AddVert_f_to_s16( int16_t* dst, const float**bufs, size_t numbufs, size_t width ) const;

			virtual void AddVert_fx_to_u8( uint8_t* dst, const Fixed** bufs, size_t numbufs, size_t width ) const;

			/* Convolution */
            virtual void ConvolveHorizontal1f( float* dst, const float* src, const size_t width, float const* weights, const size_t wn, IBorderType btype ) const;
            virtual void ConvolveHorizontal2f( float* dst, const float* src, const size_t width, float const* weights, const size_t wn, IBorderType btype ) const;
            virtual void ConvolveHorizontal4f( float* dst, const float* src, const size_t width, float const* weights, const size_t wn, IBorderType btype ) const;

            virtual void ConvolveHorizontalSym1f( float* dst, const float* src, const size_t width, float const* weights, const size_t wn, IBorderType btype ) const;
            virtual void ConvolveHorizontalSym2f( float* dst, const float* src, const size_t width, float const* weights, const size_t wn, IBorderType btype ) const;
            virtual void ConvolveHorizontalSym4f( float* dst, const float* src, const size_t width, float const* weights, const size_t wn, IBorderType btype ) const;

            virtual void ConvolveHorizontal1u8_to_fx( Fixed* dst, const uint8_t* src, const size_t width, const Fixed* weights, const size_t wn, IBorderType btype ) const;
            virtual void ConvolveHorizontal2u8_to_fx( Fixed* dst, const uint8_t* src, const size_t width, const Fixed* weights, const size_t wn, IBorderType btype ) const;
            virtual void ConvolveHorizontal4u8_to_fx( Fixed* dst, const uint8_t* src, const size_t width, const Fixed* weights, const size_t wn, IBorderType btype ) const;

            virtual void ConvolveHorizontalSym1u8_to_fx( Fixed* dst, const uint8_t* src, const size_t width, const Fixed* weights, const size_t wn, IBorderType btype ) const;
            virtual void ConvolveHorizontalSym2u8_to_fx( Fixed* dst, const uint8_t* src, const size_t width, const Fixed* weights, const size_t wn, IBorderType btype ) const;
            virtual void ConvolveHorizontalSym4u8_to_fx( Fixed* dst, const uint8_t* src, const size_t width, const Fixed* weights, const size_t wn, IBorderType btype ) const;

            virtual void ConvolveHorizontal1u8_to_f( float* dst, const uint8_t* src, const size_t width, const float* weights, const size_t wn, IBorderType btype ) const;
            virtual void ConvolveHorizontal2u8_to_f( float* dst, const uint8_t* src, const size_t width, const float* weights, const size_t wn, IBorderType btype ) const;
            virtual void ConvolveHorizontal4u8_to_f( float* dst, const uint8_t* src, const size_t width, const float* weights, const size_t wn, IBorderType btype ) const;

            virtual void ConvolveHorizontalSym1u8_to_f( float* dst, const uint8_t* src, const size_t width, const float* weights, const size_t wn, IBorderType btype ) const;
            virtual void ConvolveHorizontalSym2u8_to_f( float* dst, const uint8_t* src, const size_t width, const float* weights, const size_t wn, IBorderType btype ) const;
            virtual void ConvolveHorizontalSym4u8_to_f( float* dst, const uint8_t* src, const size_t width, const float* weights, const size_t wn, IBorderType btype ) const;

			virtual void ConvolveClampVert_fx_to_u8( uint8_t* dst, const Fixed** bufs, const Fixed* weights, size_t numw, size_t width ) const;
			virtual void ConvolveClampVert_f( float* dst, const float** bufs, const float* weights, size_t numw, size_t width ) const;
			virtual void ConvolveClampVert_f_to_u8( uint8_t* dst, const float** bufs, const float* weights, size_t numw, size_t width ) const;

			virtual void ConvolveClampVertSym_fx_to_u8( uint8_t* dst, const Fixed** bufs, const Fixed* weights, size_t numw, size_t width ) const;
			virtual void ConvolveClampVertSym_f( float* dst, const float** bufs, const float* weights, size_t numw, size_t width ) const;
			virtual void ConvolveClampVertSym_f_to_u8( uint8_t* dst, const float** bufs, const float* weights, size_t numw, size_t width ) const;

			virtual void Conv_fx_to_u8( uint8_t* dst, const Fixed* src, const size_t n ) const;
			virtual void Conv_u16_to_f( float* dst, const uint16_t* src, const size_t n ) const;

			virtual void Conv_YUYVu8_to_RGBAu8( uint8_t* dst, const uint8_t* src, const size_t n ) const;
			virtual void Conv_YUYVu8_to_BGRAu8( uint8_t* dst, const uint8_t* src, const size_t n ) const;
			virtual void Conv_UYVYu8_to_RGBAu8( uint8_t* dst, const uint8_t* src, const size_t n ) const;
			virtual void Conv_UYVYu8_to_BGRAu8( uint8_t* dst, const uint8_t* src, const size_t n ) const;
			virtual void Conv_YUYVu8_to_GRAYu8( uint8_t* dst, const uint8_t* src, const size_t n ) const;
			virtual void Conv_UYVYu8_to_GRAYu8( uint8_t* dst, const uint8_t* src, const size_t n ) const;
			virtual void Conv_YUYVu8_to_GRAYALPHAu8( uint8_t* dst, const uint8_t* src, const size_t n ) const;
			virtual void Conv_UYVYu8_to_GRAYALPHAu8( uint8_t* dst, const uint8_t* src, const size_t n ) const;

			virtual void BoxFilterHorizontal_1u8_to_f( float* dst, const uint8_t* src, size_t radius, size_t width ) const;
			virtual void BoxFilterHorizontal_1f( float* dst, const float* src, size_t radius, size_t width ) const;

			virtual void BoxFilterVert_f_to_u8( uint8_t* dst, float* accum, const float* add, const float* sub, size_t radius, size_t width ) const;
			virtual void BoxFilterVert_f( float* dst, float* accum, const float* add, const float* sub, size_t radius, size_t width ) const;

			virtual void pyrdownHalfHorizontal_1u8_to_1u16( uint16_t* dst, const uint8_t* src, size_t n ) const;
			virtual void pyrdownHalfVertical_1u16_to_1u8( uint8_t* dst, uint16_t* rows[ 5 ], size_t n ) const;

			virtual void harrisScore1f( float* dst, const float* boxdx2, const float* boxdy2, const float* boxdxdy, float kappa, size_t width ) const;

			virtual float harrisResponse1u8( const uint8_t* _src, size_t srcStride, size_t w, size_t h, const float k ) const;
			virtual float harrisResponse1u8( float & xx, float & xy, float & yy, const uint8_t* _src, size_t srcStride, size_t w, size_t h, const float k ) const;
			virtual float harrisResponseCircular1u8( float & xx, float & xy, float & yy, const uint8_t* _src, size_t srcStride, const float k ) const;

            virtual void prefixSum1_u8_to_f( float * dst, size_t dstStride, const uint8_t * src, size_t srcStride, size_t width, size_t height ) const;
            virtual void prefixSumSqr1_u8_to_f( float * dst, size_t dStride, const uint8_t * src, size_t srcStride, size_t width, size_t height ) const;

			virtual void boxFilterPrefixSum1_f_to_u8( uint8_t* dst, size_t dstride, const float* src, size_t srcstride, size_t width, size_t height, size_t boxwidth, size_t boxheight ) const;

			virtual void debayer_EVEN_RGGBu8_BGRAu8( uint32_t* dst, const uint32_t* src1, const uint32_t* src2, const uint32_t* src3, size_t n ) const;
			virtual void debayer_ODD_RGGBu8_BGRAu8( uint32_t* dst, const uint32_t* src1, const uint32_t* src2, const uint32_t* src3, size_t n ) const;
			virtual void debayer_EVEN_RGGBu8_RGBAu8( uint32_t* dst, const uint32_t* src1, const uint32_t* src2, const uint32_t* src3, size_t n ) const;
			virtual void debayer_ODD_RGGBu8_RGBAu8( uint32_t* dst, const uint32_t* src1, const uint32_t* src2, const uint32_t* src3, size_t n ) const;
			virtual void debayer_EVEN_RGGBu8_GRAYu8( uint32_t* dst, const uint32_t* src1, const uint32_t* src2, const uint32_t* src3, size_t n ) const;
			virtual void debayer_ODD_RGGBu8_GRAYu8( uint32_t* dst, const uint32_t* src1, const uint32_t* src2, const uint32_t* src3, size_t n ) const;

			virtual void adaptiveThreshold1_f_to_u8( uint8_t* dst, const float* src, const float* srcmean, size_t n, float t ) const;
			virtual void adaptiveThreshold1_f_to_f( float* dst, const float* src, const float* srcmean, size_t n, float t ) const;

			virtual void sumPoints( Vector2f& dst, const Vector2f* src, size_t n ) const;
			virtual void sumPoints( Vector3f& dst, const Vector3f* src, size_t n ) const;

			using SIMDSSE::transformPoints;
			virtual void transformPoints( Vector2f* dst, const Matrix2f& mat, const Vector2f* src, size_t n ) const;
			virtual void transformPoints( Vector2f* dst, const Matrix3f& mat, const Vector2f* src, size_t n ) const;
			virtual void transformPoints( Vector3f* dst, const Matrix4f& mat, const Vector3f* src, size_t n ) const;

			using SIMDSSE::transformPointsHomogenize;
			virtual void transformPointsHomogenize( Vector3f* dst, const Matrix4f& mat, const Vector3f* src, size_t n ) const;

            using SIMDSSE::projectPoints;
            virtual void projectPoints( Vector2f* dst, const Matrix4f& mat, const Vector3f* src, size_t n ) const;

		public:
			virtual std::string name() const;
			virtual SIMDType type() const;
	};

	inline std::string SIMDSSE2::name() const
	{
		return "SIMD-SSE2";
	}

	inline SIMDType SIMDSSE2::type() const
	{
		return SIMD_SSE2;
	}
}

#endif
