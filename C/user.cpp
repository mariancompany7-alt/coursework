#include "user.h"
#include <QUuid>

User::User() : id(0), role("user") {}

User::User(const QString& login, const QString& password, const QString& role)
    : id(0), login(login), role(role)
{
    setPassword(password);
}

User::User(int id, const QString& login, const QString& passwordHash, const QString& salt, const QString& role)
    : id(id), login(login), passwordHash(passwordHash), salt(salt), role(role) {}


QString User::generateSalt()
{
    // Генерація унікального рядка (UUID) для солі
    return QUuid::createUuid().toString().remove('{').remove('}');
}

void User::setPassword(const QString& password)
{
    salt = generateSalt();
    passwordHash = hashPassword(password, salt);
}

void User::setId(int id) { this->id = id; }
void User::setRole(const QString& role) { this->role = role; }
void User::setLogin(const QString& login) { this->login = login; }

QString User::hashPassword(const QString& password, const QString& salt)
{
    QString data = password + salt;
    QByteArray hash = QCryptographicHash::hash(data.toUtf8(), QCryptographicHash::Sha256);
    return QString(hash.toHex());
}

bool User::verifyPassword(const QString& password) const
{
    return hashPassword(password, salt) == passwordHash;
}

int User::getId() const { return id; }
QString User::getLogin() const { return login; }
QString User::getPasswordHash() const { return passwordHash; }
QString User::getSalt() const { return salt; }
QString User::getRole() const { return role; }
