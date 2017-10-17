add_executable(test test.cpp)

target_link_libraries(test ${Boost_LIBRARIES} ${OpenCV_LIBS})

add_executable(jsontest jsontest.cpp)

target_link_libraries(jsontest ${JSONCPP_LIBRARY})

add_executable(c++11 c++11.cpp)
add_executable(testqt qt.cpp)
target_link_libraries(testqt ${QT_LIBRARIES} )
#add_executable(thread thread.cpp)

add_executable(testDshow testDshow.cpp testDshow.h CameraDS.h CameraDS.cpp)
target_link_libraries(testDshow ${Boost_LIBRARIES} ${OpenCV_LIBS})