find_package(Qt5 COMPONENTS Core Qml)

set(libqmljsc_srcs
  compiler.cpp
  error.cpp
  compilerpipeline.cpp
  compilerpass.cpp

  ir/ir.cpp
  ir/module.cpp
  ir/file.cpp

  compilerpasses/parserpass.cpp
  compilerpasses/prettygeneratorpass.cpp

  moduleloading/abstractmoduleloader.cpp
  moduleloading/javascriptmoduleloader.cpp
  moduleloading/moduleloading.cpp
)
add_library(libqmljsc SHARED ${libqmljsc_srcs})
qt5_use_modules(libqmljsc Core Qml)

include_directories(${Qt5Qml_PRIVATE_INCLUDE_DIRS})

set(qmljsc_srcs
    main.cpp
)
add_executable(qmljsc ${qmljsc_srcs})

target_link_libraries(qmljsc libqmljsc)
