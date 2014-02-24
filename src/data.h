#include "ceptr.h"


typedef size_t (*sizeFunction)(Receptor *, Symbol, void *);

sizeFunction size_table[BUFFER_SIZE];

sizeFunction size_table_get(Symbol noun) {
    if (size_table[noun] == 0) {
        raise_error("null size fn for noun %d \n", noun);
    }
    return size_table[noun];
}

int data_sem_check(Receptor *r, Xaddr xaddr) {
    if (xaddr.key < 0) {
        if (xaddr.noun == -4) {
            return 1;
        }
        if (util_xaddr_eq(xaddr, r->cspecXaddr)) {
            return 1;
        }
    }
    Symbol noun = r->data.xaddr_scape[xaddr.key];
    return (noun == xaddr.noun);
}

void *_data_get(Data *d,int key) {
    return _t_get_child_surface(d->root,key);
}

void *data_get(Receptor *r, Xaddr xaddr) {
    if (util_xaddr_eq(xaddr, r->rootXaddr)){
        return &r->rootSurface;
    }
    if (!data_sem_check(r, xaddr)) {
        raise_error2("semcheck failed getting xaddr { %d, %d }\n", xaddr.key, xaddr.noun );
    }
    return _data_get(&r->data,xaddr.key);
}

void size_table_set(Symbol noun, sizeFunction func) {
    size_table[noun] = func;
}

void _data_record_existence(Data *d, size_t current_index, Symbol noun) {
    d->current_xaddr++;
    d->xaddrs[d->current_xaddr].key = current_index;
    d->xaddrs[d->current_xaddr].noun = noun;
    d->xaddr_scape[current_index] = noun;
}

void *_data_new_uninitialized(Data *d, Xaddr *new_xaddr, Symbol noun, size_t size) {
    Tnode *n = _t_new(d->root,noun,0,size);
    new_xaddr->key = _t_children(d->root);
    new_xaddr->noun = noun;
    _data_record_existence(d, new_xaddr->key, noun);
    return _t_surface(n);
}


// this should go somewhere once the dependencies are better resolved.  right now it depends on everything.
size_t size_of_named_surface(Receptor *r, Symbol instanceNoun, void *surface) {
    size_t result = 0;
    if (instanceNoun <= 0){
        switch (instanceNoun) {
            case XADDR_NOUN:
                result = sizeof(Xaddr);
                break;
            case CSPEC_NOUN:
                result = (size_t) 0;
                break;
            case CSTRING_NOUN:
                result = strlen((char *) surface) + 1;
                break;
            case PATTERN_SPEC_DATA_NOUN:
                result = sizeof(PatternSpecData);
                break;
            default:
                raise_error("can't get size of instance of %d \n", instanceNoun);
        }
    } else {
        result = (*size_table_get(spec_noun_for_noun(r, instanceNoun)))(r, instanceNoun, surface);
    }
    return result;
}


void data_set(Receptor *r, Xaddr xaddr, void *value, size_t size) {
    void *surface = _data_get(&r->data,xaddr.key);
    if (xaddr.key == 0 || !data_sem_check(r, xaddr)) {
        raise_error("I do not think that word (%d) means what you think it means!\n", xaddr.noun);
    }
    memcpy(surface, value, size);
}

Xaddr data_new(Receptor *r, Symbol noun, void *surface, size_t size) {
    Xaddr new_xaddr;
    //    printf("DATA_NEW: noun-%d,size-%ld\n",noun,size);
    _data_new_uninitialized(&r->data, &new_xaddr, noun, size);
    data_set(r, new_xaddr, surface, size);
    return new_xaddr;
}

void data_new_by_reference(Receptor *r, Xaddr *new_xaddr, Symbol noun, void *surface, size_t size) {
    _data_new_uninitialized(&r->data, new_xaddr, noun, size);
    data_set(r, *new_xaddr, surface, size);
}

Symbol data_new_noun(Receptor *r, Xaddr xaddr, char *label) {
    size_t s = sizeof(NounSurface)+strlen(label)+1;
    Tnode *n = _t_new(r->data.root,r->nounSpecXaddr.noun,0,s);
    int key = _t_children(r->data.root);
    NounSurface *ns = (NounSurface *)_t_surface(n);
    ns->specXaddr.key = xaddr.key;
    ns->specXaddr.noun = xaddr.noun;
    memcpy(&ns->label,label,strlen(label)+1);
    _data_record_existence(&r->data, key, r->nounSpecXaddr.noun);
    return key;
}


void data_init(Receptor *r) {
    int i;
    for (i = 0; i < DEFAULT_CACHE_SIZE; i++) r->data.xaddr_scape[i] = CSPEC;
    init_scapes(r);
    r->data.root = _t_new_root();

    r->data.current_xaddr = -1;

    r->data.log_head = 0;
    r->data.log_tail = 0;

    LogMeta lm,*l;
    r->data.log = _t_new(0,LOG_META_NOUN,&lm,sizeof(LogMeta));
    l = _t_surface(r->data.log);
    assert( pthread_mutex_init(&l->mutex, NULL) == 0);
    assert( pthread_cond_init(&l->changed, NULL) == 0);
}

void data_free(Receptor *r) {
    _t_free(r->data.log);
    _t_free(r->data.root);
}
