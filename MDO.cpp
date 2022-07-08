#include "IteratorMDO.h"
#include "MDO.h"
#include <iostream>
#include <vector>

#include <exception>
using namespace std;

nod::nod(Relatie r, TElem val) {
	rel = r;
	left = NULL;
	right = NULL;
	up = NULL;
	minim = this;
	heightLeft = 0;
	heightRight = 0;
	values = val;
}
bool nod::operator<=(nod a) {
	return rel(this->values.first, a.values.first);
}
MDO::MDO(Relatie r) {
	/* Theta(1) */
	parent = NULL;
	nrElems = 0;
	rel = r;
}


void MDO::adauga(TCheie c, TValoare v) {
	/* de adaugat */

	parent = adaugaRec(parent, TElem(c, v));
	++nrElems;
}

nod* MDO::adaugaRec(nod* par, TElem value) {
	if (par == NULL) {
		par = new nod(this->rel, value);
		return par;
	}
	if (rel(value.first, par->values.first)) {
		par->heightLeft++;
		par->left = adaugaRec(par->left, value);
		par->left->up = par;

		par->minim = par->left->minim; 
	
	}
	else {
		par->heightRight++;
		par->right = adaugaRec(par->right, value);
		par->right->up = par;
	}
	return par;
}

void MDO::cauta_rec(nod* par, TCheie c, vector<nod*>& aux) const {
	if (par != NULL) {
		if (par->values.first == c) {
			aux.push_back(par);
			cauta_rec(par->left, c, aux);
			cauta_rec(par->right, c, aux);
		}
		else {
			if (rel(c, par->values.first)) {
				cauta_rec(par->left, c, aux);
			}
			else {
				cauta_rec(par->right, c, aux);
			}
		}
	}
}

vector<TValoare> MDO::cauta(TCheie c) const {
	/* de adaugat */
	vector<nod*> aux;
	vector<TValoare> to_return;
	cauta_rec(parent, c, aux);
	for (vector<nod*>::iterator i = aux.begin(); i != aux.end(); ++i) {
		to_return.push_back((*i)->values.second);
	}

	return to_return;
}

nod* MDO::urmator(nod* present) const {
	nod* succ;
	nod* prec;
	if (present->right != NULL) {
		succ = present->right->minim;
	}
	else {
		prec = present->up;
		while (prec != NULL && present == prec->right) {
			present = prec;
			prec = present->up;
		}
		succ = prec;
	}
	return succ;

}

void MDO::sterge_rec(nod* to_delete) {
	if (to_delete != NULL) {
		if (to_delete->left == NULL && to_delete->right == NULL) {
			if (to_delete->up != NULL) {
				if (to_delete->up->left == to_delete)
					to_delete->up->left = NULL;
				else
					to_delete->up->right = NULL;
			}
			delete to_delete;
		}
		else {
			if (to_delete->left == NULL) {
				to_delete->heightRight--;
				to_delete->values = to_delete->right->values;
				sterge_rec(to_delete->right);

			}
			else if (to_delete->right == NULL) {
				to_delete->heightLeft--;
				to_delete->values = to_delete->left->values;
				sterge_rec(to_delete->left);
			}
			else {
				if (to_delete->heightLeft < to_delete->heightRight) {
					to_delete->heightLeft--;
					to_delete->values = to_delete->left->values;
					sterge_rec(to_delete->left);

				}
				else {
					to_delete->heightRight--;
					to_delete->values = to_delete->right->values;
					sterge_rec(to_delete->right);

				}
			}
		}
	}
}

bool MDO::sterge(TCheie c, TValoare v) {
	/* de adaugat */
	vector<nod*> aux;
	if (dim() == 0)
	{
		return false;
	}
	cauta_rec(parent, c, aux);
	for (vector<nod*>::iterator i = aux.begin(); i != aux.end(); ++i) {
		if ((*i)->values.second == v) {
			sterge_rec(*i);
			--nrElems;
			return true;
		}
	}

	return false;
}




 
/*			gaseste si returneaza valoarea minima a multidictionarului
			Caz favorabil = Theta(m)
			Caz defavorabil = Theta(m)
			Caz mediu = Theta(m)
			Complexitate generala: Theta(m)
			Subalgoritm valoareMinima(mdo, min)
			pre: mdo apartine MultiDictionar Ordonat
			post: min este valoarea minima din multidictionar

			it <- IteratorMDO(mdo)

			Daca not it.valid() atunci
				min <- NULL_TVALOARE
			SfDaca

			min <- mdo.parrent.valoare.second

			Cat timp it.valid() executa
				valCurent <- it.element().valoare
				Daca valCurent < minim atunci
					minim <- valCurent
				it.urmator()
			SfCatTimp

			Sf Subalgoritm
*/
TValoare MDO::valoareMinima() const {
	IteratorMDO it(*this);

	if (!it.valid())
		return -1;

	TValoare minim = parent->values.second;

	while (it.valid()) {
		TValoare valCurent = it.element().second;
		if (valCurent < minim)
			minim = valCurent;
		it.urmator();
	}

	return minim;
}



int MDO::dim() const {
	/* de adaugat */
	return nrElems;
}

bool MDO::vid() const {
	/* de adaugat */
	return nrElems == 0;
}

IteratorMDO MDO::iterator() const {
	return IteratorMDO(*this);
}

void MDO::destructor_rec(nod* par) {
	if (par != NULL && nrElems > 0) {
		destructor_rec(par->right);
		destructor_rec(par->left);
		delete par;
	}
}

MDO::~MDO() {
	/* de adaugat */
	destructor_rec(parent);
}