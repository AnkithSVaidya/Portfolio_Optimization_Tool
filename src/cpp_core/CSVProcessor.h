#ifndef CSV_PROCESSOR_H
#define CSV_PROCESSOR_H

#include <string>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <memory>

namespace portfolio {

struct CSVData {
    std::vector<std::string> headers;
    std::vector<std::vector<std::string>> rows;
    size_t num_rows;
    size_t num_cols;
};

class CSVProcessor {
private:
    std::string delimiter;
    bool has_header;
    
    // Parsing helpers
    std::vector<std::string> splitLine(const std::string& line);
    std::string trim(const std::string& str);
    
public:
    CSVProcessor(const std::string& delimiter = ",", bool has_header = true);
    ~CSVProcessor();
    
    // File operations
    CSVData readCSV(const std::string& filename);
    bool writeCSV(const std::string& filename, const CSVData& data);
    
    // Stream processing for large files
    void processLargeCSV(const std::string& filename, 
                        std::function<void(const std::vector<std::string>&)> callback);
    
    // Data conversion
    std::vector<std::vector<double>> convertToNumeric(const CSVData& data, 
                                                      const std::vector<int>& columns);
    
    // Filtering and selection
    CSVData filterRows(const CSVData& data, 
                      std::function<bool(const std::vector<std::string>&)> predicate);
    CSVData selectColumns(const CSVData& data, const std::vector<int>& column_indices);
    
    // Aggregation
    std::unordered_map<std::string, double> aggregateColumn(const CSVData& data, 
                                                           int column_index,
                                                           const std::string& operation);
};

} // namespace portfolio

#endif