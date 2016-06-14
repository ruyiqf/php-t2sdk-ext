#ifndef T2SDK_T2CONNECTION_H
#define T2SDK_T2CONNECTION_H


typedef CConfigInterface* (*config)(); 
typedef CConnectionInterface* (*connection)(CConfigInterface*); 
typedef IF2Packer* (*packer)(int); 
typedef IF2UnPacker* (*unpacker)(void *, unsigned int);
typedef IBizMessage* (*biz_message)();

class T2Connection {
public:
    T2Connection(char *lib_t2sdk_file, char *ini_file);
    void connect();

private:
    char *ini_file;
    char *lib_t2sdk_file;
};

#endif /* T2SDK_T2CONNECTION_H */