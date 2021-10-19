# config.py


def can_build(env, platform):
    return True


def configure(env):
    env.Prepend(CFLAGS=["-std=gnu11"])
    env.Prepend(CXXFLAGS=["-std=gnu++14"])

def get_doc_classes():
    return [
        "VMD",
        "VMDPlayer",
        "VMDAnimator",
    ]


def get_doc_path():
    return "doc_classes"
