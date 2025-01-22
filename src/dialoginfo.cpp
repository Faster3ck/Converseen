/*
* This file is part of Converseen, an open-source batch image converter
* and resizer.
*
* (C) Francesco Mondello 2009 - 2025
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
* Contact e-mail: Francesco Mondello <faster3ck@gmail.com>
*
*/

#include <QUrl>
#include <QDesktopServices>
#include <QTextDocument>
#include "dialoginfo.h"
#include "globals.h"
#include "qpainter.h"

DialogInfo::DialogInfo(QWidget *parent) :
    QDialog(parent){
    setupUi(this);

    connect(pushClose, SIGNAL(clicked()), this, SLOT(close()));
    connect(pushFacebook, SIGNAL(clicked()), this, SLOT(openFacebookPage()));

    QString appVersion;
    QString magickVersion;
    QString iMVersionStrTr = tr("ImageMagick Version:");
    QString qtVersionStrTr = tr("Qt Version:");

#ifdef Q_OS_WIN32
    appVersion = QString("%1%2")
            .arg(QCoreApplication::applicationVersion())
            .arg(globals::Globals::signature());
#else
    appVersion = QCoreApplication::applicationVersion();
#endif

    QString title = QString("<p><span style=\" font-size:12pt; font-weight:600;\">Converseen %1</span><br />"
                            "<span style=\" font-size:10pt;\">%2</span></p>")
            .arg(appVersion)
            .arg(tr("The batch image converter and resizer."));

    labelTitle->setText(title);

    magickVersion = globals::Globals::magickVersion();
    QString im_versionString = QString("<p><span style=\" font-size:8pt; font-weight:600;\">%1 </span><span style=\" font-size:8pt;\">%2</span></p>")
                                   .arg(iMVersionStrTr)
                                   .arg(magickVersion);

    labelMagickVersion->setText(im_versionString);

    QString qt_versionString = QString("<p><span style=\" font-size:8pt; font-weight:600;\">%1 </span><span style=\" font-size:8pt;\">%2</span></p>")
                                   .arg(qtVersionStrTr)
                                   .arg(QT_VERSION_STR);
    labelQtVersion->setText(qt_versionString);

    // This section generates a Text + Png pixmap icon for the donazion button!

    QString donateText = tr("Make a Donation!");
    QString donateDescriptionText = tr("Donate using PayPal, Ko-Fi or Cryptocurrencies.");

    QString htmlDonationText = QString(R"(
        <style>
            table {
                border-collapse: collapse;
            }
            td {
                text-align: center;
                vertical-align: middle;
                padding: 5px;
            }
        </style>

        <table>
            <tr>
                <td><img src=':/Images/res/heart.png' width='52' height='52'></td>
                <td>
                    <span style='font-weight: bold; font-size:18pt; text-align: center;'>%1</span><br />
                    <span style='font-style: italic; font-size:8pt; text-align: center;'>%2</span>
                </td>
            </tr>
        </table>
    )")
    .arg(donateText, donateDescriptionText);

    QTextDocument donateHtmlText;
    donateHtmlText.setHtml(htmlDonationText);

    QPixmap pixmapDonationText(donateHtmlText.size().width(), donateHtmlText.size().height());
    pixmapDonationText.fill( Qt::transparent );

    QPainter painter( &pixmapDonationText );
    painter.setRenderHint(QPainter::Antialiasing, true);

    donateHtmlText.drawContents(&painter, pixmapDonationText.rect());

    QIcon donateButtonIcon(pixmapDonationText);
    pushDonatePayPal->setIcon(donateButtonIcon);
    pushDonatePayPal->setIconSize(pixmapDonationText.rect().size());

    adjustSize();
    this->resize(512, 800);
}

void DialogInfo::openFacebookPage()
{
    QDesktopServices::openUrl(QUrl("https://www.facebook.com/converseen", QUrl::TolerantMode));
}

void DialogInfo::on_pushDonatePayPal_clicked()
{
    QDesktopServices::openUrl(QUrl("https://converseen.fasterland.net/donate/", QUrl::TolerantMode));
}

