import os

env = Environment(CXX = "C:/software_lib/MinGW/bin/g++", CCFLAGS = "-g", tools=['mingw'])

def filtered_glob(env, pattern, omit=[], ondisk=True, source=False, strings=False):
    return filter(lambda f: os.path.basename(f.path) not in omit, env.Glob(pattern))

env.AddMethod(filtered_glob, "FilteredGlob");


sourceList = env.Glob("#/src/*.cpp")



# ------------ Headers and Libraries ------------- 

pathList = ['./include', 'C:/software_lib/boost', 'C:/software_lib/wiiuse']

libPathList = ['C:/software_lib/boost/stage/lib', 'C:/software_lib/wiiuse']

libList = ['boost_thread-mgw45-mt-1_46_1', 'wiiuse']


# ------------ Dynamic libraries and executables ------------- 

mainLibrary = env.SharedLibrary(
target='./bin/cmwii.dll',
source=sourceList,
CPPPATH=pathList,
LIBPATH=libPathList,
LIBS=libList
)

Default(mainLibrary)
