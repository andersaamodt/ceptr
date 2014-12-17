/**
 * @ingroup tree
 *
 * @{
 *
 * @file mtree.c
 * @brief semantic tree matrix implementation
 *
 * @copyright Copyright (C) 2013-2014, The MetaCurrency Project (Eric Harris-Braun, Arthur Brock, et. al).  This file is part of the Ceptr platform and is released under the terms of the license contained in the file LICENSE (GPLv3).
 */

#include "mtree.h"
#include "ceptr_error.h"
#include "hashfn.h"
#include "def.h"

const H null_H = {0,{NULL_ADDR,NULL_ADDR}};

// @todo make this not realloc each time?
void __m_add_level(M *m) {
    if (!m->levels++) {
	m->lP = malloc(sizeof(L));
    }
    else {
	m->lP = realloc(m->lP,sizeof(L)*m->levels);
    }
    int i = m->levels-1;
    m->lP[i].nodes = 0;
}

// @todo make this not realloc each time!!
N *__m_add_nodes(H h,L *l,int c) {
    N *n;
    Mindex i = l->nodes;
    if (!i) {
    	l->nP = malloc(sizeof(N)*c);
	l->nodes = c;
    }
    else {
	l->nodes += c;
    	l->nP = realloc(l->nP,sizeof(N)*l->nodes);
    }
    n = _GET_NODE(h,l,i);
    return n;
}

void __m_new_init(H parent,H *h,L **l) {
    h->m = parent.m;
    h->a.l = parent.a.l+1;

    if (parent.a.l >= parent.m->levels) {
	raise_error0("address too deep!");
    }
    else if (parent.a.l == parent.m->levels-1) {
	h->a.i = 0;
	__m_add_level(h->m);
	*l = GET_LEVEL(*h);
    }
    else {
	*l = GET_LEVEL(*h);
	h->a.i = (*l)->nodes;
    }
}

void __m_new_root(H *h, L **l) {
    M *m = h->m = malloc(sizeof(M));
    m->magic = matrixImpl;
    m->levels = 0;
    h->a.l = 0;
    h->a.i = 0;
    __m_add_level(m);
    *l = GET_LEVEL(*h);
}

/**
 * Create a new tree node
 *
 * @param[in] parent handle to parent node
 * @param[in] symbol semantic symbol for the node to be create
 * @param[in] surface pointer to node's data
 * @param[in] size size in bytes of the surface
 * @returns updated handle
 */
H _m_new(H parent,Symbol symbol,void *surface,size_t size) {
    H h;
    L *l = 0;

    if (parent.m) {
	__m_new_init(parent,&h,&l);
    }
    else {
	__m_new_root(&h,&l);
    }

    // add a node
    N *n,*nl;
    n = __m_add_nodes(h,l,1);

    n->symbol = symbol;
    n->size = size;
    n->parenti = parent.m ? parent.a.i : NULL_ADDR;
    n->flags = 0;
    if (size) {
	if (size == sizeof(int)) {
	    *((int *)&n->surface) = *(int *)surface;
	}
	else {
	    n->flags |= TFLAG_ALLOCATED;
	    n->surface = malloc(size);
	    if (surface)
		memcpy(n->surface,surface,size);
	}
    }
    else {
	n->flags = 0;
    }

    return h;
}

H _m_newi(H parent,Symbol symbol,int surface) {
    return _m_new(parent,symbol,&surface,sizeof(int));
}

H __mnft(H parent,T *t) {
    int i, c = _t_children(t);
    H h = _m_new(parent,_t_symbol(t),_t_surface(t),_t_size(t));
    for(i=1;i<=c;i++) {
	__mnft(h,_t_child(t,i));
    }
    return h;
}

H _m_new_from_t(T *t) {
    H h = null_H;
    h = __mnft(h,t);
    h.a.l = 0;
    h.a.i = 0;
    return h;
}

void _m_2tfn(H h,N *n,void *data,MwalkState *s,Maddr ap) {

    T **tP = (T**) &(((struct {T *t;} *)data)->t);
    T *t =  h.a.l ? (s[h.a.l-1].user.t) : NULL;
    *tP = _t_new(t,n->symbol,(n->flags & TFLAG_ALLOCATED)?n->surface:&n->surface,n->size);
    s[h.a.l].user.t = *tP;
}

