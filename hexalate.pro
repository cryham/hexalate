lessThan(QT_VERSION, 4.6) {
	error("Hexalate requires Qt 4.6 or greater")
}

TEMPLATE = app
greaterThan(QT_MAJOR_VERSION, 4) {
	QT += widgets
}
CONFIG += warn_on

VERSION = 1.0.3
DEFINES += VERSIONSTR=\\\"$${VERSION}\\\"

unix: !macx {
	TARGET = hexalate
} else {
	TARGET = Hexalate
}

HEADERS = src/board.h \
	src/locale_dialog.h \
	src/piece.h \
	src/puzzle.h \
	src/window.h

SOURCES = src/board.cpp \
	src/locale_dialog.cpp \
	src/main.cpp \
	src/piece.cpp \
	src/puzzle.cpp \
	src/window.cpp

TRANSLATIONS = $$files(translations/hexalate_*.ts)

RESOURCES = icons/icon.qrc
macx {
	ICON = icons/hexalate.icns
} else:win32 {
	RC_FILE = icons/icon.rc
} else:unix: {
	isEmpty(PREFIX) {
		PREFIX = /usr/local
	}
	isEmpty(BINDIR) {
		BINDIR = bin
	}

	target.path = $$PREFIX/$$BINDIR/

	icon.files = icons/hicolor/*
	icon.path = $$PREFIX/share/icons/hicolor

	pixmap.files = icons/hexalate.xpm
	pixmap.path = $$PREFIX/share/pixmaps

	desktop.files = icons/hexalate.desktop
	desktop.path = $$PREFIX/share/applications

	appdata.files = icons/hexalate.appdata.xml
	appdata.path = $$PREFIX/share/appdata/

	qm.files = translations/*.qm
	qm.path = $$PREFIX/share/hexalate/translations

	man.files = doc/hexalate.6
	man.path = $$PREFIX/share/man/man6

	INSTALLS += target icon pixmap desktop appdata qm man
}
