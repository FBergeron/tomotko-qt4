#!/usr/bin/ruby

require 'fileutils'

$VERSION = '0.11.2'

unless( File.exists?( 'toMOTko' ) )
    $STDERR.put( "The executable file doesn't exist.  Cannot make deb file." )
    exit( -1 )
end

# Create file hierarchy.
FileUtils.rm_rf( 'debian' )
FileUtils.mkdir_p( 'debian/DEBIAN' )
FileUtils.mkdir_p( 'debian/usr/bin' )
FileUtils.mkdir_p( 'debian/usr/share/applications' )
FileUtils.mkdir_p( 'debian/usr/share/pixmaps' )
FileUtils.cp( 'toMOTko', 'debian/usr/bin' )
FileUtils.cp( 'etc/deb/toMOTko.desktop', 'debian/usr/share/applications' )
FileUtils.cp( 'pics/toMOTko.png', 'debian/usr/share/pixmaps' )

# Copy control file and update dynamic data on the fly.
$SIZE_IN_BYTES =
    File.size( 'toMOTko' ) + 
    File.size( 'pics/toMOTko.png' ) 
$SIZE_IN_KB = $SIZE_IN_BYTES / 1024

File.open( 'etc/deb/control' ) {
    | input |
    File.open( 'debian/DEBIAN/control', 'w' ) {
        | output |
        input.each_line {
            | line |
            modifLine = line.sub( /[$]SIZE_IN_KB/, $SIZE_IN_KB.to_s )
            output.puts( modifLine )
        }
    }
}

# Build the deb file.
system( 'dpkg-deb --build debian' )
FileUtils.mv( 'debian.deb', "toMOTko-#{$VERSION}_i386.deb" )

exit( 0 )
