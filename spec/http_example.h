/**
 * @file http_example.h
 * @copyright Copyright (C) 2013-2016, The MetaCurrency Project (Eric Harris-Braun, Arthur Brock, et. al).  This file is part of the Ceptr platform and is released under the terms of the license contained in the file LICENSE (GPLv3).
 * @ingroup tests
 */

#ifndef _CEPTR_TEST_HTTP_EXAMPLE_H
#define _CEPTR_TEST_HTTP_EXAMPLE_H

#include "../src/ceptr.h"
#include "../src/receptor.h"
#include "../src/protocol.h"
#include "../src/def.h"
#include "../src/semtrex.h"
#include "spec_utils.h"
#include "../src/vmhost.h"
#include "../src/accumulator.h"
#include <unistd.h>

//! [makeTestHTTPRequestTree]
/**
 * generate a test semantic tree that represents an HTTP request
 *
 * @snippet spec/http_example.h makeTestHTTPRequestTree
 */
T *_makeTestHTTPRequestTree() {
    T *t = _t_clone(G_http_req_example);

    // confirm that we built the request right!
     T *stx = _d_build_def_semtrex(G_sem,HTTP_REQUEST,0);
     puts(t2s(stx));
     puts(t2s(t));
     stx = _t_parse(G_sem,0,"(SEMTREX_SYMBOL_LITERAL (SEMTREX_SYMBOL:HTTP_REQUEST) (SEMTREX_SEQUENCE (SEMTREX_SYMBOL_LITERAL (SEMTREX_SYMBOL:HTTP_REQUEST_LINE)) (SEMTREX_SYMBOL_LITERAL (SEMTREX_SYMBOL:HTTP_REQUEST_HEADERS) (SEMTREX_SEQUENCE (SEMTREX_ZERO_OR_MORE (SEMTREX_SYMBOL_LITERAL (SEMTREX_SYMBOL:CONTENT_ENCODING))))) (SEMTREX_SYMBOL_LITERAL (SEMTREX_SYMBOL:HTTP_REQUEST_BODY))))");
     if (!_t_match(stx,t)) {raise_error("BAD HTTP_REQUEST semtrex");}
     _t_free(stx);

     // all our examples are expecting the HTTP_REQUEST_LINE
     T *c =  _t_detach_by_idx(t,1);
     _t_free(t);
     return c;
}
//! [makeTestHTTPRequestTree]


//! [makeTestHTTPResponseProcess]
/**
 * generate a test process that would generate a response to an HTTP request
 *
 * @snippet spec/http_example.h makeTestHTTPResponseProcess
 */
void _makeTestHTTPResponseProcess(Receptor *r,T **paramsP,Process *pP) {
    // the action simply responds back with the method that was originally sent
    // this test should be made more real... but for now it responds back with a ping
    // like message that contains what the first path segment was

    T *resp = _t_new_root(RESPOND);
    _t_news(resp,CARRIER,HTTP_RESPONSE);
    int pt1[] = {2,1,TREE_PATH_TERMINATOR};
    _t_new(resp,PARAM_REF,pt1,sizeof(int)*3);

    Process p = _r_define_process(r,resp,"code path ping","respond with the value of the first parameter",NULL,NULL);
    *pP = p;

    T *params = _t_build(G_sem,0,
                         PARAMS,
                         HTTP_RESPONSE,
                         HTTP_RESPONSE_STATUS,STATUS_VALUE,200,STATUS_TEXT,"OK",NULL_SYMBOL,
                         HTTP_HEADERS,
                         CONTENT_TYPE,MEDIA_TYPE_IDENT,TEXT_MEDIA_TYPE,MEDIA_SUBTYPE_IDENT,CEPTR_TEXT_MEDIA_SUBTYPE,NULL_SYMBOL,NULL_SYMBOL,
                         HTTP_RESPONSE_BODY,
                         SLOT,USAGE,HTTP_REQUEST_PATH_SEGMENT,NULL_SYMBOL,
                         NULL_SYMBOL,NULL_SYMBOL,NULL_SYMBOL
                         );


    /*     _t_new_root(PARAMS); */
    /* T *http_resp = _t_newr(params,HTTP_RESPONSE); */
    /* _t_new(http_resp,HTTP_RESPONSE_CONTENT_TYPE,"CeptrSymbol/HTTP_REQUEST_PATH_SEGMENT",38); */
    /* T *s = _t_newr(http_resp,SLOT); */
    /* _t_news(s,USAGE,HTTP_REQUEST_PATH_SEGMENT); */
    *paramsP = params;
}
//! [makeTestHTTPResponseProcess]

