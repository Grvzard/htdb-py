import glob

from setuptools import setup
from pybind11.setup_helpers import Pybind11Extension, build_ext

__version__ = "0.2.0"


ext_modules=[
    Pybind11Extension(
        name="htdb",
        sources=sorted(glob.glob("htdb/*.c")) + sorted(glob.glob("src/*.cpp")),
        include_dirs=["htdb/"],
    ),
]

setup(
    name="htdb-py",
    version=__version__,
    ext_modules=ext_modules,
    cmdclass={"build_ext": build_ext},
    zip_safe=False,
    python_requires=">=3.9",
    extras_require={"test": ["pytest"]},
)
