/***********************************************************************
 *
 * Copyright (C) 2009, 2010, 2012, 2013, 2014, 2015, 2018 Graeme Gott <graeme@gottcode.org>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 ***********************************************************************/

#include "window.h"

#include "locale_dialog.h"

#include <QApplication>

int main(int argc, char** argv) {
#if !defined(Q_OS_MAC)
	if (!qEnvironmentVariableIsSet("QT_DEVICE_PIXEL_RATIO")
			&& !qEnvironmentVariableIsSet("QT_AUTO_SCREEN_SCALE_FACTOR")
			&& !qEnvironmentVariableIsSet("QT_SCALE_FACTOR")
			&& !qEnvironmentVariableIsSet("QT_SCREEN_SCALE_FACTORS")) {
		QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
	}
#endif
	QApplication app(argc, argv);
	app.setApplicationName("Hexalate");
	app.setApplicationVersion(VERSIONSTR);
	app.setApplicationDisplayName(Window::tr("Hexalate"));
	app.setOrganizationDomain("gottcode.org");
	app.setOrganizationName("GottCode");
#if !defined(Q_OS_WIN) && !defined(Q_OS_MAC)
	app.setWindowIcon(QIcon::fromTheme("hexalate", QIcon(":/hexalate.png")));
	app.setDesktopFileName("hexalate");
#endif
	app.setAttribute(Qt::AA_UseHighDpiPixmaps, true);

	LocaleDialog::loadTranslator("hexalate_");

	Window window;
	window.show();

	return app.exec();
}
