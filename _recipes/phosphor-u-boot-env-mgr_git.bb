SUMMARY = "Phosphor u-Boot Env Mgr"
DESCRIPTION = "Manage data sourced thru u-Boot"
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

PACKAGECONFIG ??= " \
    phosphor-u-boot-env-mgrd \
    phosphor-u-boot-env-devtree-nodesd \
    "

PACKAGECONFIG[phosphor-u-boot-env-mgrd] = "-Dphosphor-u-boot-env-mgrd=enabled, -Dphosphor-u-boot-env-mgrd=disabled"
PACKAGECONFIG[phosphor-u-boot-env-devtree-nodesd] = "-Ddevtree-nodesd=enabled, -Ddevtree-nodesd=disabled"

SYSTEMD_SERVICE:${PN} += "${@bb.utils.contains('PACKAGECONFIG', 'phosphor-u-boot-env-mgrd', \
                                               'xyz.openbmc_project.U_Boot.Environment.Manager.service', \
                                               '', d)}"

SYSTEMD_SERVICE:${PN} += "${@bb.utils.contains('PACKAGECONFIG', 'phosphor-u-boot-env-devtree-nodesd', \
                                               'xyz.openbmc_project.U_Boot.Environment.DevTreeNodesd.service', \
                                               '', d)}"

#SYSTEMD_SERVICE:${PN} += "xyz.openbmc_project.U_Boot.Environment.Manager.service"
#SYSTEMD_SERVICE:${PN} += "xyz.openbmc_project.U_Boot.Environment.DevTreeNodesd.service"

inherit pkgconfig meson systemd python3native
