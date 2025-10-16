set(SYSTEMD_UNITS
    resourcemanager.service
    resourcemanager.socket
    )

add_custom_target(resourcemanager-systemd-units ALL
    DEPENDS ${SYSTEMD_UNITS}
    SOURCES ${SYSTEMD_UNITS}
    COMMAND echo "Install systemd units..."
    )

# better suggestions?
get_filename_component(SYSTEMD_UNIT_INSTALL_PATH "${CMAKE_INSTALL_PREFIX}/../lib/systemd/system" ABSOLUTE)

install(
    FILES ${SYSTEMD_UNITS}
    DESTINATION ${SYSTEMD_UNIT_INSTALL_PATH}
    )
