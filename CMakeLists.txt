cmake_minimum_required(VERSION 2.8.9)
project(gizmo)

set(libgizmo_sources_base_dir "${CMAKE_CURRENT_SOURCE_DIR}/src/libgizmo")
set(libgizmo_sources_glloadgen_dir "${CMAKE_CURRENT_SOURCE_DIR}/src/glloadgen")
set(LIBGIZMO_SOURCES_CORE
    ${CMAKE_CURRENT_SOURCE_DIR}/inc/IGizmo.h
    ${libgizmo_sources_base_dir}/GizmoTransform.h
    ${libgizmo_sources_base_dir}/GizmoTransformMove.cpp
    ${libgizmo_sources_base_dir}/GizmoTransformMove.h
    ${libgizmo_sources_base_dir}/GizmoTransformRender.cpp
    ${libgizmo_sources_base_dir}/GizmoTransformRender.h
    ${libgizmo_sources_base_dir}/GizmoTransformRotate.cpp
    ${libgizmo_sources_base_dir}/GizmoTransformRotate.h
    ${libgizmo_sources_base_dir}/GizmoTransformScale.cpp
    ${libgizmo_sources_base_dir}/GizmoTransformScale.h
    ${libgizmo_sources_base_dir}/LibBase.h
    ${libgizmo_sources_base_dir}/ZBaseDefs.h
    ${libgizmo_sources_base_dir}/ZBaseMaths.cpp
    ${libgizmo_sources_base_dir}/ZBaseMaths.h
    ${libgizmo_sources_base_dir}/ZBaseMaths.inl
    ${libgizmo_sources_base_dir}/ZCollisionsUtils.h
    ${libgizmo_sources_base_dir}/ZMathsFunc.cpp
    ${libgizmo_sources_base_dir}/ZMathsFunc.h
    ${libgizmo_sources_base_dir}/stdafx.h
    ${libgizmo_sources_glloadgen_dir}/gl_gizmo.c
    ${libgizmo_sources_glloadgen_dir}/gl_gizmo.h
)

add_library(${PROJECT_NAME} ${LIBGIZMO_SOURCES_CORE})
include_directories(${libgizmo_sources_glloadgen_dir})

find_package(OpenGL REQUIRED)
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/inc" ${OPENGL_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} ${OPENGL_gl_LIBRARY})

if(EXAMPLE_GLFW)
  set(LIBGIZMO_SOURCES_EXAMPLE_GLFW ${CMAKE_CURRENT_SOURCE_DIR}/src/glfw/main.cpp)
  add_executable(gizmo_glfw ${LIBGIZMO_SOURCES_EXAMPLE_GLFW})
  find_library(GLFW_LIBRARY glfw)
  target_link_libraries(gizmo_glfw ${GLFW_LIBRARY} ${PROJECT_NAME})
endif()
