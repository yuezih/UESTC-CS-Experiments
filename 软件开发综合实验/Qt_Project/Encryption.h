#ifndef ENCRYPTION_H
#define ENCRYPTION_H

#include <iostream>
#include <QString>

using namespace std;

class Encryption
{
public:
    Encryption();
    ~Encryption();

    void handle_file(string path);
    QString getKey();

};

#endif // ENCRYPTION_H