char *G_stxs = "/ASCII_CHARS/<HTTP_REQUEST:<HTTP_REQUEST_METHOD:ASCII_CHAR!=' '+>,ASCII_CHAR=' ',<HTTP_REQUEST_PATH:<HTTP_REQUEST_PATH_SEGMENTS:(ASCII_CHAR='/',<HTTP_REQUEST_PATH_SEGMENT:ASCII_CHAR!={'/','?',' '}*>)+>>,(ASCII_CHAR='?',<HTTP_REQUEST_PATH_QUERY:<HTTP_REQUEST_PATH_QUERY_PARAMS:(<HTTP_REQUEST_PATH_QUERY_PARAM:<PARAM_KEY:ASCII_CHAR!={'&',' ','='}+>,ASCII_CHAR='=',<PARAM_VALUE:ASCII_CHAR!={'&',' '}*>>,ASCII_CHAR='&'?)+>+>)?,ASCII_CHAR=' ',ASCII_CHAR='H',ASCII_CHAR='T',ASCII_CHAR='T',ASCII_CHAR='P',ASCII_CHAR='/',<HTTP_REQUEST_VERSION:<VERSION_MAJOR:ASCII_CHAR='0'>,ASCII_CHAR='.',<VERSION_MINOR:ASCII_CHAR='9'>>>";

/**
 * generate a semtrex that will match a valid HTTP request in an ASCII_CHARS tree
 *
 */
