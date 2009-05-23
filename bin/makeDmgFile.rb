#!/usr/bin/ruby

require 'ftools'
require 'fileutils'

$TOMOTKO_VERSION = '0.11.2'
$BUILD_DIR = "/tmp/toMOTko-#{$TOMOTKO_VERSION}"
$QT4_HOME = ENV[ 'QT4_HOME' ]
$APP_FW_HOME = "toMOTko.app/Contents/Frameworks"


if( FileTest.exists?( "toMOTko.app" ) )
    if( !FileTest.exists?( $APP_FW_HOME ) )
        Dir.mkdir( $APP_FW_HOME ) 

        FileUtils.cp_r( "#{$QT4_HOME}/lib/QtCore.framework", $APP_FW_HOME ) 
        FileUtils.cp_r( "#{$QT4_HOME}/lib/QtGui.framework", $APP_FW_HOME ) 
        FileUtils.cp_r( "#{$QT4_HOME}/lib/QtXml.framework", $APP_FW_HOME ) 

        system( "install_name_tool -id @executable_path/../Frameworks/QtCore.framework/Versions/4/QtCore " +
            "#{$APP_FW_HOME}/QtCore.framework/Versions/4/QtCore" )
        system( "install_name_tool -id @executable_path/../Frameworks/QtGui.framework/Versions/4/QtGui " +
            "#{$APP_FW_HOME}/QtGui.framework/Versions/4/QtGui" )
        system( "install_name_tool -id @executable_path/../Frameworks/QtXml.framework/Versions/4/QtXml " +
            "#{$APP_FW_HOME}/QtXml.framework/Versions/4/QtXml" )

        system( "install_name_tool -change #{$QT4_HOME}/lib/QtCore.framework/Versions/4/QtCore " +
            "@executable_path/../Frameworks/QtCore.framework/Versions/4/QtCore toMOTko.app/Contents/MacOS/tomotko" )
        system( "install_name_tool -change #{$QT4_HOME}/lib/QtGui.framework/Versions/4/QtGui " +
            "@executable_path/../Frameworks/QtGui.framework/Versions/4/QtGui toMOTko.app/Contents/MacOS/tomotko" )
        system( "install_name_tool -change #{$QT4_HOME}/lib/QtXml.framework/Versions/4/QtXml " +
            "@executable_path/../Frameworks/QtXml.framework/Versions/4/QtXml toMOTko.app/Contents/MacOS/tomotko" )

        system( "install_name_tool -change #{$QT4_HOME}/lib/QtCore.framework/Versions/4/QtCore " +
            "@executable_path/../Frameworks/QtCore.framework/Versions/4/QtCore toMOTko.app/Contents/Frameworks/QtGui.framework/Versions/4/QtGui" )
        system( "install_name_tool -change #{$QT4_HOME}/lib/QtCore.framework/Versions/4/QtCore " +
            "@executable_path/../Frameworks/QtCore.framework/Versions/4/QtCore toMOTko.app/Contents/Frameworks/QtXml.framework/Versions/4/QtXml" )
    end

    FileUtils.rm( "toMOTko-#{$TOMOTKO_VERSION}.dmg" ) if( FileTest.exists?( "toMOTko-#{$TOMOTKO_VERSION}.dmg" ) )
    FileUtils.rm( "appl.dmg" ) if( FileTest.exists?( "appl.dmg" ) )
    system( "hdiutil create -size 150m -fs HFS+ -volname \"toMOTko-#{$TOMOTKO_VERSION}\" appl.dmg" )
    system( "hdiutil attach appl.dmg" )
    FileUtils.cp_r( "toMOTko.app", "/Volumes/toMOTko-#{$TOMOTKO_VERSION}" )
    # FileUtils.ln_s( "/Applications", "/Volumes/toMOTko-#{$TOMOTKO_VERSION}/Applications" )
    system( "hdiutil detach /Volumes/toMOTko-#{$TOMOTKO_VERSION}" )
    system( "hdiutil convert appl.dmg -format UDZO -o appl.compressed.dmg" )
    FileUtils.rm( "appl.dmg" )
    FileUtils.mv( "appl.compressed.dmg", "toMOTko-#{$TOMOTKO_VERSION}.dmg" )
    system( "hdiutil internet-enable -yes toMOTko-#{$TOMOTKO_VERSION}.dmg" )
end

