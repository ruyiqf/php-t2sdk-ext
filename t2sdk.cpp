#include "php_t2sdk.h"
#include "source/t2connection.h"
#include <string.h>
/*
 *  zend_parse_parameters 接受参数不全会导致Segmentation fault
 */

//全局变量初始化
ZEND_DECLARE_MODULE_GLOBALS(t2sdk);

zend_class_entry *t2connection_ce;
zend_object_handlers t2connection_object_handlers;

config T2NewConfig;
connection T2NewConnection;
packer T2NewPacker;
unpacker T2NewUnPacker;
biz_message T2NewBizMessage;

static void php_t2sdk_init_globals(zend_t2sdk_globals *t2sdk_globals)
{

}

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
	char *lib_t2sdk_file;
	// char *fund_account;
	// char *password;

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
	    	// else if(strcmp(key, "fund_account") == 0)
	    	// {
	    	// 	convert_to_string(&temp);
	    	// 	fund_account = Z_STRVAL(temp);
	    	// }
	    	// else if(strcmp(key, "password") == 0)
	    	// {
	    	// 	convert_to_string(&temp);
	    	// 	password = Z_STRVAL(temp);
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
    int ret = -2;
    t2connection = obj->t2connection;
    if (t2connection != NULL) {
        ret = t2connection->connect(error);
    }

    RETURN_LONG(ret);
}

PHP_METHOD(T2Connection, p_disconnect)
{
	puts("t2sdk begin disconnect");
	T2Connection *t2connection;
    t2connection_object *obj = (t2connection_object *)zend_object_store_get_object(
        getThis() TSRMLS_CC);

    	puts("t2sdk begin get obj");

    // if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &name) == FAILURE) {
    //     RETURN_NULL();
    // }

    t2connection = obj->t2connection;

        	puts("t2sdk begin get t2connection");

    if (t2connection != NULL) {
        t2connection->disconnect();
    }
}

PHP_METHOD(T2Connection, p_login)
{
	T2Connection *t2connection;
    t2connection_object *obj = (t2connection_object *)zend_object_store_get_object(
        getThis() TSRMLS_CC);

    char *fund_account;
    uint fund_account_len;
    char *password;
    uint password_len;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ss", &fund_account, &fund_account_len, &password, &password_len) == FAILURE) {
        RETURN_NULL();
    }

    zval * result;
    t2connection = obj->t2connection;
    if (t2connection != NULL) {
        result = t2connection->login(fund_account, password);
    }

    RETURN_ZVAL(result, 1, 0);
}

PHP_METHOD(T2Connection, p_req330300)
{
    //puts("t2sdk req330300");
	T2Connection *t2connection;
    t2connection_object *obj = (t2connection_object *)zend_object_store_get_object(
        getThis() TSRMLS_CC);

    char *stock_id;
    uint stock_id_len;
    char *exchange_type;
    uint exchange_type_len;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ss", &stock_id, &stock_id_len, &exchange_type, &exchange_type_len) == FAILURE) {
        RETURN_NULL();
    }

    zval * result;
    t2connection = obj->t2connection;
    if (t2connection != NULL) {
        result = t2connection->req330300(stock_id, exchange_type);
    }

    RETURN_ZVAL(result, 1, 0);
}

PHP_METHOD(T2Connection, p_req400)
{
    T2Connection *t2connection;
    t2connection_object *obj = (t2connection_object *)zend_object_store_get_object(
        getThis() TSRMLS_CC);

    char *stock_id;
    uint stock_id_len;
    char *exchange_type;
    uint exchange_type_len;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ss", &stock_id, &stock_id_len, &exchange_type, &exchange_type_len) == FAILURE) {
        RETURN_NULL();
    }

    zval * result;
    t2connection = obj->t2connection;
    if (t2connection != NULL) {
        result = t2connection->req400(stock_id, exchange_type);
    }

    RETURN_ZVAL(result, 1, 0);
}

PHP_METHOD(T2Connection, p_req333001)
{
    T2Connection *t2connection;
    t2connection_object *obj = (t2connection_object *)zend_object_store_get_object(
        getThis() TSRMLS_CC);

    char *stock_id;
    uint stock_id_len;
    char *exchange_type;
    uint exchange_type_len;
    double entrust_price;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ssd", &stock_id, &stock_id_len, &exchange_type, &exchange_type_len, &entrust_price) == FAILURE) {
        RETURN_NULL();
    }

    printf("%s %s %f", stock_id, exchange_type, entrust_price);

    zval * result;
    t2connection = obj->t2connection;
    if (t2connection != NULL) {
        result = t2connection->req333001(stock_id, exchange_type, entrust_price);
    }

    RETURN_ZVAL(result, 1, 0);
}