T *_makeHTTPRequestSemtrex() {
    T *t = parseSemtrex(G_sem,G_stxs);
    return t;

    T *stx= _sl(0,ASCII_CHARS);
    t = _t_news(stx,SEMTREX_GROUP,HTTP_REQUEST);
    T *sq = _t_newr(t,SEMTREX_SEQUENCE);
    t = _t_news(sq,SEMTREX_GROUP,HTTP_REQUEST_METHOD);
    t = _t_newr(t,SEMTREX_ONE_OR_MORE);
    __stxcvm(t,1,1,' ');  //!= ' '
    __stxcv(sq,' ');

    t = _t_news(sq,SEMTREX_GROUP,HTTP_REQUEST_PATH);
    T *sqq = _t_newr(t,SEMTREX_SEQUENCE);

    t = _t_news(sqq,SEMTREX_GROUP,HTTP_REQUEST_PATH_SEGMENTS);
    t = _t_newr(t,SEMTREX_ONE_OR_MORE);
    t = _t_newr(t,SEMTREX_SEQUENCE);
    __stxcv(t,'/');
    t = _t_news(t,SEMTREX_GROUP,HTTP_REQUEST_PATH_SEGMENT);
    t = _t_newr(t,SEMTREX_ZERO_OR_MORE);
    __stxcvm(t,1,3,'/','?',' ');

    /* t = _t_news(sq,SEMTREX_GROUP,HTTP_REQUEST_PATH_FILE); */
    /* T *f = _t_newr(t,SEMTREX_SEQUENCE); */
    /* t = _t_news(f,SEMTREX_GROUP,FILE_NAME); */
    /* t = _t_newr(t,SEMTREX_ONE_OR_MORE); */
    /* __stxcvm(t,1,2,'?',' '); */

    /* t = _t_newr(f,SEMTREX_ZERO_OR_ONE); */
    /* t = _t_newr(t,SEMTREX_SEQUENCE); */
    /* __stxcv(t,'.'); */
    /* t = _t_news(t,SEMTREX_GROUP,FILE_EXTENSION); */
    /* t = _t_newr(t,SEMTREX_ONE_OR_MORE); */
    /* __stxcvm(t,1,2,'?',' '); */

    t = _t_newr(sq,SEMTREX_ZERO_OR_ONE);
    t = _t_newr(t,SEMTREX_SEQUENCE);
    __stxcv(t,'?');

    T *f;
    t = _t_news(t,SEMTREX_GROUP,HTTP_REQUEST_PATH_QUERY);
    t = _t_newr(t,SEMTREX_ONE_OR_MORE);
    t = _t_news(t,SEMTREX_GROUP,HTTP_REQUEST_PATH_QUERY_PARAMS);
    t = _t_newr(t,SEMTREX_ONE_OR_MORE);
    T *qps = _t_newr(t,SEMTREX_SEQUENCE);
    t = _t_news(qps,SEMTREX_GROUP,HTTP_REQUEST_PATH_QUERY_PARAM);
    f = _t_newr(t,SEMTREX_SEQUENCE);
    t = _t_news(f,SEMTREX_GROUP,PARAM_KEY);
    t = _t_newr(t,SEMTREX_ONE_OR_MORE);
    __stxcvm(t,1,3,'&',' ','=');
    __stxcv(f,'=');
    t = _t_news(f,SEMTREX_GROUP,PARAM_VALUE);
    t = _t_newr(t,SEMTREX_ZERO_OR_MORE);
    __stxcvm(t,1,2,'&',' ');

    t = _t_newr(qps,SEMTREX_ZERO_OR_ONE);
    __stxcv(t,'&');

    __stxcv(sq,' ');

    __stxcv(sq,'H');
    __stxcv(sq,'T');
    __stxcv(sq,'T');
    __stxcv(sq,'P');
    __stxcv(sq,'/');
    t = _t_news(sq,SEMTREX_GROUP,HTTP_REQUEST_VERSION);
    t = _t_newr(t,SEMTREX_SEQUENCE);
    f = _t_news(t,SEMTREX_GROUP,VERSION_MAJOR);
    __stxcv(f,'0');
    __stxcv(t,'.');
    f = _t_news(t,SEMTREX_GROUP,VERSION_MINOR);
    __stxcv(f,'9');
    return stx;
}

T *makeDeltaAdd(T *src,T *t) {
    int *path = _t_get_path(src);
    T *d = makeDelta(TREE_DELTA_ADD,path,t,0);
    free(path);
    return d;
}

Symbol getTag(char *otag,Symbol tag_sym[],char *tag_str[]) {
    Symbol ts = NULL_SYMBOL;
    int i;
    for(i=0;i<15;i++) {
    if (!strcicmp(otag,tag_str[i])) {ts = tag_sym[i];break;}
    }
    if (semeq(ts,NULL_SYMBOL)) {raise_error("invalid tag: %s",otag);}
    return ts;
}

