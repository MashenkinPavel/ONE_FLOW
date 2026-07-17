#include <iostream>
#include <Eigen/Sparse>
#include <vector>
#include <gtest/gtest.h>


TEST(EigenTest, Eigen_SLAE){
    Eigen::SparseMatrix<double> matr(100,100);
    Eigen::VectorXd vect(100);
    std::vector< Eigen::Triplet<double> > matr_triplet;
    matr_triplet.reserve(100);
    for (int i=0;i<100;i++){
        matr_triplet.push_back({i,i,10});
        vect[i] = 1.0;
    }
    matr.setFromTriplets(matr_triplet.begin(), matr_triplet.end());
    matr.makeCompressed(); 
    
    Eigen::BiCGSTAB<Eigen::SparseMatrix<double>, Eigen::IncompleteLUT<double> > bicg_ilut;
    bicg_ilut.preconditioner().setFillfactor(2);  // уровень заполнения
    
    bicg_ilut.compute(matr);
    bicg_ilut.setTolerance(1e-1);
    bicg_ilut.setMaxIterations(1000);
    Eigen::VectorXd x = bicg_ilut.solve(vect);
    std::cout << "BiCGSTAB решение за " << bicg_ilut.iterations()<<" итерации"<<std::endl ;    
    double residual_bicg = (matr * x - vect).norm();
    std::cout << "Норма невязки: " << residual_bicg << std::endl;
    /*for (int i=0;i<x.size();i++){
        std::cout<<"x["<<i<<"] = "<<x[i]<<std::endl;
    }  
    */
    EXPECT_TRUE( fabs (x[0] - 0.1) < 0.000001);
}