PHP_METHOD(T2Connection, p_req333002)
{
    T2Connection *t2connection;
    t2connection_object *obj = (t2connection_object *)zend_object_store_get_object(
        getThis() TSRMLS_CC);

    char *stock_id;
    uint stock_id_len;
    char *exchange_type;
    uint exchange_type_len;
    double entrust_amount;
    double entrust_price;
    char *entrust_bs;
    uint entrust_bs_len;
    char *entrust_prop;
    uint entrust_prop_len;


    //zend_parse_parameters 接受参数不全会导致Segmentation fault
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ssddss", &stock_id, &stock_id_len, &exchange_type, &exchange_type_len, &entrust_amount, &entrust_price, &entrust_bs, &entrust_bs_len, &entrust_prop, &entrust_prop_len) == FAILURE) {
        RETURN_NULL();
    }

    zval * result;
    t2connection = obj->t2connection;
    if (t2connection != NULL) {
        result = t2connection->req333002(stock_id, exchange_type, entrust_amount, entrust_price, entrust_bs[0], entrust_prop);
    }

    puts("return zval !!!");
    RETURN_ZVAL(result, 1, 0);
    //RETURN_NULL();
}

PHP_METHOD(T2Connection, p_req333104)
{
    T2Connection *t2connection;
    t2connection_object *obj = (t2connection_object *)zend_object_store_get_object(
        getThis() TSRMLS_CC);

    char *stock_id;
    uint stock_id_len;
    char *exchange_type;
    uint exchange_type_len;
    char *query_mode;
    uint query_mode_len;
    char *position_str;
    uint position_str_len;
    uint request_num;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ssssl", &stock_id, &stock_id_len, &exchange_type, &exchange_type_len, &query_mode, &query_mode_len, &position_str, &position_str_len, &request_num) == FAILURE) {
        RETURN_NULL();
    }

    zval * result;
    t2connection = obj->t2connection;
    if (t2connection != NULL) {
        result = t2connection->req333104(stock_id, exchange_type, query_mode[0], position_str, request_num);
    }

    RETURN_ZVAL(result, 1, 0);
    //RETURN_NULL();
}

PHP_METHOD(T2Connection, p_req333101)
{
    T2Connection *t2connection;
    t2connection_object *obj = (t2connection_object *)zend_object_store_get_object(
        getThis() TSRMLS_CC);

    char *sort_direction;
    uint sort_direction_len;
    int action_in;
    char *query_type;
    uint query_type_len;
    char *query_mode;
    uint query_mode_len;
    char *position_str;
    uint position_str_len;
    uint request_num;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "slsssl", &sort_direction, &sort_direction_len, &action_in, &query_type, &query_type_len, &query_mode, &query_mode_len, &position_str, &position_str_len, &request_num) == FAILURE) {
        RETURN_NULL();
    }

    zval * result;
    t2connection = obj->t2connection;
    if (t2connection != NULL) {
        result = t2connection->req333101(sort_direction[0], action_in, query_type[0], query_mode[0], position_str, request_num);
    }

    RETURN_ZVAL(result, 1, 0);
    //RETURN_NULL();
}

PHP_METHOD(T2Connection, p_req333102)
{
    T2Connection *t2connection;
    t2connection_object *obj = (t2connection_object *)zend_object_store_get_object(
        getThis() TSRMLS_CC);

    char *sort_direction;
    uint sort_direction_len;
    char *query_type;
    uint query_type_len;
    char *query_mode;
    uint query_mode_len;
    char *position_str;
    uint position_str_len;
    uint request_num;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ssssl", &sort_direction, &sort_direction_len, &query_type, &query_type_len, &query_mode, &query_mode_len, &position_str, &position_str_len, &request_num) == FAILURE) {
        RETURN_NULL();
    }

    zval * result;
    t2connection = obj->t2connection;
    if (t2connection != NULL) {
        result = t2connection->req333102(sort_direction[0], query_type[0], query_mode[0], position_str, request_num);
    }

    RETURN_ZVAL(result, 1, 0);
}

PHP_METHOD(T2Connection, p_req333103)
{
    T2Connection *t2connection;
    t2connection_object *obj = (t2connection_object *)zend_object_store_get_object(
        getThis() TSRMLS_CC);


    char *position_str;
    uint position_str_len;
    uint request_num;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sl", &position_str, &position_str_len, &request_num) == FAILURE) {
        RETURN_NULL();
    }

    zval * result;
    t2connection = obj->t2connection;
    if (t2connection != NULL) {
        result = t2connection->req333103(position_str, request_num);
    }

    RETURN_ZVAL(result, 1, 0);
}

PHP_METHOD(T2Connection, p_req333017)
{
    T2Connection *t2connection;
    t2connection_object *obj = (t2connection_object *)zend_object_store_get_object(
        getThis() TSRMLS_CC);
    
    int entrust_no;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &entrust_no) == FAILURE) {
        RETURN_NULL();
    }

    zval * result;
    t2connection = obj->t2connection;
    if (t2connection != NULL) {
        result = t2connection->req333017(entrust_no);
    }

    RETURN_ZVAL(result, 1, 0);
}

