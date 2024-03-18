// Copyright (C) 2024 Erik Inkinen
// SPDX-License-Identifier: LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "waylandtextinputv3contextplugin.h"

QPlatformInputContext *
WaylandTextInputV3ContextPlugin::create(const QString &system,
					const QStringList &paramList)
{
	Q_UNUSED(paramList);

	if (system.compare(system, QStringLiteral("textinputv3"),
			   Qt::CaseInsensitive) == 0)
		return new QtWaylandClient::WaylandTextInputV3Context;
	return 0;
}