#include "custompropertiesmodel.h"

#include "_debug.h"

#include <KConfigGroup>
#include <QCollator>

CustomPropertiesModel::CustomPropertiesModel(QObject *parent)
    : QAbstractListModel(parent)
{
    m_customPropsConfig = KSharedConfig::openConfig("georgefb/haruna-custom-properties.conf",
                                                    KConfig::SimpleConfig);
    getProperties();
}

void CustomPropertiesModel::getProperties()
{
    m_customProperties.clear();

    QStringList groups = m_customPropsConfig->groupList();

    QCollator collator;
    collator.setNumericMode(true);
    std::sort(groups.begin(), groups.end(), collator);

    beginInsertRows(QModelIndex(), 0, groups.size());
    for (const QString &_group : qAsConst((groups))) {
        auto configGroup = m_customPropsConfig->group(_group);
        Property p;
        p.commandId = _group;
        p.command = configGroup.readEntry("Command", QString());
        p.setAtStartUp = configGroup.readEntry("SetAtStartUp", false);
        m_customProperties << p;
    }
    endInsertRows();
}

int CustomPropertiesModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_customProperties.size();
}

QVariant CustomPropertiesModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    Property prop = m_customProperties[index.row()];

    switch (role) {
    case CommandIdRole:
        return QVariant(prop.commandId);
    case CommandRole:
        return QVariant(prop.command);
    case SetAtStartUpRole:
        return QVariant(prop.setAtStartUp);
    }

    return QVariant();
}

QHash<int, QByteArray> CustomPropertiesModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[CommandIdRole] = "commandId";
    roles[CommandRole] = "command";
    roles[SetAtStartUpRole] = "setAtStartUp";
    return roles;
}

void CustomPropertiesModel::moveRows(int oldIndex, int newIndex)
{
    if (oldIndex < newIndex) {
        beginMoveRows(QModelIndex(), oldIndex, oldIndex, QModelIndex(), newIndex + 1);
    } else {
        beginMoveRows(QModelIndex(), oldIndex, oldIndex, QModelIndex(), newIndex);
    }
    endMoveRows();
}

void CustomPropertiesModel::saveCustomProperty(
        const QString &groupName,
        const QString &command,
        bool setAtStartUp)
{
    if (!m_customPropsConfig->group(groupName).exists()) {
        return;
    }
    m_customPropsConfig->group(groupName).writeEntry(QStringLiteral("Command"), command);
    m_customPropsConfig->group(groupName).writeEntry(QStringLiteral("SetAtStartUp"), setAtStartUp);
    m_customPropsConfig->sync();
}
