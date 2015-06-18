#include <cmath>
#include <vector>
#include <stdexcept>

// Reference-implementation of B-spline basis functions and utilities.
namespace bspline_storve {

// Simple implementation of functionality like NumPy's linspace()
template <typename T>
void linspace(T left, T right, int n, std::vector<T>& res) {
    if (n < 0) {
        return;
    }
    res.resize(n);
    if (n == 0) {
        return;
    } else if (n == 1) {
        res[0] = left;
    } else {
        for (int i = 0; i < n; i++) {
            res[i] = left+i*(right-left)/(n-1);
        }
    }
}

// Test if floating point number is zero.
template <typename T>
bool _floatIsZero(T value, double eps=1e-6) {
    return std::abs(value) < eps;
}

//Return num/dev with the special rule that 0/0 is 0.
template <typename T>
T _specialDiv(T num, T den) {
    if (_floatIsZero(num) && _floatIsZero(den)) {
        return 0.0;
    } else {
        return num / den;
    }
}

// Compute B-splines directly using recursive definition.        
//  j:  B-spline basis no.
//  p:  Polynomial degree
//  knots: knot vector (must support indexing with [])
template <typename T>
T bsplineBasis(int j, int p, T x, const std::vector<T>& knots) {
    if (p == 0 ) {
        if ( (x >= knots[j]) && (x < knots[j+1]) ) {
            return 1.0;
        } else {
            return 0.0;
        }
    } else {
        T left = _specialDiv((x-knots[j])*bsplineBasis(j,p-1,x,knots), knots[j+p]-knots[j]);
        T right = _specialDiv((knots[j+1+p]-x)*bsplineBasis(j+1,p-1,x,knots), knots[j+1+p]-knots[j+1]);
        return left + right;
    }
}


// Direkte
template <typename T>
T B0(int j, T x, const std::vector<T>& knots) {
    if (knots[j] <= x && x < knots[j+1]) {
        return 1.0;
    } else {
        return 0.0;
    }
}

template <typename T>
T B1(int j, T x, const std::vector<T>& knots) {
    if (knots[j] <= x && x < knots[j+1]) {
        // *B_{j,0}
        return (
            (x-knots[j])/(knots[j+1]-knots[j])
        );
    } else if (knots[j+1] <= x && x < knots[j+2]) {
        // *B_{j+1,0}
        return (
            (knots[j+2]-x) / (knots[j+2]-knots[j+1])
        );
    } else {
        return 0.0;
    } 
}

template <typename T>
T B2(int j, T x, const std::vector<T>& knots) {
    if (knots[j] <= x && x < knots[j+1]) {
        // *B_{j,0}
        return (
            ((x-knots[j])/(knots[j+2]-knots[j]))*((x-knots[j])/(knots[j+1]-knots[j]))
        );
    } else if (knots[j+1] <= x && x < knots[j+2]) {
        // *B_{j+1,0}
        return (
            ((x-knots[j])/(knots[j+2]-knots[j]))*((knots[j+2]-x)/(knots[j+2]-knots[j+1]))
            + ((knots[j+3]-x)/(knots[j+3]-knots[j+1]))*((x-knots[j+1])/(knots[j+2]-knots[j+1]))
        );
    } else if (knots[j+2] <= x && x < knots[j+3]) {
        // *B_{j+2,0}
        return (
            ((knots[j+3]-x)/(knots[j+3]-knots[j+1]))*((knots[j+3]-x)/(knots[j+3]-knots[j+2]))
        );
    } else {
        return 0.0;
    }
}

template <typename T>
T B3(int j, T x, const std::vector<T>& knots) {
    if (knots[j] <= x && x < knots[j+1]) {
        // *B_{j,0}
        return (
            ((x-knots[j])/(knots[j+3]-knots[j]))*((x-knots[j])/(knots[j+2]-knots[j]))*((x-knots[j])/(knots[j+1]-knots[j]))
        );
    } else if (knots[j+1] <= x && x < knots[j+2]) {
        // *B_{j+1,0}
        return (
            ((x-knots[j])/(knots[j+3]-knots[j]))*((x-knots[j])/(knots[j+2]-knots[j]))*((knots[j+2]-x)/(knots[j+2]-knots[j+1]))
            + ((x-knots[j])/(knots[j+3]-knots[j]))*((knots[j+3]-x)/(knots[j+3]-knots[j+1]))*((x-knots[j+1])/(knots[j+2]-knots[j+1]))
            + ((knots[j+4]-x)/(knots[j+4]-knots[j+1]))*((x-knots[j+1])/(knots[j+3]-knots[j+1]))*((x-knots[j+1])/(knots[j+2]-knots[j+1]))
        );
    } else if (knots[j+2] <= x && x < knots[j+3]) {
        // *B_{j+2,0}
        return (
            ((x-knots[j])/(knots[j+3]-knots[j]))*((knots[j+3]-x)/(knots[j+3]-knots[j+1]))*((knots[j+3]-x)/(knots[j+3]-knots[j+2]))
            + ((knots[j+4]-x)/(knots[j+4]-knots[j+1]))*((x-knots[j+1])/(knots[j+3]-knots[j+1]))*((knots[j+3]-x)/(knots[j+3]-knots[j+2]))
            + ((knots[j+4]-x)/(knots[j+4]-knots[j+1]))*((knots[j+4]-x)/(knots[j+4]-knots[j+2]))*((x-knots[j+2])/(knots[j+3]-knots[j+2]))
        );
    } else if (knots[j+3] <= x && x < knots[j+4]) {
        // *B_{j+3,0}
        return (
            ((knots[j+4]-x)/(knots[j+4]-knots[j+1]))*((knots[j+4]-x)/(knots[j+4]-knots[j+2]))*((knots[j+4]-x)/(knots[j+4]-knots[j+3]))
        );
    } else {
        return 0.0;
    }
}

/*
Create a p+1-regular uniform knot vector for
a given number of control points
Throws if n is too small
endHack: Make the last knot bigger than the one before it, which has the
effect of making it non-zero in last knot value.
*/
template <typename T>
std::vector<T> uniformRegularKnotVector(int numPoints,
                                        int degree,
                                        T tStart=0.0,
                                        T tEnd=1.0,
                                        bool endHack=false) {

    // The minimum length of a degree+1-regular knot vector is 2*(degree+1)
    if (numPoints < degree+1) {
        throw std::runtime_error("Too small n for a uniform regular knot vector");
    }
    
    std::vector<T> knots;
    // degree+1 copies of tStart left and degree+1 copies of tEnd right
    // but one of each in linspace
    for (int i = 0; i < degree; i++) {
        knots.push_back(tStart);
    }
    std::vector<T> temp;
    linspace(tStart, tEnd, numPoints+1-degree, temp);
    for (T t : temp) {
        knots.push_back(t);
    }
    for (int i = 0; i < degree; i++) {
        knots.push_back(tEnd);
    }
    
    if (endHack) {
        int numKnots = knots.size();
        knots[numKnots-1] += 1.0;
    }
    return knots;
}

// Create a uniformly spaced open knot vector on [tStart, tEnd]
template <typename T>
std::vector<T> uniformOpenKnotVector(int numPoints,
                                     int degree,
                                     T tStart=0.0,
                                     T tEnd=1.0,
                                     bool endHack=false) {

    const auto num_knots = numPoints + degree + 1;
    std::vector<T> res;
    linspace(tStart, tEnd, num_knots, res);
    if (endHack) {
        res[res.size()-1] += static_cast<T>(1.0);
    }
    return res;
}

// Create a knot vector for a closed spline curve intended to be
// evaluated on the closed interval [t_start, t_end]
template <typename T>
std::vector<T> closedKnotVector(int numPoints,
                                int degree) {

    auto d = static_cast<T>(numPoints-degree);
    std::vector<T> knots;
    linspace(-degree/d, numPoints/d, numPoints+degree+1, knots);
    return knots;
}


/*
Return the control point abscissa for the control polygon
of a one-dimensional spline.
*/
template <typename T>
std::vector<T> controlPolygon(int p, const std::vector<T>& knots) {
    int n = knots.size() - p - 1;
    std::vector<T> abscissas;
    for (int i = 0; i < n; i++) { 
        T sum = 0.0;
        for (int j = 1; j <= p; j++) {
            sum += knots[i+j];
        }
        abscissas.push_back(sum / p);
    }
    return abscissas;
}

}   // end namespace bspline_storve
