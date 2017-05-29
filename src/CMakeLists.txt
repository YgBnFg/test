add_executable(test test.cpp)

target_link_libraries(test ${Boost_LIBRARIES})

add_executable(jsontest jsontest.cpp)

target_link_libraries(jsontest ${JSONCPP_LIBRARY})

add_executable(c++11 c++11.cpp)
add_executable(testqt qt.cpp)
target_link_libraries(testqt ${QT_LIBRARIES})
#add_executable(thread thread.cpp)
