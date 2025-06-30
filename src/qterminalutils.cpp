/***************************************************************************
 *   Copyright (C) 2022 by LXQt team                                       *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 ***************************************************************************/

#include <QRegExp>

#include "qterminalutils.h"

QStringList parse_command(const QString& str)
{
    // These are simplified and may not fully match all edge cases
    QRegExp separator(QLatin1String("'|\\s|\"|$"));
    QRegExp doubleQuote(QLatin1String("\""));
    QRegExp escapedSpace(QLatin1String("\\\\s"));
    QRegExp singleQuote(QLatin1String("'"));

    QStringList list;
    int index = 0;
    int nextIndex = 0;
    while ((nextIndex = separator.indexIn(str, index)) != -1)
    {
        if (nextIndex > index)
        {
            QString part = str.mid(index, nextIndex - index);
            part.replace(escapedSpace, QLatin1String(" "));
            list << part;
        }
        if (separator.cap(0).isEmpty())
        { // end of string
            break;
        }
        index = nextIndex + separator.matchedLength();
        QChar c = str.at(index - 1);
        if (!c.isSpace())
        {
            int quoteIndex = -1;
            if (c == QLatin1Char('\''))
            {
                quoteIndex = singleQuote.indexIn(str, index);
            }
            else
            {
                quoteIndex = doubleQuote.indexIn(str, index);
            }
            if (quoteIndex == -1)
            {
                break;
            }
            else
            {
                if (quoteIndex > index)
                {
                    QString part = str.mid(index, quoteIndex - index);
                    part.replace(escapedSpace, QLatin1String(" "));
                    list << part;
                }
                index = quoteIndex + (c == QLatin1Char('\'') ? singleQuote.matchedLength() : doubleQuote.matchedLength());
            }
        }
    }
    return list;
}
