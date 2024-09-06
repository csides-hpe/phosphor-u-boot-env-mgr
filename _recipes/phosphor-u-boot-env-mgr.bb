SUMMARY = "Phosphor u-Boot Env Mgr"
DESCRIPTION = "Makes Device Tree node data avaliable thru D-Bus"
LICENSE = "Apache-2.0"
LIC_FILES_CHKSUM = "file://LICENSE;md5=2b42edef8fa55315f34f2370b4715ca9"
DEPENDS = "boost \
           dbus \
           nlohmann-json \
           sdbusplus \
           phosphor-dbus-interfaces \
                   phosphor-logging \
"
SRCREV = "${AUTOREV}"
PV = "0.1+git${SRCPV}"

SRC_URI = "git://github.com/csides-hpe/phosphor-u-boot-env-mgr.git;branch=master;protocol=https \
          "

S = "${WORKDIR}/git"

SYSTEMD_SERVICE:${PN} += "xyz.openbmc_project.U_Boot.Environment.Manager.service"
SYSTEMD_SERVICE:${PN} += "xyz.openbmc_project.DeviceTree.VPD.Parser.service"

inherit pkgconfig meson systemd python3native
