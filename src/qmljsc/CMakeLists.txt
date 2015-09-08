find_package(Qt5 COMPONENTS Core Qml)

set(libqmljsc_srcs
  compiler.cpp
  compilerpipeline.cpp
  compilerpass.cpp
  purejavascriptgenerator.cpp

  ir/objecttree.cpp
  ir/module.cpp
  ir/file.cpp
  ir/typesystem.cpp
  ir/builtintypes.cpp

  compilerpasses/parserpass.cpp
  compilerpasses/prettygeneratorpass.cpp

  moduleloading/moduleloading.cpp
  moduleloading/abstractmoduleloader.cpp
  moduleloading/javascriptmoduleloader.cpp
  moduleloading/qtqmlmoduleloader.cpp

  utils/error.cpp
  utils/shortsymbolname.cpp
)
add_library(libqmljsc SHARED ${libqmljsc_srcs})
qt5_use_modules(libqmljsc Core Qml)

include_directories(${Qt5Qml_PRIVATE_INCLUDE_DIRS})

set(qmljsc_srcs
    main.cpp
)
add_executable(qmljsc ${qmljsc_srcs})

target_link_libraries(qmljsc libqmljsc)
