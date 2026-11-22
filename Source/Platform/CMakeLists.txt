file(GLOB_RECURSE SOURCECODE CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/*.h") 
add_library(Vulkan STATIC ${SOURCECODE})
find_package(Vulkan REQUIRED)
find_package(VulkanMemoryAllocator CONFIG REQUIRED)

target_include_directories(Vulkan PUBLIC "${CMAKE_SOURCE_DIR}/Source")
target_link_libraries(Vulkan 
    PRIVATE
        RHI
        Vulkan::Vulkan
	    GPUOpen::VulkanMemoryAllocator
)