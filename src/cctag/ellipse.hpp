#ifndef _ROM_VISION_CCTAG_ELLIPSE_HPP_
#define _ROM_VISION_CCTAG_ELLIPSE_HPP_

#include "types.hpp"
#include "EdgePoint.hpp"

#include "geometry/Ellipse.hpp"
#include "geometry/distance.hpp"

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/matrix_expression.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/vector_expression.hpp>

#include <cstddef>
#include <vector>

namespace rom
{
namespace vision
{
namespace marker
{

class CCTag;

namespace cctag
{

inline bool isInEllipse(
        const rom::numerical::geometry::Ellipse& ellipse,
        const Point2dN<double> & p)
{
  using namespace boost::numeric::ublas;
  // x'Q x > 0
  return ( ( inner_prod( p, prec_prod( ellipse.matrix(), p ) ) ) *
           inner_prod( ellipse.center(), prec_prod( ellipse.matrix(), ellipse.center() ))
           > 0 );
}

/**
 * Check if ellipses overlap.
 * @param ellipse1
 * @param ellipse2
 * @return
 */
inline bool isOverlappingEllipses(
        const rom::numerical::geometry::Ellipse& ellipse1,
        const rom::numerical::geometry::Ellipse& ellipse2 )
{
  return ( isInEllipse( ellipse1, ellipse2.center() ) || isInEllipse( ellipse2, ellipse1.center() ) );
}

bool initMarkerCenter(
        rom::Point2dN<double> & markerCenter,
        const std::vector< std::vector< Point2dN<double> > > & markerPoints,
        int realPixelPerimeter);

bool addCandidateFlowtoCCTag(
        const std::vector< EdgePoint* > & filteredChildrens,
        const std::vector< EdgePoint* > & outerEllipsePoints,
        const rom::numerical::geometry::Ellipse& outerEllipse,
        std::vector< std::vector< Point2dN<double> > >& cctagPoints,
        std::size_t numCircles);

bool ellipseGrowingInit(
        std::vector<EdgePoint> & points,
        const std::vector<EdgePoint*>& filteredChildrens,
        rom::numerical::geometry::Ellipse& ellipse);

/** @brief Is a point in an elliptical hull ?
 * @param ellipse ellipse which defines the hull
 * @param delta larger of the hull
 * @param abscissa of the tested point
 * @param ordinate of the tested point
 * @return true if located between qIn and qOut false otherwise
 */
inline bool isInHull( const rom::numerical::geometry::Ellipse& qIn, const rom::numerical::geometry::Ellipse& qOut, const EdgePoint* p )
{
  using namespace boost::numeric;
  return ( ublas::inner_prod( *p, ublas::prec_prod( qIn.matrix(), *p ) ) * ublas::inner_prod( *p, ublas::prec_prod( qOut.matrix(), *p ) ) < 0 ) ;
}

inline bool isInHull( const rom::numerical::geometry::Ellipse& qIn, const rom::numerical::geometry::Ellipse& qOut, const Point2dN<double> p )
{
  using namespace boost::numeric;
  return ( ublas::inner_prod( p, ublas::prec_prod( qIn.matrix(), p ) ) * ublas::inner_prod( p, ublas::prec_prod( qOut.matrix(), p ) ) < 0 ) ;
}

// todo@Lilian to be templated
inline bool isOnTheSameSide(const Point2dN<double> & p1, const Point2dN<double> &  p2, boost::numeric::ublas::bounded_vector<double, 3> line)
{
  using namespace boost::numeric;
  return ( ublas::inner_prod( p1, line ) * ublas::inner_prod( p2, line ) > 0 ) ;
}

/** @brief Search recursively connected points from a point and add it in pts if it is in the ellipse hull
 * @param list of points to complete
 * @param img map of edge points
 * @param map of already processed edge points
 * @param abscissa of the point
 * @param ordinate of the point
 */
void connectedPoint( std::vector<EdgePoint*>& pts, const int runId, const EdgePointsImage& img, rom::numerical::geometry::Ellipse& qIn, rom::numerical::geometry::Ellipse& qOut, int x, int y );

/** @brief Compute the hull from ellipse
 * @param ellipse ellipse from which the hull is computed
 * @param delta larger of the hull
 */
void computeHull( const rom::numerical::geometry::Ellipse& ellipse, double delta, rom::numerical::geometry::Ellipse& qIn, rom::numerical::geometry::Ellipse& qOut );

/** @brief Ellipse hull
 * @param[in,out] pts initial points to compute all the points which are in the hull formed by the ellipse
 * which fits pt. New points will be added in pts
 * @param ellipse ellipse is an optionnal parameter if the user decide to choose his hull from an ellipse
 */
void ellipseHull( const EdgePointsImage& img, std::vector<EdgePoint*>& pts, rom::numerical::geometry::Ellipse& ellipse, double delta );

void readPointsFromFile( char* file, std::vector<EdgePoint* >& pts );

/** @brief Ellipse growing
 * @param childrens vote winner children points
 * @param outerEllipsePoints outer ellipse points
 * @param ellipse target ellipse
 * @param Width of elliptic hull in ellipse growing
 */
void ellipseGrowing( const EdgePointsImage& img, const std::vector<EdgePoint*>& filteredChildrens,
                     std::vector<EdgePoint*>& outerEllipsePoints, rom::numerical::geometry::Ellipse& ellipse,
                     const double ellipseGrowingEllipticHullWidth, std::size_t & nSegmentOut, std::size_t & nLabel, bool goodInit);

void ellipseGrowing2( const EdgePointsImage& img, const std::vector<EdgePoint*>& filteredChildrens,
                      std::vector<EdgePoint*>& outerEllipsePoints, numerical::geometry::Ellipse& ellipse,
                      const double ellipseGrowingEllipticHullWidth, std::size_t & nSegmentOut, std::size_t & nLabel, bool goodInit);

} // namespace cctag
} // namespace marker
} // namespace vision
} // namespace rom

#endif
