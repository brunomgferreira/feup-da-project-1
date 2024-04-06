#ifndef WATER_SUPPLY_ANALYSIS_SYSTEM_DATA_LOAD_ERROR_H
#define WATER_SUPPLY_ANALYSIS_SYSTEM_DATA_LOAD_ERROR_H


#include <stdexcept>

/**
* @brief Exception class for errors that occur during data loading.
*/

class DataLoadError : public std::runtime_error {
public:
    explicit DataLoadError(const std::string& message);
};


#endif //WATER_SUPPLY_ANALYSIS_SYSTEM_DATA_LOAD_ERROR_H
