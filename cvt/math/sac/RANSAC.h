/*
 * File:   RANSAC.h
 * Author: sebi, heise
 *
 * Created on July 19, 2011, 1:34 PM
 */

#ifndef RANSAC_H
#define	RANSAC_H

#include <cvt/math/Math.h>
#include <cvt/math/sac/SampleConsensusModel.h>

namespace cvt
{
    template <class Model>
    class RANSAC
    {
      public:
        typedef typename Model::ResultType   ResultType;
        typedef typename Model::DistanceType DistanceType;

        RANSAC( SampleConsensusModel<Model> & model,
                float maxDistance,
                float outlierProb = 0.05f ) :
            _model( model ), _maxDistance( maxDistance ), _outlierProb( outlierProb )
        {
        }

        ResultType estimate();

      private:
        SampleConsensusModel<Model>&  _model;

        float                         _maxDistance;
        float                         _outlierProb;

        void randomSamples( std::vector<size_t> & indices );
    };

    template<class Model>
    inline typename RANSAC<Model>::ResultType RANSAC<Model>::estimate()
    {
        size_t n = SIZE_T_MAX;
        size_t samples = 0;

        ResultType result;

        std::vector<size_t> indices;
        std::vector<size_t> inliers;

        std::vector<size_t> bestIndices;
        size_t numBest = 0;

        while( n > samples ){
            randomSamples( indices );
            result = _model.estimate( indices );

            _model.inliers( inliers, result, _maxDistance );

            if( inliers.size() > numBest ){
                numBest = inliers.size();
                bestIndices = indices;
            }

            float epsilon = 1.0f - ( float )inliers.size() / ( float )_model.size();
            n = Math::log( _outlierProb ) / Math::log( 1.0f - Math::pow( 1.0f - epsilon, (float)_model.minSampleSize() ) );
            samples++;
        }

        result = _model.estimate( bestIndices );
        _model.inliers( inliers, result, _maxDistance );

        return _model.refine( inliers );
    }

    template<class Model>
    inline void RANSAC<Model>::randomSamples( std::vector<size_t> & indices )
	{
        indices.clear();

		size_t idx;
		while( indices.size() < _model.minSampleSize() ){
			idx = Math::rand( 0, _model.size() - 1 );


            bool insert = true;
            for( size_t i = 0; i < indices.size(); i++ ){
                if( idx == indices[ i ] ){
                    insert = false;
                    break;
                }
            }

            if( insert )
                indices.push_back( idx );
		}
	}
}

#endif	/* RANSAC_H */

