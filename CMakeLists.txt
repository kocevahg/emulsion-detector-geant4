#----------------------------------------------------------------------------
# Setup the project
#
cmake_minimum_required(VERSION 3.16...3.27)
project(B1)

#----------------------------------------------------------------------------
# Find Geant4 package, activating all available UI and Vis drivers by default
#
find_package(Geant4 REQUIRED ui_all vis_all)

#----------------------------------------------------------------------------
# Locate sources and headers for this project
#
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

#----------------------------------------------------------------------------
# Add the executable, use our local headers, and link it to the Geant4 libraries
#
add_executable(exampleB1 exampleB1.cc ${sources} ${headers})
target_include_directories(exampleB1 PRIVATE include)
target_link_libraries(exampleB1 PRIVATE ${Geant4_LIBRARIES})

#----------------------------------------------------------------------------
# Copy the macros to the build directory so that the executable can be run
# directly from there (it reads init_vis.mac from the working directory).
#
set(EXAMPLEB1_SCRIPTS
  init_vis.mac
  run1.mac
  run2.mac
  vis.mac
  )

foreach(_script ${EXAMPLEB1_SCRIPTS})
  configure_file(
    ${PROJECT_SOURCE_DIR}/macros/${_script}
    ${PROJECT_BINARY_DIR}/${_script}
    COPYONLY
    )
endforeach()
