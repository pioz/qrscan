# Creating a gem that includes an extension that is built at install time
# https://guides.rubygems.org/gems-with-extensions/

require "mkmf"

dir_config("qrscan", "/opt/homebrew/include", "/opt/homebrew/lib")

# Verifica la presenza dell'header e della libreria (opzionale)
have_header("zbar.h") or abort "zbar.h not found"
have_library("zbar") or abort "libzbar not found"

create_makefile("qrscan")
