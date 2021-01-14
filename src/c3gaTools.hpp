// Copyright (c) 2018 by University Paris-Est Marne-la-Vallee
// c3gaTools.hpp
// Authors: Stephane Breuils and Vincent Nozick
// Contact: vincent.nozick@u-pem.fr

/// \file c3gaTools.hpp
/// \author Stephane Breuils, Vincent Nozick
/// \brief some useful functions when using conformal geometric algebra of R^3. Use this file if you generated the lib using the "c3ga.conf" configuration file.


// Anti-doublon
#ifndef C3GA_TOOLS_HPP__
#define C3GA_TOOLS_HPP__
#pragma once

// External Includes
#include <random>
#include <chrono>
#include <string>
#include <limits>

// Internal Includes
#include <c3ga/Mvec.hpp>


/// \namespace grouping the multivectors object
namespace c3ga{


    
    /// \brief build a point from a vector
    /// \param x vector component related to e1
    /// \param y vector component related to e2
    /// \param z vector component related to e3
    /// \return a multivector corresponding to a point p = e0 +  x e1 + y e2 + z e3 + 0.5 || (x e1 + y e2 + z e3) ||^2 einf
    template<typename T>
    c3ga::Mvec<T> point(const T &x, const T &y, const T &z){

        c3ga::Mvec<T> mv;
        mv[c3ga::E1] = x;
        mv[c3ga::E2] = y;
        mv[c3ga::E3] = z;
        mv[c3ga::Ei] = 0.5 * (x*x + y*y + z*z);
        mv[c3ga::E0] = 1.0;

        return mv;
    }

    /// \brief build a point from a vector
    /// \param vec is should be a multivector of grade 1 vec = v1 e1 + v2 e2 + v3 e3. If vec has other components, they will be ignored during the execution.
    /// \return a multivector corresponding to a point p = e0 + v1 e1 + v2 e2 + v3 e3 + 0.5 || vec ||^2 einf
    template<typename T>
    c3ga::Mvec<T> point(const c3ga::Mvec<T> &vec){
        return point(vec[c3ga::E1], vec[c3ga::E2], vec[c3ga::E3]);
    }


    static std::default_random_engine generator;

    inline void setRandomSeed(unsigned int seed){
        generator.seed(seed);
    }

    inline void setRandomSeed(){
        generator.seed(std::chrono::system_clock::now().time_since_epoch().count());
    }

    /// \brief build a random point with Euclidean coordinates ranging in [-1,1]
    /// \param seed optional random seem
    /// \return a multivector corresponding to a point p = e0 + v1 e1 + v2 e2 + v3 e3 + 0.5 || vec ||^2 einf
    template<typename T>
    c3ga::Mvec<T> randomPoint(){

		// uniform distribution over [-1,1]
		std::uniform_real_distribution<T> uniformRealDistribution(-1.0,1.0);

		// build the point
        return point(uniformRealDistribution(generator), uniformRealDistribution(generator), uniformRealDistribution(generator));
    }


    /// \brief build a dual sphere from a center and a radius
    /// \param centerX dual sphere center component related to e1
    /// \param centerY dual sphere center component related to e2
    /// \param centerZ dual sphere center component related to e3
    /// \param radius of the sphere
    /// \return a multivector corresponding to a dual sphere s = center - 0.5 radius ei, with center being e0 +  x e1 + y e2 + z e3 + 0.5 || (x e1 + y e2 + z e3) ||^2 einf.
    template<typename T>
    c3ga::Mvec<T> dualSphere(const T &centerX, const T &centerY, const T &centerZ, const T &radius){
        c3ga::Mvec<T> dualSphere = point(centerX,centerY,centerZ);
        dualSphere[c3ga::Ei] -= 0.5*radius;
        return dualSphere;
    }

    /// \brief extract the center and radius of a dual sphere
    /// \param dualSphere the dual sphere
    /// \param radius positive number for real spheres and negative for imaginary spheres.
    /// \param center center of the dual sphere
    template<typename T>
    void radiusAndCenterFromDualSphere(const c3ga::Mvec<T> &dualSphere, T &radius, c3ga::Mvec<T> &center){
        radius = (dualSphere | dualSphere) / dualSphere[c3ga::E0];
        center = dualSphere / dualSphere[c3ga::E0];
    }

