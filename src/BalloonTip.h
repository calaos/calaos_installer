/*******************************************************************
** Copyright (C) 2012 Youann erdez.
** All rights reserved.
** Contact: Erd@gmail.com
**
** This file is part of Devil's Tattoo.
**
**     baloon is free software: you can redistribute it and/or modify
**     it under the terms of the GNU General Public License as published by
**     the Free Software Foundation, either version 3 of the License, or
**     (at your option) any later version.
**
**     ballon is distributed in the hope that it will be useful,
**     but WITHOUT ANY WARRANTY; without even the implied warranty of
**     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**     GNU General Public License for more details.
**
**     You should have received a copy of the GNU General Public License
**     along with balloon.  If not, see <http://www.gnu.org/licenses/>.
**
**********************************************************************/
#ifndef BALLONTIP_H
#define BALLONTIP_H

#include <QFrame>
#include <QPushButton>
#include <QPainter>
#include <QPainterPath>
#include <QPropertyAnimation>
#include <QEnterEvent>
#include <QStyle>


/*
  Class TipButton : button used in the BalloonTip class
*/
class TipButton : public QPushButton {
  Q_OBJECT
public:
  /* The role determines the look-and-feel of the button. Not used for now... */
  enum TipButtonRoles {
    Close,
    Config
  };
Q_DECLARE_FLAGS( TipButtonRole, TipButtonRoles )
Q_ENUMS( TipButtonRoles )

protected:
  TipButtonRole my_role;

public:

  TipButton( TipButtonRole role, QWidget* parent = 0 ) : QPushButton( parent ) {
    setFixedSize( 20, 20 );
    my_role = role;
  }

  void paintEvent(QPaintEvent *) {
    QPainter painter( this );
    painter.setRenderHint( QPainter::Antialiasing );

    QColor color( 150, 150, 150 );

    QRect border( 2, 2, 16, 16 );
    painter.setPen( QPen( color, 2 ) );
    painter.drawRoundedRect( border, 3, 3 );

    if ( my_role == TipButton::Close ) {
    painter.setPen( QPen( color, 3 ) );
    painter.drawLine( 6, 6, 14, 14 );
    painter.drawLine( 6, 14, 14, 6 );

    } else if ( my_role == TipButton::Config ) {
      painter.setPen( Qt::transparent );
      painter.setBrush( color );
      QPainterPath path;
      QPainterPath path2;
      path.addEllipse( 4, 3, 12, 9 );
      path.addRect( 8, 3, 4, 4 );
      path2.addRect( 8, 11, 4, 5 );

      path = path.united(path2).simplified();
      painter.drawPath( path );
    }
  }
};

/*
  Class BalloonTip : A balloon tip used to display messages
*/
class BalloonTip : public QWidget {
  Q_OBJECT
  Q_PROPERTY( ArrowPosition my_arrowPos READ arrowPosition WRITE setArrowPosition )
public:
  /* This enum determine in which corner will be displayed the arrow */
  enum ArrowPositions {
    TopLeft     = 0,
    TopRight    = 1,
    BottomLeft  = 2,
    BottomRight = 3
  };
Q_DECLARE_FLAGS( ArrowPosition, ArrowPositions )
Q_ENUMS( ArrowPositions )

protected:
  QPixmap my_icon;
  QString my_text;
  QString my_title;
  QPoint my_pos;
  ArrowPosition my_arrowPos;
  QRect my_popupRect;
  QRect my_textRect;
  int my_duration;
  bool my_animated;

  QPropertyAnimation* my_animation;

  TipButton* my_closeButton;
  TipButton* my_configButton;

  void   paintEvent( QPaintEvent* ev) override;

  QRect  relativePopupRect();
  QRect  relativeTextRect();

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
  void   enterEvent( QEnterEvent* ev ) override;
#else
  void   enterEvent( QEvent* ev ) override;
#endif
  void   leaveEvent( QEvent* ev ) override;
  void   init();
  void   createAnimation();
  void   defineArrowPosition();
  void   createRects();
  bool   eventFilter(QObject *, QEvent *) override;

public:
  explicit BalloonTip( QString title, QString text,
                       int duration = 2000, QWidget* parent = 0 );
  explicit BalloonTip( QPixmap pix, QString title, QString text,
                       int duration = 2000, QWidget* parent = 0 );
  explicit BalloonTip( QStyle::StandardPixmap pix, QString title,
                       QString text, int duration = 2000, QWidget* parent = 0 );

  ~BalloonTip();
  ArrowPosition arrowPosition();
  void          setArrowPosition( ArrowPosition pos );
  void          move( QPoint pos );

signals:
  void finished();

public slots:
  void   show();
  bool   close();
};

#endif // BALLONTIP_H
