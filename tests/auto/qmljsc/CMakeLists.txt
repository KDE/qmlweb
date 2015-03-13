find_package(Qt5 ${REQUIRED_QT_VERSION} CONFIG REQUIRED Test)

macro(executable_tests)
  foreach(_testname ${ARGN})
    add_executable(${_testname} ${_testname}.cpp)
    target_link_libraries(${_testname} Qt5::Test libqmljsc)
  endforeach(_testname)
endmacro()

executable_tests(
  
)
