#!/usr/bin/ruby

require 'ftools'
require 'fileutils'

$DEPLOY_DIR = "debug" # Hard-coded for now.  Should be a parameter or an environment variable.

[ 'en', 'fr', 'ja', 'es', 'zh' ].each {
    | lang |
    destDir = "#{ $DEPLOY_DIR }/i18n/#{ lang }"
    FileUtils.mkdir_p( destDir )
    File.copy( "i18n/#{ lang }/toMOTko.qm", "#{ destDir }/toMOTko.qm" )
}

File.copy( "lib/toMOTko/digraphs.conf", "#{ $DEPLOY_DIR }" );
