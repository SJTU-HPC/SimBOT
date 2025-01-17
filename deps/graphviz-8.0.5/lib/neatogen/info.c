/*************************************************************************
 * Copyright (c) 2011 AT&T Intellectual Property 
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors: Details at https://graphviz.org
 *************************************************************************/

#include <neatogen/neato.h>
#include <stdio.h>
#include <neatogen/mem.h>
#include <neatogen/info.h>


Info_t *nodeInfo;		/* Array of node info */
static Freelist pfl;

void infoinit()
{
    freeinit(&pfl, sizeof(PtItem));
}

/* compare:
 * returns -1 if p < q
 *          0 if p = q
 *          1 if p > q
 * if q if NULL, returns -1
 * Ordering is by angle from -pi/2 to 3pi/4.
 * For equal angles (which should not happen in our context)
 * ordering is by closeness to origin.
 */
static int compare(Point * o, PtItem * p, PtItem * q)
{
    double x0;
    double y0;
    double x1;
    double y1;
    double a, b;

    if (q == NULL)
	return -1;
    if (p->p.x == q->p.x && p->p.y == q->p.y)
	return 0;

    x0 = (double)p->p.x - (double)o->x;
    y0 = (double)p->p.y - (double)o->y;
    x1 = (double)q->p.x - (double)o->x;
    y1 = (double)q->p.y - (double)o->y;
    if (x0 >= 0.0) {
	if (x1 < 0.0)
	    return -1;
	else if (x0 > 0.0) {
	    if (x1 > 0.0) {
		a = y1 / x1;
		b = y0 / x0;
		if (b < a)
		    return -1;
		else if (b > a)
		    return 1;
		else if (x0 < x1)
		    return -1;
		else
		    return 1;
	    } else {		/* x1 == 0.0 */
		if (y1 > 0.0)
		    return -1;
		else
		    return 1;
	    }
	} else {		/* x0 == 0.0 */
	    if (x1 > 0.0) {
		if (y0 <= 0.0)
		    return -1;
		else
		    return 1;
	    } else {		/* x1 == 0.0 */
		if (y0 < y1) {
		    if (y1 <= 0.0)
			return 1;
		    else
			return -1;
		} else {
		    if (y0 <= 0.0)
			return -1;
		    else
			return 1;
		}
	    }
	}
    } else {
	if (x1 >= 0.0)
	    return 1;
	else {
	    a = y1 / x1;
	    b = y0 / x0;
	    if (b < a)
		return -1;
	    else if (b > a)
		return 1;
	    else if (x0 > x1)
		return -1;
	    else
		return 1;
	}
    }
}

void addVertex(Site * s, double x, double y)
{
    Info_t *ip;
    PtItem *p;
    PtItem *curr;
    PtItem *prev;
    Point *origin_point = &s->coord;
    PtItem tmp;
    int cmp;

    ip = nodeInfo + s->sitenbr;
    curr = ip->verts;

    tmp.p.x = x;
    tmp.p.y = y;

    cmp = compare(origin_point, &tmp, curr);
    if (cmp == 0)
	return;
    else if (cmp < 0) {
	p = getfree(&pfl);
	p->p.x = x;
	p->p.y = y;
	p->next = curr;
	ip->verts = p;
	return;
    }

    prev = curr;
    curr = curr->next;
    while ((cmp = compare(origin_point, &tmp, curr)) > 0) {
	prev = curr;
	curr = curr->next;
    }
    if (cmp == 0)
	return;
    p = getfree(&pfl);
    p->p.x = x;
    p->p.y = y;
    prev->next = p;
    p->next = curr;
}
