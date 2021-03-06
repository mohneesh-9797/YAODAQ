include(ExternalProject)

if (NOT TARGET magic_enum)
# ----- magic_enum package -----
ExternalProject_Add(
                    magic_enum
                    GIT_REPOSITORY ${magic_enum_repository}
                    GIT_TAG ${magic_enum_version}
                    GIT_PROGRESS TRUE
                    GIT_SHALLOW TRUE
                    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX} -DCMAKE_CXX_STANDARD=${CMAKE_CXX_STANDARD} -DCMAKE_CXX_STANDARD_REQUIRED=${CMAKE_CXX_STANDARD_REQUIRED} -DCMAKE_CXX_EXTENSIONS=${CMAKE_CXX_EXTENSIONS} -DCMAKE_POSITION_INDEPENDENT_CODE=${CMAKE_POSITION_INDEPENDENT_CODE} -DMAGIC_ENUM_OPT_BUILD_EXAMPLES=OFF -DMAGIC_ENUM_OPT_BUILD_TESTS=OFF
                    GIT_SUBMODULES ""
                    PREFIX ${CMAKE_BINARY_DIR}/magic_enum-prefix
                    SOURCE_DIR ${CMAKE_BINARY_DIR}/magic_enum
                    INSTALL_DIR ${CMAKE_INSTALL_PREFIX}
                    )
endif()
