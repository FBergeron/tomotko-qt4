#!/usr/bin/ruby

require 'fileutils'

$VERSION  = '0.11.2'
$DVERSION = '1'

unless( File.exists?( "tomotko-#{$VERSION}-#{$DVERSION}_i386.deb" ) )
    $stderr.puts( "The .deb file doesn't exist. Cannot make .rpm file." )
    exit( -1 )
end

# convert the Debian package
system( "fakeroot alien --keep-version --to-rpm tomotko-#{$VERSION}-#{$DVERSION}_i386.deb" )

exit( 0 )
