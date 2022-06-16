#ifndef FCPP_QFCPP_ROM_INFO_HPP
#define FCPP_QFCPP_ROM_INFO_HPP

#include <QDataStream>
#include <QMetaType>
#include <QString>

struct RomInfo
{
    QString path;
    QString name;
};

inline QDataStream& operator<< (QDataStream& stream, const RomInfo& object)
{
    return stream << object.path << object.name;
}

inline QDataStream& operator>> (QDataStream& stream, RomInfo& object)
{
    return stream >> object.path >> object.name;
}

Q_DECLARE_METATYPE(RomInfo)

#endif
