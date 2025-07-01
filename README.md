# Portfolio Optimization Tool

This project implements a machine learning-based approach to index replication, which aims to construct a portfolio of stocks that closely tracks the performance of a given stock market index. The goal is to mimic the returns of indices that are not directly investable by most individuals.

The main features of this project are:

**Data Acquisition:** Downloads historical stock data and index data from Yahoo Finance.

**Portfolio Optimization:** Formulates index replication as a constrained optimization problem, incorporating tracking error minimization, regularization, and expected return matching.

**Solver Integration:** Utilizes the CVXPY library to solve the quadratic optimization problem and obtain optimal portfolio weights.

**Hyperparameter Tuning:** Performs a grid search over regularization parameters to find the optimal configuration that minimizes the tracking error.

**Performance Evaluation:** Computes and reports key performance metrics, such as tracking error and information ratio, to assess the quality of the index replication.

**Synthetic Data Testing:** Includes a testing framework to validate the optimizer's performance using synthetically generated data, ensuring robustness and correctness.

This project provides a practical solution for portfolio managers to replicate the performance of various stock market indices using a data-driven approach and machine learning techniques.


# To run
python main.py

# Prerequisites
1) Make sure you use a python version <= 3.11.5
2) Ensure that cvxpy is properly installed
