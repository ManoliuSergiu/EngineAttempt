# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file LICENSE.rst or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION ${CMAKE_VERSION}) # this file comes with cmake

# If CMAKE_DISABLE_SOURCE_CHANGES is set to true and the source directory is an
# existing directory in our source tree, calling file(MAKE_DIRECTORY) on it
# would cause a fatal error, even though it would be a no-op.
if(NOT EXISTS "/Users/sergiumanoliu/GameEngine1/EngineAttempt/build2/_deps/cgltf-src")
  file(MAKE_DIRECTORY "/Users/sergiumanoliu/GameEngine1/EngineAttempt/build2/_deps/cgltf-src")
endif()
file(MAKE_DIRECTORY
  "/Users/sergiumanoliu/GameEngine1/EngineAttempt/build2/_deps/cgltf-build"
  "/Users/sergiumanoliu/GameEngine1/EngineAttempt/build2/_deps/cgltf-subbuild/cgltf-populate-prefix"
  "/Users/sergiumanoliu/GameEngine1/EngineAttempt/build2/_deps/cgltf-subbuild/cgltf-populate-prefix/tmp"
  "/Users/sergiumanoliu/GameEngine1/EngineAttempt/build2/_deps/cgltf-subbuild/cgltf-populate-prefix/src/cgltf-populate-stamp"
  "/Users/sergiumanoliu/GameEngine1/EngineAttempt/build2/_deps/cgltf-subbuild/cgltf-populate-prefix/src"
  "/Users/sergiumanoliu/GameEngine1/EngineAttempt/build2/_deps/cgltf-subbuild/cgltf-populate-prefix/src/cgltf-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/Users/sergiumanoliu/GameEngine1/EngineAttempt/build2/_deps/cgltf-subbuild/cgltf-populate-prefix/src/cgltf-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/Users/sergiumanoliu/GameEngine1/EngineAttempt/build2/_deps/cgltf-subbuild/cgltf-populate-prefix/src/cgltf-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
