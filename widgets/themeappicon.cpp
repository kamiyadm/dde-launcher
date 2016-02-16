/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include "themeappicon.h"
#include <QFile>
#include <QPainter>
#include <QSvgRenderer>
#include <QPixmap>
#include <QDir>
#include <QDebug>

#undef signals
extern "C" {
    #include <string.h>
    #include <gtk/gtk.h>
    #include <gio/gdesktopappinfo.h>
}
#define signals public

static GtkIconTheme* them = NULL;

inline char* get_icon_theme_name()
{
    GtkSettings* gs = gtk_settings_get_default();
    char* name = NULL;
    g_object_get(gs, "gtk-icon-theme-name", &name, NULL);
    return name;
}

ThemeAppIcon::ThemeAppIcon(QObject *parent) : QObject(parent)
{

}

void ThemeAppIcon::gtkInit(){
    gtk_init(NULL, NULL);
    gdk_error_trap_push();
}

QPixmap ThemeAppIcon::getIconPixmap(QString iconPath, int width, int height){
    qDebug() << iconPath;
    if (iconPath.length() == 0){
        iconPath = "application-x-desktop";
    }
    QPixmap pixmap(width, height);
    // iconPath is an absolute path of the system
    qDebug() << QFile::exists(iconPath) << iconPath.contains(QDir::separator()) << iconPath.startsWith("data:image/");
    if (QFile::exists(iconPath) && iconPath.contains(QDir::separator())) {
        pixmap = QPixmap(iconPath);
    } else if (iconPath.startsWith("data:image/")){
        // iconPath is a string representing an inline image.
        QStringList strs = iconPath.split("base64,");
        if (strs.length() == 2) {
            QByteArray data = QByteArray::fromBase64(strs.at(1).toLatin1());
            pixmap.loadFromData(data);
        }
    }else {
        // try to read the iconPath as a icon name.
        QString path = getThemeIconPath(iconPath, width);
        qDebug() << "getThemeIconPath:" << path;
        if (path.length() == 0){
            path = ":/images/skin/images/application-default-icon.svg";
        }
        if (path.endsWith(".svg")) {
            QSvgRenderer renderer(path);
            pixmap.fill(Qt::transparent);
            pixmap.scaled(width, height);
            QPainter painter;
            painter.begin(&pixmap);

            renderer.render(&painter);

            painter.end();
        } else {
            pixmap.load(path);
        }

    }

    return pixmap;
}

QString ThemeAppIcon::getThemeIconPath(QString iconName, int size)
{
    QByteArray bytes = iconName.toUtf8();
    char *name = bytes.data();

    if (g_path_is_absolute(name))
            return g_strdup(name);

        g_return_val_if_fail(name != NULL, NULL);

        int pic_name_len = strlen(name);
        char* ext = strrchr(name, '.');
        if (ext != NULL) {
            if (g_ascii_strcasecmp(ext+1, "png") == 0 || g_ascii_strcasecmp(ext+1, "svg") == 0 || g_ascii_strcasecmp(ext+1, "jpg") == 0) {
                pic_name_len = ext - name;
                qDebug() << "desktop's Icon name should an absoulte path or an basename without extension";
            }
        }

        // In pratice, default icon theme may not gets the right icon path when program starting.
        if (them == NULL)
            them = gtk_icon_theme_new();
        char* icon_theme_name = get_icon_theme_name();
        gtk_icon_theme_set_custom_theme(them, icon_theme_name);
        g_free(icon_theme_name);

        char* pic_name = g_strndup(name, pic_name_len);
        GtkIconInfo* info = gtk_icon_theme_lookup_icon(them, pic_name, size, GTK_ICON_LOOKUP_GENERIC_FALLBACK);

        if (info == NULL) {
            info = gtk_icon_theme_lookup_icon(gtk_icon_theme_get_default(), pic_name, size, GTK_ICON_LOOKUP_GENERIC_FALLBACK);
            if (info == NULL) {
                qWarning() << "get gtk icon theme info failed for" << pic_name;
                g_free(pic_name);
                return "";
            }
        }
        g_free(pic_name);

        char* path = g_strdup(gtk_icon_info_get_filename(info));

    #if GTK_MAJOR_VERSION >= 3
        g_object_unref(info);
    #elif GTK_MAJOR_VERSION == 2
        gtk_icon_info_free(info);
    #endif
        g_debug("get icon from icon theme is: %s", path);
        return path;
}



ThemeAppIcon::~ThemeAppIcon()
{

}

