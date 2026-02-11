#include "PythonHighlighter.h"
#include <QColor>

PythonHighlighter::PythonHighlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    // ---- keyword / blue (import, new, booleans)
    keywordFormat.setForeground(QColor(60, 120, 200));
    QStringList keywordPatterns = {"\\bimport\\b", "\\bnew\\b", "\\bTrue\\b", "\\bFalse\\b"};
    for (const QString &pattern : keywordPatterns) {
        HighlightRule rule;
        rule.pattern = QRegularExpression(pattern);
        rule.format = keywordFormat;
        rules.append(rule);
    }

    // ---- numbers / orange
    numberFormat.setForeground(QColor(255,165,0)); // orange
    HighlightRule numberRule;
    numberRule.pattern = QRegularExpression("\\b\\d+(\\.\\d+)?\\b"); // integers + decimals
    numberRule.format = numberFormat;
    rules.append(numberRule);

    // ---- strings / green
    stringFormat.setForeground(Qt::green);
    HighlightRule stringRule;
    stringRule.pattern = QRegularExpression("\".*?\"|'.*?'"); // anything inside quotes
    stringRule.format = stringFormat;
    rules.append(stringRule);

    // ---- parentheses / pink
    parenFormat.setForeground(QColor(255,105,180)); // pink
    HighlightRule parenRule;
    parenRule.pattern = QRegularExpression("[\\(\\)]"); // match ( or )
    parenRule.format = parenFormat;
    rules.append(parenRule);

    parenFormat.setForeground(QColor(255,105,180));
    QStringList defReturnPatterns = {"\\bdef\\b", "\\breturn\\b", "\\bprint\\b"};
    for (const QString &pattern : defReturnPatterns) {
        HighlightRule rule;
        rule.pattern = QRegularExpression(pattern);
        rule.format = parenFormat;
        rules.append(rule);
    }
}

void PythonHighlighter::highlightBlock(const QString &text)
{
    for (const HighlightRule &rule : qAsConst(rules)) {
        QRegularExpressionMatchIterator i = rule.pattern.globalMatch(text);
        while (i.hasNext()) {
            QRegularExpressionMatch match = i.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }
}
