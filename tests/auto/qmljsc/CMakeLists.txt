find_package(Qt5 CONFIG REQUIRED Core Test Qml)

function(new_test)
  set(options )
  set(oneValueArgs TEST)
  set(multiValueArgs RESOURCES)

  cmake_parse_arguments(ARGS "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

  include_directories(${Qt5Qml_PRIVATE_INCLUDE_DIRS})

  add_test(NAME ${ARGS_TEST} COMMAND ${ARGS_TEST})
  generate_qrc_file(FILENAME "${ARGS_TEST}_resources.qrc" RESOURCES ${ARGS_RESOURCES} FILEPATH_VAR qrcfilepath)
  qt5_add_resources(ResourceSources ${qrcfilepath})
  add_executable(${ARGS_TEST} ${ARGS_TEST}.cpp ${ResourceSources})
  qt5_use_modules(${ARGS_TEST} Core Qml Test)
  target_link_libraries(${ARGS_TEST} libqmljsc)

endfunction()

new_test(TEST testcompiler)
new_test(TEST testparserpass RESOURCES ../data/minimal/minimal.qml)
new_test(TEST testprettygeneratorpass RESOURCES ../data/minimal/minimal.qml.js)
new_test(TEST testcompilerpipeline RESOURCES ../data/minimal/minimal.qml)
new_test(TEST testmodules RESOURCES ../data/imports/MinimalModule.0.1.js ../data/imports/TestModule.0.1.js)
new_test(TEST testir)
new_test(TEST testpurejavascriptgenerator)
new_test(TEST testpurejavascriptgenerator_integration RESOURCES ../data/javascript/expressions.js ../data/javascript/functions.js)
