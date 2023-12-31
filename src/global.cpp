/*
 * SPDX-FileCopyrightText: 2021 George Florea Bănuș <georgefb899@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "global.h"

#include <QFileInfo>

Global *Global::instance()
{
    static Global g;
    return &g;
}

Global::Global()
    : m_config(KSharedConfig::openConfig(QStringLiteral("%1/%2").arg(m_configFolderName, m_configFileName)))
    , m_ccConfig(KSharedConfig::openConfig(QStringLiteral("%1/%2").arg(m_configFolderName, m_ccConfigFileName)))
{
}

const QString Global::systemConfigPath()
{
    return QStandardPaths::writableLocation(m_config->locationType()).append(QStringLiteral("/"));
}

const QString Global::appConfigDirPath()
{
    QFileInfo configFolder(QString(systemConfigPath()).append(m_configFolderName));
    if (configFolder.exists()) {
        return configFolder.absoluteFilePath();
    }
    return QString();
}

const QString Global::appConfigFilePath(ConfigFile configFile)
{
    switch (configFile) {
    case ConfigFile::Main: {
        return QString(systemConfigPath()).append(m_config->name());
    }
    case ConfigFile::CustomCommands: {
        return QString(systemConfigPath()).append(m_ccConfig->name());
    }
    default:
        return QString();
    }
}

#include "moc_global.cpp"
