/*
 * Copyright (C) Kreogist Dev Team
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */
#include <QTimeLine>
#include <QPainter>

#include "knthememanager.h"

#include "knpreferencelanguagepanelitem.h"

#define m_itemHeight 40
#define m_iconX 10
#define m_iconSize 40
#define m_textX 60 //Default: IconX+IconSize+Spacing(10)

KNPreferenceLanguagePanelItem::KNPreferenceLanguagePanelItem(QWidget *parent) :
    QAbstractButton(parent),
    m_highlight(QLinearGradient(0, 0, 0, m_itemHeight)),
    m_languageIcon(QPixmap()),
    m_languageName(QString()),
    m_mouseAnime(new QTimeLine(100, this)),
    m_highLightOpacity(0.0),
    m_languageIndex(-1)
{
    setObjectName("PreferenceLanguagePanelItem");
    //Set properties.
    setAutoFillBackground(true);
    setContentsMargins(0,0,0,0);
    setFixedHeight(m_itemHeight);

    //Configure the high light.
    m_highlight.setColorAt(0, QColor(0xff, 0xff, 0xff, 0x79));
    m_highlight.setColorAt(1, QColor(0xff, 0xff, 0xff, 0x4a));
    //Configure the time line.
    m_mouseAnime->setUpdateInterval(16);
    m_mouseAnime->setEasingCurve(QEasingCurve::OutCubic);
    connect(m_mouseAnime, &QTimeLine::frameChanged,
            this, &KNPreferenceLanguagePanelItem::onActionChangeHighlight);
}

void KNPreferenceLanguagePanelItem::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    //Paint the highlight at the opacity.
    QPainter painter(this);
    //Initialize the render hints.
    painter.setRenderHints(QPainter::Antialiasing |
                           QPainter::TextAntialiasing |
                           QPainter::SmoothPixmapTransform, true);
    //Draw the backend.
    painter.fillRect(rect(), palette().color(QPalette::Window));
    painter.setOpacity(m_highLightOpacity);
    painter.fillRect(rect(), m_highLightOpacity);
    //Restore the opacity.
    painter.setOpacity(1.0);
    //Draw the icon.
    if(!m_languageIcon.isNull())
    {
        //Update the language icon.
        painter.drawPixmap(m_iconX, 0, m_languageIcon);
    }
    //Draw the text.
    painter.setPen(palette().color(QPalette::WindowText));
    painter.drawText(m_textX, 0, width()-m_textX, m_itemHeight,
                     Qt::AlignLeft | Qt::AlignVCenter,
                     m_languageName);
}

void KNPreferenceLanguagePanelItem::onActionChangeHighlight(const int &frame)
{
    //Update the high light opacity.
    m_highLightOpacity=(qreal)frame/100.0;
    //Update the widget.
    update();
}

inline void KNPreferenceLanguagePanelItem::startAnime(const int &endFrame)
{
    //Stop the animation time line.
    m_mouseAnime->stop();
    //Configure the range.
    m_mouseAnime->setFrameRange(m_highLightOpacity*100, endFrame);
    //Start the mouse anime.
    m_mouseAnime->start();
}

QString KNPreferenceLanguagePanelItem::languageName() const
{
    return m_languageName;
}

void KNPreferenceLanguagePanelItem::setLanguageName(const QString &languageName)
{
    //Save the language name.
    m_languageName = languageName;
    //Update the preference item.
    update();
}

QPixmap KNPreferenceLanguagePanelItem::languageIcon() const
{
    return m_languageIcon;
}

void KNPreferenceLanguagePanelItem::setLanguageIcon(const QPixmap &languageIcon)
{
    //Scaled the language icon.
    m_languageIcon = languageIcon.scaled(m_iconSize,
                                         m_iconSize,
                                         Qt::KeepAspectRatio,
                                         Qt::SmoothTransformation);
    //Update the preference item.
    update();
}

void KNPreferenceLanguagePanelItem::enterEvent(QEvent *event)
{
    //Do original enter event.
    QAbstractButton::enterEvent(event);
    //Start the animation to high light.
    startAnime(100);
}

void KNPreferenceLanguagePanelItem::leaveEvent(QEvent *event)
{
    //Do original leave event.
    QAbstractButton::leaveEvent(event);
    //Start the animation to high light.
    startAnime(0);
}
