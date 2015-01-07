#!/usr/bin/env python

import sen as senpai
import os, sys
import argparse

# some utilities
def directories(path):
    return [os.path.join(path, p) for p in os.listdir(path) if os.path.isdir(os.path.join(path, p))]

# command line stuff
parser = argparse.ArgumentParser(usage='%(prog)s [options...]')
parser.add_argument('--debug', action='store_true', help='compile with debug flags')
parser.add_argument('--ci', action='store_true', help=argparse.SUPPRESS)
parser.add_argument('--cxx', metavar='<compiler>', help='compiler name to use (default: g++)', default='g++')
parser.add_argument('--quiet', '-q', action='store_true', help='suppress warning output')
parser.add_argument('--console', action='store_true', help='shows a command prompt on Windows')
args = parser.parse_args()

project = senpai.Project(name='BulletLua', compiler=senpai.compiler(args.cxx), builddir='bin', objdir='obj')
project.includes = ['../bulletlua', '../ext/sol']
# project.dependencies = directories('dep')

executable = senpai.BuildOutput(name='sdl_test', target='build', type='executable')
executable.files = senpai.files_from('../bulletlua', '**.cpp')
executable.files.extend(senpai.files_from('src', '**.cpp'))

if sys.platform == 'win32':
    project.libraries = ['mingw32', 'SDL2main', 'SDL2', 'GL', 'GLEW']
    if not args.console:
        project.link_flags = ['-mwindows']
else:
    project.libraries = ['SDL2', 'GL', 'GLEW']

if args.ci:
    project.libraries.extend(['lua5.2'])
    project.includes.extend(['/usr/include/lua5.2', './lua-5.2.2/src', './include'])
else:
    project.libraries.extend(['lua'])

def warning(string):
    if not args.quiet:
        print('warning: {}'.format(string))

# configuration
if 'g++' not in args.cxx:
    warning('compiler not explicitly supported: {}'.format(args.cxx))

cxxflags = ['-Wall', '-Wextra', '-pedantic', '-std=c++11', '-Wno-switch']

if args.debug:
    cxxflags.extend(['-g', '-O0', '-DDEBUG'])
else:
    cxxflags.extend(['-DNDEBUG', '-O3'])

if args.cxx == 'clang++':
    cxxflags.extend(['-Wno-constexpr-not-const', '-Wno-unused-value', '-Wno-mismatched-tags'])

project.flags = cxxflags
project.add_executable(executable)
project.dump(open('build.ninja', 'w'))
