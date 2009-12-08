/*
 * Copyright (c) 2009 Nokia Corporation
 */

#ifndef QANIMATIONLABEL_H_
#define QANIMATIONLABEL_H_

#include <QVBoxLayout>

#include <QLabel>
#include <QMovie>

/**
 * QAnimationLabel
 *
 * Uses animation from the path
 * to display it in a QLabel.
 */
class QAnimationLabel : public QWidget {
        Q_OBJECT
public:
        QAnimationLabel(QString animationPath,
                        QWidget* parent);
        QAnimationLabel(QString animationPath,
                        QSize size,
                        QWidget* parent);
        virtual ~QAnimationLabel();

public slots:
        void start();
        void stop();

private:
        QPointer<QLabel> _container;
        QPointer<QMovie> _animation;

        void init(const QString& animationPath,
                  const QSize& size);
};

#endif /* QANIMATIONLABEL_H_ */
