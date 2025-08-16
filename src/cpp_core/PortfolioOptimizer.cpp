#include "PortfolioOptimizer.h"
#include "MatrixOperations.h"
#include <algorithm>
#include <numeric>
#include <cmath>
#include <iostream>

namespace portfolio {

PortfolioOptimizer::PortfolioOptimizer(double risk_free_rate) 
    : risk_free_rate(risk_free_rate) {}

PortfolioOptimizer::~PortfolioOptimizer() {}

void PortfolioOptimizer::addAsset(const Asset& asset) {
    assets.push_back(asset);
    // Recalculate covariance matrix when new asset is added
    if (assets.size() > 1) {
        calculateCovarianceMatrix();
    }
}

void PortfolioOptimizer::calculateCovarianceMatrix() {
    size_t n = assets.size();
    covariance_matrix.resize(n, std::vector<double>(n, 0.0));
    
    for (size_t i = 0; i < n; ++i) {
        std::vector<double> returns_i = calculateReturns(assets[i].historical_prices);
        
        for (size_t j = i; j < n; ++j) {
            std::vector<double> returns_j = calculateReturns(assets[j].historical_prices);
            
            // Calculate covariance
            double mean_i = std::accumulate(returns_i.begin(), returns_i.end(), 0.0) / returns_i.size();
            double mean_j = std::accumulate(returns_j.begin(), returns_j.end(), 0.0) / returns_j.size();
            
            double covariance = 0.0;
            size_t min_size = std::min(returns_i.size(), returns_j.size());
            
            for (size_t k = 0; k < min_size; ++k) {
                covariance += (returns_i[k] - mean_i) * (returns_j[k] - mean_j);
            }
            
            covariance /= (min_size - 1);
            covariance_matrix[i][j] = covariance;
            covariance_matrix[j][i] = covariance; // Symmetric matrix
        }
    }
}

std::vector<double> PortfolioOptimizer::calculateReturns(const std::vector<double>& prices) {
    std::vector<double> returns;
    returns.reserve(prices.size() - 1);
    
    for (size_t i = 1; i < prices.size(); ++i) {
        double daily_return = (prices[i] - prices[i-1]) / prices[i-1];
        returns.push_back(daily_return);
    }
    
    return returns;
}

OptimizationResult PortfolioOptimizer::optimizePortfolio(const std::string& strategy) {
    auto start = std::chrono::high_resolution_clock::now();
    
    OptimizationResult result;
    
    if (strategy == "sharpe") {
        result = maximizeSharpeRatio();
    } else if (strategy == "min_variance") {
        result = minimizeVariance();
    } else if (strategy == "markowitz") {
        double target_return = 0.10; // 10% target return
        result = markowitzOptimization(target_return);
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    result.computation_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    return result;
}

OptimizationResult PortfolioOptimizer::maximizeSharpeRatio() {
    OptimizationResult result;
    size_t n = assets.size();
    result.optimal_weights.resize(n);
    
    // Simplified implementation - equal weight for now
    // In production, would use quadratic programming solver
    for (size_t i = 0; i < n; ++i) {
        result.optimal_weights[i] = 1.0 / n;
    }
    
    // Calculate expected return
    result.expected_return = 0.0;
    for (size_t i = 0; i < n; ++i) {
        result.expected_return += result.optimal_weights[i] * assets[i].expected_return;
    }
    
    // Calculate portfolio variance
    result.portfolio_variance = 0.0;
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            result.portfolio_variance += result.optimal_weights[i] * 
                                        result.optimal_weights[j] * 
                                        covariance_matrix[i][j];
        }
    }
    
    // Calculate Sharpe ratio
    double portfolio_std = std::sqrt(result.portfolio_variance);
    result.sharpe_ratio = (result.expected_return - risk_free_rate) / portfolio_std;
    
    return result;
}

OptimizationResult PortfolioOptimizer::minimizeVariance() {
    OptimizationResult result;
    size_t n = assets.size();
    
    // Using simplified inverse variance weighting
    std::vector<double> inv_variances(n);
    double sum_inv_var = 0.0;
    
    for (size_t i = 0; i < n; ++i) {
        inv_variances[i] = 1.0 / covariance_matrix[i][i];
        sum_inv_var += inv_variances[i];
    }
    
    result.optimal_weights.resize(n);
    for (size_t i = 0; i < n; ++i) {
        result.optimal_weights[i] = inv_variances[i] / sum_inv_var;
    }
    
    // Calculate metrics
    result.expected_return = 0.0;
    result.portfolio_variance = 0.0;
    
    for (size_t i = 0; i < n; ++i) {
        result.expected_return += result.optimal_weights[i] * assets[i].expected_return;
        for (size_t j = 0; j < n; ++j) {
            result.portfolio_variance += result.optimal_weights[i] * 
                                        result.optimal_weights[j] * 
                                        covariance_matrix[i][j];
        }
    }
    
    double portfolio_std = std::sqrt(result.portfolio_variance);
    result.sharpe_ratio = (result.expected_return - risk_free_rate) / portfolio_std;
    
    return result;
}

double PortfolioOptimizer::calculateVaR(double confidence_level) {
    // Calculate Value at Risk using historical simulation
    std::vector<double> portfolio_returns;
    
    // Simulate portfolio returns
    for (size_t t = 0; t < assets[0].historical_prices.size() - 1; ++t) {
        double portfolio_return = 0.0;
        for (size_t i = 0; i < assets.size(); ++i) {
            if (t < assets[i].historical_prices.size() - 1) {
                double asset_return = (assets[i].historical_prices[t+1] - 
                                     assets[i].historical_prices[t]) / 
                                     assets[i].historical_prices[t];
                portfolio_return += assets[i].weight * asset_return;
            }
        }
        portfolio_returns.push_back(portfolio_return);
    }
    
    // Sort returns
    std::sort(portfolio_returns.begin(), portfolio_returns.end());
    
    // Find VaR at confidence level
    size_t var_index = static_cast<size_t>((1.0 - confidence_level) * portfolio_returns.size());
    return -portfolio_returns[var_index];
}

double PortfolioOptimizer::calculateSharpeRatio(const std::vector<double>& weights) {
    double expected_return = 0.0;
    double variance = 0.0;
    
    for (size_t i = 0; i < assets.size(); ++i) {
        expected_return += weights[i] * assets[i].expected_return;
        for (size_t j = 0; j < assets.size(); ++j) {
            variance += weights[i] * weights[j] * covariance_matrix[i][j];
        }
    }
    
    double std_dev = std::sqrt(variance);
    return (expected_return - risk_free_rate) / std_dev;
}

} // namespace portfolio