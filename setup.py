from distutils.core import setup, Extension

module = Extension('LazySelect', sources = ['LazySelect.c'])

setup (name = 'LazySelect',
       version = '1.0',
       description = 'This is a LazySelect module package.',
       ext_modules = [module])