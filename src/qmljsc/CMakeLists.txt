

find_package(Qt5 COMPONENTS Core Declarative)

set(libqmljsc_srcs
)
add_library(libqmljsc SHARED ${libqmljsc_srcs})
qt5_use_modules(libqmljsc Core Qml)


set(qmljsc_srcs
    main.cpp
)
add_executable(qmljsc ${qmljsc_srcs})

target_link_libraries(qmljsc libqmljsc)
