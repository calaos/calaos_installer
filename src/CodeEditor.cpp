#include "CodeEditor.h"

CodeEditor::CodeEditor(QWidget *parent) : QPlainTextEdit(parent)
{
        QTextCharFormat format;

        QFont font("Courier New", 12);
        format.setFont(font);
        font.setFixedPitch(true);

        setCurrentCharFormat(format);

        highlighter = new LuaHighlighter(document());

        lineNumberArea = new LineNumberArea(this);

        connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth(int)));
        connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(updateLineNumberArea(QRect,int)));
        connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));

        updateLineNumberAreaWidth(0);
        highlightCurrentLine();
}

int CodeEditor::lineNumberAreaWidth()
{
        int digits = 1;
        int max = qMax(1, blockCount());
        while (max >= 10)
        {
                max /= 10;
                ++digits;
        }

        int space = 3 + fontMetrics().width(QLatin1Char('9'));

        if (digits < 2)
                space *= 2;
        else
                space *= digits;

        return space;
}

void CodeEditor::updateLineNumberAreaWidth(int /* newBlockCount */)
{
        setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

void CodeEditor::updateLineNumberArea(const QRect &rect, int dy)
{
        if (dy)
                lineNumberArea->scroll(0, dy);
        else
                lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());

        if (rect.contains(viewport()->rect()))
                updateLineNumberAreaWidth(0);
}

void CodeEditor::resizeEvent(QResizeEvent *e)
{
        QPlainTextEdit::resizeEvent(e);

        QRect cr = contentsRect();
        lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}

void CodeEditor::highlightCurrentLine()
{
        QList<QTextEdit::ExtraSelection> extraSelections;

        if (!isReadOnly())
        {
                QTextEdit::ExtraSelection selection;

                QColor lineColor = QColor(238, 246, 255);

                selection.format.setBackground(lineColor);
                selection.format.setProperty(QTextFormat::FullWidthSelection, true);
                selection.cursor = textCursor();
                selection.cursor.clearSelection();
                extraSelections.append(selection);
        }

        setExtraSelections(extraSelections);

        //Also match parentheses
        matchParentheses();
}

void CodeEditor::lineNumberAreaPaintEvent(QPaintEvent *event)
{
        QPainter painter(lineNumberArea);
        painter.fillRect(event->rect(), Qt::lightGray);

        QTextBlock block = firstVisibleBlock();
        int blockNumber = block.blockNumber();
        int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
        int bottom = top + (int) blockBoundingRect(block).height();

        while (block.isValid() && top <= event->rect().bottom())
        {
                if (block.isVisible() && bottom >= event->rect().top())
                {
                        QString number = QString::number(blockNumber + 1);
                        painter.setPen(Qt::black);
                        painter.drawText(0, top, lineNumberArea->width(), fontMetrics().height(),
                                         Qt::AlignRight, number);
                }

                block = block.next();
                top = bottom;
                bottom = top + (int) blockBoundingRect(block).height();
                ++blockNumber;
        }
}

// walk through and check that we don't exceed 80 chars per line
void CodeEditor::matchParentheses()
{
        TextBlockData *data = static_cast<TextBlockData *>(textCursor().block().userData());

        if (data)
        {
                QVector<ParenthesisInfo *> infos = data->parentheses();

                int pos = textCursor().block().position();
                for (int i = 0; i < infos.size(); ++i)
                {
                        ParenthesisInfo *info = infos.at(i);

                        int curPos = textCursor().position() - textCursor().block().position();
                        if (info->position == curPos - 1 && info->character == '(')
                        {
                                if (matchLeftParenthesis(textCursor().block(), i + 1, 0))
                                        createParenthesisSelection(pos + info->position);
                        }
                        else if (info->position == curPos - 1 && info->character == ')')
                        {
                                if (matchRightParenthesis(textCursor().block(), i - 1, 0))
                                        createParenthesisSelection(pos + info->position);
                        }
                }
        }
}

bool CodeEditor::matchLeftParenthesis(QTextBlock currentBlock, int i, int numLeftParentheses)
{
        TextBlockData *data = static_cast<TextBlockData *>(currentBlock.userData());
        QVector<ParenthesisInfo *> infos = data->parentheses();

        int docPos = currentBlock.position();
        for (; i < infos.size(); ++i)
        {
                ParenthesisInfo *info = infos.at(i);

                if (info->character == '(')
                {
                        ++numLeftParentheses;
                        continue;
                }

                if (info->character == ')' && numLeftParentheses == 0)
                {
                        createParenthesisSelection(docPos + info->position);
                        return true;
                }
                else
                {
                        --numLeftParentheses;
                }
        }

        currentBlock = currentBlock.next();
        if (currentBlock.isValid())
                return matchLeftParenthesis(currentBlock, 0, numLeftParentheses);

        return false;
}

