# Configuration file for the Sphinx documentation builder.
#
# For the full list of built-in configuration values, see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html

# -- Project information -----------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#project-information

project = 'CellGFX'
copyright = '2026, Yousei Saitou'
author = 'Yousei Saitou'

# -- General configuration ---------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#general-configuration

extensions = ['breathe','myst_parser']

templates_path = ['_templates']
exclude_patterns = []

language = 'ja'

html_logo = "_static/logo.png"

breathe_projects = {
    "GameEngine": "./xml"  # Doxyfile で指定した XML 出力フォルダへのパス
}

source_suffix = {
    '.rst': 'restructuredtext',
    '.md': 'markdown',
}

reathe_default_project = "GameEngine"

# -- Options for HTML output -------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#options-for-html-output

html_theme = 'sphinxdoc'
html_static_path = []

latex_docclass = {'mydocument': 'jsbook'}