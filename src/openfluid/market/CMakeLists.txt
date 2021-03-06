

INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS} ${QT_INCLUDES})


FILE(GLOB OPENFLUID_MARKET_CPP *.cpp)
FILE(GLOB OPENFLUID_MARKET_HPP *.hpp)


ADD_LIBRARY(openfluid-market SHARED ${OPENFLUID_MARKET_CPP} ${OPENFLUID_MARKET_HPP})

SET_TARGET_PROPERTIES(openfluid-market PROPERTIES VERSION "${VERSION_MAJOR}.${VERSION_MINOR}" 
                                        SOVERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}"
                                        LIBRARY_OUTPUT_DIRECTORY "${LIB_OUTPUT_PATH}"
                                        RUNTIME_OUTPUT_DIRECTORY "${LIB_OUTPUT_PATH}"
                                        ARCHIVE_OUTPUT_DIRECTORY "${LIB_OUTPUT_PATH}"
                                        COMPILE_DEFINITIONS "OPENFLUID_DLL"
                                        DEFINE_SYMBOL "OPENFLUID_DLL_EXPORTS")

TARGET_LINK_LIBRARIES(openfluid-market
                      openfluid-base
                      openfluid-tools
                      openfluid-utils
                      ${QT_QTCORE_LIBRARIES})



INSTALL(TARGETS openfluid-market
        RUNTIME DESTINATION ${BIN_INSTALL_PATH}
        LIBRARY DESTINATION ${LIB_INSTALL_PATH}
        ARCHIVE DESTINATION ${LIB_INSTALL_PATH})
   

INSTALL(FILES ${OPENFLUID_MARKET_HPP} DESTINATION ${INCLUDE_OPENFLUID_INSTALL_PATH}/market)


IF(NOT (CMAKE_BUILD_TYPE MATCHES "Release"))
  ADD_SUBDIRECTORY(tests)
ENDIF(NOT (CMAKE_BUILD_TYPE MATCHES "Release"))  


   