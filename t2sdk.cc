#include "php_t2sdk.h"
#include "source/t2connection.h"
#include <string.h>

zend_class_entry *t2connection_ce;
zend_object_handlers t2connection_object_handlers;
char * lib_t2sdk_file;

CConnectionInterface *g_pConnection = NULL;
CConnectionInterface *g_pConnectionHq = NULL;

CBusiness g_szBusiness;
CBusiness g_szBusinessHq;

config T2NewConfig;
connection T2NewConnection;
packer T2NewPacker;
unpacker T2NewUnPacker;
biz_message T2NewBizMessage;

struct t2connection_object {
    zend_object std;
    T2Connection *t2connection;
};

void t2connection_free_storage(void *object TSRMLS_DC)
{
    t2connection_object *obj = (t2connection_object *)object;
    delete obj->t2connection; 

    zend_hash_destroy(obj->std.properties);
    FREE_HASHTABLE(obj->std.properties);

    efree(obj);
}

zend_object_value t2connection_create_handler(zend_class_entry *type TSRMLS_DC)
{
    zval *tmp;
    zend_object_value retval;

    t2connection_object *obj = (t2connection_object *)emalloc(sizeof(t2connection_object));
    memset(obj, 0, sizeof(t2connection_object));
    obj->std.ce = type;

    ALLOC_HASHTABLE(obj->std.properties);
    zend_hash_init(obj->std.properties, 0, NULL, ZVAL_PTR_DTOR, 0);
#if PHP_VERSION_ID < 50399
    zend_hash_copy(obj->std.properties, type->default_properties_table, (copy_ctor_func_t)zval_add_ref, (void *)&tmp, sizeof(zval *));
#else
	object_properties_init((zend_object*) obj, type);
#endif

    retval.handle = zend_objects_store_put(obj, NULL,
        t2connection_free_storage, NULL TSRMLS_CC);
    retval.handlers = &t2connection_object_handlers;

    return retval;
}

PHP_METHOD(T2Connection, __construct)
{
	char *ini_file;
	// long maxGear;
    T2Connection *t2connection = NULL;
    zval *object = getThis();
    zval *config;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &config) == FAILURE) {
         RETURN_NULL();
    }

    if(Z_TYPE_P(config) != IS_ARRAY)
    {
    	RETURN_NULL();
    }

    HashTable *arr_hash;
    HashPosition pointer;
    zval **data;
    arr_hash = Z_ARRVAL_P(config);

	for(zend_hash_internal_pointer_reset_ex(arr_hash, &pointer); zend_hash_get_current_data_ex(arr_hash, (void**) &data, &pointer) == SUCCESS; zend_hash_move_forward_ex(arr_hash, &pointer)) 
	{
	    zval temp;
	    char *key;
	    uint key_len;
	    ulong index;

	    if (zend_hash_get_current_key_ex(arr_hash, &key, &key_len, &index, 0, &pointer) == HASH_KEY_IS_STRING) 
	    {
	    	temp = **data;
	    	zval_copy_ctor(&temp);
	    	if(strcmp(key, "ini_file") == 0)
	    	{
	    		convert_to_string(&temp);
	    		ini_file = Z_STRVAL(temp);
	    	}
	    	else if(strcmp(key, "lib_t2sdk_file") == 0)
	    	{
	    		convert_to_string(&temp);
	    		lib_t2sdk_file = Z_STRVAL(temp);
	    	}
	    	// else if(strcmp(key, "license_file") == 0)
	    	// {
	    	// 	convert_to_string(&temp);
	    	// 	license_file = Z_STRVAL(temp);
	    	// }
	    	// else if(strcmp(key, "send_queue_size") == 0)
	    	// {
	    	// 	convert_to_string(&temp);
	    	// 	send_queue_size = Z_STRVAL(temp);
	    	// }
	    	// else if(strcmp(key, "auto_reconnect") == 0)
	    	// {
	    	// 	convert_to_string(&temp);
	    	// 	auto_reconnect = Z_STRVAL(temp);
	    	// }
	    }
	}

    t2connection = new T2Connection(lib_t2sdk_file, ini_file);
    t2connection_object *obj = (t2connection_object *)zend_object_store_get_object(object TSRMLS_CC);
    obj->t2connection = t2connection;
}


PHP_METHOD(T2Connection, p_connect)
{
	T2Connection *t2connection;
    t2connection_object *obj = (t2connection_object *)zend_object_store_get_object(
        getThis() TSRMLS_CC);

    // if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &name) == FAILURE) {
    //     RETURN_NULL();
    // }

    char *error;
    bool ret = false;
    t2connection = obj->t2connection;
    if (t2connection != NULL) {
        ret = t2connection->connect(error);
    }

    RETURN_BOOL(ret);
}

PHP_METHOD(T2Connection, p_disconnect)
{
	T2Connection *t2connection;
    t2connection_object *obj = (t2connection_object *)zend_object_store_get_object(
        getThis() TSRMLS_CC);

    // if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &name) == FAILURE) {
    //     RETURN_NULL();
    // }

    t2connection = obj->t2connection;
    if (t2connection != NULL) {
        t2connection->disconnect();
    }
}

PHP_METHOD(T2Connection, p_login)
{
	T2Connection *t2connection;
    t2connection_object *obj = (t2connection_object *)zend_object_store_get_object(
        getThis() TSRMLS_CC);

    // if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &name) == FAILURE) {
    //     RETURN_NULL();
    // }

    t2connection = obj->t2connection;
    if (t2connection != NULL) {
        t2connection->login();
    }
}

zend_function_entry t2connection_methods[] = {
    PHP_ME(T2Connection,  __construct,     NULL, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
    PHP_ME(T2Connection,  p_connect,  NULL, ZEND_ACC_PUBLIC)
    PHP_ME(T2Connection,  p_disconnect,  NULL, ZEND_ACC_PUBLIC)
    PHP_ME(T2Connection,  p_login,  NULL, ZEND_ACC_PUBLIC)

    {NULL, NULL, NULL}
};

PHP_MINIT_FUNCTION(t2sdk)
{
    zend_class_entry ce;
    INIT_CLASS_ENTRY(ce, "T2Connection", t2connection_methods);
    t2connection_ce = zend_register_internal_class(&ce TSRMLS_CC);
    t2connection_ce->create_object = t2connection_create_handler;
    memcpy(&t2connection_object_handlers,
        zend_get_std_object_handlers(), sizeof(zend_object_handlers));
    t2connection_object_handlers.clone_obj = NULL;
    return SUCCESS;
}

zend_module_entry t2sdk_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
    STANDARD_MODULE_HEADER,
#endif
    PHP_T2SDK_EXTNAME,
    NULL,        /* Functions */
    PHP_MINIT(t2sdk),        /* MINIT */
    NULL,        /* MSHUTDOWN */
    NULL,        /* RINIT */
    NULL,        /* RSHUTDOWN */
    NULL,        /* MINFO */
#if ZEND_MODULE_API_NO >= 20010901
    PHP_T2SDK_EXTVER,
#endif
    STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_T2SDK
extern "C" {
ZEND_GET_MODULE(t2sdk)
}
#endif
