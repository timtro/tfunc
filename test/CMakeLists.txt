macro (create_test target)
  add_test (NAME ${target} COMMAND $<TARGET_FILE:${target}>)
endmacro (create_test)

add_library (CATCH catch.cpp)


set(testname utest-function-composition-and-currying)
    add_executable (${testname} ${testname}.cpp)
    target_link_libraries (${testname} CATCH)
    create_test(${testname})

set(testname utest-monad-list)
    add_executable (${testname} ${testname}.cpp)
    target_link_libraries (${testname} CATCH)
    create_test(${testname})
