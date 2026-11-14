file(GLOB_RECURSE SOURCECODE CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/*.h") 
add_executable(RiftEditor ${SOURCECODE})
add_reflection(RiftEditor)

find_package(imgui CONFIG REQUIRED)

if (WIN32)
	include(Windows)
	add_win32_resources(RiftEditor "${ENGINE_ICON_PATH}")
endif()

target_include_directories(RiftEditor 
	PRIVATE 
        ${REFLECTION_INCLUDE_PATH} 
		${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(RiftEditor 
	PRIVATE
		Engine
		imgui::imgui
)