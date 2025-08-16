#ifndef MATRIX_OPERATIONS_H
#define MATRIX_OPERATIONS_H

#include <vector>
#include <memory>

namespace portfolio {

class MatrixOperations {
public:
    // Matrix multiplication
    static std::vector<std::vector<double>> multiply(
        const std::vector<std::vector<double>>& A,
        const std::vector<std::vector<double>>& B);
    
    // Matrix transpose
    static std::vector<std::vector<double>> transpose(
        const std::vector<std::vector<double>>& matrix);
    
    // Matrix inversion using Gaussian elimination
    static std::vector<std::vector<double>> inverse(
        const std::vector<std::vector<double>>& matrix);
    
    // Eigenvalue decomposition
    static std::pair<std::vector<double>, std::vector<std::vector<double>>> 
        eigenDecomposition(const std::vector<std::vector<double>>& matrix);
    
    // Cholesky decomposition for positive definite matrices
    static std::vector<std::vector<double>> choleskyDecomposition(
        const std::vector<std::vector<double>>& matrix);
    
    // Solve linear system Ax = b
    static std::vector<double> solveLinearSystem(
        const std::vector<std::vector<double>>& A,
        const std::vector<double>& b);
    
    // Calculate determinant
    static double determinant(const std::vector<std::vector<double>>& matrix);
    
    // Check if matrix is positive definite
    static bool isPositiveDefinite(const std::vector<std::vector<double>>& matrix);
};

} // namespace portfolio

#endif