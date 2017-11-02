from building import *

cwd = GetCurrentDir()
src = Split('''
pb_common.c
pb_decode.c
pb_encode.c
''')
CPPPATH = [cwd]

group = DefineGroup('nanopb', src, depend = ['PKG_USING_NANOPB'], CPPPATH = CPPPATH)

Return('group')
