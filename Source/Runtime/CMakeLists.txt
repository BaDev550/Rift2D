file(GLOB_RECURSE SOURCECODE CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/*.h") 
add_library(Runtime SHARED ${SOURCECODE})

find_package(imgui CONFIG REQUIRED)
find_package(unofficial-shaderc CONFIG REQUIRED)

configure_dll_exports(Runtime)
add_target_defines(Runtime)

target_include_directories(Runtime PUBLIC "${CMAKE_SOURCE_DIR}/Source")
target_link_libraries(Runtime 
    PUBLIC 
        imgui::imgui
    PRIVATE
        Core
        Math
        unofficial::shaderc::shaderc
)