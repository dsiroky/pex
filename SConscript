import os

env = Environment()

for cmd in ("CC", "CXX"):
    env[cmd] = os.environ.get(cmd) or env[cmd]

env.AppendUnique(
        CPPPATH=[
            "#src",
        ],
        CCFLAGS=[
            "-pedantic",
            "-Wall",
            "-Wextra",

            # lazy looking for newer ffmpeg examples
            "-Wno-deprecated-declarations",

            "-fdiagnostics-color=always",
        ],
        CXXFLAGS=[
            "-std=c++17",
        ],
        LIBS=[
            "avcodec",
            "avformat",
            "avutil",
            "swscale",
        ],
    )

env.Program("medianizer", Glob("src/*.cpp") + Glob("src/medianizer/*.cpp"))
