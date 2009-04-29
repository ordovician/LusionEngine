# SCons optimization
DefaultEnvironment(tools = []) # Initialize default environment with no tools
SourceCode('/', None)          # avoid the I/O needed to test for the existance of RCS and SCCS directories.

import os
import sys

helpers_path = os.getenv('SCONS_BUILD_HELPERS')
if helpers_path:
    sys.path.insert(0, helpers_path)
from scons_build.setup import CreateEnvironment

env = CreateEnvironment(args = ARGUMENTS)

parent_env = env
SConscript('SConscript', build_dir = env['BUILD_DIR'], duplicate = env['DUPLICATE'], exports = ['parent_env'])

Default('install')
