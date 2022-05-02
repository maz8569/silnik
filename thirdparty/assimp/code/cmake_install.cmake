# Install script for directory: E:/prog/OpenGLPAG/OpenGLPAG/thirdparty/assimp/code

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/OpenGLPAG")
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

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "E:/prog/OpenGLPAG/OpenGLPAG/Build/thirdparty/assimp/code/Debug/assimp-vc140-mt.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "E:/prog/OpenGLPAG/OpenGLPAG/Build/thirdparty/assimp/code/Release/assimp-vc140-mt.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "E:/prog/OpenGLPAG/OpenGLPAG/Build/thirdparty/assimp/code/MinSizeRel/assimp-vc140-mt.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "E:/prog/OpenGLPAG/OpenGLPAG/Build/thirdparty/assimp/code/RelWithDebInfo/assimp-vc140-mt.lib")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xassimp-devx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/assimp" TYPE FILE FILES
    "E:/prog/OpenGLPAG/OpenGLPAG/thirdparty/assimp/code/../include/assimp/anim.h"
    "E:/prog/OpenGLPAG/OpenGLPAG/thirdparty/assimp/code/../include/assimp/ai_assert.h"
    "E:/prog/OpenGLPAG/OpenGLPAG/thirdparty/assimp/code/../include/assimp/camera.h"
    "E:/prog/OpenGLPAG/OpenGLPAG/thirdparty/assimp/code/../include/assimp/color4.h"
    "E:/prog/OpenGLPAG/OpenGLPAG/thirdparty/assimp/code/../include/assimp/color4.inl"
    "E:/prog/OpenGLPAG/OpenGLPAG/Build/thirdparty/assimp/code/../include/assimp/config.h"
    "E:/prog/OpenGLPAG/OpenGLPAG/thirdparty/assimp/code/../include/assimp/defs.h"
    "E:/prog/OpenGLPAG/OpenGLPAG/thirdparty/assimp/code/../include/assimp/Defines.h"
    "E:/prog/OpenGLPAG/OpenGLPAG/thirdparty/assimp/code/../include/assimp/cfileio.h"
    "E:/prog/OpenGLPAG/OpenGLPAG/thirdparty/assimp/code/../include/assimp/light.h"
    "E:/prog/OpenGLPAG/OpenGLPAG/thirdparty/assimp/code/../include/assimp/material.h"
    "E:/prog/OpenGLPAG/OpenGLPAG/thirdparty/assimp/code/../include/assimp/material.inl"
    "E:/prog/OpenGLPAG/OpenGLPAG/thirdparty/assimp/code/../include/assimp/matrix3x3.h"
    "E:/prog/OpenGLPAG/OpenGLPAG/thirdparty/assimp/code/../include/assimp/matrix3x3.inl"
    "E:/prog/OpenGLPAG/OpenGLPAG/thirdparty/assimp/code/../include/assimp/matrix4x4.h"
    "E:/prog/OpenGLPAG/OpenGLPAG/thirdparty/assimp/code/../include/assimp/matrix4x4.inl"
    "E:/prog/OpenGLPAG/OpenGLPAG/thirdparty/assimp/code/../include/assimp/mesh.h"
    "E:/prog/OpenGLPAG/OpenGLPAG/thirdparty/assimp/code/../include/assimp/pbrmaterial.h"
    "E:/prog/OpenGLPAG/OpenGLPAG/thirdparty/assimp/code/../include/assimp/postprocess.h"
    "E:/prog/OpenGLPAG/OpenGLPAG/thirdparty/assimp/code/../include/assimp/quaternion.h"
    "E:/prog/OpenGLPAG/OpenGLPAG/thirdparty/assimp/code/../include/assimp/quaternion.inl"
    "E:/prog/OpenGLPAG/OpenGLPAG/thirdparty/assimp/code/../include/assimp/scene.h"
    "E:/prog/OpenGLPAG/OpenGLPAG/thirdparty/assimp/code/../include/assimp/metadata.h"
    "E:/prog/OpenGLPAG/OpenGLPAG/thirdparty/assimp/code/../include/assimp/texture.h"
    "E:/prog/OpenGLPAG/OpenGLPAG/thirdparty/assimp/code/../include/assimp/types.h"
    "E:/prog/OpenGLPAG/OpenGLPAG/thirdparty/assimp/code/../include/assimp/vector2.h"
    "E:/prog/OpenGLPAG/OpenGLPAG/thirdparty/assimp/code/../include/assimp/vector2.inl"
    "E:/prog/OpenGLPAG/OpenGLPAG/thirdparty/assimp/code/../include/assimp/vector3.h"
    "E:/prog/OpenGLPAG/OpenGLPAG/thirdparty/assimp/code/../include/assimp/vector3.inl"
    "E:/prog/OpenGLPAG/OpenGLPAG/thirdparty/assimp/code/../include/assimp/version.h"
    "E:/prog/OpenGLPAG/OpenGLPAG/thirdparty/assimp/code/../include/assimp/cimport.h"
    "E:/prog/OpenGLPAG/OpenGLPAG/thirdparty/assimp/code/../include/assimp/importerdesc.h"
    "E:/prog/OpenGLPAG/OpenGLPAG/thirdparty/assimp/code/../include/assimp/Importer.hpp"
    "E:/prog/OpenGLPAG/OpenGLPAG/thirdparty/assimp/code/../include/assimp/DefaultLogger.hpp"
    "E:/prog/OpenGLPAG/OpenGLPAG/thirdparty/assimp/code/../include/assimp/ProgressHandler.hpp"
    "E:/prog/OpenGLPAG/OpenGLPAG/thirdparty/assimp/code/../include/assimp/IOStream.hpp"
    "E:/prog/OpenGLPAG/OpenGLPAG/thirdparty/assimp/code/../include/assimp/IOSystem.hpp"
    "E:/prog/OpenGLPAG/OpenGLPAG/thirdparty/assimp/code/../include/assimp/Logger.hpp"
    "E:/prog/OpenGLPAG/OpenGLPAG/thirdparty/assimp/code/../include/assimp/LogStream.hpp"
    "E:/prog/OpenGLPAG/OpenGLPAG/thirdparty/assimp/code/../include/assimp/NullLogger.hpp"
    "E:/prog/OpenGLPAG/OpenGLPAG/thirdparty/assimp/code/../include/assimp/cexport.h"
    "E:/prog/OpenGLPAG/OpenGLPAG/thirdparty/assimp/code/../include/assimp/Exporter.hpp"
    "E:/prog/OpenGLPAG/OpenGLPAG/thirdparty/assimp/code/../include/assimp/DefaultIOStream.h"
    "E:/prog/OpenGLPAG/OpenGLPAG/thirdparty/assimp/code/../include/assimp/DefaultIOSystem.h"
    "E:/prog/OpenGLPAG/OpenGLPAG/thirdparty/assimp/code/../include/assimp/SceneCombiner.h"
    "E:/prog/OpenGLPAG/OpenGLPAG/thirdparty/assimp/code/../include/assimp/fast_atof.h"
    "E:/prog/OpenGLPAG/OpenGLPAG/thirdparty/assimp/code/../include/assimp/qnan.h"
    "E:/prog/OpenGLPAG/OpenGLPAG/thirdparty/assimp/code/../include/assimp/BaseImporter.h"
    "E:/prog/OpenGLPAG/OpenGLPAG/thirdparty/assimp/code/../include/assimp/Hash.h"
    "E:/prog/OpenGLPAG/OpenGLPAG/thirdparty/assimp/code/../include/assimp/MemoryIOWrapper.h"
    "E:/prog/OpenGLPAG/OpenGLPAG/thirdparty/assimp/code/../include/assimp/ParsingUtils.h"
    "E:/prog/OpenGLPAG/OpenGLPAG/thirdparty/assimp/code/../include/assimp/StreamReader.h"
    "E:/prog/OpenGLPAG/OpenGLPAG/thirdparty/assimp/code/../include/assimp/StreamWriter.h"
    "E:/prog/OpenGLPAG/OpenGLPAG/thirdparty/assimp/code/../include/assimp/StringComparison.h"
    "E:/prog/OpenGLPAG/OpenGLPAG/thirdparty/assimp/code/../include/assimp/StringUtils.h"
    "E:/prog/OpenGLPAG/OpenGLPAG/thirdparty/assimp/code/../include/assimp/SGSpatialSort.h"
    "E:/prog/OpenGLPAG/OpenGLPAG/thirdparty/assimp/code/../include/assimp/GenericProperty.h"
    "E:/prog/OpenGLPAG/OpenGLPAG/thirdparty/assimp/code/../include/assimp/SpatialSort.h"
    "E:/prog/OpenGLPAG/OpenGLPAG/thirdparty/assimp/code/../include/assimp/SkeletonMeshBuilder.h"
    "E:/prog/OpenGLPAG/OpenGLPAG/thirdparty/assimp/code/../include/assimp/SmoothingGroups.h"
    "E:/prog/OpenGLPAG/OpenGLPAG/thirdparty/assimp/code/../include/assimp/SmoothingGroups.inl"
    "E:/prog/OpenGLPAG/OpenGLPAG/thirdparty/assimp/code/../include/assimp/StandardShapes.h"
    "E:/prog/OpenGLPAG/OpenGLPAG/thirdparty/assimp/code/../include/assimp/RemoveComments.h"
    "E:/prog/OpenGLPAG/OpenGLPAG/thirdparty/assimp/code/../include/assimp/Subdivision.h"
    "E:/prog/OpenGLPAG/OpenGLPAG/thirdparty/assimp/code/../include/assimp/Vertex.h"
    "E:/prog/OpenGLPAG/OpenGLPAG/thirdparty/assimp/code/../include/assimp/LineSplitter.h"
    "E:/prog/OpenGLPAG/OpenGLPAG/thirdparty/assimp/code/../include/assimp/TinyFormatter.h"
    "E:/prog/OpenGLPAG/OpenGLPAG/thirdparty/assimp/code/../include/assimp/Profiler.h"
    "E:/prog/OpenGLPAG/OpenGLPAG/thirdparty/assimp/code/../include/assimp/LogAux.h"
    "E:/prog/OpenGLPAG/OpenGLPAG/thirdparty/assimp/code/../include/assimp/Bitmap.h"
    "E:/prog/OpenGLPAG/OpenGLPAG/thirdparty/assimp/code/../include/assimp/XMLTools.h"
    "E:/prog/OpenGLPAG/OpenGLPAG/thirdparty/assimp/code/../include/assimp/IOStreamBuffer.h"
    "E:/prog/OpenGLPAG/OpenGLPAG/thirdparty/assimp/code/../include/assimp/CreateAnimMesh.h"
    "E:/prog/OpenGLPAG/OpenGLPAG/thirdparty/assimp/code/../include/assimp/irrXMLWrapper.h"
    "E:/prog/OpenGLPAG/OpenGLPAG/thirdparty/assimp/code/../include/assimp/BlobIOSystem.h"
    "E:/prog/OpenGLPAG/OpenGLPAG/thirdparty/assimp/code/../include/assimp/MathFunctions.h"
    "E:/prog/OpenGLPAG/OpenGLPAG/thirdparty/assimp/code/../include/assimp/Macros.h"
    "E:/prog/OpenGLPAG/OpenGLPAG/thirdparty/assimp/code/../include/assimp/Exceptional.h"
    "E:/prog/OpenGLPAG/OpenGLPAG/thirdparty/assimp/code/../include/assimp/ByteSwapper.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xassimp-devx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/assimp/Compiler" TYPE FILE FILES
    "E:/prog/OpenGLPAG/OpenGLPAG/thirdparty/assimp/code/../include/assimp/Compiler/pushpack1.h"
    "E:/prog/OpenGLPAG/OpenGLPAG/thirdparty/assimp/code/../include/assimp/Compiler/poppack1.h"
    "E:/prog/OpenGLPAG/OpenGLPAG/thirdparty/assimp/code/../include/assimp/Compiler/pstdint.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE FILES "E:/prog/OpenGLPAG/OpenGLPAG/Build/thirdparty/assimp/code/Debug/assimp-vc140-mt.pdb")
  endif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE FILES "E:/prog/OpenGLPAG/OpenGLPAG/Build/thirdparty/assimp/code/RelWithDebInfo/assimp-vc140-mt.pdb")
  endif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
endif()