T *_t_new_from_m(H h) {
    struct {T *t;} d = {NULL};
    Maddr ac = {0,0};
    _m_walk(h,_m_2tfn,&d);
    return _t_root(d.t);
}

N *__m_get(H h) {
    L *l = GET_LEVEL(h);
    N *n = GET_NODE(h,l);
    return n;
}

size_t _m_size(H h) {return __m_get(h)->size;}

void __m_free(H h,int free_surface) {
    int i = h.m->levels;
    while(i--) {
	L *l = _GET_LEVEL(h,i);
	Mindex j = l->nodes;
	if (free_surface) {
	    while(j--) {
		N *n = _GET_NODE(h,l,j);
		if (n->flags & TFLAG_ALLOCATED) {
		    free(n->surface);
		}
	    }
	}
	free(l->nP);
    }
    free(h.m->lP);
    free(h.m);
}

int _m_children(H h) {
    Mlevel levels = h.m->levels;

    if (h.a.l >= levels) {
	raise_error0("address too deep!");
    }
    else if (h.a.l == levels-1) {
	return 0;
    }
    L *l = _GET_LEVEL(h,h.a.l+1);
    Mindex c = 0;
    Mindex i = 0,pi = h.a.i;
    Mindex max = l->nodes;
    N *n = &l->nP[0];

/* this works if nodes are sorted
    while (i < max && n->parenti != h.a.i) {
	n++;i++;
    }
    while (i < max && n->parenti == h.a.i) {
	n++;i++;c++;
    }
*/
    while (i < max) {
	if (!(n->flags & TFLAG_DELETED) && pi == n->parenti) c++;
	n++;i++;
    }

    return c;
}

void * _m_surface(H h) {
    N *n = __m_get(h);
    if (n->flags & TFLAG_ALLOCATED)
	return n->surface;
    else
	return &n->surface;
}

Maddr _m_parent(H h) {
    Maddr a = {NULL_ADDR,NULL_ADDR};
    if (h.a.l > 0) {
	N *n = __m_get(h);
	a.l = h.a.l-1;
	a.i = n->parenti;
    }
    return a;
}

Maddr _m_child(H h,Mindex c) {
    Maddr a = {NULL_ADDR,NULL_ADDR};
    Mlevel levels = h.m->levels;
    if (h.a.l >= levels) {
	raise_error0("address too deep!");
    }
    else if (h.a.l == levels-1) {
	return a;
    }
    a.l = h.a.l+1;
    L *l = &h.m->lP[a.l];
    Mindex ci = 0,max = l->nodes;
    N *n = &l->nP[0];
    if (c == NULL_ADDR) {
	a.i = NULL_ADDR;
	while(ci < max) {
	    if (n->parenti == h.a.i) a.i = ci;
	    ci++;
	    n++;
	}
	if (a.i == NULL_ADDR)
	    a.l = NULL_ADDR;
    }
    else {
	a.i = 0;
	while(a.i < max) {
	    if (n->parenti == h.a.i) ci++;
	    if (ci == c) return a;
	    a.i++;n++;
	}
	a.l = NULL_ADDR;
	a.i = NULL_ADDR;
    }
/* works if nodes are sorted
    //skip past nodes of children of parents before our parent
    while (a.i < max && n->parenti < h.a.i) {
	a.i++;n++;
    }

    // if you pass in NULL_ADDR for the child,
    // this routine returns the last child address
    if (c == NULL_ADDR) {
	while (a.i < max && n->parenti == h.a.i) {
	    a.i++;n++;
	}
	a.i--;
	if (a.i == -1) a.l = NULL_ADDR;
    }
    else {
	if (a.i+c > l->nodes) {
	    raise_error0("address too deep!");
	}
	a.i += c-1;
    }
*/

    return a;
}

Symbol _m_symbol(H h) {
    N *n = __m_get(h);
    return n->symbol;
}

Maddr _m_next_sibling(H h) {
    L *l = GET_LEVEL(h);
    int i = h.a.i+1;
    Maddr r;
    N *n = GET_NODE(h,l);
    Mindex pi = n->parenti;
    while (i<l->nodes) {
	n++;
	if (n->parenti == pi) {
	    r.l = h.a.l;
	    r.i = i;
	    return r;
	}
	i++;
    }
    return null_H.a;
}

H _m_new_root(Symbol s) {
    return _m_new(null_H,s,0,0);
}

