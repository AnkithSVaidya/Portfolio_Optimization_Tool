#ifndef PORTFOLIO_OPTIMIZER_H
#define PORTFOLIO_OPTIMIZER_H

#include <vector>
#include <string>
#include <unordered_map>
#include <memory>
#include <chrono>

namespace portfolio {

struct Asset {
    std::string symbol;
    double current_price;
    double expected_return;
    double volatility;
    double weight;
    std::vector<double> historical_prices;
};

struct OptimizationResult {
    std::vector<double> optimal_weights;
    double expected_return;
    double portfolio_variance;
    double sharpe_ratio;
    std::chrono::microseconds computation_time;
};

class PortfolioOptimizer {
private:
    std::vector<Asset> assets;
    std::vector<std::vector<double>> covariance_matrix;
    std::vector<std::vector<double>> correlation_matrix;
    double risk_free_rate;
    
    // Matrix operations for optimization
    void calculateCovarianceMatrix();
    void calculateCorrelationMatrix();
    std::vector<double> calculateReturns(const std::vector<double>& prices);
    
    // Optimization algorithms
    OptimizationResult markowitzOptimization(double target_return);
    OptimizationResult maximizeSharpeRatio();
    OptimizationResult minimizeVariance();
    
public:
    PortfolioOptimizer(double risk_free_rate = 0.02);
    ~PortfolioOptimizer();
    
    // Asset management
    void addAsset(const Asset& asset);
    void removeAsset(const std::string& symbol);
    void updateAssetData(const std::string& symbol, const std::vector<double>& prices);
    
    // Portfolio optimization methods
    OptimizationResult optimizePortfolio(const std::string& strategy = "sharpe");
    OptimizationResult efficientFrontier(int num_points = 50);
    
    // Risk metrics calculation
    double calculateVaR(double confidence_level = 0.95);
    double calculateCVaR(double confidence_level = 0.95);
    double calculateBeta(const std::vector<double>& market_returns);
    
    // Performance metrics
    double calculateSharpeRatio(const std::vector<double>& weights);
    double calculateTreynorRatio(const std::vector<double>& weights, double beta);
    double calculateInformationRatio(const std::vector<double>& weights, 
                                   const std::vector<double>& benchmark_returns);
    
    // Getters
    std::vector<Asset> getAssets() const { return assets; }
    std::vector<std::vector<double>> getCovarianceMatrix() const { return covariance_matrix; }
};

} // namespace portfolio

#endif