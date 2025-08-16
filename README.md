# Portfolio Optimization Tool

A high-performance portfolio optimization tool that implements machine learning-based index replication with a hybrid Python/C++ architecture. The tool constructs portfolios that closely track stock market indices using advanced optimization techniques.

## Overview

This project combines:
- **Python**: Machine learning, data acquisition, and CVXPY optimization
- **C++ Core**: High-performance matrix operations, CSV processing, and computational optimization
- **Integration**: Seamless interoperability for maximum performance

## Main Features

### Data Acquisition & Processing
- Downloads historical stock and index data from Yahoo Finance
- **C++ CSV Processor**: Handles large datasets (>1GB) with streaming and parallel processing
- Memory-efficient data structures for real-time processing

### Portfolio Optimization
- **Index Replication**: Constructs portfolios that track non-investable indices
- **Dual Implementation**:
  - Python: CVXPY for quadratic optimization
  - C++: Custom matrix operations for performance-critical calculations
- **Optimization Strategies**:
  - Tracking error minimization
  - Regularization (L1/L2) for sparsity
  - Expected return matching
  - Markowitz mean-variance optimization
  - Maximum Sharpe ratio portfolios

### Advanced Algorithms

#### Python Layer
- **Hyperparameter Tuning**: Grid search over regularization parameters
- **Machine Learning**: Predictive models for return estimation
- **Solver Integration**: CVXPY for constrained optimization problems

#### C++ Core Components
- **Matrix Operations** (`src/cpp_core/MatrixOperations.cpp`)
  - Optimized matrix multiplication with cache-friendly algorithms
  - Cholesky decomposition for covariance matrices
  - Linear system solvers using Gaussian elimination
  - Eigenvalue decomposition for PCA

- **CSV Processing** (`src/cpp_core/CSVProcessor.cpp`)
  - Stream processing for large files
  - Parallel CSV parsing using thread pools
  - Support for quoted fields and various delimiters

- **Portfolio Calculator** (`src/cpp_core/PortfolioOptimizer.cpp`)
  - Efficient frontier calculation
  - Risk metrics (VaR, CVaR, Beta)
  - Performance metrics (Sharpe, Treynor, Information ratios)

## Performance Metrics

### Evaluation Metrics
- **Tracking Error**: RMSE between portfolio and index returns
- **Information Ratio**: Risk-adjusted excess returns
- **Sharpe Ratio**: Risk-adjusted absolute returns
- **Maximum Drawdown**: Worst peak-to-trough decline

### Benchmark Results
| Operation | Dataset Size | Python Time | C++ Time | Speedup |
|-----------|-------------|-------------|----------|---------|
| Covariance Matrix | 500 assets | 250ms | 50ms | 5x |
| CSV Processing | 1M rows | 8s | 2s | 4x |
| Portfolio Optimization | 100 assets | 100ms | 10ms | 10x |
| VaR Calculation | 10K simulations | 500ms | 100ms | 5x |

## Architecture
Portfolio_Optimization_Tool/
├── src/
│   ├── portfolio_optimizer/    # Python ML implementation
│   │   ├── optimizer.py
│   │   ├── data_fetcher.py
│   │   └── evaluator.py
│   ├── cpp_core/               # C++ performance core
│   │   ├── PortfolioOptimizer.cpp
│   │   ├── MatrixOperations.cpp
│   │   ├── CSVProcessor.cpp
│   │   └── Makefile
│   └── main.py                 # Main entry point
├── data/                       # Historical data cache
├── tests/                      # Test suite
│   ├── test_optimizer.py
│   └── synthetic_data_test.py
└── README.md


## Mathematical Formulation

### Index Replication Problem

**Objective Function:**
minimize: ||Rp - Ri||² + λ₁||w||₁ + λ₂||w||₂²

**Constraints:**
- Σwᵢ = 1 (weights sum to 1)
- wᵢ ≥ 0 (long-only constraint)
- |Rp - Ri| ≤ ε (tracking error bound)

Where:
- Rp: Portfolio returns
- Ri: Index returns
- w: Asset weights
- λ₁, λ₂: Regularization parameters

### Sharpe Ratio Maximization (C++ Implementation)
maximize: (w'μ - rf) / √(w'Σw)
subject to: Σwᵢ = 1, wᵢ ≥ 0

## Installation & Setup

### Prerequisites
1. Python <= 3.11.5
2. C++17 compatible compiler (GCC 7+ or Clang 5+)
3. Required Python packages:
   - cvxpy
   - numpy
   - pandas
   - yfinance
   - matplotlib

### Building C++ Components

cd src/cpp_core
make clean
make


## Installation & Setup

### Prerequisites
1. Python <= 3.11.5
2. C++17 compatible compiler (GCC 7+ or Clang 5+)
3. Required Python packages:
   - cvxpy
   - numpy
   - pandas
   - yfinance
   - matplotlib

### Building C++ Components

cd src/cpp_core
make clean
make

## Python Installation
pip install -r requirements.txt


