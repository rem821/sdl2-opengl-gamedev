# Install script for directory: /home/standa/Desktop/Projekty/sdl2-vulkan-engine/libs/sdl

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/bin/objdump")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES
    "/home/standa/Desktop/Projekty/sdl2-vulkan-engine/cmake-build-release/libs/sdl/libSDL2-2.0.so.0.2301.0"
    "/home/standa/Desktop/Projekty/sdl2-vulkan-engine/cmake-build-release/libs/sdl/libSDL2-2.0.so.0"
    )
  foreach(file
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libSDL2-2.0.so.0.2301.0"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libSDL2-2.0.so.0"
      )
    if(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      if(CMAKE_INSTALL_DO_STRIP)
        execute_process(COMMAND "/usr/bin/strip" "${file}")
      endif()
    endif()
  endforeach()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/home/standa/Desktop/Projekty/sdl2-vulkan-engine/cmake-build-release/libs/sdl/libSDL2-2.0.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libSDL2-2.0.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libSDL2-2.0.so")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libSDL2-2.0.so")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/home/standa/Desktop/Projekty/sdl2-vulkan-engine/cmake-build-release/libs/sdl/libSDL2main.a")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/home/standa/Desktop/Projekty/sdl2-vulkan-engine/cmake-build-release/libs/sdl/libSDL2.a")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/home/standa/Desktop/Projekty/sdl2-vulkan-engine/cmake-build-release/libs/sdl/libSDL2_test.a")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/SDL2/SDL2Targets.cmake")
    file(DIFFERENT _cmake_export_file_changed FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/SDL2/SDL2Targets.cmake"
         "/home/standa/Desktop/Projekty/sdl2-vulkan-engine/cmake-build-release/libs/sdl/CMakeFiles/Export/f084604df1a27ef5b4fef7c7544737d1/SDL2Targets.cmake")
    if(_cmake_export_file_changed)
      file(GLOB _cmake_old_config_files "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/SDL2/SDL2Targets-*.cmake")
      if(_cmake_old_config_files)
        string(REPLACE ";" ", " _cmake_old_config_files_text "${_cmake_old_config_files}")
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/SDL2/SDL2Targets.cmake\" will be replaced.  Removing files [${_cmake_old_config_files_text}].")
        unset(_cmake_old_config_files_text)
        file(REMOVE ${_cmake_old_config_files})
      endif()
      unset(_cmake_old_config_files)
    endif()
    unset(_cmake_export_file_changed)
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/SDL2" TYPE FILE FILES "/home/standa/Desktop/Projekty/sdl2-vulkan-engine/cmake-build-release/libs/sdl/CMakeFiles/Export/f084604df1a27ef5b4fef7c7544737d1/SDL2Targets.cmake")
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/SDL2" TYPE FILE FILES "/home/standa/Desktop/Projekty/sdl2-vulkan-engine/cmake-build-release/libs/sdl/CMakeFiles/Export/f084604df1a27ef5b4fef7c7544737d1/SDL2Targets-release.cmake")
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/SDL2/SDL2mainTargets.cmake")
    file(DIFFERENT _cmake_export_file_changed FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/SDL2/SDL2mainTargets.cmake"
         "/home/standa/Desktop/Projekty/sdl2-vulkan-engine/cmake-build-release/libs/sdl/CMakeFiles/Export/f084604df1a27ef5b4fef7c7544737d1/SDL2mainTargets.cmake")
    if(_cmake_export_file_changed)
      file(GLOB _cmake_old_config_files "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/SDL2/SDL2mainTargets-*.cmake")
      if(_cmake_old_config_files)
        string(REPLACE ";" ", " _cmake_old_config_files_text "${_cmake_old_config_files}")
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/SDL2/SDL2mainTargets.cmake\" will be replaced.  Removing files [${_cmake_old_config_files_text}].")
        unset(_cmake_old_config_files_text)
        file(REMOVE ${_cmake_old_config_files})
      endif()
      unset(_cmake_old_config_files)
    endif()
    unset(_cmake_export_file_changed)
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/SDL2" TYPE FILE FILES "/home/standa/Desktop/Projekty/sdl2-vulkan-engine/cmake-build-release/libs/sdl/CMakeFiles/Export/f084604df1a27ef5b4fef7c7544737d1/SDL2mainTargets.cmake")
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/SDL2" TYPE FILE FILES "/home/standa/Desktop/Projekty/sdl2-vulkan-engine/cmake-build-release/libs/sdl/CMakeFiles/Export/f084604df1a27ef5b4fef7c7544737d1/SDL2mainTargets-release.cmake")
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/SDL2/SDL2staticTargets.cmake")
    file(DIFFERENT _cmake_export_file_changed FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/SDL2/SDL2staticTargets.cmake"
         "/home/standa/Desktop/Projekty/sdl2-vulkan-engine/cmake-build-release/libs/sdl/CMakeFiles/Export/f084604df1a27ef5b4fef7c7544737d1/SDL2staticTargets.cmake")
    if(_cmake_export_file_changed)
      file(GLOB _cmake_old_config_files "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/SDL2/SDL2staticTargets-*.cmake")
      if(_cmake_old_config_files)
        string(REPLACE ";" ", " _cmake_old_config_files_text "${_cmake_old_config_files}")
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/SDL2/SDL2staticTargets.cmake\" will be replaced.  Removing files [${_cmake_old_config_files_text}].")
        unset(_cmake_old_config_files_text)
        file(REMOVE ${_cmake_old_config_files})
      endif()
      unset(_cmake_old_config_files)
    endif()
    unset(_cmake_export_file_changed)
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/SDL2" TYPE FILE FILES "/home/standa/Desktop/Projekty/sdl2-vulkan-engine/cmake-build-release/libs/sdl/CMakeFiles/Export/f084604df1a27ef5b4fef7c7544737d1/SDL2staticTargets.cmake")
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/SDL2" TYPE FILE FILES "/home/standa/Desktop/Projekty/sdl2-vulkan-engine/cmake-build-release/libs/sdl/CMakeFiles/Export/f084604df1a27ef5b4fef7c7544737d1/SDL2staticTargets-release.cmake")
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/SDL2/SDL2testTargets.cmake")
    file(DIFFERENT _cmake_export_file_changed FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/SDL2/SDL2testTargets.cmake"
         "/home/standa/Desktop/Projekty/sdl2-vulkan-engine/cmake-build-release/libs/sdl/CMakeFiles/Export/f084604df1a27ef5b4fef7c7544737d1/SDL2testTargets.cmake")
    if(_cmake_export_file_changed)
      file(GLOB _cmake_old_config_files "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/SDL2/SDL2testTargets-*.cmake")
      if(_cmake_old_config_files)
        string(REPLACE ";" ", " _cmake_old_config_files_text "${_cmake_old_config_files}")
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/SDL2/SDL2testTargets.cmake\" will be replaced.  Removing files [${_cmake_old_config_files_text}].")
        unset(_cmake_old_config_files_text)
        file(REMOVE ${_cmake_old_config_files})
      endif()
      unset(_cmake_old_config_files)
    endif()
    unset(_cmake_export_file_changed)
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/SDL2" TYPE FILE FILES "/home/standa/Desktop/Projekty/sdl2-vulkan-engine/cmake-build-release/libs/sdl/CMakeFiles/Export/f084604df1a27ef5b4fef7c7544737d1/SDL2testTargets.cmake")
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/SDL2" TYPE FILE FILES "/home/standa/Desktop/Projekty/sdl2-vulkan-engine/cmake-build-release/libs/sdl/CMakeFiles/Export/f084604df1a27ef5b4fef7c7544737d1/SDL2testTargets-release.cmake")
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Devel" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/SDL2" TYPE FILE FILES
    "/home/standa/Desktop/Projekty/sdl2-vulkan-engine/cmake-build-release/libs/sdl/SDL2Config.cmake"
    "/home/standa/Desktop/Projekty/sdl2-vulkan-engine/cmake-build-release/libs/sdl/SDL2ConfigVersion.cmake"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/SDL2" TYPE FILE FILES
    "/home/standa/Desktop/Projekty/sdl2-vulkan-engine/libs/sdl/include/SDL.h"
    "/home/standa/Desktop/Projekty/sdl2-vulkan-engine/libs/sdl/include/SDL_assert.h"
    "/home/standa/Desktop/Projekty/sdl2-vulkan-engine/libs/sdl/include/SDL_atomic.h"
    "/home/standa/Desktop/Projekty/sdl2-vulkan-engine/libs/sdl/include/SDL_audio.h"
    "/home/standa/Desktop/Projekty/sdl2-vulkan-engine/libs/sdl/include/SDL_bits.h"
    "/home/standa/Desktop/Projekty/sdl2-vulkan-engine/libs/sdl/include/SDL_blendmode.h"
    "/home/standa/Desktop/Projekty/sdl2-vulkan-engine/libs/sdl/include/SDL_clipboard.h"
    "/home/standa/Desktop/Projekty/sdl2-vulkan-engine/libs/sdl/include/SDL_copying.h"
    "/home/standa/Desktop/Projekty/sdl2-vulkan-engine/libs/sdl/include/SDL_cpuinfo.h"
    "/home/standa/Desktop/Projekty/sdl2-vulkan-engine/libs/sdl/include/SDL_egl.h"
    "/home/standa/Desktop/Projekty/sdl2-vulkan-engine/libs/sdl/include/SDL_endian.h"
    "/home/standa/Desktop/Projekty/sdl2-vulkan-engine/libs/sdl/include/SDL_error.h"
    "/home/standa/Desktop/Projekty/sdl2-vulkan-engine/libs/sdl/include/SDL_events.h"
    "/home/standa/Desktop/Projekty/sdl2-vulkan-engine/libs/sdl/include/SDL_filesystem.h"
    "/home/standa/Desktop/Projekty/sdl2-vulkan-engine/libs/sdl/include/SDL_gamecontroller.h"
    "/home/standa/Desktop/Projekty/sdl2-vulkan-engine/libs/sdl/include/SDL_gesture.h"
    "/home/standa/Desktop/Projekty/sdl2-vulkan-engine/libs/sdl/include/SDL_guid.h"
    "/home/standa/Desktop/Projekty/sdl2-vulkan-engine/libs/sdl/include/SDL_haptic.h"
    "/home/standa/Desktop/Projekty/sdl2-vulkan-engine/libs/sdl/include/SDL_hidapi.h"
    "/home/standa/Desktop/Projekty/sdl2-vulkan-engine/libs/sdl/include/SDL_hints.h"
    "/home/standa/Desktop/Projekty/sdl2-vulkan-engine/libs/sdl/include/SDL_joystick.h"
    "/home/standa/Desktop/Projekty/sdl2-vulkan-engine/libs/sdl/include/SDL_keyboard.h"
    "/home/standa/Desktop/Projekty/sdl2-vulkan-engine/libs/sdl/include/SDL_keycode.h"
    "/home/standa/Desktop/Projekty/sdl2-vulkan-engine/libs/sdl/include/SDL_loadso.h"
    "/home/standa/Desktop/Projekty/sdl2-vulkan-engine/libs/sdl/include/SDL_locale.h"
    "/home/standa/Desktop/Projekty/sdl2-vulkan-engine/libs/sdl/include/SDL_log.h"
    "/home/standa/Desktop/Projekty/sdl2-vulkan-engine/libs/sdl/include/SDL_main.h"
    "/home/standa/Desktop/Projekty/sdl2-vulkan-engine/libs/sdl/include/SDL_messagebox.h"
    "/home/standa/Desktop/Projekty/sdl2-vulkan-engine/libs/sdl/include/SDL_metal.h"
    "/home/standa/Desktop/Projekty/sdl2-vulkan-engine/libs/sdl/include/SDL_misc.h"
    "/home/standa/Desktop/Projekty/sdl2-vulkan-engine/libs/sdl/include/SDL_mouse.h"
    "/home/standa/Desktop/Projekty/sdl2-vulkan-engine/libs/sdl/include/SDL_mutex.h"
    "/home/standa/Desktop/Projekty/sdl2-vulkan-engine/libs/sdl/include/SDL_name.h"
    "/home/standa/Desktop/Projekty/sdl2-vulkan-engine/libs/sdl/include/SDL_opengl.h"
    "/home/standa/Desktop/Projekty/sdl2-vulkan-engine/libs/sdl/include/SDL_opengl_glext.h"
    "/home/standa/Desktop/Projekty/sdl2-vulkan-engine/libs/sdl/include/SDL_opengles.h"
    "/home/standa/Desktop/Projekty/sdl2-vulkan-engine/libs/sdl/include/SDL_opengles2.h"
    "/home/standa/Desktop/Projekty/sdl2-vulkan-engine/libs/sdl/include/SDL_opengles2_gl2.h"
    "/home/standa/Desktop/Projekty/sdl2-vulkan-engine/libs/sdl/include/SDL_opengles2_gl2ext.h"
    "/home/standa/Desktop/Projekty/sdl2-vulkan-engine/libs/sdl/include/SDL_opengles2_gl2platform.h"
    "/home/standa/Desktop/Projekty/sdl2-vulkan-engine/libs/sdl/include/SDL_opengles2_khrplatform.h"
    "/home/standa/Desktop/Projekty/sdl2-vulkan-engine/libs/sdl/include/SDL_pixels.h"
    "/home/standa/Desktop/Projekty/sdl2-vulkan-engine/libs/sdl/include/SDL_platform.h"
    "/home/standa/Desktop/Projekty/sdl2-vulkan-engine/libs/sdl/include/SDL_power.h"
    "/home/standa/Desktop/Projekty/sdl2-vulkan-engine/libs/sdl/include/SDL_quit.h"
    "/home/standa/Desktop/Projekty/sdl2-vulkan-engine/libs/sdl/include/SDL_rect.h"
    "/home/standa/Desktop/Projekty/sdl2-vulkan-engine/libs/sdl/include/SDL_render.h"
    "/home/standa/Desktop/Projekty/sdl2-vulkan-engine/libs/sdl/include/SDL_rwops.h"
    "/home/standa/Desktop/Projekty/sdl2-vulkan-engine/libs/sdl/include/SDL_scancode.h"
    "/home/standa/Desktop/Projekty/sdl2-vulkan-engine/libs/sdl/include/SDL_sensor.h"
    "/home/standa/Desktop/Projekty/sdl2-vulkan-engine/libs/sdl/include/SDL_shape.h"
    "/home/standa/Desktop/Projekty/sdl2-vulkan-engine/libs/sdl/include/SDL_stdinc.h"
    "/home/standa/Desktop/Projekty/sdl2-vulkan-engine/libs/sdl/include/SDL_surface.h"
    "/home/standa/Desktop/Projekty/sdl2-vulkan-engine/libs/sdl/include/SDL_system.h"
    "/home/standa/Desktop/Projekty/sdl2-vulkan-engine/libs/sdl/include/SDL_syswm.h"
    "/home/standa/Desktop/Projekty/sdl2-vulkan-engine/libs/sdl/include/SDL_test.h"
    "/home/standa/Desktop/Projekty/sdl2-vulkan-engine/libs/sdl/include/SDL_test_assert.h"
    "/home/standa/Desktop/Projekty/sdl2-vulkan-engine/libs/sdl/include/SDL_test_common.h"
    "/home/standa/Desktop/Projekty/sdl2-vulkan-engine/libs/sdl/include/SDL_test_compare.h"
    "/home/standa/Desktop/Projekty/sdl2-vulkan-engine/libs/sdl/include/SDL_test_crc32.h"
    "/home/standa/Desktop/Projekty/sdl2-vulkan-engine/libs/sdl/include/SDL_test_font.h"
    "/home/standa/Desktop/Projekty/sdl2-vulkan-engine/libs/sdl/include/SDL_test_fuzzer.h"
    "/home/standa/Desktop/Projekty/sdl2-vulkan-engine/libs/sdl/include/SDL_test_harness.h"
    "/home/standa/Desktop/Projekty/sdl2-vulkan-engine/libs/sdl/include/SDL_test_images.h"
    "/home/standa/Desktop/Projekty/sdl2-vulkan-engine/libs/sdl/include/SDL_test_log.h"
    "/home/standa/Desktop/Projekty/sdl2-vulkan-engine/libs/sdl/include/SDL_test_md5.h"
    "/home/standa/Desktop/Projekty/sdl2-vulkan-engine/libs/sdl/include/SDL_test_memory.h"
    "/home/standa/Desktop/Projekty/sdl2-vulkan-engine/libs/sdl/include/SDL_test_random.h"
    "/home/standa/Desktop/Projekty/sdl2-vulkan-engine/libs/sdl/include/SDL_thread.h"
    "/home/standa/Desktop/Projekty/sdl2-vulkan-engine/libs/sdl/include/SDL_timer.h"
    "/home/standa/Desktop/Projekty/sdl2-vulkan-engine/libs/sdl/include/SDL_touch.h"
    "/home/standa/Desktop/Projekty/sdl2-vulkan-engine/libs/sdl/include/SDL_types.h"
    "/home/standa/Desktop/Projekty/sdl2-vulkan-engine/libs/sdl/include/SDL_version.h"
    "/home/standa/Desktop/Projekty/sdl2-vulkan-engine/libs/sdl/include/SDL_video.h"
    "/home/standa/Desktop/Projekty/sdl2-vulkan-engine/libs/sdl/include/SDL_vulkan.h"
    "/home/standa/Desktop/Projekty/sdl2-vulkan-engine/libs/sdl/include/begin_code.h"
    "/home/standa/Desktop/Projekty/sdl2-vulkan-engine/libs/sdl/include/close_code.h"
    "/home/standa/Desktop/Projekty/sdl2-vulkan-engine/cmake-build-release/libs/sdl/include/SDL_revision.h"
    "/home/standa/Desktop/Projekty/sdl2-vulkan-engine/cmake-build-release/libs/sdl/include-config-release/SDL_config.h"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  
            execute_process(COMMAND /home/standa/.local/share/JetBrains/Toolbox/apps/CLion/ch-0/223.8617.54/bin/cmake/linux/x64/bin/cmake -E create_symlink
              "libSDL2-2.0.so" "libSDL2.so"
              WORKING_DIRECTORY "/home/standa/Desktop/Projekty/sdl2-vulkan-engine/cmake-build-release/libs/sdl")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE FILES "/home/standa/Desktop/Projekty/sdl2-vulkan-engine/cmake-build-release/libs/sdl/libSDL2.so")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/pkgconfig" TYPE FILE FILES "/home/standa/Desktop/Projekty/sdl2-vulkan-engine/cmake-build-release/libs/sdl/sdl2.pc")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE PROGRAM FILES "/home/standa/Desktop/Projekty/sdl2-vulkan-engine/cmake-build-release/libs/sdl/sdl2-config")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/aclocal" TYPE FILE FILES "/home/standa/Desktop/Projekty/sdl2-vulkan-engine/libs/sdl/sdl2.m4")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/licenses/SDL2" TYPE FILE FILES "/home/standa/Desktop/Projekty/sdl2-vulkan-engine/libs/sdl/LICENSE.txt")
endif()

