TEMPLATE = subdirs
SUBDIRS = engine cli twodgame
cli.depends = engine
twodgame.depends = engine
