
include_directories(${ROOT_INCLUDE_DIRS}
                    ${CLHEP_INCLUDE_DIRS}
                    ${Boost_INCLUDE_DIRS}
                    ${CMAKE_CURRENT_SOURCE_DIR})

set(FragmentDists_SOURCES
   AngDist.cpp
   AngDistC.cpp
   AngDistY.cpp
   AngMarginalEta2.cpp
   AngMarginalEta2C.cpp
   MDataRange.cpp
   Eta2.cpp
   Eta2C.cpp
   Parameter.cpp
   Parameters.cpp
   ParameterFileLoader.cpp
   SphIntegrand.cpp
   TH2Func.cpp
   TAngFoamIntegrandWrapper.cpp
   SphCoordsIntegrand.cpp
   dictFragmentDists.cxx)

set(FragmentDists_HEADERS
   AngDist.h
   AngDistY.h
   AngDistC.h
   AngMarginalEta2.h
   AngMarginalEta2C.h
   MDataRange.h
   Eta2.h
   Eta2C.h
   Parameter.h
   Parameters.h
   Polarity.h
   ParameterFileLoader.h
   SphIntegrand.h
   TAngFoamIntegrandWrapper.h
   TH2Func.h
   SphCoordsIntegrand.h)


 find_package(ROOT CONFIG REQUIRED COMPONENTS Foam MathMore)
include(${ROOT_USE_FILE})

ROOT_GENERATE_DICTIONARY(dictFragmentDists ${FragmentDists_HEADERS} LINKDEF LinkDef.h OPTIONS -c)

include_directories(${Boost_INCLUDE_DIRS})

add_library(FragmentDists SHARED ${FragmentDists_SOURCES})
target_link_libraries(FragmentDists ${ROOT_LIBRARIES} ${Boost_LIBRARIES})
install(TARGETS FragmentDists LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
install(FILES ${FragmentDists_HEADERS} DESTINATION ${CMAKE_INSTALL_PREFIX}/include)


