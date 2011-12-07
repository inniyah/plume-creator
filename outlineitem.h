/***************************************************************************
 *   Copyright (C) 2011 by Cyril Jacquet                                   *
 *   terreville@google.com                                                 *
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
#ifndef OUTLINEITEM_H
#define OUTLINEITEM_H

#include <QtGui>
#include <QWidget>

class OutlineItem : public QWidget
{
    Q_OBJECT
public:
    explicit OutlineItem(QWidget *parent = 0);

protected:
    void resizeEvent (QResizeEvent *event);


signals:
    void resizingSignal();
    void newOutlineTitleSignal(QString newTitle, int number);
    void writeThisSignal(int number);

public slots:
    void showList();
    void hideList();
    void showNote();
    void hideNote();
    void expandTexts(bool expand = true);
    void showListSlot(bool buttonToggled);
    void showNoteSlot(bool buttonToggled);
    void updateSizeSlot();
    void setDocuments(QTextDocument *synDocument, QTextDocument *noteDocument);
    void setTitle(QString title);
    void changeListWidth(int width);
    void changeSynWidth(int width);
    void changeNoteWidth(int width);
    void changeSynFont(QFont font);
    void changeSynTextHeight(int height);
void changeSynIndent(int indent);
void changeSynMargin(int margin);
void changeNoteFont(QFont font);
    void changeNoteTextHeight(int height);
    void changeNoteIndent(int indent);
void changeNoteMargin(int margin);
void connectUpdateTextsSlot();
void disconnectUpdateTextsSlot();


private slots:
    void writeThis();
    void resizeExpandedText();
    void collapseTexts(){expandTexts(false);}

    void updateSynClone(int position, int removed, int added);
    void updateSyn(int position, int removed, int added);

    void updateNoteClone(int position, int removed, int added);
    void updateNote(int position, int removed, int added);

    void editTitleSlot();
    void applyConfig();

    void mergeFormatOnWordOrSelection(const QTextCharFormat &format);

   void setSynFirstLineFont();
   void setNoteFirstLineFont();

   void applySynFont();
   void applyNoteFont();

private:
    int itemWidth;

    int fixedHeight;
    int fixedWidth;

    bool m_expand;

    int synLineCount;
    int noteLineCount;

    QFrame *frame;

    QLineEdit *titleEdit;
    QListWidget *listWidget;
    QLabel *listLabel;
    QPushButton *showListButton;
    QPushButton *hideListButton;
    QFrame *vSeparator1;

    QTextEdit *synEdit;
    QPushButton *expandButton;
    QPushButton *collapseButton;
    QPushButton *expandButton1;
    QPushButton *collapseButton1;

    QTextEdit *noteEdit;
    QLabel *noteLabel;
    QPushButton *showNoteButton;
    QPushButton *hideNoteButton;
    QFrame *vSeparator2;

    QString baseStyleSheet;

    QTextDocument *synDoc;
    QTextDocument *noteDoc;
    QTextDocument *synOutlineDoc;
    QTextDocument *noteOutlineDoc;

QFont synDefaultFont;
int synDefaultHeight;
int synDefaultIndent;
int synDefaultMargin;

QFont  noteDefaultFont;
int noteDefaultHeight;
int noteDefaultIndent;
int noteDefaultMargin;

    int number;
};

#endif // OUTLINEITEM_H
