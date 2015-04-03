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
    
	switch( _notification->GetType() )
	{
		case Notification::Type_ValueAdded:
		{
            break;
		}

		case Notification::Type_ValueRemoved:
		{
			break;
		}

		case Notification::Type_ValueChanged:
		{
			break;
		}

		case Notification::Type_Group:
		{
			break;
		}

		case Notification::Type_NodeAdded:
		{
			break;
		}

		case Notification::Type_NodeRemoved:
		{
			break;
		}

		case Notification::Type_NodeEvent:
		{
			break;
		}

		case Notification::Type_PollingDisabled:
		{
			break;
		}

		case Notification::Type_PollingEnabled:
		{
			break;
		}

		case Notification::Type_DriverReady:
		{
			break;
		}

		case Notification::Type_DriverFailed:
		{
			break;
		}

		case Notification::Type_AwakeNodesQueried:
		case Notification::Type_AllNodesQueried:
		case Notification::Type_AllNodesQueriedSomeDead:
		{
			break;
		}

		case Notification::Type_DriverReset:
		case Notification::Type_Notification:
		case Notification::Type_NodeNaming:
		case Notification::Type_NodeProtocolInfo:
		case Notification::Type_NodeQueriesComplete:
		default:
		{
		}
	}
    
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
