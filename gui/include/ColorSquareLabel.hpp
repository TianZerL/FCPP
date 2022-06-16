#ifndef FCPP_QFCPP_COLOR_SQUARE_LABEL_HPP
#define FCPP_QFCPP_COLOR_SQUARE_LABEL_HPP

#include <QLabel>
#include <QColor>

class ColorSquareLabel : public QLabel
{
    Q_OBJECT

public:
    explicit ColorSquareLabel(const QColor& color, QWidget* parent = nullptr);
    ~ColorSquareLabel() override = default;

    void setColor(const QColor& color);
private:
    void mouseDoubleClickEvent(QMouseEvent* e) override;
signals:
    void colorChanged(const QColor& color);
private:
    int idx;
};

#endif
