#include <boost/filesystem/operations.hpp>
#include <boost/test/tools/old/interface.hpp>
#include <cstdlib>
#include <sstream>
#include <streambuf>

#include "function.h"

#define BOOST_TEST_MODULE test_version
#include <boost/filesystem.hpp>
#include <boost/test/unit_test.hpp>

/**
    \brief Структура для перенаправления стандартного потока std::cout

    Структура позволяет временно перенаправить вывод `std::cout` в строковый буфер

    При создании сохраняет текущий буфер `std::cout`, затем перенаправляет его
    на свой внутренний буфер `buffer`. При уничтожении восстанавливает исходный
    буфер `std::cout`.

*/
struct CoutRedirect {
    std::streambuf *original_buf;
    std::ostringstream buffer;

    CoutRedirect() {
        original_buf = std::cout.rdbuf();
        std::cout.rdbuf(buffer.rdbuf());
    }
    ~CoutRedirect() {
        std::cout.rdbuf(original_buf);
    }

    std::string GetOutput() const {
        return buffer.str();
    }
};

BOOST_AUTO_TEST_SUITE(test_print_ip)

BOOST_AUTO_TEST_CASE(test_tuple_same_type) {
    CoutRedirect capture;

    print_ip(std::make_tuple(123, 456, 789, 0));

    std::string out_str = capture.GetOutput();

    BOOST_TEST(out_str == "123.456.789.0");
}

BOOST_AUTO_TEST_CASE(test_tuple_diff_type) {

    CoutRedirect capture;


    std::tuple container = {1, 2, "3"};
    using container_type =
        std::remove_cv_t<std::remove_reference_t<decltype(container)>>;

    
    if(!tuple_has_only_one_type<container_type>::value) 
        std::cout << "Not same type!"<< std::endl;
    else 
        std::cout << "Same type" << std::endl;

    std::string out_str = capture.GetOutput();

    BOOST_TEST(out_str == "Not same type!\n");
}
BOOST_AUTO_TEST_SUITE_END()