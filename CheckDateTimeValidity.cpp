#include <map>
#include <string>
#include <array>
#include <exception>

namespace datetime{

    constexpr int MAX_YEAR=9999;
    constexpr int MAX_MONTH=12;
    constexpr int MAX_HOUR=23;
    constexpr int MAX_MINUTE=59;
    constexpr int MAX_SECOND=59;

    constexpr int MIN_YEAR=1;
    constexpr int MIN_MONTH=1;
    constexpr int MIN_DAY=1;
    constexpr int MIN_HOUR=0;
    constexpr int MIN_MINUTE=0;
    constexpr int MIN_SECOND=0;

    namespace error{
        struct DateTimeErrorCode{
            enum class ITEM{
                NONE,
                YEAR,
                MONTH,
                DAY,
                HOUR,
                MINUTE,
                SECOND
            };

            enum class ERROR{
                NONE,
                BIG,
                SMALL
            };

            ERROR error;
            ITEM item;

            bool HasError(){
                return !(error == ERROR::NONE);
            }
        };

        using namespace std::string_literals;

        static const std::map<DateTimeErrorCode::ERROR,std::map<DateTimeErrorCode::ITEM,std::string>> ERROR_MESSAGES={
            {DateTimeErrorCode::ERROR::BIG,{{DateTimeErrorCode::ITEM::YEAR,"year is too big"s},
                                            {DateTimeErrorCode::ITEM::MONTH,"month is too big"s},
                                            {DateTimeErrorCode::ITEM::DAY,"day is too big"s},
                                            {DateTimeErrorCode::ITEM::HOUR,"hour is too big"s},
                                            {DateTimeErrorCode::ITEM::MINUTE,"minute is too big"s},
                                            {DateTimeErrorCode::ITEM::SECOND,"second is too big"s}}},
            {DateTimeErrorCode::ERROR::SMALL,{{DateTimeErrorCode::ITEM::YEAR,"year is too small"s},
                                            {DateTimeErrorCode::ITEM::MONTH,"month is too small"s},
                                            {DateTimeErrorCode::ITEM::DAY,"day is too small"s},
                                            {DateTimeErrorCode::ITEM::HOUR,"hour is too small"s},
                                            {DateTimeErrorCode::ITEM::MINUTE,"minute is too small"s},
                                            {DateTimeErrorCode::ITEM::SECOND,"second is too small"s}}}
            };
    }

    namespace functional{

        using namespace datetime::error;

        DateTimeErrorCode CheckDateTimeValidity(const DateTime& dt) {
        if (dt.year < MIN_YEAR) {
            return {DateTimeErrorCode::ERROR::SMALL, DateTimeErrorCode::ITEM::YEAR};
        }

        if (dt.year > MAX_YEAR) {
            return {DateTimeErrorCode::ERROR::BIG, DateTimeErrorCode::ITEM::YEAR};
        }

        if (dt.month < MIN_MONTH) {
            return {DateTimeErrorCode::ERROR::SMALL, DateTimeErrorCode::ITEM::MONTH};
        }

        if (dt.month > MAX_MONTH) {
            return {DateTimeErrorCode::ERROR::BIG, DateTimeErrorCode::ITEM::MONTH};
        }

        const bool is_leap_year = (dt.year % 4 == 0) && !(dt.year % 100 == 0 && dt.year % 400 != 0);
        const std::array<int,12> month_lengths = {31, 28 + is_leap_year, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        const int MAX_DAY = month_lengths[dt.month - 1];


        if (dt.day < MIN_DAY){
            return {DateTimeErrorCode::ERROR::SMALL, DateTimeErrorCode::ITEM::DAY};
        }

        if (dt.day> MAX_DAY) {
            return {DateTimeErrorCode::ERROR::BIG, DateTimeErrorCode::ITEM::DAY};
        }

        if (dt.hour < MIN_HOUR) {
            return {DateTimeErrorCode::ERROR::SMALL, DateTimeErrorCode::ITEM::HOUR};
        }

        if (dt.hour>MAX_HOUR) {
            return {DateTimeErrorCode::ERROR::BIG, DateTimeErrorCode::ITEM::HOUR};
        }

        if (dt.minute < MIN_MINUTE) {
            return {DateTimeErrorCode::ERROR::SMALL, DateTimeErrorCode::ITEM::MINUTE};
        }

        if (dt.minute > MAX_MINUTE) {
            return {DateTimeErrorCode::ERROR::BIG, DateTimeErrorCode::ITEM::MINUTE};
        }

        if (dt.second < MIN_SECOND) {
            return {DateTimeErrorCode::ERROR::SMALL, DateTimeErrorCode::ITEM::SECOND};
        }

        if (dt.second > MAX_SECOND) {
            return {DateTimeErrorCode::ERROR::BIG, DateTimeErrorCode::ITEM::SECOND};
        }
        }
    }

    void CheckDateTimeValidity(const DateTime& dt){
        auto result = functional::CheckDateTimeValidity(dt);
        if(result.HasError())
            throw domain_error(error::ERROR_MESSAGES.at(result.error).at(result.item));
    }
}
