/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the QtWebEngine module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
**
** $QT_END_LICENSE$
**
****************************************************************************/
#ifndef WEB_CONTENTS_ADAPTER_CLIENT_H
#define WEB_CONTENTS_ADAPTER_CLIENT_H

#include "qtwebenginecoreglobal.h"

#include <QFlags>
#include <QRect>
#include <QSharedPointer>
#include <QString>
#include <QStringList>
#include <QUrl>

QT_FORWARD_DECLARE_CLASS(QVariant)

class JavaScriptDialogController;
class RenderWidgetHostViewQt;
class RenderWidgetHostViewQtDelegate;
class RenderWidgetHostViewQtDelegateClient;
class WebContentsAdapter;
class WebContentsDelegateQt;

// FIXME: make this ref-counted and implicitely shared and expose as public API maybe ?
class WebEngineContextMenuData {

public:
    QPoint pos;
    QUrl linkUrl;
    QString linkText;
    QString selectedText;
// Some likely candidates for future additions as we add support for the related actions:
//    bool isImageBlocked;
//    bool isEditable;
//    bool isSpellCheckingEnabled;
//    QStringList spellCheckingSuggestions;
//    <enum tbd> mediaType;
//    ...
};

class QWEBENGINE_EXPORT WebContentsAdapterClient {
public:
    // This must match window_open_disposition_list.h.
    enum WindowOpenDisposition {
        UnknownDisposition = 0,
        SuppressOpenDisposition = 1,
        CurrentTabDisposition = 2,
        SingletonTabDisposition = 3,
        NewForegroundTabDisposition = 4,
        NewBackgroundTabDisposition = 5,
        NewPopupDisposition = 6,
        NewWindowDisposition = 7,
        SaveToDiskDisposition = 8,
        OffTheRecordDisposition = 9,
        IgnoreActionDisposition = 10,
    };

    // Must match the values in javascript_message_type.h.
    enum JavascriptDialogType {
        AlertDialog,
        ConfirmDialog,
        PromptDialog,
        // Leave room for potential new specs
        InternalAuthorizationDialog = 0x10,
    };

    // Must match the ones in file_chooser_params.h
    enum FileChooserMode {
        Open,
        OpenMultiple,
        UploadFolder,
        Save
    };

    enum JavaScriptConsoleMessageLevel {
        Info = 0,
        Warning,
        Error
    };

    enum MediaRequestFlag {
        MediaNone = 0,
        MediaAudioCapture = 0x01,
        MediaVideoCapture = 0x02,
    };
    Q_DECLARE_FLAGS(MediaRequestFlags, MediaRequestFlag)

    virtual ~WebContentsAdapterClient() { }

    virtual RenderWidgetHostViewQtDelegate* CreateRenderWidgetHostViewQtDelegate(RenderWidgetHostViewQtDelegateClient *client) = 0;
    virtual RenderWidgetHostViewQtDelegate* CreateRenderWidgetHostViewQtDelegateForPopup(RenderWidgetHostViewQtDelegateClient *client) = 0;
    virtual void titleChanged(const QString&) = 0;
    virtual void urlChanged(const QUrl&) = 0;
    virtual void iconChanged(const QUrl&) = 0;
    virtual void loadProgressChanged(int progress) = 0;
    virtual void didUpdateTargetURL(const QUrl&) = 0;
    virtual void selectionChanged() = 0;
    virtual QRectF viewportRect() const = 0;
    virtual QPoint mapToGlobal(const QPoint &posInView) const = 0;
    virtual qreal dpiScale() const = 0;
    virtual void loadStarted(const QUrl &provisionalUrl) = 0;
    virtual void loadCommitted() = 0;
    virtual void loadFinished(bool success, int error_code = 0, const QString &error_description = QString()) = 0;
    virtual void focusContainer() = 0;
    virtual void adoptNewWindow(WebContentsAdapter *newWebContents, WindowOpenDisposition disposition, bool userGesture, const QRect & initialGeometry) = 0;
    virtual void close() = 0;
    virtual bool contextMenuRequested(const WebEngineContextMenuData&) = 0;
    virtual void requestFullScreen(bool) = 0;
    virtual bool isFullScreen() const = 0;
    virtual void javascriptDialog(QSharedPointer<JavaScriptDialogController>) = 0;
    virtual void runFileChooser(FileChooserMode, const QString &defaultFileName, const QStringList &acceptedMimeTypes) = 0;
    virtual void didRunJavaScript(quint64 requestId, const QVariant& result) = 0;
    virtual void didFetchDocumentMarkup(quint64 requestId, const QString& result) = 0;
    virtual void didFetchDocumentInnerText(quint64 requestId, const QString& result) = 0;
    virtual void didFindText(quint64 requestId, int matchCount) = 0;
    virtual void passOnFocus(bool reverse) = 0;
    virtual void javaScriptConsoleMessage(JavaScriptConsoleMessageLevel level, const QString& message, int lineNumber, const QString& sourceID) = 0;
    virtual void authenticationRequired(const QUrl &requestUrl, const QString &realm, bool isProxy, const QString &challengingHost, QString *outUser, QString *outPassword) = 0;
    virtual void runMediaAccessPermissionRequest(const QUrl &securityOrigin, MediaRequestFlags requestFlags) = 0;
};

#endif // WEB_CONTENTS_ADAPTER_CLIENT_H
