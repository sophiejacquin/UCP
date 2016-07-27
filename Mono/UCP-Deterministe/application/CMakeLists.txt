######################################################################################
### 1) Include the sources
######################################################################################

INCLUDE_DIRECTORIES(/home/jacquin/Bureau/UCP/src)
INCLUDE_DIRECTORIES(/home/jacquin/Bureau/UCP/param)
INCLUDE_DIRECTORIES(/home/jacquin/Bureau/UCP/input)
INCLUDE_DIRECTORIES(/home/jacquin/ParadisEO-2.0/eo/src)
INCLUDE_DIRECTORIES(/home/jacquin/ParadisEO-2.0/eo/src/do)

######################################################################################



######################################################################################
### 2) Specify where CMake can find the libraries
######################################################################################

IF(NOT WIN32 OR CYGWIN)
  LINK_DIRECTORIES(/home/jacquin/ParadisEO-2.0/eo/lib)
ENDIF(NOT WIN32 OR CYGWIN)

# especially for Visual Studio
IF(WIN32 AND NOT CYGWIN)
  	LINK_DIRECTORIES(${EO_BINARY_DIR}\\lib\\${CMAKE_BUILD_TYPE})
ENDIF(WIN32 AND NOT CYGWIN)

######################################################################################



######################################################################################
### 3) Define your targets
######################################################################################

ADD_EXECUTABLE(UCP UCP.cpp)
ADD_EXECUTABLE(Test Test.cpp)
ADD_EXECUTABLE(Generateur Generateur.cpp)
######################################################################################


######################################################################################
### 4) Link the librairies for the targets
######################################################################################

TARGET_LINK_LIBRARIES(UCP eo eoutils ga es)
TARGET_LINK_LIBRARIES(Test eo eoutils ga es)
TARGET_LINK_LIBRARIES(Generateur eo eoutils ga es)
######################################################################################
