if (NOT EXISTS "/home/standa/Desktop/Projekty/sdl2-vulkan-engine/cmake-build-release/install_manifest.txt")
    message(FATAL_ERROR "Cannot find install manifest: \"/home/standa/Desktop/Projekty/sdl2-vulkan-engine/cmake-build-release/install_manifest.txt\"")
endif(NOT EXISTS "/home/standa/Desktop/Projekty/sdl2-vulkan-engine/cmake-build-release/install_manifest.txt")

file(READ "/home/standa/Desktop/Projekty/sdl2-vulkan-engine/cmake-build-release/install_manifest.txt" files)
string(REGEX REPLACE "\n" ";" files "${files}")
foreach (file ${files})
    message(STATUS "Uninstalling \"$ENV{DESTDIR}${file}\"")
    execute_process(
        COMMAND /home/standa/.local/share/JetBrains/Toolbox/apps/CLion/ch-0/223.8617.54/bin/cmake/linux/x64/bin/cmake -E remove "$ENV{DESTDIR}${file}"
        OUTPUT_VARIABLE rm_out
        RESULT_VARIABLE rm_retval
    )
    if(NOT ${rm_retval} EQUAL 0)
        message(FATAL_ERROR "Problem when removing \"$ENV{DESTDIR}${file}\"")
    endif (NOT ${rm_retval} EQUAL 0)
endforeach(file)

