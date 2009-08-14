#!/usr/bin/ruby

require 'fileutils'

$VERSION  = '0.11.2'
$DVERSION = '1'

$ARCH = ARGV[ 0 ]
if( $ARCH != 'i386' && $ARCH != 'armel' )
    $stderr.puts( "Wrong architecture or architecture not specified.  Cannot make deb file." )
    exit( -1 )
end

unless( File.exists?( 'toMOTko' ) )
    $stderr.puts( "The executable file doesn't exist. Cannot make deb file." )
    exit( -1 )
end

# prepare (strip) the binary
system( 'strip toMOTko') if( $ARCH == 'i386' )

# Create file hierarchy.
FileUtils.rm_rf( 'debian' )
FileUtils.mkdir_p( 'debian/DEBIAN' )
FileUtils.mkdir_p( 'debian/usr/bin' )
FileUtils.mkdir_p( 'debian/usr/share/applications' )
FileUtils.mkdir_p( 'debian/usr/share/applications/hildon' )
FileUtils.mkdir_p( 'debian/usr/share/pixmaps' )
FileUtils.mkdir_p( 'debian/usr/share/doc' )
FileUtils.mkdir_p( 'debian/usr/share/doc/tomotko' )
FileUtils.mkdir_p( 'debian/usr/share/man' )
FileUtils.mkdir_p( 'debian/usr/share/man/man1' )
FileUtils.cp( 'toMOTko', 'debian/usr/bin' )
FileUtils.cp( 'etc/deb/toMOTko.desktop', 'debian/usr/share/applications/hildon' )
FileUtils.cp( 'pics/toMOTko.png', 'debian/usr/share/pixmaps' )
FileUtils.cp( 'etc/deb/copyright', 'debian/usr/share/doc/tomotko' )
FileUtils.cp( 'etc/deb/changelog', 'debian/usr/share/doc/tomotko' )
FileUtils.cp( 'etc/deb/changelog.Debian', 'debian/usr/share/doc/tomotko' )
FileUtils.cp( 'etc/deb/toMOTko.1', 'debian/usr/share/man/man1' )

system( 'chmod 644 debian/usr/share/applications/hildon/toMOTko.desktop' );

# changelog files must be compressed
system( 'gzip --best debian/usr/share/doc/tomotko/changelog')
system( 'gzip --best debian/usr/share/doc/tomotko/changelog.Debian')

# compress manual page
system( 'gzip --best debian/usr/share/man/man1/toMOTko.1')

# Copy control file and update dynamic data on the fly.
$SIZE_IN_BYTES =
    File.size( 'toMOTko' ) + 
    File.size( 'pics/toMOTko.png' ) +
    File.size( 'etc/deb/copyright' ) +
    File.size( 'debian/usr/share/doc/tomotko/changelog.Debian.gz' ) +
    File.size( 'debian/usr/share/doc/tomotko/changelog.gz' ) +
    File.size( 'debian/usr/share/man/man1/toMOTko.1.gz' )
$SIZE_IN_KB = $SIZE_IN_BYTES / 1024

File.open( 'etc/deb/control' ) {
    | input |
    File.open( 'debian/DEBIAN/control', 'w' ) {
        | output |
        input.each_line {
            | line |
            modifLine = line.sub( /[$]ARCH/, "#{$ARCH}" )
            modifLine = modifLine.sub( /[$]SIZE_IN_KB/, $SIZE_IN_KB.to_s )
            modifLine = modifLine.sub( /[$]VERSION/, "#{$VERSION}-#{$DVERSION}" )
            output.puts( modifLine )
        }
    }
}

# Build the deb file.
system( 'fakeroot dpkg-deb --build debian' )
FileUtils.mv( 'debian.deb', "tomotko-#{$VERSION}-#{$DVERSION}maemo1_#{$ARCH}.deb" )

# Execute the lintian check
system( 'lintian ' + "tomotko-#{$VERSION}-#{$DVERSION}maemo1_#{$ARCH}.deb" )

exit( 0 )
