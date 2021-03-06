#include "LightDate.hpp"
#include "gtest/gtest.h"

// Constructor tests

TEST(ConstructorTest, ConstructorCreatesValidDate)
{
    Date test{2018, Month::jan, 1};
    ASSERT_EQ(test.getYear(), 2018);
    ASSERT_EQ(test.getMonth(), Month::jan);
    ASSERT_EQ(test.getDay(), 1);
    ASSERT_EQ(test.getWeekday(), Weekday::mon);
}

TEST(ValidDateTest, InvalidDateThrowsException)
{
    try {
        Date invalid{2018, Month::feb, 29};
    }
    catch (std::runtime_error& e) {
        ASSERT_EQ(e.what(), std::string("Invalid date constructed: 2018-2-29"));
    }
    catch (...) {
        FAIL() << "Expected std::runtime_error";
    }
}

TEST(ValidDateTest, NegativeDayThrowsException)
{
    try {
        Date invalid{2018, Month::jan, -1};
    }
    catch (std::runtime_error& e) {
        ASSERT_EQ(e.what(), std::string("Invalid date constructed: 2018-1--1"));
    }
    catch (...) {
        FAIL() << "Expected std::runtime_error";
    }
}

TEST(ValidDateTest, NegativeYearThrowsException)
{
    try {
        Date invalid{-1, Month::jan, 1};
    }
    catch (std::runtime_error& e) {
        ASSERT_EQ(e.what(), std::string("Invalid date constructed: -1-1-1"));
    }
    catch (...) {
        FAIL() << "Expected std::runtime_error";
    }
}

TEST(ValidDateTest, Day29ValidInFebruaryIfLeapYear)
{
    Date{2004, Month::feb, 29};
}

// Setter tests

TEST(SetterTest, SetYearSetsAValidDate)
{
    Date test{2010, Month::jan, 1};
    test.setYear(2000);
    ASSERT_EQ(test.getYear(), 2000);
}

TEST(SetterTest, SetYearThrowsExceptionOnInvalidDate)
{
    Date test{2010, Month::jan, 1};
    try {
        test.setYear(-10);
    }
    catch (std::runtime_error& e) {
        ASSERT_EQ(e.what(), std::string("Invalid year set"));
    }
    catch (...) {
        FAIL() << "Expected std::runtime_error";
    }
}

TEST(SetterTest, SetMonthSetsAValidDate)
{
    Date test{2010, Month::jan, 1};
    test.setMonth(Month::mar);
    ASSERT_EQ(test.getMonth(), Month::mar);
}

TEST(SetterTest, SetMonthThrowsExceptionOnInvaldDate)
{
    Date test{2004, Month::mar, 31};
    try {
        test.setMonth(Month::apr);
    }
    catch (std::runtime_error& e) {
        ASSERT_EQ(e.what(), std::string("Invalid month set"));
    }
    catch (...) {
        FAIL() << "Expected std::runtime_error";
    }
}

TEST(SetterTest, SetDaySetsAValidDay)
{
    Date test{2010, Month::jan, 1};
    test.setDay(20);
    ASSERT_EQ(test.getDay(), 20);
}

TEST(SetterTest, SetDayThrowsExceptionOnInvalidDate)
{
    Date test{2010, Month::jan, 1};
    try {
        test.setDay(50);
    }
    catch (std::runtime_error& e) {
        ASSERT_EQ(e.what(), std::string("Invalid day set"));
    }
    catch (...) {
        FAIL() << "Expected std::runtime_error";
    }
}

// Conversion tests

TEST(ConversionTest, CorrectConversionFromTm)
{
    // test_tm represents 2000-02-15
    tm test_tm;
    test_tm.tm_year = 100;
    test_tm.tm_mon = 1;
    test_tm.tm_mday = 15;
    test_tm.tm_wday = 2;

    Date conv(test_tm);

    ASSERT_EQ(conv.getYear(), 2000);
    ASSERT_EQ(conv.getMonth(), Month::feb);
    ASSERT_EQ(conv.getDay(), 15);
    ASSERT_EQ(conv.getWeekday(), Weekday::tue);

    // test_tm2 represents 2000-05-07
    tm test_tm2;
    test_tm2.tm_year = 100;
    test_tm2.tm_mon = 4;
    test_tm2.tm_mday = 7;
    test_tm2.tm_wday = 0;

    Date conv2(test_tm2);

    ASSERT_EQ(conv2.getYear(), 2000);
    ASSERT_EQ(conv2.getMonth(), Month::may);
    ASSERT_EQ(conv2.getDay(), 7);
    ASSERT_EQ(conv2.getWeekday(), Weekday::sun);
}