T *parseHTML(char *html) {

    Symbol G_tag_sym[] = {HTML_HTML,HTML_HEAD,HTML_TITLE,HTML_BODY,HTML_DIV,HTML_P,HTML_A,HTML_B,HTML_UL,HTML_OL,HTML_LI,HTML_SPAN,HTML_H1,HTML_H2,HTML_H3,HTML_H4,HTML_FORM};
    char *G_tag_str[] ={"html","head","title","body","div","p","a","b","ul","ol","li","span","h1","h2","h3","h4","form"};

    Symbol G_stag_sym[] = {HTML_IMG,HTML_INPUT,HTML_BUTTON};
    char *G_stag_str[] ={"img","input","button"};

    T *t,*h = makeASCIITree(html);

    /////////////////////////////////////////////////////
    // build the token stream out of an ascii stream
    // PATTERN
    T *s;
    char *stx = "/ASCII_CHARS/<HTML_TOKENS:(ASCII_CHAR='<',<HTML_TOK_TAG_SELFCLOSE:ASCII_CHAR!={'>',' '}+>,<HTML_ATTRIBUTES:(ASCII_CHAR=' ',<HTML_ATTRIBUTE:<PARAM_KEY:ASCII_CHAR!={'>',' ','='}+>,ASCII_CHAR='=',ASCII_CHAR='\"',<PARAM_VALUE:ASCII_CHAR!='\"'+>,ASCII_CHAR='\"'>)*>,ASCII_CHAR='/',ASCII_CHAR='>'|ASCII_CHAR='<',ASCII_CHAR='/',<HTML_TOK_TAG_CLOSE:ASCII_CHAR!='>'+>,ASCII_CHAR='>'|ASCII_CHAR='<',<HTML_TOK_TAG_OPEN:ASCII_CHAR!={'>',' '}+>,<HTML_ATTRIBUTES:(ASCII_CHAR=' ',<HTML_ATTRIBUTE:<PARAM_KEY:ASCII_CHAR!={'>',' ','='}+>,ASCII_CHAR='=',ASCII_CHAR='\"',<PARAM_VALUE:ASCII_CHAR!='\"'+>,ASCII_CHAR='\"'>)*>,ASCII_CHAR='>'|<HTML_TEXT:ASCII_CHAR!='<'+>)+>";

    s = parseSemtrex(G_sem,stx);

    int fnc = 0;
    wjson(G_sem,s,"htmlparse",-1);
    wjson(G_sem,h,"htmlascii",-1);

    T *results,*tokens;
    if (_t_matchr(s,h,&results)) {
    tokens = _t_new_root(HTML_TOKENS);
    int i,m = _t_children(results);
    wjson(G_sem,tokens,"html",fnc++);
    T *delta,*src;
    for(i=4;i<=m;i++) {
        T *c = _t_child(results,i);
        T *sn = _t_child(c,1);
        Symbol ts = *(Symbol *)_t_surface(sn);
        if (semeq(ts,HTML_ATTRIBUTES)) {
        T *a = _t_new_root(HTML_ATTRIBUTES);
        int j,ac = _t_children(c);
        for(j=4;j<=ac;j++) {
            T *attr = _t_newr(a,HTML_ATTRIBUTE);
            T *at = _t_child(c,j);
            T *m = _t_get_match(at,PARAM_KEY);
            asciiT_tos(h,m,attr,PARAM_KEY);
            m = _t_get_match(at,PARAM_VALUE);
            asciiT_tos(h,m,attr,PARAM_VALUE);
        }
        // we can just add the attribute directly to the previous token which will be the open tag tokens
        src = _t_child(tokens,_t_children(tokens));
        delta = a;
        _t_add(src,a);
        }
        else {
        src = tokens;
        delta = asciiT_tos(h,c,tokens,ts);
        }
        delta = makeDeltaAdd(src,delta);
        wjson(G_sem,delta,"html",fnc++);
        _t_free(delta);
    }
    _t_free(results);
    _t_free(s);

    s = _t_new_root(SEMTREX_WALK);
    //  T *st = _t_newr(sq,SEMTREX_ZERO_OR_MORE);
    //  _t_newr(st,SEMTREX_SYMBOL_ANY);
    T *g =  _t_news(s,SEMTREX_GROUP,HTML_TAG);
    T *sq = _t_newr(g,SEMTREX_SEQUENCE);
    _sl(sq,HTML_TOK_TAG_OPEN);
    g = _t_news(sq,SEMTREX_GROUP,HTML_CONTENT);
    T* st = _t_newr(g,SEMTREX_ZERO_OR_MORE);
    __sl(st,1,2,HTML_TOK_TAG_OPEN,HTML_TOK_TAG_CLOSE);
    _sl(sq,HTML_TOK_TAG_CLOSE);

    //  stx = "%<HTML_TAG:HTML_TOK_TAG_OPEN,!{HTML_TOK_TAG_OPEN,HTML_TOK_TAG_CLOSE},HTML_TOK_TAG_CLOSE>";
    //s = parseSemtrex(d,stx);
    //  return tokens;
    while (_t_matchr(s,tokens,&results)) {
        T *m = _t_get_match(results,HTML_TAG);
        int *path = _t_surface(_t_child(m,2));
        int count = *(int *)_t_surface(_t_child(results,3));
        T *ot = _t_get(tokens,path);
        path[_t_path_depth(path)-1] += count-1;
        T *ct = _t_get(tokens,path);
        char *otag = _t_surface(ot);
        char *ctag = _t_surface(ct);
        if (strcmp(otag,ctag)) {raise_error("Mismatched tags %s,%s",otag,ctag)};

        Symbol ts = getTag(otag,G_tag_sym,G_tag_str);
        path[_t_path_depth(path)-1] -= count-1;
        T *content = wrap(tokens,results,HTML_CONTENT,HTML_TAG);
        T *attributes = _t_detach_by_idx(content,1);
        __t_morph(content,HTML_CONTENT,0,0,0);
        T *p = _t_parent(content);
        _t_detach_by_ptr(p,content);
        T *tag = _t_new_root(ts);
        _t_add(tag,attributes);
        _t_add(tag,content);
        _t_insert_at(tokens,path,tag);
        delta = makeDelta(TREE_DELTA_REPLACE,path,tag,count);
        wjson(G_sem,delta,"html",fnc++);
        _t_free(delta);
        _t_free(results);
    }
    _t_free(s);

    s = _t_new_root(SEMTREX_WALK);
    g = _t_news(s,SEMTREX_GROUP,HTML_TAG);
    _sl(g,HTML_TOK_TAG_SELFCLOSE);
    while (_t_matchr(s,tokens,&results)) {
        T *m = _t_get_match(results,HTML_TAG);
        int *path = _t_surface(_t_child(m,2));
        T *t = _t_get(tokens,path);
        char *otag = _t_surface(t);
        Symbol ts = getTag(otag,G_stag_sym,G_stag_str);
        __t_morph(t,ts,0,0,0);
        _t_newr(t,HTML_CONTENT);
        delta = makeDelta(TREE_DELTA_REPLACE,path,t,1);
        wjson(G_sem,delta,"html",fnc++);
        _t_free(delta);
        _t_free(results);

    }
    _t_free(s);
    results = _t_detach_by_idx(tokens,1);
    _t_free(tokens);
    _t_free(h);
    return results;
    }
    raise_error("HTML doesn't match");
}

