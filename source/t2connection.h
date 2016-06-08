#ifndef T2SDK_T2CONNECTION_H
#define T2SDK_T2CONNECTION_H

class T2Connection {
public:
    T2Connection(char *domain, char* lib_t2sdk_file, char *license_file, char *send_queue_size = "100", char *auto_reconnect = "1");
    void connect();

private:
    char *domain;
    char *lib_t2sdk_file;
    char *license_file;
    char *send_queue_size;
    char *auto_reconnect;
};

#endif /* T2SDK_T2CONNECTION_H */