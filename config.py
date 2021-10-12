# config.py


def can_build(env, platform):
    return True


def configure(env):
    env.Prepend(CFLAGS=["-std=gnu11"])
    env.Prepend(CXXFLAGS=["-std=gnu++14"])
