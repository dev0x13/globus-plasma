file(GLOB_RECURSE kaldi-base-src base/*.cc)
file(GLOB_RECURSE kaldi-util-src util/*.cc)

add_library(kaldi-base STATIC ${kaldi-base-src})
add_library(kaldi-util STATIC ${kaldi-util-src})