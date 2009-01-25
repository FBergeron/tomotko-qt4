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
FileUtils.cp( 'etc/deb/control', 'debian/DEBIAN/control' )
FileUtils.cp( 'toMOTko', 'debian/usr/bin/toMOTko' )

# Build the deb file.
system( 'dpkg-deb --build debian' )
FileUtils.mv( 'debian.deb', "toMOTko-#{$VERSION}_i386.deb" )

exit( 0 )
