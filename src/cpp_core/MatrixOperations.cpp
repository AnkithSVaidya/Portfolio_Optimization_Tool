#include "MatrixOperations.h"
#include <cmath>
#include <stdexcept>
#include <algorithm>

namespace portfolio {

std::vector<std::vector<double>> MatrixOperations::multiply(
    const std::vector<std::vector<double>>& A,
    const std::vector<std::vector<double>>& B) {
    
    size_t m = A.size();
    size_t n = A[0].size();
    size_t p = B[0].size();
    
    if (n != B.size()) {
        throw std::invalid_argument("Matrix dimensions incompatible for multiplication");
    }
    
    std::vector<std::vector<double>> result(m, std::vector<double>(p, 0.0));
    
    // Optimized matrix multiplication with cache-friendly access pattern
    for (size_t i = 0; i < m; ++i) {
        for (size_t k = 0; k < n; ++k) {
            double a_ik = A[i][k];
            for (size_t j = 0; j < p; ++j) {
                result[i][j] += a_ik * B[k][j];
            }
        }
    }
    
    return result;
}

std::vector<std::vector<double>> MatrixOperations::transpose(
    const std::vector<std::vector<double>>& matrix) {
    
    size_t m = matrix.size();
    size_t n = matrix[0].size();
    
    std::vector<std::vector<double>> result(n, std::vector<double>(m));
    
    for (size_t i = 0; i < m; ++i) {
        for (size_t j = 0; j < n; ++j) {
            result[j][i] = matrix[i][j];
        }
    }
    
    return result;
}

std::vector<std::vector<double>> MatrixOperations::inverse(
    const std::vector<std::vector<double>>& matrix) {
    
    size_t n = matrix.size();
    if (n != matrix[0].size()) {
        throw std::invalid_argument("Matrix must be square for inversion");
    }
    
    // Create augmented matrix [A | I]
    std::vector<std::vector<double>> augmented(n, std::vector<double>(2 * n, 0.0));
    
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            augmented[i][j] = matrix[i][j];
        }
        augmented[i][i + n] = 1.0;
    }
    
    // Gaussian elimination with partial pivoting
    for (size_t i = 0; i < n; ++i) {
        // Find pivot
        size_t max_row = i;
        for (size_t k = i + 1; k < n; ++k) {
            if (std::abs(augmented[k][i]) > std::abs(augmented[max_row][i])) {
                max_row = k;
            }
        }
        
        // Swap rows
        std::swap(augmented[i], augmented[max_row]);
        
        // Check for singular matrix
        if (std::abs(augmented[i][i]) < 1e-10) {
            throw std::runtime_error("Matrix is singular, cannot invert");
        }
        
        // Scale pivot row
        double pivot = augmented[i][i];
        for (size_t j = 0; j < 2 * n; ++j) {
            augmented[i][j] /= pivot;
        }
        
        // Eliminate column
        for (size_t k = 0; k < n; ++k) {
            if (k != i) {
                double factor = augmented[k][i];
                for (size_t j = 0; j < 2 * n; ++j) {
                    augmented[k][j] -= factor * augmented[i][j];
                }
            }
        }
    }
    
    // Extract inverse matrix
    std::vector<std::vector<double>> inverse(n, std::vector<double>(n));
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            inverse[i][j] = augmented[i][j + n];
        }
    }
    
    return inverse;
}

std::vector<std::vector<double>> MatrixOperations::choleskyDecomposition(
    const std::vector<std::vector<double>>& matrix) {
    
    size_t n = matrix.size();
    std::vector<std::vector<double>> L(n, std::vector<double>(n, 0.0));
    
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j <= i; ++j) {
            double sum = 0.0;
            
            if (j == i) {
                // Diagonal elements
                for (size_t k = 0; k < j; ++k) {
                    sum += L[j][k] * L[j][k];
                }
                L[j][j] = std::sqrt(matrix[j][j] - sum);
            } else {
                // Off-diagonal elements
                for (size_t k = 0; k < j; ++k) {
                    sum += L[i][k] * L[j][k];
                }
                L[i][j] = (matrix[i][j] - sum) / L[j][j];
            }
        }
    }
    
    return L;
}

double MatrixOperations::determinant(const std::vector<std::vector<double>>& matrix) {
    size_t n = matrix.size();
    if (n != matrix[0].size()) {
        throw std::invalid_argument("Matrix must be square to calculate determinant");
    }
    
    if (n == 1) {
        return matrix[0][0];
    }
    
    if (n == 2) {
        return matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
    }
    
    // LU decomposition for larger matrices
    std::vector<std::vector<double>> U = matrix;
    double det = 1.0;
    
    for (size_t i = 0; i < n; ++i) {
        // Partial pivoting
        size_t max_row = i;
        for (size_t k = i + 1; k < n; ++k) {
            if (std::abs(U[k][i]) > std::abs(U[max_row][i])) {
                max_row = k;
            }
        }
        
        if (max_row != i) {
            std::swap(U[i], U[max_row]);
            det *= -1;
        }
        
        det *= U[i][i];
        
        if (std::abs(U[i][i]) < 1e-10) {
            return 0.0;
        }
        
        for (size_t k = i + 1; k < n; ++k) {
            double factor = U[k][i] / U[i][i];
            for (size_t j = i; j < n; ++j) {
                U[k][j] -= factor * U[i][j];
            }
        }
    }
    
    return det;
}

} // namespace portfolio