PHP_METHOD(T2Connection, p_req332200)
{
    T2Connection *t2connection;
    t2connection_object *obj = (t2connection_object *)zend_object_store_get_object(
        getThis() TSRMLS_CC);
    
    char *transfer_direction;
    uint transfer_direction_len;
    double occur_balance;
    char *fund_password;
    uint fund_password_len;
    char *bank_password;
    uint bank_password_len;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sdss", &transfer_direction, &transfer_direction_len, &occur_balance, &fund_password, &fund_password_len, &bank_password, &bank_password_len) == FAILURE) {
        RETURN_NULL();
    }

    zval * result;
    t2connection = obj->t2connection;
    if (t2connection != NULL) {
        result = t2connection->req332200(transfer_direction[0], occur_balance, fund_password, bank_password);
    }

    RETURN_ZVAL(result, 1, 0);
}

PHP_METHOD(T2Connection, p_req332250)
{
    T2Connection *t2connection;
    t2connection_object *obj = (t2connection_object *)zend_object_store_get_object(
        getThis() TSRMLS_CC);
    
    int entrust_no;
    int action_in;
    char *position_str;
    int position_str_len;
    int request_num;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "llsl", &entrust_no, &action_in, &position_str, &position_str_len, &request_num) == FAILURE) {
        RETURN_NULL();
    }

    zval * result;
    t2connection = obj->t2connection;
    if (t2connection != NULL) {
        result = t2connection->req332250(entrust_no, action_in, position_str, request_num);
    }

    RETURN_ZVAL(result, 1, 0);
}

PHP_METHOD(T2Connection, p_req332253)
{
    T2Connection *t2connection;
    t2connection_object *obj = (t2connection_object *)zend_object_store_get_object(
        getThis() TSRMLS_CC);
    
    char *fund_password;
    uint fund_password_len;
    char *bank_password;
    uint bank_password_len;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "llsl", &fund_password, &fund_password_len, &bank_password, &bank_password_len) == FAILURE) {
        RETURN_NULL();
    }

    zval * result;
    t2connection = obj->t2connection;
    if (t2connection != NULL) {
        result = t2connection->req332253(fund_password, bank_password);
    }

    RETURN_ZVAL(result, 1, 0);
}

PHP_METHOD(T2Connection, p_req332254)
{
    T2Connection *t2connection;
    t2connection_object *obj = (t2connection_object *)zend_object_store_get_object(
        getThis() TSRMLS_CC);
    
    zval * result;
    t2connection = obj->t2connection;
    if (t2connection != NULL) {
        result = t2connection->req332254();
    }

    RETURN_ZVAL(result, 1, 0);
}


zend_function_entry t2connection_methods[] = {
    PHP_ME(T2Connection,  __construct,     NULL, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
    PHP_ME(T2Connection,  p_connect,  NULL, ZEND_ACC_PUBLIC)
    PHP_ME(T2Connection,  p_disconnect,  NULL, ZEND_ACC_PUBLIC)
    PHP_ME(T2Connection,  p_login,  NULL, ZEND_ACC_PUBLIC)
    PHP_ME(T2Connection,  p_req330300,  NULL, ZEND_ACC_PUBLIC)
    PHP_ME(T2Connection,  p_req400,  NULL, ZEND_ACC_PUBLIC)
    PHP_ME(T2Connection,  p_req333001,  NULL, ZEND_ACC_PUBLIC)
    PHP_ME(T2Connection,  p_req333002,  NULL, ZEND_ACC_PUBLIC)
    PHP_ME(T2Connection,  p_req333104,  NULL, ZEND_ACC_PUBLIC)
    PHP_ME(T2Connection,  p_req333101,  NULL, ZEND_ACC_PUBLIC)
    PHP_ME(T2Connection,  p_req333102,  NULL, ZEND_ACC_PUBLIC)
    PHP_ME(T2Connection,  p_req333103,  NULL, ZEND_ACC_PUBLIC)
    PHP_ME(T2Connection,  p_req333017,  NULL, ZEND_ACC_PUBLIC)
    PHP_ME(T2Connection,  p_req332200,  NULL, ZEND_ACC_PUBLIC)
    PHP_ME(T2Connection,  p_req332250,  NULL, ZEND_ACC_PUBLIC)
    PHP_ME(T2Connection,  p_req332253,  NULL, ZEND_ACC_PUBLIC)
    PHP_ME(T2Connection,  p_req332254,  NULL, ZEND_ACC_PUBLIC)
    {NULL, NULL, NULL}
};

PHP_MINIT_FUNCTION(t2sdk)
{
	ZEND_INIT_MODULE_GLOBALS(t2sdk, php_t2sdk_init_globals,NULL);

    zend_class_entry ce;
    INIT_CLASS_ENTRY(ce, "T2Connection", t2connection_methods);
    t2connection_ce = zend_register_internal_class(&ce TSRMLS_CC);
    t2connection_ce->create_object = t2connection_create_handler;
    memcpy(&t2connection_object_handlers,
        zend_get_std_object_handlers(), sizeof(zend_object_handlers));
    t2connection_object_handlers.clone_obj = NULL;
    return SUCCESS;
}

PHP_RINIT_FUNCTION(t2sdk)
{
	T2SDK_G(g_pConnection) = NULL;
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
    PHP_RINIT(t2sdk),        /* RINIT */
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