    /// \brief extract the center and radius of a dual sphere
    /// \param dualSphere the dual sphere
    /// \param radius positive number for real spheres and negative for imaginary spheres.
    /// \param center center of the dual sphere
    template<typename T>
    void extractDualCircle(c3ga::Mvec<T> dualcircle, T &radius, c3ga::Mvec<T> &center, c3ga::Mvec<T> &direction){

        c3ga::Mvec<T> pt1, pt2;
        extractPairPoint(dualcircle, pt1, pt2);
        radius = 0.5 * sqrt( (pt2[c3ga::E1]-pt1[c3ga::E1])*(pt2[c3ga::E1]-pt1[c3ga::E1]) + (pt2[c3ga::E2]-pt1[c3ga::E2])*(pt2[c3ga::E2]-pt1[c3ga::E2]) + (pt2[c3ga::E3]-pt1[c3ga::E3])*(pt2[c3ga::E3]-pt1[c3ga::E3]) );
        center = point(0.5*(pt2[c3ga::E1] + pt1[c3ga::E1]), 0.5*(pt2[c3ga::E2] + pt1[c3ga::E2]), 0.5*(pt2[c3ga::E3] + pt1[c3ga::E3]));
        direction = pt2 - pt1;
        direction[c3ga::Ei] = 0;
        direction /= direction.norm();
    }

    /// \brief extract the 2 points of a pair point
    /// \param pairPoint : the pair point to be analysed
    /// \param pt1 : the output point 1.
    /// \param pt2 : the output point 2.
    template<typename T>
    void extractPairPoint(const c3ga::Mvec<T> &pairPoint, c3ga::Mvec<T> &pt1, c3ga::Mvec<T> &pt2){
        c3ga::Mvec<T> denominator = - c3ga::ei<double>() | pairPoint;
        pt1 = (pairPoint + sqrt(fabs(pairPoint | pairPoint)) ) / denominator;
        pt2 = (pairPoint - sqrt(fabs(pairPoint | pairPoint)) ) / denominator;

        // remove numerical error (nearly zero remaining parts)
        pt1.roundZero();
        pt2.roundZero();
    }

    /// \brief extract the point in a flat point
    /// \param flatPoint : the flat point to be analysed
    /// \param pt : the output point
    template<typename T>
    void extractFlatPoint(const c3ga::Mvec<T> &flatPoint, c3ga::Mvec<T> &pt){
        pt = - ( c3ga::e0i<double>() | (c3ga::e0<double>() ^ flatPoint) ) / ( c3ga::e0i<double>() | flatPoint);
        
        // remove numerical error (nearly zero remaining parts)
        pt.roundZero();

        // make a point from this Euclidean vector
        pt = c3ga::point(pt[c3ga::E1],pt[c3ga::E2],pt[c3ga::E3]);
    }

    /// \brief extract the position and orientation from a tangent vector (grade 2)
    /// \param tangent vector (of grade 2) to be analysed
    /// \param position : the position point of the tangent
    /// \param orientation : the orientation of the tangent
    template<typename T>
    void extractTangentVector(const c3ga::Mvec<T> &tangent, c3ga::Mvec<T> &position, c3ga::Mvec<T> &orientation){
        position =  tangent / (tangent | c3ga::ei<T>()); 
        orientation = (c3ga::ei<T>() | tangent) ^ c3ga::ei<T>();
        // from bivector to euclidean
        orientation = orientation | c3ga::e0<T>();
        orientation /= orientation.norm();
    }

    /// \brief extract the position and orientation from a tangent bivector (grade 3)
    /// \param tangent bivector (of grade 3) to be analysed
    /// \param position : the position point of the tangent
    /// \param orientation : the orientation of the tangent
    template<typename T>
    void extractTangentBivector(const c3ga::Mvec<T> &tangent, c3ga::Mvec<T> &position, c3ga::Mvec<T> &orientation){
        extractTangentVector(tangent.dual(),position,orientation);
    }


