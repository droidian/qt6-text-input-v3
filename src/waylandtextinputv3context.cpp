// Copyright (C) 2016 Klarälvdalens Datakonsult AB, a KDAB Group company, info@kdab.com
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "waylandtextinputv3context_p.h"

#include <QLoggingCategory>
#include <QtGui/QGuiApplication>
#include <QtGui/QTextCharFormat>
#include <QtGui/QWindow>
#include <QtCore/QVarLengthArray>

#include "QtWaylandClient/private/qwaylanddisplay_p.h"
#include "QtWaylandClient/private/qwaylandinputdevice_p.h"
#include "QtWaylandClient/private/qwaylandwindow_p.h"

QT_BEGIN_NAMESPACE

Q_LOGGING_CATEGORY(qLcQpaInputMethods, "qt.qpa.input.methods")

namespace QtWaylandClient
{

WaylandTextInputV3Context::WaylandTextInputV3Context()
	: mTextInput(new QWaylandTextInputv3Manager())
{
}

WaylandTextInputV3Context::~WaylandTextInputV3Context()
{
	delete mTextInput;
}

bool WaylandTextInputV3Context::isValid() const
{
	return true;
}

void WaylandTextInputV3Context::reset()
{
	qCDebug(qLcQpaInputMethods) << Q_FUNC_INFO;

	QPlatformInputContext::reset();

	QWaylandTextInputInterface *inputInterface = textInput();
	if (!inputInterface)
		return;

	inputInterface->reset();
}

void WaylandTextInputV3Context::commit()
{
	qCDebug(qLcQpaInputMethods) << Q_FUNC_INFO;

	QWaylandTextInputInterface *inputInterface = textInput();
	if (!inputInterface)
		return;

	inputInterface->commit();
}

static ::wl_surface *surfaceForWindow(QWindow *window)
{
	if (!window || !window->handle())
		return nullptr;

	auto *waylandWindow = static_cast<QWaylandWindow *>(window->handle());
	return waylandWindow->wlSurface();
}

void WaylandTextInputV3Context::update(Qt::InputMethodQueries queries)
{
	qCDebug(qLcQpaInputMethods) << Q_FUNC_INFO << queries;

	QWaylandTextInputInterface *inputInterface = textInput();
	if (!QGuiApplication::focusObject() || !inputInterface)
		return;

	auto *currentSurface = surfaceForWindow(mCurrentWindow);

	if (currentSurface && !inputMethodAccepted()) {
		inputInterface->disableSurface(currentSurface);
		mCurrentWindow.clear();
	} else if (!currentSurface && inputMethodAccepted()) {
		QWindow *window = QGuiApplication::focusWindow();
		if (auto *focusSurface = surfaceForWindow(window)) {
			inputInterface->enableSurface(focusSurface);
			mCurrentWindow = window;
		}
	}

	inputInterface->updateState(
		queries, QWaylandTextInputInterface::update_state_change);
}

void WaylandTextInputV3Context::invokeAction(QInputMethod::Action action,
					     int cursorPostion)
{
	QWaylandTextInputInterface *inputInterface = textInput();
	if (!inputInterface)
		return;

	if (action == QInputMethod::Click)
		inputInterface->setCursorInsidePreedit(cursorPostion);
}

void WaylandTextInputV3Context::showInputPanel()
{
	qCDebug(qLcQpaInputMethods) << Q_FUNC_INFO;

	QWaylandTextInputInterface *inputInterface = textInput();
	if (!inputInterface)
		return;

	inputInterface->showInputPanel();
}

void WaylandTextInputV3Context::hideInputPanel()
{
	qCDebug(qLcQpaInputMethods) << Q_FUNC_INFO;

	QWaylandTextInputInterface *inputInterface = textInput();
	if (!inputInterface)
		return;

	inputInterface->hideInputPanel();
}

bool WaylandTextInputV3Context::isInputPanelVisible() const
{
	qCDebug(qLcQpaInputMethods) << Q_FUNC_INFO;

	QWaylandTextInputInterface *inputInterface = textInput();
	if (!inputInterface)
		return QPlatformInputContext::isInputPanelVisible();

	return inputInterface->isInputPanelVisible();
}

QRectF WaylandTextInputV3Context::keyboardRect() const
{
	qCDebug(qLcQpaInputMethods) << Q_FUNC_INFO;

	QWaylandTextInputInterface *inputInterface = textInput();
	if (!inputInterface)
		return QPlatformInputContext::keyboardRect();

	return inputInterface->keyboardRect();
}

QLocale WaylandTextInputV3Context::locale() const
{
	qCDebug(qLcQpaInputMethods) << Q_FUNC_INFO;

	QWaylandTextInputInterface *inputInterface = textInput();
	if (!inputInterface)
		return QPlatformInputContext::locale();

	return inputInterface->locale();
}

Qt::LayoutDirection WaylandTextInputV3Context::inputDirection() const
{
	qCDebug(qLcQpaInputMethods) << Q_FUNC_INFO;

	QWaylandTextInputInterface *inputInterface = textInput();
	if (!inputInterface)
		return QPlatformInputContext::inputDirection();

	return inputInterface->inputDirection();
}

void WaylandTextInputV3Context::setFocusObject(QObject *object)
{
	qCDebug(qLcQpaInputMethods) << Q_FUNC_INFO;

	QWaylandTextInputInterface *inputInterface = textInput();
	if (!inputInterface)
		return;

	QWindow *window = QGuiApplication::focusWindow();

	if (mCurrentWindow && mCurrentWindow->handle()) {
		if (mCurrentWindow.data() != window || !inputMethodAccepted()) {
			auto *surface = static_cast<QWaylandWindow *>(
						mCurrentWindow->handle())
						->wlSurface();
			if (surface)
				inputInterface->disableSurface(surface);
			mCurrentWindow.clear();
		}
	}

	if (window && window->handle() && inputMethodAccepted()) {
		if (mCurrentWindow.data() != window) {
			auto *surface =
				static_cast<QWaylandWindow *>(window->handle())
					->wlSurface();
			if (surface) {
				inputInterface->enableSurface(surface);
				mCurrentWindow = window;
			}
		}
		inputInterface->updateState(
			Qt::ImQueryAll,
			QWaylandTextInputInterface::update_state_enter);
	}
}

QWaylandTextInputInterface *WaylandTextInputV3Context::textInput() const
{
	return mTextInput;
}

}

QT_END_NAMESPACE

#include "moc_waylandtextinputv3context_p.cpp"
