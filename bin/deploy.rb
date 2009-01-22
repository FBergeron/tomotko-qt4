#!/usr/bin/ruby

require 'ftools'
require 'fileutils'

$QT4_HOME = ENV[ 'QT4_HOME' ]
$DEBUG_DIR = 'debug'
$RELEASE_DIR = 'release'


[ $DEBUG_DIR, $RELEASE_DIR ].each {
    | deployDir |

    Dir.foreach( "i18n" ) {
        | lang |
        
        if( lang =~ /^[a-z][a-z]$/ ) 
            destDir = "#{ deployDir }/i18n/#{ lang }"
            FileUtils.mkdir_p( destDir )
            File.copy( "i18n/#{ lang }/toMOTko.qm", "#{ destDir }/toMOTko.qm" )
        end
    }

    [ "lib/toMOTko/digraphs.conf", "pics/toMOTko.ico", "LICENSE.txt" ].each {
        | file |
        File.copy( file, deployDir );
    }
}

[ 'mingwm10.dll', 'QtCored4.dll', 'QtGuid4.dll', 'QtXmld4.dll' ].each {
    | dllFile |
    File.copy( $QT4_HOME + "/bin/#{ dllFile }", $DEBUG_DIR ) if( !File.exists?( $DEBUG_DIR + "/#{ dllFile }" ) ) 
    
}
[ 'mingwm10.dll', 'QtCore4.dll', 'QtGui4.dll', 'QtXml4.dll' ].each {
    | dllFile |
    File.copy( $QT4_HOME + "/bin/#{ dllFile }", $RELEASE_DIR ) if( !File.exists?( $RELEASE_DIR + "/#{ dllFile }" ) );
}
