# Copyright (C) 2017, Yeolar

set(RDD_IO_TEST_SRCS
    CompressionTest.cpp
    FileTest.cpp
    FSUtilTest.cpp
    IOBufTest.cpp
)

foreach(test_src ${RDD_IO_TEST_SRCS})
    get_filename_component(test_name ${test_src} NAME_WE)
    set(test rdd_io_${test_name})
    add_executable(${test} ${test_src})
    target_link_libraries(${test}
        ${GTEST_BOTH_LIBRARIES} ${GMOCK_LIBRARIES} rdd_static)
    add_test(${test} ${test} CONFIGURATIONS Debug)
endforeach()
