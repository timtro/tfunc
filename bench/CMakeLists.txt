add_executable (function-composition-and-piping function-composition-and-piping.cpp)
target_link_libraries (function-composition-and-piping -lbenchmark -lpthread)

add_executable (function-currying function-currying.cpp)
target_link_libraries (function-currying -lbenchmark -lpthread)
