#include <ruby.h>

#include <Options.h>
#include <Manager.h>
#include "Notification.h"

using namespace OpenZWave;

/*
 * Options
 */
static VALUE options_init(VALUE self, VALUE configPath, VALUE userPath, VALUE commandLine) {
    Check_Type(configPath, T_STRING);
    Check_Type(userPath, T_STRING);
    Check_Type(commandLine, T_STRING);
    
    Options::Create(RSTRING_PTR(configPath), RSTRING_PTR(userPath), RSTRING_PTR(commandLine));
    
    return self;
}

static VALUE options_add(VALUE self, VALUE name, VALUE value) {
    Check_Type(name, T_STRING);
        
    bool result = false;
    
    switch (rb_type(value)) {
        case T_FIXNUM:
            result = Options::Get()->AddOptionInt(RSTRING_PTR(name), FIX2INT(value));
            break;
        case T_TRUE:
            result = Options::Get()->AddOptionBool(RSTRING_PTR(name), true);
            break;
        case T_FALSE:
            result = Options::Get()->AddOptionBool(RSTRING_PTR(name), false);
            break;
        default:
            Check_Type(value, T_STRING);
            result = Options::Get()->AddOptionString(RSTRING_PTR(name), RSTRING_PTR(value), false);
            break;
    }
    
    return result ? Qtrue : Qfalse;
}

static VALUE options_get(VALUE self, VALUE name) {
    Check_Type(name, T_STRING);
    
    VALUE value;
    bool b;
    int32 i;
    string s;
    
    switch (Options::Get()->GetOptionType(RSTRING_PTR(name))) {
		case Options::OptionType_Bool: 
            if (Options::Get()->GetOptionAsBool(RSTRING_PTR(name), &b)) {
                value = b ? Qtrue : Qfalse;
            }
            break;
		case Options::OptionType_Int:
            if (Options::Get()->GetOptionAsInt(RSTRING_PTR(name), &i)) {
                value = INT2FIX(i);
            }
            break;
		case Options::OptionType_String:
            if (Options::Get()->GetOptionAsString(RSTRING_PTR(name), &s)) {
                value = rb_str_new2(s.c_str());
            }
            break;
    	case Options::OptionType_Invalid:
        default:
            value = Qnil;
            break;
    }
    
    return value;
}

static VALUE options_lock(VALUE self) {
    return Options::Get()->Lock() ? Qtrue : Qfalse;
}

static VALUE options_are_locked(VALUE self) {
    return Options::Get()->AreLocked() ? Qtrue : Qfalse;
}

/*
 *  Manager
 */

void OnNotification( Notification const* _notification, void* _context ) {
    VALUE self = (VALUE)_context;
    
    VALUE notification = rb_hash_new();
    rb_hash_aset(notification, rb_str_new2("home_id"), INT2FIX(_notification->GetHomeId()));
    rb_hash_aset(notification, rb_str_new2("node_id"), INT2FIX(_notification->GetNodeId()));
    rb_hash_aset(notification, rb_str_new2("byte"), INT2FIX(_notification->GetByte()));
    // rb_hash_aset(notification, rb_str_new2("value_id"), INT2FIX(_notification->GetValueID()));
    
    static const char * type_name[] = {
        "value_added", "value_removed", "value_changed", "value_refreshed", "group", "node_new",						
        "node_added", "node_removed", "node_protocol_info", "node_naming", "node_event",						
        "polling_disabled", "polling_enabled", "scene_event", "create_button", "delete_button", 
        "button_on", "button_off", "driver_ready", "driver_failed", "driver_reset", 
        "essential_node_queries_complete", "node_queries_complete", "awake_nodes_queried", 
        "all_nodes_queried_some_dead", "all_nodes_queried", "notification", "driver_removed"					
    };
    
    static const char * value_genre_name[] = {
        "basic", "user", "config", "system", "count"
    };
    
    static const char * value_type_name[] = {
        "bool", "byte", "decimal", "int", "list", "schedule", "short", "string", "button", "raw"
    };
    
    ID type, genre, value_type;
    CONST_ID(type, type_name[_notification->GetType()]);

    rb_hash_aset(notification, rb_str_new2("type"), type);
    rb_hash_aset(notification, rb_str_new2("genre"), genre);
    rb_hash_aset(notification, rb_str_new2("value_type"), value_type);
}

static VALUE manager_init(VALUE self) {
    Manager::Create();
	Manager::Get()->AddWatcher( OnNotification, (void*)self );
    return self;
}

extern "C" {
    VALUE mOpenZWave;
    VALUE cOptions;
    VALUE cManager;

    static void Init_OpenZWave_Options() {
        cOptions = rb_define_class_under(mOpenZWave, "Options", rb_cObject);
        rb_define_method(cOptions, "initialize", RUBY_METHOD_FUNC(options_init), 3);
        rb_define_method(cOptions, "add", RUBY_METHOD_FUNC(options_add), 2);
        rb_define_method(cOptions, "get", RUBY_METHOD_FUNC(options_get), 1);
        rb_define_method(cOptions, "lock", RUBY_METHOD_FUNC(options_lock), 0);
        rb_define_method(cOptions, "are_locked", RUBY_METHOD_FUNC(options_are_locked), 0);
    }

    static void Init_OpenZWave_Manager() {
        cManager = rb_define_class_under(mOpenZWave, "Manager", rb_cObject);
        rb_define_method(cManager, "initialize", RUBY_METHOD_FUNC(manager_init), 0);
    }

    void Init_OpenZWave()
    {
        mOpenZWave = rb_define_module("OpenZWave");
        
        Init_OpenZWave_Options();
        Init_OpenZWave_Manager();
    }

} // extern "C"
