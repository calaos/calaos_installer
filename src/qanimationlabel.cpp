/*
 * Copyright (c) 2009 Nokia Corporation
 */

#include "qanimationlabel.h"

QAnimationLabel::QAnimationLabel(QString animationPath,
                                 QWidget* parent) : QWidget(parent) {
    init(animationPath, QSize());
}

QAnimationLabel::QAnimationLabel(QString animationPath,
                                 QSize size,
                                 QWidget* parent) : QWidget(parent) {
    init(animationPath, size);
}

QAnimationLabel::~QAnimationLabel() {
    _container->deleteLater();
    _animation->deleteLater();
}

void QAnimationLabel::init(const QString& animationPath,
                           const QSize& size) {
    /* We'll create the QMovie for the animation */
    _animation = new QMovie(animationPath);
    QSize s = size;
    /* If the size is empty we'll try to detect it.*/
    if(s.isEmpty()) {
        /* Go to the first frame.*/
        _animation->jumpToNextFrame();
        /* Take the size from there. */
        s = _animation->currentPixmap().size();
        /* Go back to the beginning. */
        _animation->jumpToFrame(0);
    }
    /* We're not automatically shown, so lets hide. */
    setHidden(true);
    /* We need a container for the QMovie, let's use QLabel */
    _container = new QLabel(this);
    /*
         * We'll set a fixed size to the QLabel
         * because we don't want to be resized
         */
    _container->setFixedSize(s);
    /* To get the movie displayed on the QLabel */
    _container->setMovie(_animation);
    /* To get the QLabel displayed we'll use a layout */
    QVBoxLayout* layout = new QVBoxLayout(this);
    /* Remove the all the extra space. */
    layout->setSpacing(0);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    layout->setMargin(0);
#else
    layout->setContentsMargins(0, 0, 0, 0);
#endif
    /* Add the movie container to the layout */
    layout->addWidget(_container);
    /* Set the layout as our layout */
    setLayout(layout);
    /* Set our size fixed. */
    setFixedSize(s);
}


void QAnimationLabel::start() {
    /* Let's check if the movie can be started. */
    if(!this->_animation.isNull() &&
       (this->_animation->state() == QMovie::NotRunning ||
        this->_animation->state() == QMovie::Paused)) {
        /* It can so we'll start the animation... */
        this->_animation->start();
        /* ...and reveal ourself. */
        this->setHidden(false);
    }
}


void QAnimationLabel::stop() {
    /* Check if the animation can be stopped. */
    if(!this->_animation.isNull()) {
        if(this->_animation->state() == QMovie::Running) {
            /* It can so we'll stop the animation... */
            this->_animation->stop();
            /* ...and hide. */
            this->setHidden(true);
        }
    }
}
