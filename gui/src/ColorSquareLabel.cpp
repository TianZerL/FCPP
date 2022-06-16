#include <QColorDialog>
#include <QMouseEvent>
#include <QPalette>

#include "ColorSquareLabel.hpp"

ColorSquareLabel::ColorSquareLabel(const QColor& color, QWidget* const parent) :
    QLabel(parent)
{
    setAutoFillBackground(true);
    setColor(color);
}

void ColorSquareLabel::setColor(const QColor& color)
{
    QPalette palette{};
    palette.setColor(backgroundRole(), color);
    palette.setColor(foregroundRole(), qGray(color.rgb()) > 127 ? Qt::black : Qt::white);
    setPalette(palette);
    setToolTip(QStringLiteral("RGB: %1\n%2").arg(color.name(), tr("Double-click to change the color")));
    emit colorChanged(color);
}

void ColorSquareLabel::mouseDoubleClickEvent(QMouseEvent* const e)
{
    QLabel::mouseDoubleClickEvent(e);
    if (e->button() == Qt::LeftButton)
    {
        QColorDialog colorDialog{};
        colorDialog.setCurrentColor(palette().color(backgroundRole()));
        if (colorDialog.exec()) setColor(colorDialog.selectedColor());
    }
}