    /// \brief interpret the nature of the geometric object (line, circle, pair point, ...)
    /// \param multivector: the multivector to be studied
    /// \todo ... todo :) see mvintc3ga.cpp in gaViewer
    template<typename T>
    std::string whoAmI(c3ga::Mvec<T> mv){

        // zero error checking
        const T epsilon = std::numeric_limits<T>::epsilon();

    	// extract grade
        std::vector<unsigned int> grades_ = mv.grades();

    	// null vector
        if(grades_.size() == 0) 
            return "null vector";

        // non-homogeneous multivector
        if(grades_.size() > 1){
            return "non-homogeous multivector";
            // check if versor ...
        }

        // blade
        if(grades_.size() == 1){

            // numerical stability: scale the multivector so that the average of the coeff is 1
            auto blade = mv.findGrade(grades_[0])->vec;
            mv /= blade.array().abs().sum() / blade.size();
            //return std::to_string((double)blade.array().abs().sum()/ blade.size());

            // extract properties
            T square = (mv | mv);
            c3ga::Mvec<T> ei_outer_mv(c3ga::ei<T>() ^ mv);
            bool squareToZero = (fabs(square) <= 1.0e3*epsilon);
            bool roundObject  = !(fabs(ei_outer_mv.quadraticNorm()) < epsilon);
            //return "squareToZero : " + std::to_string(squareToZero) + " | roundObject : " +  std::to_string(roundObject) + " | ei_outer_mv.norm() : " + std::to_string(ei_outer_mv.quadraticNorm()) + " | square : " + std::to_string(fabs(square));

        	switch(grades_[0]){

        		////////////////////////////////
        		case 0 : // grade 0
        			return "scalar";
        			break;

        		////////////////////////////////
        		case 1 : // grade 1
        			{
        				// euclidean vector
        				// if(fabs(mv[c3ga::E0]) < epsilon && fabs(mv[c3ga::Ei]) < epsilon)
        				// 	return "Euclidean vector";

        				// point (dual tangent trivector)
	        			if( squareToZero && roundObject )
        					return "point (dual tangent trivector)";

        				// dual sphere
	        			if( square > epsilon && roundObject )
    	    				return "dual sphere";

    	    			        // imaginary dual sphere (dual sphere with negative radius)
        				if( square < -epsilon && roundObject)
        					return "imaginary dual sphere";

                        // dual plane   
                        if((!roundObject))
                            return "dual plane";

        				// unknown
        				return "unknown 1-vector";
						break;        				
        			}

        		////////////////////////////////
        		case 2 : // grade 2
        			{
                        // tangent vector (put this test first in the grade 2 list)
                        bool smallerSquareToZero = (fabs(square) <= 1.0e3*epsilon);
                        if(smallerSquareToZero && roundObject)
                        	return "tangent vector (dual tangent bivector)";

        				// pair point
                        if(roundObject && (square > epsilon) ) 
            				return "pair point (imaginary dual circle)";

                        // imaginary pair point
                        if(roundObject && (square < -epsilon))
                            return "imaginary pair point (dual circle)";

                        // for flat points and dual lines
                        bool onlyBivectorInfinity = (fabs( ( (mv ^ c3ga::ei<double>() ) | c3ga::e0<double>() ).quadraticNorm()) < epsilon);

        				// flat point
                        // no euclidian or eO bivector : only e_ix
#if 1
                        if((!roundObject) && onlyBivectorInfinity) 
#else
                        if(fabs(mv[c3ga::E12]) < epsilon && 
                           fabs(mv[c3ga::E13]) < epsilon && 
                           fabs(mv[c3ga::E23]) < epsilon && 
                           fabs(mv[c3ga::E01]) < epsilon && 
                           fabs(mv[c3ga::E02]) < epsilon && 
                           fabs(mv[c3ga::E03]) < epsilon )
#endif
            				return "flat point";

        				// dual line
                        // no origine bivector 
#if 1
                        if((!roundObject) && (!onlyBivectorInfinity)) 
#else
                        if(fabs(mv[c3ga::E01]) < epsilon && 
                           fabs(mv[c3ga::E02]) < epsilon && 
                           fabs(mv[c3ga::E03]) < epsilon && 
                           fabs(mv[c3ga::E0i]) < epsilon )
#endif
                            return "dual line";

        				// unknown
        				return "unknown 2-vector";
						break;        				
        			}


        		////////////////////////////////
        		case 3 : // grade 3
        			{

                        // tangent bivector (put this test first in the grade 2 list)
                        bool smallerSquareToZero = (fabs(square) <= 1.0e3*epsilon);
                        if(smallerSquareToZero && roundObject)
                        	return "tangent bivector (dual tangent vector)";

                        // circle
                        if(roundObject && (square > epsilon) ) 
                            return "circle (imaginary dual pair point)";

        				// imaginary circle
                        if(roundObject && (square < -epsilon))
            				return "imaginary circle (dual pair point)";


            			// for dual flat points and lines
                        bool onlyTrivectorInfinity = (fabs( ( (mv ^ c3ga::ei<double>() ) | c3ga::e0<double>() ).quadraticNorm()) < epsilon);

        				// dual flat point
                        // no e0 trivector
#if 1
                        if((!roundObject) && !onlyTrivectorInfinity) 
#else
                        if(fabs(mv[c3ga::E012]) < epsilon && 
                           fabs(mv[c3ga::E013]) < epsilon && 
                           fabs(mv[c3ga::E023]) < epsilon && 
                           fabs(mv[c3ga::E01i]) < epsilon && 
                           fabs(mv[c3ga::E02i]) < epsilon && 
                           fabs(mv[c3ga::E03i]) < epsilon )
#endif
            				return "dual flat point";

        				// line
                        // only e_ixx trivectors
#if 1
                        if((!roundObject) && onlyTrivectorInfinity) 
#else
                        if(fabs(mv[c3ga::E012]) < epsilon && 
                           fabs(mv[c3ga::E023]) < epsilon && 
                           fabs(mv[c3ga::E013]) < epsilon && 
                           fabs(mv[c3ga::E123]) < epsilon )
#endif
                            return "line";


        				// unknown
        				return "unknown 3-vector";
						break;        				
        			}


        		////////////////////////////////
        		case 4 : // grade 4
        			{
        				// dual euclidean vector (removed because a plane can also have this form)
        				// if(fabs(mv[c3ga::E0123]) < epsilon && fabs(mv[c3ga::E123i]) < epsilon)
        				// 	return "dual Euclidean vector";

        				// dual point
	        			if( squareToZero && roundObject )
        					return "tangent trivector (dual point)";

        				// sphere and imaginary sphere: compute radius from dual 
        				c3ga::Mvec<double> dualSphere = mv.dual();
        				T dualSquare = dualSphere | dualSphere;

        				// sphere
	        			if( dualSquare > epsilon && roundObject )
    	    				return "sphere";

    	    			// imaginary sphere (sphere with negative radius)
        				if( dualSquare < -epsilon && roundObject)
        					return "imaginary sphere";

                        // plane   
                        if((!roundObject))
                            return "plane";

        				// unknown
        				return "unknown 4-vector";
						break;        				
        			}


        		////////////////////////////////
        		case 5 : // grade 5
        			{
        				// unknown
        				return "pseudo-scalar";
						break;        				
        			}

        		////////////////////////////////
        		default : return "unknown";

        	}
        }

       	return std::to_string(grades_[0]) + "-vector (homogeneous)";
    }


/*
    /// \brief interpret the nature of the geometric object (line, circle, pair point, ...)
    /// \param multivector: the multivector to be studied
    /// \todo ... todo :) see mvintc3ga.cpp in gaViewer
    template<typename T>
    std::string whoAmI_gaviewer(const c3ga::Mvec<T> &mv){

        // numerical error epsilon
        //T epsilon = epsilon;

        // extract grade
        std::vector<unsigned int> grades_ = mv.grades();

        // extract properties
        T square = (mv | mv);
        c3ga::Mvec<T> ei_outer_mv(c3ga::ei<T>() ^ mv);
        //c3ga::Mvec<T> ei_inner_mv(c3ga::ei<T>() | mv);  // inner product
        c3ga::Mvec<T> ei_inner_mv( (c3ga::ei<T>() * mv).grade(1-mv.grade()));  // left contractin

        bool squareToZero = (fabs(square) <= epsilon);
        bool ei_outer_mv_toZero = (ei_outer_mv.norm() < epsilon);
        bool ei_inner_mv_toZero = (ei_inner_mv.norm() < epsilon);

        //return "squareToZero : " + std::to_string(squareToZero) + " | ei_outer_mv_toZero : " +  std::to_string(ei_outer_mv_toZero) + " | ei_inner_mv_toZero : " +  std::to_string(ei_inner_mv_toZero);

        // null vector
        if(grades_.size() == 0) {
            return "null vector";
        }

        // non-homogeneous multivector
        if(grades_.size() > 1){
            return "non-homogeous multivector";
            // check if versor ...
        }

        // blade
        if(grades_.size() == 1){

            switch(grades_[0]){

                case 0 : // grade 0
                    return "scalar";
                    break;

                case 1 : // grade 1
                    {

                        // euclidean vector
                        if(fabs(mv[c3ga::E0]) < epsilon && fabs(mv[c3ga::Ei]) < epsilon)
                            return "Euclidean vector";

                        // point
                        if( squareToZero && !ei_outer_mv_toZero && !ei_inner_mv_toZero)
                            return "point";

                        // dual sphere
                        if( square > epsilon && !ei_outer_mv_toZero && !ei_inner_mv_toZero )
                            return "dual sphere r = " + std::to_string(square);

                        // free dual sphere (dual sphere with negative radius)
                        if( square < -epsilon && !ei_outer_mv_toZero && !ei_inner_mv_toZero)
                            return "free dual sphere";

                        // dual plane
                        if(ei_outer_mv_toZero && ei_inner_mv_toZero)
                            return "dual plane";

                        // unknown
                        return "unknown 1-vector";
                        break;                      
                    }

                case 2 : // grade 2
                    {
                        // pair point
                        if(!ei_outer_mv_toZero && ei_inner_mv_toZero)
                        return "pair point";

                        // flat point
                        if(ei_outer_mv_toZero && ei_inner_mv_toZero)
                        return "flat point";

                        // dual line
                        if(ei_outer_mv_toZero && ei_inner_mv_toZero)
                        return "dual line";

                        // dual circle
                        if(!ei_outer_mv_toZero && ei_inner_mv_toZero)
                        return "dual circle";

                        // free dual circle
                        if(!ei_outer_mv_toZero && ei_inner_mv_toZero)
                        return "free dual circle";

                        // unknown
                        return "unknown 2-vector";
                        break;                      
                    }


                case 3 : // grade 3
                    {
                        // unknown
                        return "unknown 3-vector";
                        break;                      
                    }


                case 4 : // grade 4
                    {
                        // unknown
                        return "unknown 3-vector";
                        break;                      
                    }


                case 5 : // grade 5
                    {
                        // unknown
                        return "pseudo-scalar";
                        break;                      
                    }

                default : return "unknown";

            }
        }

        return std::to_string(grades_[0]) + "-vector (homogeneous)";
    }

*/

