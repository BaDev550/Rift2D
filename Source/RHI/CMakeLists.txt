add_library(RHI INTERFACE)

target_include_directories(RHI INTERFACE "${CMAKE_SOURCE_DIR}/Source/RHI")
target_link_libraries(RHI INTERFACE Math)