void testHTTPparseHTML() {
    T *t = parseHTML("<html><body><div>Hello <b>world!</b></div></body></html>");

    spec_is_str_equal(t2s(t),"(HTML_HTML (HTML_ATTRIBUTES) (HTML_CONTENT (HTML_BODY (HTML_ATTRIBUTES) (HTML_CONTENT (HTML_DIV (HTML_ATTRIBUTES) (HTML_CONTENT (HTML_TEXT:Hello ) (HTML_B (HTML_ATTRIBUTES) (HTML_CONTENT (HTML_TEXT:world!)))))))))");

    _t_free(t);

}
//Host: fish.com\n\n
void *_httptester(void *arg) {
    char *result = doSys("echo 'GET /path/to/file.ext HTTP/0.9' | nc localhost 8888");
    spec_is_str_equal(result,"HTTP/1.1 200 OK\nContent-Type: text/ceptr\n\nsuper cept\n314159\n");
    free(result);
    G_done = true;
    pthread_exit(NULL);
}

void testHTTPedgeReceptor() {

    //setup vmhost instance
    VMHost *v = G_vm = _v_new();
    SemTable *gsem = G_sem;
    G_sem = v->sem;
    //    _v_instantiate_builtins(G_vm);

    Symbol HTTP;
    _sem_get_by_label(G_sem,"HTTP",&HTTP);

    T *http_def = _sem_get_def(G_sem,HTTP);

    // check the HTTP protocol definition
    spec_is_str_equal(t2s(http_def),"(PROTOCOL_DEFINITION (PROTOCOL_LABEL (ENGLISH_LABEL:HTTP)) (PROTOCOL_SEMANTICS (GOAL:HTTP_REQUEST_HANDLER)) (INCLUSION (PNAME:REQUESTING) (LINKAGE (WHICH_ROLE (ROLE:REQUESTER) (ROLE:HTTP_CLIENT))) (LINKAGE (WHICH_ROLE (ROLE:RESPONDER) (ROLE:HTTP_SERVER))) (RESOLUTION (WHICH_SYMBOL (USAGE:REQUEST_TYPE) (ACTUAL_SYMBOL:HTTP_REQUEST))) (RESOLUTION (WHICH_SYMBOL (USAGE:RESPONSE_TYPE) (ACTUAL_SYMBOL:HTTP_RESPONSE))) (RESOLUTION (WHICH_SYMBOL (USAGE:CHANNEL) (ACTUAL_SYMBOL:HTTP_ASPECT))) (RESOLUTION (WHICH_PROCESS (GOAL:REQUEST_HANDLER) (ACTUAL_PROCESS:httpresp)))))");

    T *sem_map = _t_new_root(SEMANTIC_MAP);
    T *t = _o_unwrap(G_sem,http_def,sem_map);
    // and also check how it gets unwrapped because it's defined in terms of REQUESTING
    // @todo, how come HTTP_SERVER and the two handler goals aren't added to the semantics?
    spec_is_str_equal(t2s(t),"(PROTOCOL_DEFINITION (PROTOCOL_LABEL (ENGLISH_LABEL:HTTP)) (PROTOCOL_SEMANTICS (GOAL:HTTP_REQUEST_HANDLER) (ROLE:HTTP_CLIENT) (USAGE:RESPONSE_HANDLER_PARAMETERS)) (PROTOCOL_DEFAULTS (SEMANTIC_LINK (USAGE:RESPONSE_HANDLER_PARAMETERS) (REPLACEMENT_VALUE (NULL_SYMBOL)))) (backnforth (INITIATE (ROLE:HTTP_CLIENT) (DESTINATION (ROLE:HTTP_SERVER)) (ACTION:send_request)) (EXPECT (ROLE:HTTP_SERVER) (SOURCE (ROLE:HTTP_CLIENT)) (PATTERN (SEMTREX_SYMBOL_LITERAL (SEMTREX_SYMBOL:HTTP_REQUEST))) (ACTION:send_response))))");

    // the unwrapping should build up a semantic map
    spec_is_str_equal(t2s(sem_map),"(SEMANTIC_MAP (SEMANTIC_LINK (ROLE:REQUESTER) (REPLACEMENT_VALUE (ROLE:HTTP_CLIENT))) (SEMANTIC_LINK (ROLE:RESPONDER) (REPLACEMENT_VALUE (ROLE:HTTP_SERVER))) (SEMANTIC_LINK (USAGE:REQUEST_TYPE) (REPLACEMENT_VALUE (ACTUAL_SYMBOL:HTTP_REQUEST))) (SEMANTIC_LINK (USAGE:RESPONSE_TYPE) (REPLACEMENT_VALUE (ACTUAL_SYMBOL:HTTP_RESPONSE))) (SEMANTIC_LINK (USAGE:CHANNEL) (REPLACEMENT_VALUE (ACTUAL_SYMBOL:HTTP_ASPECT))) (SEMANTIC_LINK (GOAL:REQUEST_HANDLER) (REPLACEMENT_VALUE (ACTUAL_PROCESS:httpresp))))");
    _t_free(sem_map);

    // create empty edge receptor
    Receptor *r = _r_makeStreamEdgeReceptor(v->sem);
    // instantiate it in the vmhost
    Xaddr edge = _v_new_receptor(v,v->r,STREAM_EDGE,r);
    // set up a socket listener that will transcode ascii to HTTP_REQUEST and send all the received requests to an HTTP aspect on the same receptor
    T *code = _t_parse(r->sem,0,"(CONVERSE (SCOPE (ITERATE (PARAMS) (STREAM_ALIVE (PARAM_REF:/2/1)) (INITIATE_PROTOCOL (PNAME:HTTP) (WHICH_INTERACTION:backnforth) (PROTOCOL_BINDINGS (RESOLUTION (WHICH_RECEPTOR (ROLE:HTTP_CLIENT) %)) (RESOLUTION (WHICH_RECEPTOR (ROLE:HTTP_SERVER) %)) (RESOLUTION (WHICH_PROCESS (GOAL:RESPONSE_HANDLER) (ACTUAL_PROCESS:echo2stream))) (RESOLUTION (WHICH_USAGE (USAGE:RESPONSE_HANDLER_PARAMETERS) (ACTUAL_VALUE (PARAM_REF:/2/1)))) (RESOLUTION (WHICH_VALUE (ACTUAL_SYMBOL:HTTP_REQUEST) (ACTUAL_VALUE (STREAM_READ (PARAM_REF:/2/1) (RESULT_SYMBOL:HTTP_REQUEST)))))) ) ) (STREAM_CLOSE (PARAM_REF:/2/1))) (BOOLEAN:1))",__r_make_addr(0,ACTUAL_RECEPTOR,r->addr),__r_make_addr(0,ACTUAL_RECEPTOR,r->addr));
    // add an error handler that just completes the iteration
    T *err_handler = _t_parse(r->sem,0,"(CONTINUE (POP_PATH (PARAM_REF:/4/1/1) (RESULT_SYMBOL:CONTINUE_LOCATION) (POP_COUNT:5)) (CONTINUE_VALUE (BOOLEAN:0)))");

    SocketListener *l = _r_addListener(r,8888,code,0,err_handler,DELIM_LF);
    _v_activate(v,edge);

    T *bindings = _t_new_root(PROTOCOL_BINDINGS);
    T *res = _t_newr(bindings,RESOLUTION);
    T *w = _t_newr(res,WHICH_RECEPTOR);
    _t_news(w,ROLE,HTTP_SERVER);
    __r_make_addr(w,ACTUAL_RECEPTOR,r->addr);
    res = _t_newr(bindings,RESOLUTION);
    w = _t_newr(res,WHICH_PROCESS);
    _t_news(w,GOAL,HTTP_REQUEST_HANDLER);
    _t_news(w,ACTUAL_PROCESS,fill_i_am);

    _o_express_role(r,HTTP,HTTP_SERVER,HTTP_ASPECT,bindings);
    _t_free(bindings);

    //    debug_enable(D_TRANSCODE+D_STEP+D_STREAM);
    debug_enable(D_SIGNALS+D_STEP+D_STREAM);
    _v_start_vmhost(G_vm);

    G_done = false;
    pthread_t thread;
    int rc;
    rc = pthread_create(&thread,0,_httptester,NULL);
    if (rc){
        raise_error("ERROR; return code from pthread_create() is %d\n", rc);
    }
    pthread_detach(thread);
    if (rc){
        raise_error("Error detaching tester thread; return code from pthread_detach() is %d\n", rc);
    }
    while(!G_done) sleepms(1);
    sleepms(1);
    debug_disable(D_STREAM+D_SIGNALS+D_TREE+D_PROTOCOL);
    debug_disable(D_TRANSCODE+D_REDUCE+D_REDUCEV+D_STEP);

    //spec_is_str_equal(t2s(r->flux),"");

    // cleanup vmhost instance
    __r_kill(G_vm->r);
    //    _v_join_thread(&G_vm->clock_thread);
    _v_join_thread(&G_vm->vm_thread);

    _v_free(v);
    G_vm = NULL;
    G_sem = gsem;

}


void testHTTP() {
    testHTTPparseHTML();
    testHTTPedgeReceptor();
}

#endif
//
