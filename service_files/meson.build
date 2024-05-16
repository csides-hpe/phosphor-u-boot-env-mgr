systemd_dep = dependency('systemd')
SYSTEMD_SYSTEM_UNIT_DIR = systemd_dep.get_pkgconfig_variable('systemdsystemunitdir')

ENV_SERVICE_FILE = 'xyz.openbmc_project.U_Boot.Environment.Manager.service'
configure_file(copy: true, input: ENV_SERVICE_FILE, install: true, install_dir: SYSTEMD_SYSTEM_UNIT_DIR, output: ENV_SERVICE_FILE)

ENV_DAEMON_FILE = 'xyz.openbmc_project.U_Boot.Environment.DevTreeNodesd.service'
configure_file(copy: true, input: ENV_DAEMON_FILE, install: true, install_dir: SYSTEMD_SYSTEM_UNIT_DIR, output: ENV_DAEMON_FILE)
