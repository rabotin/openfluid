

ADD_DEFINITIONS(-DOPENFLUID_VERSION=${FULL_VERSION})

INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}/src" "${PROJECT_BINARY_DIR}/src")
INCLUDE_DIRECTORIES(${GDAL_INCLUDE_DIRS} ${QT_INCLUDES})

OPNFLD_ADD_OBSERVER_WITH_INSTALL(export.vars.files.csv export.vars.files.csv ${OBSERVERS_OUTPUT_PATH} OFF)

OPNFLD_ADD_OBSERVER_WITH_INSTALL(export.vars.files.kml-anim export.vars.files.kml-anim ${OBSERVERS_OUTPUT_PATH} OFF)
TARGET_LINK_LIBRARIES(export.vars.files.kml-anim${OPENFLUID_OBSERVERS_SUFFIX}
                      openfluid-utils)
                      
OPNFLD_ADD_OBSERVER_WITH_INSTALL(export.vars.files.kml-plot export.vars.files.kml-plot ${OBSERVERS_OUTPUT_PATH} OFF)
TARGET_LINK_LIBRARIES(export.vars.files.kml-plot${OPENFLUID_OBSERVERS_SUFFIX}
                      openfluid-utils)

OPNFLD_ADD_OBSERVER_WITH_INSTALL(export.vars.files.geovector export.vars.files.geovector ${OBSERVERS_OUTPUT_PATH} OFF)
TARGET_LINK_LIBRARIES(export.vars.files.geovector${OPENFLUID_OBSERVERS_SUFFIX}
                      openfluid-utils)
                      
OPNFLD_ADD_OBSERVER_WITH_INSTALL(export.vars.plot.gnuplot export.vars.plot.gnuplot ${OBSERVERS_OUTPUT_PATH} OFF)
TARGET_LINK_LIBRARIES(export.vars.plot.gnuplot${OPENFLUID_OBSERVERS_SUFFIX}
                      openfluid-utils)
                      
OPNFLD_ADD_OBSERVER_WITH_INSTALL(export.spatial-graph.files.dot export.spatial-graph.files.dot ${OBSERVERS_OUTPUT_PATH} OFF)

                            