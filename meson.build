project('cxl-reg-parser', 'cpp',
  version : '0.1',
  default_options : ['warning_level=3', 'cpp_std=c++2a'])

libpci = dependency('libpci')

executable('cxl-reg-parser',
  './src/main.cpp',
  install : true,
  dependencies : [ libpci ])
