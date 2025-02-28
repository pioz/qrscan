# Creating a gem that includes an extension that is built at install time
# https://guides.rubygems.org/gems-with-extensions/

require "mkmf"
require "rbconfig"
host_os = RbConfig::CONFIG["host_os"]

dir_config("qrscan", "/opt/homebrew/include", "/opt/homebrew/lib") if host_os.include?("darwin")
if host_os.match?(/mswin|mingw|cygwin/)
  dir_config("qrscan", "C:/Program Files/ZBar/include", "C:/Program Files/ZBar/lib")
  dir_config("qrscan", "C:/Program Files (x86)/ZBar/include", "C:/Program Files (x86)/ZBar/lib")
end

# Verifica la presenza dell'header e della libreria (opzionale)
have_header("zbar.h") or abort "zbar.h not found"
have_library("zbar") or abort "libzbar not found"

create_makefile("qrscan/qrscan")
