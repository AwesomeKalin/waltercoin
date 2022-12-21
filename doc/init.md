Sample init scripts and service configuration for waltercoind
==========================================================

Sample scripts and configuration files for systemd, Upstart and OpenRC
can be found in the contrib/init folder.

    contrib/init/waltercoind.service:    systemd service unit configuration
    contrib/init/waltercoind.openrc:     OpenRC compatible SysV style init script
    contrib/init/waltercoind.openrcconf: OpenRC conf.d file
    contrib/init/waltercoind.conf:       Upstart service configuration file
    contrib/init/waltercoind.init:       CentOS compatible SysV style init script

Service User
---------------------------------

All three Linux startup configurations assume the existence of a "waltercoin" user
and group.  They must be created before attempting to use these scripts.
The macOS configuration assumes waltercoind will be set up for the current user.

Configuration
---------------------------------

Running waltercoind as a daemon does not require any manual configuration. You may
set the `rpcauth` setting in the `waltercoin.conf` configuration file to override
the default behaviour of using a special cookie for authentication.

This password does not have to be remembered or typed as it is mostly used
as a fixed token that waltercoind and client programs read from the configuration
file, however it is recommended that a strong and secure password be used
as this password is security critical to securing the wallet should the
wallet be enabled.

If waltercoind is run with the "-server" flag (set by default), and no rpcpassword is set,
it will use a special cookie file for authentication. The cookie is generated with random
content when the daemon starts, and deleted when it exits. Read access to this file
controls who can access it through RPC.

By default the cookie is stored in the data directory, but it's location can be overridden
with the option '-rpccookiefile'.

This allows for running waltercoind without having to do any manual configuration.

`conf`, `pid`, and `wallet` accept relative paths which are interpreted as
relative to the data directory. `wallet` *only* supports relative paths.

For an example configuration file that describes the configuration settings,
see `share/examples/waltercoin.conf`.

Paths
---------------------------------

### Linux

All three configurations assume several paths that might need to be adjusted.

    Binary:              /usr/bin/waltercoind
    Configuration file:  /etc/waltercoin/waltercoin.conf
    Data directory:      /var/lib/waltercoind
    PID file:            /var/run/waltercoind/waltercoind.pid (OpenRC and Upstart) or
                         /run/waltercoind/waltercoind.pid (systemd)
    Lock file:           /var/lock/subsys/waltercoind (CentOS)

The PID directory (if applicable) and data directory should both be owned by the
waltercoin user and group. It is advised for security reasons to make the
configuration file and data directory only readable by the waltercoin user and
group. Access to waltercoin-cli and other waltercoind rpc clients can then be
controlled by group membership.

NOTE: When using the systemd .service file, the creation of the aforementioned
directories and the setting of their permissions is automatically handled by
systemd. Directories are given a permission of 710, giving the waltercoin group
access to files under it _if_ the files themselves give permission to the
waltercoin group to do so (e.g. when `-sysperms` is specified). This does not allow
for the listing of files under the directory.

NOTE: It is not currently possible to override `datadir` in
`/etc/waltercoin/waltercoin.conf` with the current systemd, OpenRC, and Upstart init
files out-of-the-box. This is because the command line options specified in the
init files take precedence over the configurations in
`/etc/waltercoin/waltercoin.conf`. However, some init systems have their own
configuration mechanisms that would allow for overriding the command line
options specified in the init files (e.g. setting `WALTERCOIND_DATADIR` for
OpenRC).

### macOS

    Binary:              /usr/local/bin/waltercoind
    Configuration file:  ~/Library/Application Support/Waltercoin/waltercoin.conf
    Data directory:      ~/Library/Application Support/Waltercoin
    Lock file:           ~/Library/Application Support/Waltercoin/.lock

Installing Service Configuration
-----------------------------------

### systemd

Installing this .service file consists of just copying it to
/usr/lib/systemd/system directory, followed by the command
`systemctl daemon-reload` in order to update running systemd configuration.

To test, run `systemctl start waltercoind` and to enable for system startup run
`systemctl enable waltercoind`

NOTE: When installing for systemd in Debian/Ubuntu the .service file needs to be copied to the /lib/systemd/system directory instead.

### OpenRC

Rename waltercoind.openrc to waltercoind and drop it in /etc/init.d.  Double
check ownership and permissions and make it executable.  Test it with
`/etc/init.d/waltercoind start` and configure it to run on startup with
`rc-update add waltercoind`

### Upstart (for Debian/Ubuntu based distributions)

Upstart is the default init system for Debian/Ubuntu versions older than 15.04. If you are using version 15.04 or newer and haven't manually configured upstart you should follow the systemd instructions instead.

Drop waltercoind.conf in /etc/init.  Test by running `service waltercoind start`
it will automatically start on reboot.

NOTE: This script is incompatible with CentOS 5 and Amazon Linux 2014 as they
use old versions of Upstart and do not supply the start-stop-daemon utility.

### CentOS

Copy waltercoind.init to /etc/init.d/waltercoind. Test by running `service waltercoind start`.

Using this script, you can adjust the path and flags to the waltercoind program by
setting the WALTERCOIND and FLAGS environment variables in the file
/etc/sysconfig/waltercoind. You can also use the DAEMONOPTS environment variable here.

### macOS

Copy org.waltercoin.waltercoind.plist into ~/Library/LaunchAgents. Load the launch agent by
running `launchctl load ~/Library/LaunchAgents/org.waltercoin.waltercoind.plist`.

This Launch Agent will cause waltercoind to start whenever the user logs in.

NOTE: This approach is intended for those wanting to run waltercoind as the current user.
You will need to modify org.waltercoin.waltercoind.plist if you intend to use it as a
Launch Daemon with a dedicated waltercoin user.

Auto-respawn
-----------------------------------

Auto respawning is currently only configured for Upstart and systemd.
Reasonable defaults have been chosen but YMMV.