H _m_newr(H parent,Symbol s) {
    return _m_new(parent,s,0,0);
}

H _m_add(H parent,H h) {
    L *pl,*l;
    H r;
    int x = _m_children(parent)+1;
    int i,levels = h.m->levels;
    H p = parent;
    Mindex d = parent.a.i;
    for (i=0;i<levels;i++) {
	__m_new_init(p,&r,&pl);
	l = _GET_LEVEL(h,i);
	N *np = __m_add_nodes(r,pl,l->nodes);
	N *n = &l->nP[0];
	Mindex j = l->nodes;
	while (j--)  {
	    *np = *n;
	    if (np->parenti == NULL_ADDR) np->parenti = 0;
	    np->parenti += d;
	    np++; n++;
	}
	d = pl->nodes-l->nodes;
	p.a.l++;
    }
    r.a = _m_child(parent,x);

    __m_free(h,0);  //free h but not it's surfaces which were copied into the parent
    return r;
}

void _m_walk(H h,void (*walkfn)(H ,N *,void *,MwalkState *,Maddr),void *user_data) {
    int levels = h.m->levels;
    MwalkState state[levels];
    int i,root;
    //    for(i=0;i<levels;i++) {state[i]=0;};
    Maddr ap = _m_parent(h);
    root = h.a.l;
    int done = 0;
    // @todo checks to make sure root isn't deleted or null?
    L *l = GET_LEVEL(h);
    N *n;
    int backup,nodes = h.a.i+1; // at root level pretend we are at last node
    //    raise(SIGINT);

    while(!done) {
	backup = 0;
	n = GET_NODE(h,l);
	// look for child of parent at this level (may be node at current handle address)
	while ((h.a.i < nodes)  && ((n->flags & TFLAG_DELETED) || (n->parenti != ap.i))) {
	    n++;h.a.i++;
	};

	// if we got one, then call the walk function
	if (h.a.i != nodes) {
	    (*walkfn)(h,n,user_data,state,ap);
	    // and go down looking for children
	    if (h.a.l+1 < levels) {
		state[h.a.l].i = h.a.i;
		ap = h.a;
		h.a.l++;
		h.a.i = 0;
		l = GET_LEVEL(h);
		nodes = l->nodes;
	    }
	    else {
		// if no more levels, then backup if no more nodes
		if (++h.a.i == nodes)
		    backup = 1;
	    }
	}
	else backup = 1;

	while(backup) {
	    // if current node is at root level we are done
	    if (h.a.l == root) {backup = 0;done = 1;}
	    else {
		// otherwise move up a level
		h.a.l--;
		// new node index is next node at that level from stored state
		h.a.i = state[h.a.l].i+1;
		l = GET_LEVEL(h);
		nodes = l->nodes;
		// if we still have nodes to check then we have finished backing up otherwise
		// we'll loop to go back up another level
		if (h.a.i < nodes) {
		    backup = 0;
		    ap.l = h.a.l -1;
		    ap.i = state[ap.l].i;
		}
	    }
	}
    }
}

void _m_detatchfn(H oh,N *on,void *data,MwalkState *s,Maddr ap) {
    struct {M *m;int l;} *d = data;

    H parent;
    H h;
    L *l;
    if (!d->m) {
	__m_new_root(&h,&l);
	parent.m = 0;
	d->m = h.m;
    }
    else {
	parent.m = d->m;
	parent.a.i = s[oh.a.l-1].user.pi;
	// we use d->l here to give us a level offset because the root of
	// the detached tree is not necessarily the same as the root of
	// the whole tree.
	parent.a.l = oh.a.l-1-d->l;
	__m_new_init(parent,&h,&l);
    }

    N *n,*nl;
    n = __m_add_nodes(h,l,1);

    // everything in the node is the same except the parenti
    *n = *on;
    on->flags = TFLAG_DELETED;
    on->surface = 0;
    // which we got from the user portion of the state data
    n->parenti = parent.m ? parent.a.i : NULL_ADDR;
    s[oh.a.l].user.pi = l->nodes-1;

}

H _m_detatch(H oh) {
    struct {M *m;int l;} d = {NULL,oh.a.l};
    _m_walk(oh,_m_detatchfn,&d);
    H h = {d.m,{0,0}};
    return h;
}

/** @}*/