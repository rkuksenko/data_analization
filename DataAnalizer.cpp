#include <fstream>
#include <set>
#include <algorithm>
#include <iomanip>
#include <functional>

#include "DataAnalizer.hpp"


size_t DataAnalizer::ParseFile(const std::string& filename) {
    std::ifstream inFile(filename, std::ios::in);
    std::string line;
    std::getline(inFile, line); // skip first line

    size_t count = 0;
    while (std::getline(inFile, line)) {
        DataRecord record;
        if (record.build(line)) {
            m_annualMonthlyRecords[record.m_salePeriod.year][record.m_salePeriod.month].push_back(record);
            count++;
        }
    }

    return count;
}

bool DataAnalizer::GenerateOutputFile(const std::string& filename) const {
    std::ofstream outFile(filename);

    for (const auto &annualRecord : m_annualMonthlyRecords) {
        const auto &year = annualRecord.first;

        for (const auto &monthlyRecord : annualRecord.second) {
            const auto &month = monthlyRecord.first;
            const auto &records = monthlyRecord.second;

            ResultRecord resultMontlyRecord;
            resultMontlyRecord.year = year;
            resultMontlyRecord.month = month;

            std::vector<DataRecord> sorted(records);
            std::sort(sorted.begin(), sorted.end());

            resultMontlyRecord.totalSales = calculateSales(sorted, 0);
            resultMontlyRecord.category1Sales = calculateSales(sorted, 1);
            resultMontlyRecord.category2Sales = calculateSales(sorted, 2);
            resultMontlyRecord.category3Sales = calculateSales(sorted, 3);
            resultMontlyRecord.totalMeanSellingPrice = calculateMeanSellingPrice(sorted);

            auto beautyFilter = [](const DataRecord &r){ return r.m_category == DataRecord::CategoryType::BEAUTY
                                    && r.m_promotion != DataRecord::PromotionType::GWP_PWP_ORDER_BUILDERS; };
            auto fashionFilter = [](const DataRecord &r){ return r.m_category == DataRecord::CategoryType::FASHION
                                    && r.m_promotion != DataRecord::PromotionType::GWP_PWP_ORDER_BUILDERS; };
            auto homeFilter = [](const DataRecord &r){ return r.m_category == DataRecord::CategoryType::HOME
                                    && r.m_promotion != DataRecord::PromotionType::GWP_PWP_ORDER_BUILDERS; };
            std::vector<DataRecord> beautyRecords = filterRecords(sorted, beautyFilter);
            std::vector<DataRecord> fashionRecords = filterRecords(sorted, fashionFilter);
            std::vector<DataRecord> homeRecords = filterRecords(sorted, homeFilter);

            resultMontlyRecord.beautyMeanSellingPrice = calculateMeanSellingPrice(beautyRecords);
            resultMontlyRecord.fashionMeanSellingPrice = calculateMeanSellingPrice(fashionRecords);
            resultMontlyRecord.homeMeanSellingPrice = calculateMeanSellingPrice(homeRecords);
            resultMontlyRecord.totalMedianSellingPrice = calculateMedianSellingPrice(sorted);
            resultMontlyRecord.beautyMedianSellingPrice = calculateMedianSellingPrice(beautyRecords);
            resultMontlyRecord.fashionMedianSellingPrice = calculateMedianSellingPrice(fashionRecords);
            resultMontlyRecord.homeMedianSellingPrice = calculateMedianSellingPrice(homeRecords);

            auto channel1Filter = [](const DataRecord &r){ return r.m_channel.name == DataRecord::Channel::Name::CHANNEL1
                                        && r.m_promotion != DataRecord::PromotionType::GWP_PWP_ORDER_BUILDERS; };
            auto channel2Filter = [](const DataRecord &r){ return r.m_channel.name == DataRecord::Channel::Name::CHANNEL2
                                        && r.m_promotion != DataRecord::PromotionType::GWP_PWP_ORDER_BUILDERS; };
            auto channel3Filter = [](const DataRecord &r){ return r.m_channel.name == DataRecord::Channel::Name::CHANNEL3
                                        && r.m_promotion != DataRecord::PromotionType::GWP_PWP_ORDER_BUILDERS; };
            std::vector<DataRecord> channel1Records = filterRecords(sorted, channel1Filter);
            std::vector<DataRecord> channel2Records = filterRecords(sorted, channel2Filter);
            std::vector<DataRecord> channel3Records = filterRecords(sorted, channel3Filter);

            resultMontlyRecord.channel1MeanSellingPrice = calculateMeanSellingPrice(channel1Records);
            resultMontlyRecord.channel2MeanSellingPrice = calculateMeanSellingPrice(channel2Records);
            resultMontlyRecord.channel3MeanSellingPrice = calculateMeanSellingPrice(channel3Records);
            resultMontlyRecord.channel1MedianSellingPrice = calculateMedianSellingPrice(channel1Records);
            resultMontlyRecord.channel2MedianSellingPrice = calculateMedianSellingPrice(channel2Records);
            resultMontlyRecord.channel3MedianSellingPrice = calculateMedianSellingPrice(channel3Records);

            outFile << resultMontlyRecord;
        }
    }

    outFile.close();

    return true;
}

