#ifndef USER_H
#define USER_H
#include <QString>
#include <QCryptographicHash>

class User {

private:
    int id;
    QString login;
    QString passwordHash;
    QString salt;
    QString role;

public:
    User();
    User(const QString& login, const QString& passwordHash, const QString& role = "user");
    User(int id, const QString& login, const QString& passwordHash, const QString& salt,const QString& role);

    int getId() const;
    QString getLogin() const;
    QString getPasswordHash() const;
    QString getSalt() const;
    QString getRole() const;

    void setId(int id);
    void setLogin(const QString& login);
    void setRole(const QString& role);
    void setPassword(const QString& password);

    bool verifyPassword(const QString& password) const;

    static QString hashPassword(const QString& password, const QString& salt);
    static QString generateSalt();
};

#endif // USER_H
