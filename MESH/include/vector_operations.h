#pragma once
#include <array>
#include <cmath>

namespace vector_operations{
    
    double dot_product(const std::array<double,3>& a, const std::array<double,3>& b ){
        double result = 0.;
        result =  a[0]* b[0] + a[1]* b[1] + a[2]* b[2];
        return result;
    }
    
    double length(const std::array<double,3>& a){
        return sqrt(a[0]*a[0]+a[1]*a[1]+a[2]*a[2]);
    }

    std::array<double,3> cross_product(const std::array<double,3>& a, const std::array<double,3>& b )
    {
        std::array<double,3> result{0.};
        result[0] = a[1]*b[2] - b[1]*a[2];
        result[1] = b[0]*a[2] - a[0]*b[2];
        result[2] = a[0]*b[1] - b[0]*a[1]; 
        return result;
    }



    /*
    @  вычисление объема тетраэдера с основанием ABC и сершиной O
    */
    double TetrVolume(   const std::array<double,3>& vecOA,
                         const std::array<double,3>& vecOB,
                         const std::array<double,3>& vecOC)
    {
        double rez = 1./6.* fabs(( vecOA[0]* (vecOB[1]*vecOC[2] -  vecOC[1]* vecOB[2])
                             -vecOA[1]* (vecOB[0]*vecOC[2] -  vecOC[0]*vecOB[2])
                             +vecOA[2]* (vecOB[0]*vecOC[1] - vecOC[0]* vecOB[1]))     );
        return rez;
    }


}

std::array<double,3> operator+ (const std::array<double,3>& lhs, const std::array<double,3>& rhs ){
    std::array<double,3> result{0.};
    for (int i= 0; i<3; i++){
        result[i] = lhs[i] + rhs[i];
    }
    return result;
}

std::array<double,3> operator- (const std::array<double,3>& lhs, const std::array<double,3>& rhs ){
    std::array<double,3> result{0.};
    for (int i= 0; i<3; i++){
        result[i] = lhs[i] - rhs[i];
    }
    return result;
}


std::array<double,3> operator*(const std::array<double,3>& lhs , const double& rhs){
    std::array<double,3> result{0.};
    for (int i= 0; i<3; i++){
        result[i] = lhs[i] * rhs;
    }
    return result;
}