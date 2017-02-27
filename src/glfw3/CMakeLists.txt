if (MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

if (WIN32)
  set(ICON glfw.rc)
elseif (APPLE)
  set(ICON glfw.icns)
  set_source_files_properties(glfw.icns PROPERTIES MAXOSX_PACKAGE_LOCATION "Resources")
endif()

set(LIBGIZMO_SOURCES_EXAMPLE_GLFW3 main.cpp ArcBall.cpp glut_teapot.c)
add_executable(gizmo_glfw3 WIN32 MACOSX_BUNDLE ${LIBGIZMO_SOURCES_EXAMPLE_GLFW3} ${ICON})
find_package(glfw3 3.2 REQUIRED)
target_link_libraries(gizmo_glfw3 glfw gizmo ${OPENGL_glu_LIBRARY})

set(WINDOWS_BINARIES gizmo_glfw3)

if (MSVC)
    # Tell MSVC to use main instead of WinMain for Windows subsystem executables
    set_target_properties(${WINDOWS_BINARIES} PROPERTIES
                          LINK_FLAGS "/ENTRY:mainCRTStartup")
endif()

if (APPLE)
    set_target_properties(gizmo_glfw3 PROPERTIES MACOSX_BUNDLE_BUNDLE_NAME "GizmoGlfw3")

    set_target_properties(${WINDOWS_BINARIES} PROPERTIES
                          RESOURCE glfw.icns
                          MACOSX_BUNDLE_SHORT_VERSION_STRING ${GLFW_VERSION}
                          MACOSX_BUNDLE_LONG_VERSION_STRING ${GLFW_VERSION_FULL}
                          MACOSX_BUNDLE_ICON_FILE glfw.icns
                          MACOSX_BUNDLE_INFO_PLIST "MacOSXBundleInfo.plist.in")
endif()
