#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "SSLL.h"

bool isEqual(const int& i, const int& a) {
    return i == a;
}

struct TestNode {
    int data;
    TestNode() {
        data = 1;
    }
};

TEST_CASE("SSLL should be able to use all of its public methods correctly", "[SSLL]") {
    COP3530::SSLL<int> list;

    SECTION("400 integers inserted using push_front") {
        int i;
        for (i = 0; i < 400; ++i)
            list.push_front(i);
        REQUIRE(list[0] == i - 1);
        REQUIRE(list.size() == 400);

        SECTION("Remove over half of the integers using pop_back") {
            for (int i = 0; i < 300; ++i)
                list.pop_back();
            REQUIRE(list.size() == 100);

            SECTION("300 more integers inserted using push_back") {
                for (int i = 0; i < 300; ++i)
                    list.push_back(i);
                REQUIRE(list.size() == 400);

                SECTION("Remove over half of the integers using pop_front") {
                    for (int i = 0; i < 300; ++i)
                        list.pop_front();
                    REQUIRE(list.size() == 100);

                    SECTION("300 more integers inserted using insert, starting at the end of the array and staying in that position") {
                        for (int i = 0; i < 300; ++i)
                            list.insert(i, 100);
                        REQUIRE(list.size() == 400);
                        REQUIRE(list.item_at(100) == 299);

                        SECTION("Remove over half the integers using remove(pos)") {
                            for (int i = 0; i < 300; ++i)
                                list.remove(99);
                            REQUIRE(list.size() == 100);

                            SECTION("Copy constructor") {
                                list.push_back(999);
                                COP3530::SSLL<int> list2 = list;
                                REQUIRE(list2.item_at(80) == list.item_at(80));
                                REQUIRE(list2.item_at(list2.size() - 1) == 999);

                                SECTION("Contains method") {
                                    REQUIRE(list.contains(999, isEqual));

                                    SECTION("Replace method") {
                                        REQUIRE_FALSE(list.item_at(1) == 900);
                                        list.replace(900, 1);
                                        REQUIRE(list.item_at(1) == 900);

                                        SECTION("Subscript Operator") {
                                            REQUIRE(list[1] == 900);

                                            SECTION("is_empty method") {
                                                size_t mySize = list.size();
                                                REQUIRE_FALSE(list.is_empty());
                                                for (int i = 0; i < mySize; ++i)
                                                    list.pop_back();
                                                REQUIRE(list.is_empty());

                                                SECTION("clear method") {
                                                    for (int i = 0; i < 30; ++i)
                                                        list.push_back(i);
                                                    list.clear();
                                                    REQUIRE(list.size() == 0);
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

TEST_CASE("SSLL should throw exceptions every time, and only when, they are necessary", "[SSLL]") {
    COP3530::SSLL<int> list;

    for (int i = 0; i < 10; ++i) {
        list.push_back(i);
    }
    SECTION("Require no throw on calling insert on one item past the end element") {
        REQUIRE_NOTHROW(list.insert(10, 10));

        SECTION("Require domain exception on calling insert on two items past the end element, and at negative location") {
            REQUIRE_THROWS_AS(list.insert(12, 12), std::domain_error);
            REQUIRE_THROWS_AS(list.insert(12, -5), std::domain_error);
        }
    }

    SECTION("Require no throw on calling remove(int) from the last element when the list is not empty") {
        REQUIRE_NOTHROW(list.remove(9));

        SECTION("Require domain exception on calling remove(int) from anywhere when the list is empty") {
            list.clear();
            REQUIRE_THROWS_AS(list.remove(0), std::domain_error);
            REQUIRE_THROWS_AS(list.remove(-9), std::domain_error);
            REQUIRE_THROWS_AS(list.remove(10), std::domain_error);
        }
    }

    SECTION("Require no throw on calling pop_back or pop_front from the last element when the list is not empty") {
        REQUIRE_NOTHROW(list.pop_back());
        REQUIRE_NOTHROW(list.pop_front());

        SECTION("Require length exception on calling remove(int) from anywhere when the list is empty") {
            list.clear();
            REQUIRE_THROWS_AS(list.pop_back(), std::length_error);
            REQUIRE_THROWS_AS(list.pop_front(), std::length_error);
        }
    }

    SECTION("Require domain error on calling item_at and subscript operators on nonexistent elements") {
        REQUIRE_THROWS_AS(list.item_at(10), std::domain_error);
        REQUIRE_THROWS_AS(list[10], std::domain_error);
        const COP3530::SSLL<int> constList = list;
        REQUIRE_THROWS_AS(constList[10], std::domain_error);
    }
}

TEST_CASE("Test to see if the iterators for SSLL works properly and throws the correct exceptions", "[SSLL]") {
    COP3530::SSLL<int> list;
    COP3530::SSLL<int>::iterator iter = list.begin();
    for (int i = 0; i < 20; ++i)
        list.push_back(i);
    const COP3530::SSLL<int> constList = list;
    COP3530::SSLL<int>::const_iterator constIter = constList.begin();

    SECTION("Check to see if each iterator can traverse the entire list and spit out correct values") {
        for (int i = 0; iter != list.end(); ++i, ++iter) {
            REQUIRE(*iter == list[i]);
        }
        for (int i = 0; constIter != constList.end(); ++i, ++constIter) {
            REQUIRE(*constIter == constList[i]);
        }

        SECTION("See if appropriate errors are thrown in both types") {
            REQUIRE_THROWS_AS(++iter, std::out_of_range);
            REQUIRE_THROWS_AS(*iter, std::out_of_range);
            REQUIRE_THROWS_AS(iter++, std::out_of_range);
            REQUIRE_THROWS_AS(++constIter, std::out_of_range);
            REQUIRE_THROWS_AS(*constIter, std::out_of_range);
            REQUIRE_THROWS_AS(constIter++, std::out_of_range);

            SECTION("Also check to see if the structure dereference is working properly and throws appropriate errors") {
                COP3530::SSLL<TestNode> structList;
                structList.push_back(*(new TestNode()));
                const COP3530::SSLL<TestNode> constStructList = structList;
                COP3530::SSLL<TestNode>::iterator strIter = structList.begin();
                COP3530::SSLL<TestNode>::const_iterator constStrIter = constStructList.begin();
                REQUIRE(strIter->data == 1);
                REQUIRE(constStrIter->data == 1);
                ++strIter;
                ++constStrIter;
                REQUIRE_THROWS_AS(strIter->data, std::out_of_range);
                REQUIRE_THROWS_AS(constStrIter->data, std::out_of_range);
            }
        }
    }
}