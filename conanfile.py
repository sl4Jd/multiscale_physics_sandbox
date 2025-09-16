from conan import ConanFile

class Conan(ConanFile):
    name = "Multiscale_sandbox"
    version = "0.1.0"
    requires = (
        "assimp/5.4.3",
        "glfw/3.4"
    )
    options = {"shared": [True, False]}
    default_options = {"shared": False}
    generators = "CMakeDeps", "CMakeToolchain"