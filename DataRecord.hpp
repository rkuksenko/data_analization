#include <string>
#include <vector>


struct DataRecord {
    struct SalePeriod {
        uint32_t year;
        uint32_t month;
    };

    enum class VersionType {
        ACTUALIZED = 0,
        PLANNED = 1,
        NONE = 2
    };

    enum class PromotionType {
        STRAIGHT_DISCOUNTING = 0,
        COMBINATION_OFFERS = 1,
        REPRESENTATIVE_OFFER = 2,
        REGULAR_PRICE = 3,
        CONDITIONAL_CONTINGENT_OFFER = 4,
        GWP_PWP_ORDER_BUILDERS = 5,
        INSTANT_EXPRESS_DELIVERY = 6,
        NONE = 7
    };

    enum class CategoryType {
        BEAUTY = 0,
        FASHION = 1,
        HOME = 2,
        NONE = 3
    };

    struct Channel {
        Channel() : name(Name::NONE),
                    commissionInPercent(0) {}

        enum class Name {
            CHANNEL1 = 0,
            CHANNEL2 = 1,
            CHANNEL3 = 2,
            NONE = 3
        };

        Name name;
        double commissionInPercent;
    };

    DataRecord();
    bool build(const std::string& lineRecord);
    friend bool operator<(const DataRecord &l, const DataRecord &r);

    SalePeriod m_salePeriod;
    VersionType m_version;
    uint64_t m_skuId;
    PromotionType m_promotion;
    CategoryType m_category;
    Channel m_channel;
    uint64_t m_offerId;
    std::vector<uint32_t> m_soldUnitsCount;
    double m_regularPrice;
    double m_sellingPrice;
    double m_productCost;

private:
    std::vector<std::string> parseLine(const std::string& recordLine);
    void setSalePeriod(const std::string& year, const std::string& month);
    void setVersion(const std::string& version);
    void setSkuId(const std::string& skuId);
    void setPromotion(const std::string& promotionType);
    void setCategory(const std::string& categoryType);
    void setChannel(const std::string& channelName, const std::string& commissionInPercent);
    void setOfferId(const std::string& offerId);
    void setUnitsNumber(const std::string& totalUnitsNumber, const std::string& unitsNumberSoldInCategory1,
                        const std::string& unitsNumberSoldInCategory2, const std::string& unitsNumberSoldInCategory3);
    void setReularPrice(const std::string& regularPrice);
    void setSellingPrice(const std::string& sellingPrice);
    void setProductCost(const std::string& productCost);
};