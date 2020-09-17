#include <string>
#include <fstream>
#include <functional>
#include <vector>
#include <map>

#include "DataRecord.hpp"

class DataAnalizer {
public:
    DataAnalizer() = default;
    DataAnalizer(const DataAnalizer&) = delete;
    DataAnalizer(DataAnalizer&&) = delete;
    DataAnalizer& operator=(const DataAnalizer&) = delete;
    DataAnalizer& operator=(DataAnalizer&&) = delete;

    struct ResultRecord {
        uint32_t year;
        uint32_t month;
        double totalSales;
        double category1Sales;
        double category2Sales;
        double category3Sales;
        double totalMeanSellingPrice;
        double beautyMeanSellingPrice;
        double fashionMeanSellingPrice;
        double homeMeanSellingPrice;
        double totalMedianSellingPrice;
        double beautyMedianSellingPrice;
        double fashionMedianSellingPrice;
        double homeMedianSellingPrice;
        double channel1MeanSellingPrice;
        double channel2MeanSellingPrice;
        double channel3MeanSellingPrice;
        double channel1MedianSellingPrice;
        double channel2MedianSellingPrice;
        double channel3MedianSellingPrice;

        friend std::ostream& operator<<(std::ostream& os, const ResultRecord& rr);
    };

    /**
     * The function read the file "filename", parse it content
     * and store collected data in the m_annualMonthlyRecords map.
     */
    size_t ParseFile(const std::string& filename);

    /**
     * The function creates and fill "outFilename".
     */
    bool GenerateOutputFile(const std::string& outFilename) const;

private:
    double calculateSales(const std::vector<DataRecord> &records, size_t categoryNumber) const;
    double calculateMedianSellingPrice(const std::vector<DataRecord> &records) const;
    double calculateMeanSellingPrice(const std::vector<DataRecord> &records) const;
    std::vector<DataRecord> filterRecords(const std::vector<DataRecord> &allRecords,
                                          const std::function<bool(const DataRecord&)> &filter) const;

private:
    typedef std::vector<DataRecord> records;
    typedef std::map<uint32_t, records> monthlyRecords;
    std::map<uint32_t, monthlyRecords> m_annualMonthlyRecords;
};