cmake_minimum_required(VERSION 3.12)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})

project(print_ip VERSION ${PROJECT_VESRION})

option(WITH_BOOST_TEST "Whether to build Boost test" ON)

add_executable(print_ip main.cpp function.h)

set_target_properties(print_ip PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

if(WITH_BOOST_TEST)
    find_package(Boost COMPONENTS unit_test_framework REQUIRED)
    add_executable(test_print_ip test_print.cpp function.h)

    target_link_libraries(test_print_ip PRIVATE Boost::unit_test_framework)

    set_target_properties(test_print_ip PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
    )

    set_target_properties(test_print_ip PROPERTIES
        COMPILE_DEFINITIONS BOOST_TEST_DYN_LINK
        INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIR}
    )
    target_include_directories(test_print_ip PRIVATE ${Boost_INCLUDE_DIRS})
endif()


set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_CONTACT example@example.com)
include(CPack)

if(WITH_BOOST_TEST)
    enable_testing()
endif()
