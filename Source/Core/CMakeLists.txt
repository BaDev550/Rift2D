file(GLOB_RECURSE SOURCECODE CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/*.h") 
add_library(Core STATIC ${SOURCECODE})

find_package(nlohmann_json REQUIRED)

target_include_directories(Core PRIVATE "${CMAKE_SOURCE_DIR}/Source")
target_link_libraries(Core PRIVATE nlohmann_json::nlohmann_json)