cmake_minimum_required(VERSION 3.7)
project(ex3adt)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES main.c list.h set.h Company.h Company.c mtm_ex3.h Room.h Escaper.h Order.h Escaper.c
        Room.c Order.c EscapeTechnion.h EscapeTechnion.c tests/OrderTests.c tests/test_utilities.h
        tests/RoomTests.c tests/CompanyTests.c tests/EscaperTests.c tests/EscapeTechnionTests.c)
add_executable(ex3adt ${SOURCE_FILES})
target_link_libraries(ex3adt ${CMAKE_SOURCE_DIR}/libmtm.a)