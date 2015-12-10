import os
from glob import glob
#import ycm_core

base = os.path.dirname(os.path.abspath(__file__))

def FlagsForFile(filename, **kwargs):
	filedir = os.path.dirname(filename)
	flags = [
		'-x', 'c++',
		'-Wall',
		'-Wextra',
		'-Wpedantic',
		'-std=c++14',
		'-isystem', os.path.join(os.environ['HOME'], '.vim/bundle/YouCompleteMe/third_party/ycmd/clang_includes'),
		'-I', os.path.join(base, 'include'),
	]

	return {
		'flags': flags,
		'do_cache': True
	}