    /// \brief extract 2 points pt1 and pt2 from a pair point p = pt1 ^ pt2
    /// \param pairPoint implicitly contains 2 points
    /// \param epsilon is the minimum threshold to specify if 2 points are disjoint
    /// \return a list of 2 points (if they are disjoint) or a single point.
    template<typename T>
    std::vector<c3ga::Mvec<T>> extractPairPoint(const c3ga::Mvec<T> &pairPoint, const T &epsilon = 1.0e-7){

        std::vector<c3ga::Mvec<T>> points;
        T innerSqrt = sqrt(pairPoint | pairPoint);
        if(innerSqrt < epsilon)
            points.push_back(pairPoint / pairPoint[c3ga::E0]);
        else {
            points.push_back( (pairPoint+innerSqrt)/ pairPoint[c3ga::E0]);
            points.push_back( (pairPoint-innerSqrt)/ pairPoint[c3ga::E0]);
        }
        return points;
    }


	/// \brief compute the normal of a surface on a point.
	/// \param surface is a quad vector (sphere of plane).
	/// \param point is a normalized point (e0 = 1) lying on the surface where the normal is estimated.
	/// \return a normal vector (e1,e2,e3) with L2 norm = 1 
    template<typename T>
    c3ga::Mvec<T> surfaceNormal(c3ga::Mvec<T> &surface, c3ga::Mvec<T> &point){
	    c3ga::Mvec<T> normal;
	    normal[c3ga::E1] = - point[c3ga::E1] * surface[c3ga::E0123] / point[c3ga::E0] + surface[c3ga::E023i];  
	    normal[c3ga::E2] = - point[c3ga::E2] * surface[c3ga::E0123] / point[c3ga::E0] - surface[c3ga::E013i]; 
	    normal[c3ga::E3] = - point[c3ga::E3] * surface[c3ga::E0123] / point[c3ga::E0] + surface[c3ga::E012i]; 

	    normal = normal / (double) sqrt(normal[c3ga::E1]*normal[c3ga::E1] + normal[c3ga::E2]*normal[c3ga::E2] + normal[c3ga::E3]*normal[c3ga::E3]);

	    return normal;
    }


} // namespace

#endif // projection_inclusion_guard

