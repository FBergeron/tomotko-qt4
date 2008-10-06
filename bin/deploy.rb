#!/usr/bin/ruby

require 'ftools'
require 'fileutils'


$DEPLOY_DIRS = [ "debug", "release" ] # Hard-coded for now.  Should be a parameter or an environment variable.

$DEPLOY_DIRS.each {
    | deployDir |
    [ 'en', 'fr', 'ja', 'es', 'zh' ].each {
        | lang |
        destDir = "#{ deployDir }/i18n/#{ lang }"
        FileUtils.mkdir_p( destDir )
        File.copy( "i18n/#{ lang }/toMOTko.qm", "#{ destDir }/toMOTko.qm" )
    }

    File.copy( "lib/toMOTko/digraphs.conf", "#{ deployDir }" );
}