bool CodeEditor::matchRightParenthesis(QTextBlock currentBlock, int i, int numRightParentheses)
{
        TextBlockData *data = static_cast<TextBlockData *>(currentBlock.userData());
        QVector<ParenthesisInfo *> parentheses = data->parentheses();

        int docPos = currentBlock.position();
        for (; i > -1 && parentheses.size() > 0; --i)
        {
                ParenthesisInfo *info = parentheses.at(i);
                if (info->character == ')')
                {
                        ++numRightParentheses;
                        continue;
                }
                if (info->character == '(' && numRightParentheses == 0)
                {
                        createParenthesisSelection(docPos + info->position);
                        return true;
                }
                else
                {
                        --numRightParentheses;
                }
        }

        currentBlock = currentBlock.previous();
        if (currentBlock.isValid())
                return matchRightParenthesis(currentBlock, 0, numRightParentheses);

        return false;
}

void CodeEditor::createParenthesisSelection(int pos)
{
        QList<QTextEdit::ExtraSelection> selections = extraSelections();

        QTextEdit::ExtraSelection selection;
        QTextCharFormat format = selection.format;
        format.setBackground(Qt::green);
        selection.format = format;

        QTextCursor cursor = textCursor();
        cursor.setPosition(pos);
        cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor);
        selection.cursor = cursor;

        selections.append(selection);

        setExtraSelections(selections);
}

TextBlockData::TextBlockData()
{
}

QVector<ParenthesisInfo *> TextBlockData::parentheses()
{
        return m_parentheses;
}


void TextBlockData::insert(ParenthesisInfo *info)
{
        int i = 0;
        while (i < m_parentheses.size() &&
               info->position > m_parentheses.at(i)->position)
                ++i;

        m_parentheses.insert(i, info);
}

LuaHighlighter::LuaHighlighter(QTextDocument *parent): QSyntaxHighlighter(parent)
{
            HighlightingRule rule;

            keywordFormat.setForeground(Qt::black);
            keywordFormat.setFontWeight(QFont::Bold);
            QStringList keywordPatterns;

            keywordPatterns << "\\band\\b" << "\\bbreak\\b" << "\\bdo\\b"
                            << "\\belse\\b" << "\\belseif\\b" << "\\bend\\b"
                            << "\\bfalse\\b" << "\\bfor\\b" << "\\bfunction\\b"
                            << "\\bif\\b" << "\\bin\\b" << "\\blocal\\b"
                            << "\\bnil\\b" << "\\bnot\\b" << "\\bor\\b"
                            << "\\brepeat\\b" << "\\breturn\\b" << "\\bthen\\b"
                            << "\\btrue\\b" << "\\buntil\\b" << "\\bwhile\\b";

            foreach (const QString &pattern, keywordPatterns)
            {
                rule.pattern = QRegExp(pattern);
                rule.format = keywordFormat;
                highlightingRules.append(rule);
            }

            calaosFormat.setForeground(Qt::darkMagenta);
            rule.pattern = QRegExp("\\bcalaos\\b");
            rule.format = calaosFormat;
            highlightingRules.append(rule);

            singleLineCommentFormat.setForeground(Qt::gray);
            singleLineCommentFormat.setFontItalic(true);
            rule.pattern = QRegExp("--[^\n]*");
            rule.format = singleLineCommentFormat;
            highlightingRules.append(rule);

            multiLineCommentFormat.setForeground(Qt::gray);
            singleLineCommentFormat.setFontItalic(true);

            quotationFormat.setForeground(Qt::red);
            rule.pattern = QRegExp("\".*\"");
            rule.format = quotationFormat;
            highlightingRules.append(rule);

            functionFormat.setForeground(Qt::darkCyan);
            rule.pattern = QRegExp("\\b[A-Za-z0-9_]+(?=\\()");
            rule.format = functionFormat;
            highlightingRules.append(rule);

            commentStartExpression = QRegExp("--\\[\\[");
            commentEndExpression = QRegExp("--\\]\\]");
}

void LuaHighlighter::highlightBlock(const QString &text)
{
        foreach (const HighlightingRule &rule, highlightingRules)
        {
                QRegExp expression(rule.pattern);
                int index = expression.indexIn(text);
                while (index >= 0)
                {
                        int length = expression.matchedLength();
                        setFormat(index, length, rule.format);
                        index = expression.indexIn(text, index + length);
                }
        }

        setCurrentBlockState(0);

        int startIndex = 0;
        if (previousBlockState() != 1)
                startIndex = commentStartExpression.indexIn(text);

        while (startIndex >= 0)
        {
                int endIndex = commentEndExpression.indexIn(text, startIndex);
                int commentLength;
                if (endIndex == -1)
                {
                        setCurrentBlockState(1);
                        commentLength = text.length() - startIndex;
                }
                else
                {
                        commentLength = endIndex - startIndex
                                        + commentEndExpression.matchedLength();
                }
                setFormat(startIndex, commentLength, multiLineCommentFormat);
                startIndex = commentStartExpression.indexIn(text, startIndex + commentLength);
        }

        TextBlockData *data = new TextBlockData;

        int leftPos = text.indexOf('(');
        while (leftPos != -1)
        {
                ParenthesisInfo *info = new ParenthesisInfo;
                info->character = '(';
                info->position = leftPos;

                data->insert(info);
                leftPos = text.indexOf('(', leftPos + 1);
        }

        int rightPos = text.indexOf(')');
        while (rightPos != -1)
        {
                ParenthesisInfo *info = new ParenthesisInfo;
                info->character = ')';
                info->position = rightPos;

                data->insert(info);

                rightPos = text.indexOf(')', rightPos +1);
        }

        setCurrentBlockUserData(data);
}
