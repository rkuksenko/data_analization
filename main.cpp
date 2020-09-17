#include <iostream>

#include "DataAnalizer.hpp"

int main() {
    DataAnalizer dataAnalizer;
    size_t readRecordsNumber = dataAnalizer.ParseFile("./dummy_data.csv");
    std::cout << "Read " << readRecordsNumber << " records\n";

    dataAnalizer.GenerateOutputFile("./result.csv");
}