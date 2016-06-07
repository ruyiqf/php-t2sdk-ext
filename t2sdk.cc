#include "php_t2sdk.h"
#include "source/connection.h"

zend_class_entry *connection_ce;

zend_object_handlers connection_object_handlers;

struct connection_object {
    zend_object std;
     *connection;
};

void connection_free_storage(void *object TSRMLS_DC)
{
    connection_object *obj = (connection_object *)object;
    delete obj->connection; 

    zend_hash_destroy(obj->std.properties);
    FREE_HASHTABLE(obj->std.properties);

    efree(obj);
}

zend_object_value connection_create_handler(zend_class_entry *type TSRMLS_DC)
{
    zval *tmp;
    zend_object_value retval;

    connection_object *obj = (connection_object *)emalloc(sizeof(connection_object));
    memset(obj, 0, sizeof(connection_object));
    obj->std.ce = type;

    ALLOC_HASHTABLE(obj->std.properties);
    zend_hash_init(obj->std.properties, 0, NULL, ZVAL_PTR_DTOR, 0);
#if PHP_VERSION_ID < 50399
    zend_hash_copy(obj->std.properties, type->default_properties_table, (copy_ctor_func_t)zval_add_ref, (void *)&tmp, sizeof(zval *));
#else
	object_properties_init((zend_object*) obj, type);
#endif

    retval.handle = zend_objects_store_put(obj, NULL,
        connection_free_storage, NULL TSRMLS_CC);
    retval.handlers = &connection_object_handlers;

    return retval;
}

PHP_METHOD(Connection, __construct)
{
	long maxGear;
    Connection *connection = NULL;
    zval *object = getThis();

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &maxGear) == FAILURE) {
        RETURN_NULL();
    }

    connection = new Connection(maxGear);
    connection_object *obj = (connection_object *)zend_object_store_get_object(object TSRMLS_CC);
    obj->connection = connection;
}


PHP_METHOD(Connection, p_connect)
{
	Connection *connection;
    connection_object *obj = (connection_object *)zend_object_store_get_object(
        getThis() TSRMLS_CC);

    // if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &name) == FAILURE) {
    //     RETURN_NULL();
    // }

    connection = obj->connection;
    if (connection != NULL) {
        connection->connect()
    }
}

zend_function_entry connection_methods[] = {
    PHP_ME(Connection,  __construct,     NULL, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
    PHP_ME(Connection,  p_connect,  NULL, ZEND_ACC_PUBLIC)
    {NULL, NULL, NULL}
};

PHP_MINIT_FUNCTION(t2sdk)
{
    zend_class_entry ce;
    INIT_CLASS_ENTRY(ce, "Connection", connection_methods);
    connection_ce = zend_register_internal_class(&ce TSRMLS_CC);
    connection_ce->create_object = connection_create_handler;
    memcpy(&connection_object_handlers,
        zend_get_std_object_handlers(), sizeof(zend_object_handlers));
    connection_object_handlers.clone_obj = NULL;
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
