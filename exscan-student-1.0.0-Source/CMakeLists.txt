
cmake_minimum_required(VERSION 3.0)
   
project(exscan LANGUAGES C)

set(SRC_DIR "${CMAKE_SOURCE_DIR}/src")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}")

find_package(OpenMP REQUIRED)

if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -Wall")

option(ENABLE_ADMIN "enable admin mode" off)

if(ENABLE_ADMIN)
    set(EXSCAN_CODE ${SRC_DIR}/exscan_solution.c)
else(ENABLE_ADMIN)
    set(EXSCAN_CODE ${SRC_DIR}/exscan.c)
endif(ENABLE_ADMIN)

add_executable(exscan_tester
        ${SRC_DIR}/exscan_tester.c
        ${EXSCAN_CODE}
)

target_link_libraries(exscan_tester m)


set(CPACK_PACKAGE_VERSION_MAJOR "1")
set(CPACK_PACKAGE_VERSION_MINOR "0")
set(CPACK_PACKAGE_VERSION_PATCH "0")
set(CPACK_SOURCE_GENERATOR "TBZ2")

set(MY_IGNORE_FILES
"/CMakeFiles/"
"/_CPack_Packages/"
"/bin/"
"/Makefile"
"\\\\.tar\\\\.bz2$"
"/\\\\.git/"
"/\\\\.DS_Store"
".project"
"cmake_install\\\\.cmake"
"CPackConfig\\\\.cmake"
"CPackSourceConfig\\\\.cmake"
"CMakeCache.txt"
"${CPACK_SOURCE_IGNORE_FILES}"
)


if(ENABLE_ADMIN)

SET(CPACK_SOURCE_IGNORE_FILES
${MY_IGNORE_FILES}
)

set(CPACK_PACKAGE_NAME "exscan-admin")

else(ENABLE_ADMIN)

SET(CPACK_SOURCE_IGNORE_FILES
${MY_IGNORE_FILES}
"/src/.*_solution.c"
)

set(CPACK_PACKAGE_NAME "exscan-student")

endif(ENABLE_ADMIN)


INCLUDE(CPack)



