macro (create_test target)
  add_test (NAME ${target} COMMAND $<TARGET_FILE:${target}>)
endmacro (create_test)

add_library (CATCH catch.cpp)

set(testname utest-test-fixtures)
    add_executable (${testname} ${testname}.cpp)
    target_link_libraries (${testname} CATCH)
    create_test(${testname})

set(testname utest-function-operations)
    add_executable (${testname} ${testname}.cpp)
    target_link_libraries (${testname} CATCH)
    create_test(${testname})

set(testname utest-flist)
    add_executable (${testname} ${testname}.cpp)
    target_link_libraries (${testname} CATCH)
    create_test(${testname})

set(testname utest-foptional)
    add_executable (${testname} ${testname}.cpp)
    target_link_libraries (${testname} CATCH)
    create_test(${testname})

set(testname utest-thesis-examples)
    add_executable (${testname} ${testname}.cpp)
    target_link_libraries (${testname} CATCH)
    create_test(${testname})