TEST(ConversionTest, InvalidTmDateFails)
{
    try {
        tm test_tm;
        test_tm.tm_year = 1000;
        test_tm.tm_mon = 10;
        test_tm.tm_mday = 31;
        test_tm.tm_wday = 6;

        Date conv(test_tm);
    }
    catch (std::runtime_error& e) {
        ASSERT_EQ(e.what(), std::string("tm date invalid"));
    }
    catch (...) {
        FAIL() << "Expected std::runtime_error";
    }
}

TEST(ConversionTest, CorrectConversionFromTmNoWeekday)
{
    // test_tm represents 2000-02-15
    tm test_tm;
    test_tm.tm_year = 100;
    test_tm.tm_mon = 1;
    test_tm.tm_mday = 15;

    Date conv(test_tm);

    ASSERT_EQ(conv.getYear(), 2000);
    ASSERT_EQ(conv.getMonth(), Month::feb);
    ASSERT_EQ(conv.getDay(), 15);
    ASSERT_EQ(conv.getWeekday(), Weekday::tue);
}

// Increment tests

TEST(PostIncrementTest, AddsOneDay)
{
    Date test{2010, Month::jun, 10};
    test++;
    ASSERT_EQ(test.getYear(), 2010);
    ASSERT_EQ(test.getMonth(), Month::jun);
    ASSERT_EQ(test.getDay(), 11);
}

TEST(PostIncrementTest, CorrectlyHandlesEndOfMonthDates)
{
    Date test{2017, Month::nov, 30};
    test++;
    ASSERT_EQ(test.getYear(), 2017);
    ASSERT_EQ(test.getMonth(), Month::dec);
    ASSERT_EQ(test.getDay(), 1);
    ASSERT_EQ(test.getWeekday(), Weekday::fri);
}

TEST(PostIncrementTest, CorrectlyHandlesEndOfYearDates)
{
    Date test{2017, Month::dec, 31};
    ++test;
    ASSERT_EQ(test.getYear(), 2018);
    ASSERT_EQ(test.getMonth(), Month::jan);
    ASSERT_EQ(test.getDay(), 1);
    ASSERT_EQ(test.getWeekday(), Weekday::mon);
}

TEST(PreIncrementTest, AddsOneDay)
{
    Date test{2010, Month::jun, 10};
    ++test;
    ASSERT_EQ(test.getYear(), 2010);
    ASSERT_EQ(test.getMonth(), Month::jun);
    ASSERT_EQ(test.getDay(), 11);
}

TEST(PreIncrementTest, CorrectlyHandlesEndOfMonthDates)
{
    Date test{2017, Month::nov, 30};
    ++test;
    ASSERT_EQ(test.getYear(), 2017);
    ASSERT_EQ(test.getMonth(), Month::dec);
    ASSERT_EQ(test.getDay(), 1);
    ASSERT_EQ(test.getWeekday(), Weekday::fri);
}

TEST(PreIncrementTest, CorrectlyHandlesEndOfYearDates)
{
    Date test{2017, Month::dec, 31};
    ++test;
    ASSERT_EQ(test.getYear(), 2018);
    ASSERT_EQ(test.getMonth(), Month::jan);
    ASSERT_EQ(test.getDay(), 1);
    ASSERT_EQ(test.getWeekday(), Weekday::mon);
}

// Compound asignment addition tests

TEST(CompoundAssignmentAddition, Addition)
{
    Date test{2010, Month::jan, 1};
    test += 10;
    ASSERT_EQ(test.getYear(), 2010);
    ASSERT_EQ(test.getMonth(), Month::jan);
    ASSERT_EQ(test.getDay(), 11);
    ASSERT_EQ(test.getWeekday(), Weekday::mon);
}

