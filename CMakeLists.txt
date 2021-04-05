cmake_minimum_required(VERSION 3.5.0 FATAL_ERROR)


set(CPM_MODULE_NAME chip_atsame5x)

#-----------------------------------------------------------------------
# Required CPM Setup - no need to modify - See: https://github.com/iauns/cpm
#-----------------------------------------------------------------------
set(CPM_LIB_TARGET_NAME ${CPM_MODULE_NAME})

if ((DEFINED CPM_DIR) AND (DEFINED CPM_UNIQUE_ID) AND (DEFINED CPM_TARGET_NAME))
    set(CPM_LIB_TARGET_NAME ${CPM_TARGET_NAME})
    set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CPM_DIR})
    include(CPM)
else()
    set (CPM_DIR "${CMAKE_CURRENT_BINARY_DIR}/cpm-packages" CACHE TYPE STRING)
    find_package(Git)
    if(NOT GIT_FOUND)
        message(FATAL_ERROR "CPM requires Git.")
    endif()
    if (NOT EXISTS ${CPM_DIR}/CPM.cmake)
        message(STATUS "Cloning repo (https://github.com/iauns/cpm)")
        execute_process(
            COMMAND "${GIT_EXECUTABLE}" clone https://github.com/iauns/cpm ${CPM_DIR}
            RESULT_VARIABLE error_code
            OUTPUT_QUIET ERROR_QUIET)
        if(error_code)
            message(FATAL_ERROR "CPM failed to get the hash for HEAD")
        endif()
    endif()
    include(${CPM_DIR}/CPM.cmake)
endif()

#------------------------------------------------------------------------------
# Dependencies
#------------------------------------------------------------------------------

CPM_InitModule(${CPM_MODULE_NAME})

#------------------------------------------------------------------------------
# Project sources / settings
#------------------------------------------------------------------------------
#GLOB_RECURSE?
file(GLOB SOURCES
    ${CPM_MODULE_NAME}/ethernet_phy/*.c
    ${CPM_MODULE_NAME}/hal/src/*.c
    ${CPM_MODULE_NAME}/hal/utils/src/*.c
    ${CPM_MODULE_NAME}/hpl/*/*.c
    ${CPM_MODULE_NAME}/temperature_sensor/*.c
    ${CPM_MODULE_NAME}/temperature_sensor/*/*.c
    )

include_directories("${CPM_MODULE_NAME}"
                    "${CPM_MODULE_NAME}/include"
                    "${CPM_MODULE_NAME}/include/component"
                    "${CPM_MODULE_NAME}/include/instance"
                    "${CPM_MODULE_NAME}/include/pio"
                    "${CPM_MODULE_NAME}/hal/include"
                    "${CPM_MODULE_NAME}/hal/utils/include"
                    "${CPM_MODULE_NAME}/hri"
                    "${CPM_MODULE_NAME}/hpl"
                    "${CPM_MODULE_NAME}/hpl/adc"
                    "${CPM_MODULE_NAME}/hpl/port"
                    "${CPM_MODULE_NAME}/hpl/core"
                    "${CPM_MODULE_NAME}/hpl/gclk"
                    "${CPM_MODULE_NAME}/hpl/pm"
                    "${CPM_MODULE_NAME}/hpl/rtc"
                    "${CPM_MODULE_NAME}/hpl/tc"
                    "${CPM_MODULE_NAME}/hpl/wdt"
                    "${CPM_MODULE_NAME}/CMSIS/Core/Include"
                    "${CPM_MODULE_NAME}/ethernet_phy"
                    "${CPM_MODULE_NAME}/temperature_sensor"
                    "${CPM_MODULE_NAME}/temperature_sensor/at30tse75x")

add_library(${CPM_LIB_TARGET_NAME} ${SOURCES})
target_link_libraries(${CPM_LIB_TARGET_NAME} ${CPM_LIBRARIES})
target_link_libraries(${CPM_LIB_TARGET_NAME} m)

CPM_ExportAdditionalIncludeDir("${CPM_MODULE_NAME}")
CPM_ExportAdditionalIncludeDir("${CPM_MODULE_NAME}/include")
CPM_ExportAdditionalIncludeDir("${CPM_MODULE_NAME}/hal/include")
CPM_ExportAdditionalIncludeDir("${CPM_MODULE_NAME}/hal/utils/include")


CPM_ExportAdditionalIncludeDir("${CPM_MODULE_NAME}"
                    "${CPM_MODULE_NAME}/include"
                    "${CPM_MODULE_NAME}/include/component"
                    "${CPM_MODULE_NAME}/include/instance"
                    "${CPM_MODULE_NAME}/include/pio"
                    "${CPM_MODULE_NAME}/hal/include"
                    "${CPM_MODULE_NAME}/hal/utils/include"
                    "${CPM_MODULE_NAME}/hri"
                    "${CPM_MODULE_NAME}/hpl"
                    "${CPM_MODULE_NAME}/hpl/adc"
                    "${CPM_MODULE_NAME}/hpl/port"
                    "${CPM_MODULE_NAME}/hpl/core"
                    "${CPM_MODULE_NAME}/hpl/gclk"
                    "${CPM_MODULE_NAME}/hpl/pm"
                    "${CPM_MODULE_NAME}/hpl/rtc"
                    "${CPM_MODULE_NAME}/hpl/tc"
                    "${CPM_MODULE_NAME}/hpl/wdt"
                    "${CPM_MODULE_NAME}/CMSIS/Core/Include"
                    "${CPM_MODULE_NAME}/ethernet_phy"
                    "${CPM_MODULE_NAME}/temperature_sensor"
                    "${CPM_MODULE_NAME}/temperature_sensor/at30tse75x")
