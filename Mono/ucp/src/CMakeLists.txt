######################################################################################
### 1) Include the sources
######################################################################################

INCLUDE_DIRECTORIES(../src)
INCLUDE_DIRECTORIES(/home/jacquin/ParadisEO-2.0/eo/src)
INCLUDE_DIRECTORIES(/home/jacquin/ParadisEO-2.0/eo/src/do)
INCLUDE_DIRECTORIES(/home/jacquin/ParadisEO-2.0/moeo/src)
INCLUDE_DIRECTORIES(/home/jacquin/ParadisEO-2.0/mo/src)
INCLUDE_DIRECTORIES(/home/jacquin/ParadisEO-2.0/eo/src/utils)


######################################################################################



######################################################################################
### 2) Specify where CMake can find the libraries
######################################################################################

IF(NOT WIN32 OR CYGWIN)
  LINK_DIRECTORIES(/home/jacquin/ParadisEO-2.0/eo/lib)
 LINK_DIRECTORIES(/home/jacquin/ParadisEO-2.0/moeo/build/lib)
ENDIF(NOT WIN32 OR CYGWIN)

# especially for Visual Studio
IF(WIN32 AND NOT CYGWIN)
  	LINK_DIRECTORIES(/home/jacquin/ParadisEO-2.0/eo/\\lib\\${CMAKE_BUILD_TYPE})
ENDIF(WIN32 AND NOT CYGWIN)

######################################################################################



######################################################################################
### 3) Define your targets
######################################################################################

ADD_EXECUTABLE(MOUCPEANorme UCPMulti.cpp)


######################################################################################


######################################################################################
### 4) Link the librairies for the targets
######################################################################################

TARGET_LINK_LIBRARIES(MOUCPEANorme moeo eo eoutils ga es)
######################################################################################
