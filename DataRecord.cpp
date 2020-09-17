#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <exception>
#include <algorithm>

#include "DataRecord.hpp"


DataRecord::DataRecord() : 
    m_salePeriod{1970, 1},
    m_version(DataRecord::VersionType::NONE),
    m_skuId(-1),
    m_promotion(DataRecord::PromotionType::NONE),
    m_offerId(-1),
    m_regularPrice(0),
    m_sellingPrice(0),
    m_productCost(0) {}

bool DataRecord::build(const std::string& recordLine) {
    bool isSuccess = true;
    try {
        std::vector<std::string> parsedTokens = parseLine(recordLine);

        setSalePeriod(parsedTokens[0], parsedTokens[1]);
        setVersion(parsedTokens[2]);
        setSkuId(parsedTokens[3]);
        setPromotion(parsedTokens[4]);
        setCategory(parsedTokens[5]);
        setChannel(parsedTokens[6], parsedTokens[15]);
        setOfferId(parsedTokens[7]);
        setUnitsNumber(parsedTokens[8], parsedTokens[9], parsedTokens[10], parsedTokens[11]);
        setReularPrice(parsedTokens[12]);
        setSellingPrice(parsedTokens[13]);
        setProductCost(parsedTokens[14]);
    } catch(std::runtime_error &err) {
        std::cerr << err.what() << std::endl;
        isSuccess = false;
    } catch (std::invalid_argument &err) {
        std::cerr << "Error [" << err.what() << "] failed to parse parameters for line:\n" << recordLine << std::endl;
        isSuccess = false;
    } catch (std::exception &err) {
        std::cerr << "Error ["<< err.what() << "] failed to parse record:\n" << recordLine << std::endl;
        isSuccess = false;
    }

    return isSuccess;
}

std::vector<std::string> DataRecord::parseLine(const std::string& line) {
    std::vector<std::string> parsedTokens;
    std::stringstream streamLine(line);
    std::string token;

    while (getline(streamLine, token, ',')) {
        parsedTokens.push_back(token);
    }
    
    return std::move(parsedTokens);
}

void DataRecord::setSalePeriod(const std::string& year, const std::string& month) {
    uint32_t iyear = (year.size())? std::stoi(year) : 1970;
    uint32_t imonth = (month.size())? std::stoi(month) : 1;

    m_salePeriod = {iyear, imonth};
}

void DataRecord::setVersion(const std::string& version) {
    if (version == "ACTUALIZED") {
        m_version = VersionType::ACTUALIZED;
    } else if (version == "PLANNED") {
        m_version = VersionType::PLANNED;
    } else {
        m_version = VersionType::NONE;
    }
}

void DataRecord::setSkuId(const std::string& skuId) {
    m_skuId = (skuId.size())? std::stoi(skuId) : -1;
}

void DataRecord::setPromotion(const std::string& promotionType) {
    if (promotionType == "STRAIGHT DISCOUNTING") {
        m_promotion = DataRecord::PromotionType::STRAIGHT_DISCOUNTING;
    } else if (promotionType == "COMBINATION OFFERS") {
        m_promotion = DataRecord::PromotionType::COMBINATION_OFFERS;
    } else if (promotionType == "REGULAR PRICE") {
        m_promotion = DataRecord::PromotionType::REGULAR_PRICE;
    } else if (promotionType == "CONDITIONAL/CONTINGENT OFFER") {
        m_promotion = DataRecord::PromotionType::CONDITIONAL_CONTINGENT_OFFER;
    } else if (promotionType == "GWP/PWP/ORDER BUILDERS") {
        m_promotion = DataRecord::PromotionType::GWP_PWP_ORDER_BUILDERS;
    } else if (promotionType == "INSTANT/EXPRESS DELIVERY") {
        m_promotion = DataRecord::PromotionType::INSTANT_EXPRESS_DELIVERY;
    } else {
        m_promotion = DataRecord::PromotionType::NONE;
    }
}

void DataRecord::setCategory(const std::string& categoryType) {
    std::vector<std::string> beautyTypes = {"FRAGRANCE", "SKIN CARE", "HAIR CARE", "COLOR"};
    std::vector<std::string> fashiotTypes = {"INNERWEAR", "ACCESSORIES", "JEWELRY", "WATCHES", "FOOTWEAR", "OUTERWEAR"};
    std::vector<std::string> homeTypes = {"KIDS", "DECORATIVE", "WELLBEING", "HOUSEWARE", "ENTERTAINMENT"};

    if (std::find(beautyTypes.begin(), beautyTypes.end(), categoryType) != beautyTypes.end()) {
        m_category = DataRecord::CategoryType::BEAUTY;
    } else if (std::find(fashiotTypes.begin(), fashiotTypes.end(), categoryType) != fashiotTypes.end()) {
        m_category = DataRecord::CategoryType::FASHION;
    } else if (std::find(homeTypes.begin(), homeTypes.end(), categoryType) != homeTypes.end()) {
        m_category = DataRecord::CategoryType::HOME;
    } else {
        m_category = DataRecord::CategoryType::NONE;
    }
}

void DataRecord::setChannel(const std::string& channelName, const std::string& commissionInPercent) {
    if (channelName == "CHANNEL1") {
        m_channel.name = DataRecord::Channel::Name::CHANNEL1;
    } else if (channelName == "CHANNEL2") {
        m_channel.name = DataRecord::Channel::Name::CHANNEL2;
    } else if (channelName == "CHANNEL3") {
        m_channel.name = DataRecord::Channel::Name::CHANNEL3;
    } else {
        m_channel.name = DataRecord::Channel::Name::NONE;
    }

    m_channel.commissionInPercent = (commissionInPercent.size()) ? std::stod(commissionInPercent) : 0;
}

void DataRecord::setOfferId(const std::string& offerId) {
    m_offerId = (offerId.size()) ? std::stod(offerId) : -1;
}

void DataRecord::setUnitsNumber(const std::string& totalUnitsNumber,
                                const std::string& unitsNumberSoldInCategory1,
                                const std::string& unitsNumberSoldInCategory2,
                                const std::string& unitsNumberSoldInCategory3) {
    m_soldUnitsCount.push_back((totalUnitsNumber.size()) ? std::stoi(totalUnitsNumber) : -1);
    m_soldUnitsCount.push_back((unitsNumberSoldInCategory1.size()) ? std::stoi(unitsNumberSoldInCategory1) : -1);
    m_soldUnitsCount.push_back((unitsNumberSoldInCategory2.size()) ? std::stoi(unitsNumberSoldInCategory2) : -1);
    m_soldUnitsCount.push_back((unitsNumberSoldInCategory3.size()) ? std::stoi(unitsNumberSoldInCategory3) : -1);
}

void DataRecord::setReularPrice(const std::string& regularPrice) {
    m_regularPrice = (regularPrice.size()) ? std::stod(regularPrice) : 0;
}

void DataRecord::setSellingPrice(const std::string& sellingPrice) {
    m_sellingPrice = (sellingPrice.size()) ? std::stod(sellingPrice) : 0;
}

void DataRecord::setProductCost(const std::string& productCost) {
    m_productCost = (productCost.size()) ? std::stod(productCost) : 0;
}

bool operator<(const DataRecord &l, const DataRecord &r) {
    return l.m_sellingPrice < r.m_sellingPrice;
}