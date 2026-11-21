file(GLOB_RECURSE SOURCECODE CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/*.h") 
add_library(RHI STATIC ${SOURCECODE})

target_include_directories(RHI PUBLIC "${CMAKE_SOURCE_DIR}/Source")
target_link_libraries(RHI
    PRIVATE
        Runtime
)