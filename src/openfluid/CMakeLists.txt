

INCLUDE_DIRECTORIES(${GDAL_INCLUDE_DIRS})

ADD_SUBDIRECTORY(core)
ADD_SUBDIRECTORY(base)
ADD_SUBDIRECTORY(machine)
ADD_SUBDIRECTORY(fluidx)
ADD_SUBDIRECTORY(ware)
ADD_SUBDIRECTORY(tools)
ADD_SUBDIRECTORY(scientific)
ADD_SUBDIRECTORY(utils)
ADD_SUBDIRECTORY(buddies)
ADD_SUBDIRECTORY(waresdev)

IF(OPENFLUID_ENABLE_GUI)
  ADD_SUBDIRECTORY(ui)
  IF (BUILD_APP_BUILDER)
    ADD_SUBDIRECTORY(builderext)
  ENDIF()
ENDIF()

IF(OPENFLUID_ENABLE_LANDR)
  ADD_SUBDIRECTORY(landr)
ENDIF()  

IF(OPENFLUID_ENABLE_MARKET)
  ADD_SUBDIRECTORY(market)
ENDIF()  



INSTALL(FILES base.hpp buddies.hpp core.hpp machine.hpp fluidx.hpp tools.hpp ware.hpp 
              dllexport.hpp debug.hpp deprecation.hpp global.hpp
        DESTINATION ${INCLUDE_OPENFLUID_INSTALL_PATH})

IF(OPENFLUID_ENABLE_MARKET)
  INSTALL(FILES market.hpp
          DESTINATION ${INCLUDE_OPENFLUID_INSTALL_PATH})  
ENDIF()  


CONFIGURE_FILE(openfluid.pc.in "${PKGCONFIG_OUTPUT_PATH}/openfluid.pc" @ONLY)
CONFIGURE_FILE(openfluid-gui.pc.in "${PKGCONFIG_OUTPUT_PATH}/openfluid-gui.pc" @ONLY)
CONFIGURE_FILE(openfluid-other.pc.in "${PKGCONFIG_OUTPUT_PATH}/openfluid-other.pc" @ONLY)

CONFIGURE_FILE(config.hpp.in config.hpp @ONLY)


INSTALL(FILES "${PKGCONFIG_OUTPUT_PATH}/openfluid.pc" 
              "${PKGCONFIG_OUTPUT_PATH}/openfluid-gui.pc"
              "${PKGCONFIG_OUTPUT_PATH}/openfluid-other.pc"
        DESTINATION ${PKGCONFIG_INSTALL_PATH})

INSTALL(FILES "${CMAKE_CURRENT_BINARY_DIR}/config.hpp" 
        DESTINATION ${INCLUDE_OPENFLUID_INSTALL_PATH})
                       