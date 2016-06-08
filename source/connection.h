#ifndef T2SDK_CONNECTION_H
#define T2SDK_CONNECTION_H

class Connection {
public:
    Connection(char *domain, char* lib_t2sdk_file, char *license_file, int send_queue_size = 100, int auto_reconnect = 1);
    void connect();

private:
    char *domain;
    char *lib_t2sdk_file;
    char *license_file;
    int send_queue_size;
    int auto_reconnect;
};

#endif /* T2SDK_CONNECTION_H */