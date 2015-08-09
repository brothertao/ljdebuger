#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include "ljdebuger.h"

void log_params(char *class_name, char *function_name);

ZEND_DLEXPORT void ljdebuger_zend_activate (void)
{
	//nothing
}


ZEND_DLEXPORT int ljdebuger_zend_startup (zend_extension * extension)
{
#if(PHP_MAJOR_VERSION == 5 && PHP_MINOR_VERSION >= 3)
	CG (compiler_options) |= ZEND_COMPILE_EXTENDED_INFO;
#else
	CG (extended_info) = 1;
#endif
	return SUCCESS;
}

ZEND_DLEXPORT void ljdebuger_zend_shutdown (zend_extension * extension)
{
	//nothing
}

ZEND_DLEXPORT void ljdebuger_zend_deactivate (void)
{
	//nothing
}

ZEND_DLEXPORT void ljdebuger_zend_fcall_begin (zend_op_array * op_array)
{
	//nothing
}

ZEND_DLEXPORT void ljdebuger_zend_fcall_end (zend_op_array * op_array)
{
	//nothing
}

ZEND_DLEXPORT void ljdebuger_statement_call (zend_op_array * op_array)
{
	char *class_name, *function_name;
	zend_class_entry *scope;
	scope = EG (scope);
	if (scope) {
		class_name = scope->name;
		//MOVEN_LOG("class name::%s\n", scope->name);
	} else {
		class_name = NULL;
	}

	zend_op_array *op;
	op = EG (active_op_array);

	if (op && op->function_name) {
		function_name = op->function_name;
		//MOVEN_LOG("function name::%s\n", op->function_name);
	} else {
		function_name = NULL;
	}

	if (!EG (active_symbol_table)) {
		zend_rebuild_symbol_table (TSRMLS_C);
	}

	log_params(class_name, function_name);
}

void log_params(char *class_name, char *function_name)
{
	if (!class_name && !function_name)
	{
		return;
	}
	if (!function_name)
	{
		return;
	}
	HashTable *ht;
	zval **data;
	zval *tmp;
	char *skey;
	ulong ikey;
	int i;
	smart_str buf = {0};
	long options = 1;

	ht = EG(active_symbol_table);

	zend_hash_internal_pointer_reset (ht);
	int count = zend_hash_num_elements (ht);
	for (i = 0; i < count; i++) {
		zend_hash_get_current_data (ht, (void **) &data);
		zend_hash_get_current_key (ht, &skey, &ikey, 0);
		zend_hash_move_forward (ht);
		php_json_encode(&buf, *data, options TSRMLS_CC);
		MOVEN_LOG("[class_name::%s] [function_name::%s] [json_string::%s]\n", class_name, function_name, buf.c);
		smart_str_free(&buf);
	}
}
ZEND_DLEXPORT void ljdebuger_oparray_init (zend_op_array * op_array)
{
	//nothing
}

ZEND_EXTENSION ();
ZEND_DLEXPORT zend_extension zend_extension_entry = {
	LJDEBUGER_NAME,
	LJDEBUGER_VERSION,
	LJDEBUGER_AUTHOR,
	"",
	"",
	ljdebuger_zend_startup,
	ljdebuger_zend_shutdown,
	ljdebuger_zend_activate,			/* activate_func_t */
	ljdebuger_zend_deactivate,		/* deactivate_func_t */
	NULL,						/* message_handler_func_t */
	NULL,						/* op_array_handler_func_t */// we may use it
	ljdebuger_statement_call,			/* statement_handler_func_t */
	ljdebuger_zend_fcall_begin,		/* fcall_begin_handler_func_t */
	ljdebuger_zend_fcall_end,			/* fcall_end_handler_func_t */
	ljdebuger_oparray_init,			/* op_array_ctor_func_t */
	NULL,						/* op_array_dtor_func_t */
	STANDARD_ZEND_EXTENSION_PROPERTIES
};