TEST(CompoundAssignmentAddition, AdditionRollOverMonth)
{
    Date test{2010, Month::jan, 31};
    test += 10;
    ASSERT_EQ(test.getYear(), 2010);
    ASSERT_EQ(test.getMonth(), Month::feb);
    ASSERT_EQ(test.getDay(), 10);
    ASSERT_EQ(test.getWeekday(), Weekday::wed);
}

TEST(CompoundAssignmentAddition, AdditionRollOverYear)
{
    Date test{2010, Month::dec, 31};
    test += 10;
    ASSERT_EQ(test.getYear(), 2011);
    ASSERT_EQ(test.getMonth(), Month::jan);
    ASSERT_EQ(test.getDay(), 10);
    ASSERT_EQ(test.getWeekday(), Weekday::mon);
}

// Compound asignment subtraction tests

TEST(CompoundAssignmentSubtraction, Subtraction)
{
    Date test{2010, Month::jan, 11};
    test -= 10;
    ASSERT_EQ(test.getYear(), 2010);
    ASSERT_EQ(test.getMonth(), Month::jan);
    ASSERT_EQ(test.getDay(), 1);
    ASSERT_EQ(test.getWeekday(), Weekday::fri);
}

TEST(CompoundAssignmentSubtraction, SubtractionRollOverMonth)
{
    Date test{2010, Month::feb, 1};
    test -= 10;
    ASSERT_EQ(test.getYear(), 2010);
    ASSERT_EQ(test.getMonth(), Month::jan);
    ASSERT_EQ(test.getDay(), 22);
    ASSERT_EQ(test.getWeekday(), Weekday::fri);
}

TEST(CompoundAssignmentSubtraction, SubtractionRollOverYear)
{
    Date test{2010, Month::jan, 1};
    test -= 10;
    ASSERT_EQ(test.getYear(), 2009);
    ASSERT_EQ(test.getMonth(), Month::dec);
    ASSERT_EQ(test.getDay(), 22);
    ASSERT_EQ(test.getWeekday(), Weekday::tue);
}

// Equality and inequality tests

TEST(EqualityTest, Equal)
{
    Date a{2010, Month::jan, 10};
    Date b{2010, Month::jan, 10};
    ASSERT_EQ(a == b, true);
}

TEST(EqualityTest, NotEqual)
{
    Date a{2010, Month::jan, 10};
    Date b{2020, Month::jan, 10};
    ASSERT_EQ(a != b, true);
}

TEST(EqualityTest, GreaterThan)
{
    Date a{2030, Month::jan, 10};
    Date b{2020, Month::jun, 22};
    ASSERT_EQ(a > b, true);
}

TEST(EqualityTest, LessThan)
{
    Date a{2020, Month::jan, 10};
    Date b{2020, Month::jun, 22};
    ASSERT_EQ(a < b, true);
}

TEST(EqualityTest, GreaterThanOrEqualTo)
{
    Date a{2030, Month::jan, 10};
    Date b{2020, Month::jun, 22};
    ASSERT_EQ(a >= b, true);

    Date c{2010, Month::jan, 10};
    Date d{2010, Month::jan, 10};
    ASSERT_EQ(c >= d, true);
    ASSERT_EQ(d >= c, true);
}

TEST(EqualityTest, LessThanOrEqualTo)
{
    Date a{2020, Month::jan, 10};
    Date b{2020, Month::jun, 22};
    ASSERT_EQ(a <= b, true);

    Date c{2010, Month::jan, 10};
    Date d{2010, Month::jan, 10};
    ASSERT_EQ(c <= d, true);
    ASSERT_EQ(d <= c, true);
}

TEST(DateDifference, GetsDifferenceBetweenDates)
{
    Date a{2010, Month::jan, 1};
    Date b{2010, Month::jan, 2};
    ASSERT_EQ(b - a, 1);

    Date c{2011, Month::jan, 1};
    ASSERT_EQ(c - a, 365);
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}