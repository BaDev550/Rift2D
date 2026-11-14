function(add_win32_resources target_name icon_path)
    if(WIN32)
        file(TO_CMAKE_PATH "${icon_path}" icon_path_fixed)
        set(RC_FILE_CONTENT "101 ICON \"${icon_path_fixed}\"\n")

        set(GENERATED_RC_PATH "${CMAKE_CURRENT_BINARY_DIR}/${target_name}_autogen.rc")
        file(WRITE "${GENERATED_RC_PATH}" "${RC_FILE_CONTENT}")

        target_sources(${target_name} PRIVATE "${GENERATED_RC_PATH}")
        message(STATUS "Win32 Resource (Icon): ${icon_path_fixed}")
    endif()
endfunction()