/***************************************************************************
 *   Copyright (C) 2012 by Cyril Jacquet                                   *
 *   terreville@gmail.com                                                 *
 *                                                                         *
 *  This file is part of Plume Creator.                                    *
 *                                                                         *
 *  Plume Creator is free software: you can redistribute it and/or modify  *
 *  it under the terms of the GNU General Public License as published by   *
 *  the Free Software Foundation, either version 3 of the License, or      *
 *  (at your option) any later version.                                    *
 *                                                                         *
 *  Plume Creator is distributed in the hope that it will be useful,       *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *  GNU General Public License for more details.                           *
 *                                                                         *
 *  You should have received a copy of the GNU General Public License      *
 *  along with Plume Creator.  If not, see <http://www.gnu.org/licenses/>. *
 ***************************************************************************/
#ifndef OUTLINERITEMNOTEDELEGATE_H
#define OUTLINERITEMNOTEDELEGATE_H

#include <QStyledItemDelegate>
#if QT_VERSION >= 0x050000
#include <QtWidgets>
#endif 
#include <QtGui>   

#include "hub.h"
#include "notezone.h"

class OutlinerItemNoteDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit OutlinerItemNoteDelegate(QObject *parent = 0);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem & option , const QModelIndex & index ) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex & index ) const;
    QSize sizeHint( const QStyleOptionViewItem& option, const QModelIndex& index ) const ;
    void paint( QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index ) const;

//    void paintBody( QPainter* painter,
//                                        const QStyleOptionViewItem& option,
//                                        const QModelIndex& index ) const;

protected:
    virtual bool editorEvent ( QEvent * event, QAbstractItemModel * model,
                                                      const QStyleOptionViewItem & option, const QModelIndex & index );
    virtual bool eventFilter ( QObject * editor, QEvent * event );

    signals:
    
public slots:
    void setHub(Hub *varHub){hub = varHub;}


private slots:


private :
    Hub *hub;
    NoteZone *note;
    NoteZone *syn;
    bool editorLaunched;
    QString noteZoneCss;
};

#endif // OUTLINERITEMNOTEDELEGATE_H
