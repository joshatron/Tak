TEMPLATE = subdirs
SUBDIRS = engine cli
cli.depends = engine
