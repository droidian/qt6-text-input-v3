// Copyright (C) 2024 Erik Inkinen
// SPDX-License-Identifier: LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef WAYLANDTEXTINPUTV3CONTEXTPLUGIN_H
#define WAYLANDTEXTINPUTV3CONTEXTPLUGIN_H

#include "waylandtextinputv3context_p.h"
#include <qpa/qplatforminputcontextplugin_p.h>

class Q_DECL_EXPORT WaylandTextInputV3ContextPlugin
	: public QPlatformInputContextPlugin {
	Q_OBJECT
	Q_PLUGIN_METADATA(
		IID
		"org.qt-project.Qt.QPlatformInputContextFactoryInterface.5.1" FILE
		"textinputv3.json")

    public:
	QPlatformInputContext *create(const QString &, const QStringList &);
};

#endif // WAYLANDTEXTINPUTV3CONTEXTPLUGIN_H