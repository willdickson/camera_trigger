from setuptools import setup
from setuptools import find_packages
import os

setup(
    name='camera_trigger',
    version='0.0.0',
    description='Library for sending camera trigger pulses via an Arduino',
    long_description=__doc__,
    author='Will Dickson',
    author_email='wbd@caltech',
    license='MIT',

    classifiers=[
        'Development Status :: 3 - Alpha',
        'Intended Audience :: Science/Research',
        'License :: OSI Approved :: MIT License',
        'Programming Language :: Python :: 2.7',
        'Programming Language :: Python :: 3',
        'Programming Language :: Python :: 3.3',
        'Programming Language :: Python :: 3.4',
        'Programming Language :: Python :: 3.5',
    ],

    packages=find_packages(exclude=['examples',]),
)
