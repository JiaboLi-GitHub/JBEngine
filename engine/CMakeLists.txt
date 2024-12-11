file(GLOB_RECURSE engine ./ *.h *.cpp  *.c)

add_library(JBEngine_lib  ${engine})
