import os

env = Environment()

for cmd in ("CC", "CXX"):
    env[cmd] = os.environ.get(cmd) or env[cmd]

env.AppendUnique(
        CPPPATH=[
            "#src",
            "#googletest/googletest",
            "#googletest/googletest/include",
        ],
        CCFLAGS=[
            "-pedantic",
            "-Wall",
            "-Wextra",

            # lazy looking for newer ffmpeg examples
            "-Wno-deprecated-declarations",

            "-fdiagnostics-color=always",
            "-O3",
        ],
        CXXFLAGS=[
            "-std=c++17",
        ],
        LIBS=[
            "avcodec",
            "avformat",
            "avutil",
            "swscale",
            "boost_program_options",
        ],
        LINKFLAGS=[
            "-pthread",
        ],
    )

gtest = Glob("googletest/googletest/src/gtest-all.cc")

lib = env.StaticLibrary("medianizer", Glob("src/medianizer/*.cpp"))
env.Program("medianizer", Glob("src/*.cpp") + lib)
env.Program("test", Glob("tests/*.cpp") + gtest + lib)
