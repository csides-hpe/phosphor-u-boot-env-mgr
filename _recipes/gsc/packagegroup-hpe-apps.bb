SUMMARY = "OpenBMC for HPE - Applications"
PR = "r1"
# START SECTION FROM UPSTREAM /meta-common/recipes-phosphor/packagegroups/packagegroup-hpe-apps.bb

PACKAGE_ARCH = "${TUNE_PKGARCH}"

inherit packagegroup
PROVIDES = "${PACKAGES}"
PACKAGES = " \
        ${PN}-chassis \
        ${PN}-fans \
        ${PN}-flash \
        ${PN}-system \
        "

PROVIDES += "virtual/obmc-chassis-mgmt"
PROVIDES += "virtual/obmc-flash-mgmt"
PROVIDES += "virtual/obmc-system-mgmt"

RPROVIDES:${PN}-flash += "virtual-obmc-flash-mgmt"
RPROVIDES:${PN}-system += "virtual-obmc-system-mgmt"



# END SECTION FROM UPSTREAM /meta-common/recipes-phosphor/packagegroups/packagegroup-hpe-apps.bb

#RDEPENDS:${PN}-system:append:hpe-dbg = " valgrind "
RDEPENDS:${PN}-system:append:hpe-dbg = " htop"
RDEPENDS:${PN}-system:append:hpe-dbg = " nfs-utils"
RDEPENDS:${PN}-system:append = " safec"

RDEPENDS:${PN}-system:append = " \
    create-emmc-store-dirs \
    safs-vrom \
    virtual-media \
    aesprovider \
    shaprovider \
	phosphor-u-boot-env-mgr \
    "

RDEPENDS:${PN}-system:append = " secure-enclave secure-enclave-service secure-enclave-early-entropy-service secure-enclave-evt-listener-service"
RDEPENDS:${PN}-system:append = " systemdsandboxing "
RDEPENDS:${PN}-system:append = " ssl-default-cert "
