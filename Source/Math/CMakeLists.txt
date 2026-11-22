file(GLOB_RECURSE SOURCECODE CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/*.h") 
add_library(Math INTERFACE ${SOURCECODE})

target_include_directories(Math INTERFACE "${CMAKE_SOURCE_DIR}/Source")
target_link_libraries(Math INTERFACE Core)