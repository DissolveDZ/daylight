project('SpriteLight', 'c')

spritelight = shared_library('SpriteLight', 'src/main.c', include_directories : include_directories('include', 'engine_include'), link_args : '-lm')

spritelight_dep = declare_dependency(link_with : spritelight)