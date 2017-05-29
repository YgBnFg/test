add_library(UnitTest SHARED "unittest.cpp")

IF (WITH_UNITTEST)
add_custom_command(TARGET UnitTest POST_BUILD
                   COMMAND "UnitTestRunner" "UnitTest"
				   WORKING_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
				   VERBATIM)
ENDIF(WITH_UNITTEST)
target_link_libraries(UnitTest ${GTEST_LIBRARY})