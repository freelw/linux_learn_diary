/*
#include "test_list.h"
#include "test_sort.h"
#include "test_overload.h"
#include "test_template.h"
*/

// x -> y
/*
{
    get(string key, out string val)

    imp_rpcx

    imp_http

    cache_layer
}
*/ 


// from X 
class Y_interface { // redis
public:
    Y_interface(class cache *_c = NULL):c(_c){}
    virtual int get(string s, string & out) {
        
    }
public:
    class cache *c;
};

class Y_imp_layer_http: public Y_interface {
public:
    virtual int get(string s, string & out) {
        // 
    }    
};

class Y_imp_layer_xxx: public Y_interface {
public:
    virtual int get(string s, string & out) {
        // 
    }    
};


#include "Y_imp_layer_cache.h"

int main()
{

    Y_interface * y_imp = new Y_imp_layer_http();
    string out;
    y_imp->get("a", out);
    Y_interface * y_imp1 = new Y_imp_layer_xxx();
    y_imp1->get("a", out);
    Y_interface * y_imp2 = new Y_imp_layer_cache();
    y_imp2->get("a", out);
    return 0;
}