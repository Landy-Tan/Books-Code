#ifndef QPERSON_H
#define QPERSON_H

#include <QObject>

class QPerson : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("Author", "LandyTan")
    Q_CLASSINFO("Version","1.0.0.1")
    Q_CLASSINFO("Date", "2019/10/04")
    Q_PROPERTY(int age READ age WRITE setAge NOTIFY ageChanged)
    Q_PROPERTY(QString name MEMBER m_name)
    Q_PROPERTY(int score MEMBER m_score)
public:
    QPerson(QString fName, QObject *parent = 0);
    ~QPerson();

private:
    int m_age = 10;
    QString m_name;
    int m_score;
public:
    int age();
    void setAge(int value);
    void incAge();

signals:
    ageChanged(int value);

public slots:
};

#endif // QPERSON_H
