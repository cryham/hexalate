TEMPLATE = app
greaterThan(QT_MAJOR_VERSION, 4) {
	QT += widgets
}
CONFIG += warn_on
macx {
	CONFIG += x86_64
}

MOC_DIR = build
OBJECTS_DIR = build
RCC_DIR = build

VERSION = 1.0.2
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

TRANSLATIONS = translations/hexalate_ca.ts \
	translations/hexalate_cs.ts \
	translations/hexalate_de.ts \
	translations/hexalate_en.ts \
	translations/hexalate_es.ts \
	translations/hexalate_et.ts \
	translations/hexalate_fr.ts \
	translations/hexalate_ro.ts \
	translations/hexalate_ru.ts \
	translations/hexalate_tr.ts

RESOURCES = icons/icon.qrc
macx {
	ICON = icons/hexalate.icns
} else:win32 {
	RC_FILE = icons/icon.rc
}

unix: !macx {
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

	qm.files = translations/*.qm
	qm.path = $$PREFIX/share/hexalate/translations

	INSTALLS += target icon pixmap desktop qm
}
