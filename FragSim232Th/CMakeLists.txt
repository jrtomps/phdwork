
include(${Geant4_USE_FILE})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include
                    ${PROJECT_SOURCE_DIR}/FragmentDists
                    ${PROJECT_SOURCE_DIR}/ROOTUtils
                    ${PROJECT_SOURCE_DIR}/NuclearChart
                    ${ROOT_INCLUDE_DIRS}
                    ${MY_INCLUDE_DIRS}
                    ${Boost_INCLUDE_DIRS})

file(GLOB sources ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${CMAKE_CURRENT_SOURCE_DIR}/include/*.hh)

set(CMAKE_CXX_FLAGS "-std=c++11")
set(CMAKE_C_FLAGS "-std=c11")
add_library(FragSim232Th SHARED ${sources} ${headers})
target_link_libraries(FragSim232Th 
  ${Geant4_LIBRARIES} 
  FragmentDists 
  ROOTUtils 
  NuclearChart 
  ${Boost_LIBRARIES})

add_executable(fragsim232th FragSim232Th.cc)
target_link_libraries(fragsim232th 
  ${Geant4_LIBRARIES} 
  FragmentDists 
  ROOTUtils 
  NuclearChart 
  FragSim232Th 
  ${Boost_LIBRARIES})

file(GLOB scripts ${PROJECT_SOURCE_DIR}/macros/*.mac)

foreach(_script ${scripts})
  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/${_script}
    ${CMAKE_CURRENT_SOURCE_DIR}/${_script}
    COPYONLY
    )
endforeach()

# (7)
install(TARGETS fragsim232th DESTINATION bin)
install(TARGETS FragSim232Th DESTINATION lib)
