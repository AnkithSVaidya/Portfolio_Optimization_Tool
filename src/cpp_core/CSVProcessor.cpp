#include "CSVProcessor.h"
#include <sstream>
#include <algorithm>
#include <numeric>
#include <iostream>
#include <iomanip>

namespace portfolio {

CSVProcessor::CSVProcessor(const std::string& delimiter, bool has_header) 
    : delimiter(delimiter), has_header(has_header) {}

CSVProcessor::~CSVProcessor() {}

std::vector<std::string> CSVProcessor::splitLine(const std::string& line) {
    std::vector<std::string> tokens;
    std::string token;
    std::stringstream ss(line);
    
    // Handle quoted fields
    bool in_quotes = false;
    std::string current_field;
    
    for (char c : line) {
        if (c == '"') {
            in_quotes = !in_quotes;
        } else if (c == delimiter[0] && !in_quotes) {
            tokens.push_back(trim(current_field));
            current_field.clear();
        } else {
            current_field += c;
        }
    }
    
    // Don't forget the last field
    tokens.push_back(trim(current_field));
    
    return tokens;
}

std::string CSVProcessor::trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\n\r");
    if (first == std::string::npos) return "";
    
    size_t last = str.find_last_not_of(" \t\n\r");
    return str.substr(first, (last - first + 1));
}

CSVData CSVProcessor::readCSV(const std::string& filename) {
    CSVData data;
    std::ifstream file(filename);
    
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file: " + filename);
    }
    
    std::string line;
    bool first_line = true;
    
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        
        std::vector<std::string> row = splitLine(line);
        
        if (first_line && has_header) {
            data.headers = row;
            first_line = false;
        } else {
            data.rows.push_back(row);
        }
    }
    
    file.close();
    
    data.num_rows = data.rows.size();
    data.num_cols = data.rows.empty() ? 0 : data.rows[0].size();
    
    return data;
}

bool CSVProcessor::writeCSV(const std::string& filename, const CSVData& data) {
    std::ofstream file(filename);
    
    if (!file.is_open()) {
        return false;
    }
    
    // Write headers if present
    if (!data.headers.empty()) {
        for (size_t i = 0; i < data.headers.size(); ++i) {
            file << data.headers[i];
            if (i < data.headers.size() - 1) {
                file << delimiter;
            }
        }
        file << "\n";
    }
    
    // Write data rows
    for (const auto& row : data.rows) {
        for (size_t i = 0; i < row.size(); ++i) {
            // Quote fields containing delimiter
            if (row[i].find(delimiter) != std::string::npos) {
                file << "\"" << row[i] << "\"";
            } else {
                file << row[i];
            }
            
            if (i < row.size() - 1) {
                file << delimiter;
            }
        }
        file << "\n";
    }
    
    file.close();
    return true;
}

void CSVProcessor::processLargeCSV(const std::string& filename, 
                                  std::function<void(const std::vector<std::string>&)> callback) {
    std::ifstream file(filename);
    
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file: " + filename);
    }
    
    std::string line;
    bool first_line = true;
    
    // Process file line by line to handle large files efficiently
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        
        if (first_line && has_header) {
            first_line = false;
            continue;
        }
        
        std::vector<std::string> row = splitLine(line);
        callback(row);
    }
    
    file.close();
}

std::vector<std::vector<double>> CSVProcessor::convertToNumeric(const CSVData& data, 
                                                               const std::vector<int>& columns) {
    std::vector<std::vector<double>> numeric_data;
    numeric_data.reserve(data.rows.size());
    
    for (const auto& row : data.rows) {
        std::vector<double> numeric_row;
        numeric_row.reserve(columns.size());
        
        for (int col : columns) {
            if (col < row.size()) {
                try {
                    numeric_row.push_back(std::stod(row[col]));
                } catch (const std::exception& e) {
                    numeric_row.push_back(0.0); // Default value for non-numeric
                }
            }
        }
        
        numeric_data.push_back(numeric_row);
    }
    
    return numeric_data;
}

CSVData CSVProcessor::filterRows(const CSVData& data, 
                                std::function<bool(const std::vector<std::string>&)> predicate) {
    CSVData filtered;
    filtered.headers = data.headers;
    
    for (const auto& row : data.rows) {
        if (predicate(row)) {
            filtered.rows.push_back(row);
        }
    }
    
    filtered.num_rows = filtered.rows.size();
    filtered.num_cols = data.num_cols;
    
    return filtered;
}

} // namespace portfolio