double DataAnalizer::calculateSales(const std::vector<DataRecord> &records, size_t categoryNumber) const {
    double overallSales = 0;

    for(const auto &record : records) {
        auto price = record.m_sellingPrice;
        auto commission = record.m_channel.commissionInPercent / 100;

        if (record.m_version == DataRecord::VersionType::PLANNED) {
            continue;
        }

        overallSales += record.m_soldUnitsCount[categoryNumber] * price * (1 - commission);
    }

    return overallSales;
}

double DataAnalizer::calculateMedianSellingPrice(const std::vector<DataRecord> &records) const {
    if (!records.size()) {
        return 0;
    }

    return records[records.size()/2].m_sellingPrice;
}

double DataAnalizer::calculateMeanSellingPrice(const std::vector<DataRecord> &records) const {
    if (!records.size()) {
        return 0;
    }

    double totalSellingPrice = 0;
    std::for_each(records.begin(), records.end(), [&totalSellingPrice](const DataRecord &r){
        totalSellingPrice += r.m_sellingPrice;
    });

    return totalSellingPrice / records.size();
}

std::vector<DataRecord> DataAnalizer::filterRecords(const std::vector<DataRecord> &allRecords,
                                        const std::function<bool(const DataRecord&)> &filter) const {
    std::vector<DataRecord> filteredRecords;
    std::copy_if(allRecords.begin(), allRecords.end(), std::back_inserter(filteredRecords), filter);

    return std::move(filteredRecords);
}


std::ostream& operator<<(std::ostream& os, const DataAnalizer::ResultRecord& rr) {
    os << std::fixed << std::setprecision(2)
       << rr.year << ","
       << rr.month << ","
       << rr.totalSales << ","
       << rr.category1Sales << ","
       << rr.category2Sales << ","
       << rr.category3Sales << ","
       << rr.totalMeanSellingPrice << ","
       << rr.beautyMeanSellingPrice << ","
       << rr.fashionMeanSellingPrice << ","
       << rr.homeMeanSellingPrice << ","
       << rr.totalMedianSellingPrice << ","
       << rr.beautyMedianSellingPrice << ","
       << rr.fashionMedianSellingPrice << ","
       << rr.homeMedianSellingPrice << ","
       << rr.channel1MeanSellingPrice << ","
       << rr.channel2MeanSellingPrice << ","
       << rr.channel3MeanSellingPrice << ","
       << rr.channel1MedianSellingPrice << ","
       << rr.channel2MedianSellingPrice << ","
       << rr.channel3MedianSellingPrice << std::endl;

    return os;
}