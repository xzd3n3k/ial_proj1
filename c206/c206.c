/* ******************************* c206.c *********************************** */
/*  Předmět: Algoritmy (IAL) - FIT VUT v Brně                                 */
/*  Úkol: c206 - Dvousměrně vázaný lineární seznam                            */
/*  Návrh a referenční implementace: Bohuslav Křena, říjen 2001               */
/*  Vytvořil: Martin Tuček, říjen 2004                                        */
/*  Upravil: Kamil Jeřábek, září 2020                                         */
/*           Daniel Dolejška, září 2021                                       */
/*           Daniel Dolejška, září 2022                                       */
/* ************************************************************************** */
/*
** Implementujte abstraktní datový typ dvousměrně vázaný lineární seznam.
** Užitečným obsahem prvku seznamu je hodnota typu int. Seznam bude jako datová
** abstrakce reprezentován proměnnou typu DLList (DL znamená Doubly-Linked
** a slouží pro odlišení jmen konstant, typů a funkcí od jmen u jednosměrně
** vázaného lineárního seznamu). Definici konstant a typů naleznete
** v hlavičkovém souboru c206.h.
**
** Vaším úkolem je implementovat následující operace, které spolu s výše
** uvedenou datovou částí abstrakce tvoří abstraktní datový typ obousměrně
** vázaný lineární seznam:
**
**      DLL_Init ........... inicializace seznamu před prvním použitím,
**      DLL_Dispose ........ zrušení všech prvků seznamu,
**      DLL_InsertFirst .... vložení prvku na začátek seznamu,
**      DLL_InsertLast ..... vložení prvku na konec seznamu,
**      DLL_First .......... nastavení aktivity na první prvek,
**      DLL_Last ........... nastavení aktivity na poslední prvek,
**      DLL_GetFirst ....... vrací hodnotu prvního prvku,
**      DLL_GetLast ........ vrací hodnotu posledního prvku,
**      DLL_DeleteFirst .... zruší první prvek seznamu,
**      DLL_DeleteLast ..... zruší poslední prvek seznamu,
**      DLL_DeleteAfter .... ruší prvek za aktivním prvkem,
**      DLL_DeleteBefore ... ruší prvek před aktivním prvkem,
**      DLL_InsertAfter .... vloží nový prvek za aktivní prvek seznamu,
**      DLL_InsertBefore ... vloží nový prvek před aktivní prvek seznamu,
**      DLL_GetValue ....... vrací hodnotu aktivního prvku,
**      DLL_SetValue ....... přepíše obsah aktivního prvku novou hodnotou,
**      DLL_Previous ....... posune aktivitu na předchozí prvek seznamu,
**      DLL_Next ........... posune aktivitu na další prvek seznamu,
**      DLL_IsActive ....... zjišťuje aktivitu seznamu.
**
** Při implementaci jednotlivých funkcí nevolejte žádnou z funkcí
** implementovaných v rámci tohoto příkladu, není-li u funkce explicitně
 * uvedeno něco jiného.
**
** Nemusíte ošetřovat situaci, kdy místo legálního ukazatele na seznam
** předá někdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodně komentujte!
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako procedury
** (v jazyce C procedurám odpovídají funkce vracející typ void).
**
**/

#include "c206.h"

bool error_flag;
bool solved;

/**
 * Vytiskne upozornění na to, že došlo k chybě.
 * Tato funkce bude volána z některých dále implementovaných operací.
 */
void DLL_Error(void) {
	printf("*ERROR* The program has performed an illegal operation.\n");
	error_flag = true;
}

/**
 * Provede inicializaci seznamu list před jeho prvním použitím (tzn. žádná
 * z následujících funkcí nebude volána nad neinicializovaným seznamem).
 * Tato inicializace se nikdy nebude provádět nad již inicializovaným seznamem,
 * a proto tuto možnost neošetřujte.
 * Vždy předpokládejte, že neinicializované proměnné mají nedefinovanou hodnotu.
 *
 * @param list Ukazatel na strukturu dvousměrně vázaného seznamu
 */
void DLL_Init( DLList *list ) {
	list->activeElement = list->firstElement = list->lastElement = NULL;

}

/**
 * Zruší všechny prvky seznamu list a uvede seznam do stavu, v jakém se nacházel
 * po inicializaci.
 * Rušené prvky seznamu budou korektně uvolněny voláním operace free.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Dispose( DLList *list ) {

	DLLElementPtr elem = list->firstElement;

	// dokud nezustane v listu jeden jediny prvek
	while (list->firstElement!=list->lastElement) {
		// jako prvni element se nastavi dalsi element, minuly se pomoci funkce free uvolni pamet a do promenne se ulozi novy first element
		list->firstElement = list->firstElement->nextElement;
		free(elem);
		elem = list->firstElement;
	}
	
	// kdyz uz v seznamu zustal posledni prvek, ktery je mimo jine ulozeny v promenne
	// nastavi se seznam do stavu po inicializaci
	list->activeElement = list->firstElement = list->lastElement = NULL;

	// uvolni se pamet
	free(elem);

}

/**
 * Vloží nový prvek na začátek seznamu list.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení na začátek seznamu
 */
void DLL_InsertFirst( DLList *list, int data ) {
	DLLElementPtr elem = (DLLElementPtr) malloc(sizeof(struct DLLElement));

	if (!elem) {
		DLL_Error();
		return;
	}

	// nastavi se data
	elem->data = data;
	// jelikoz vkladame jako prvni prvek, nebude zde zadny predchozi
	elem->previousElement = NULL;
	// jako dalsi se nastavi prvni prvek listu
	elem->nextElement = list->firstElement;
	
	if (list->firstElement) {
		// prvnimu prvku listu se nastavi ukazatel na predchozi na nas element
		list->firstElement->previousElement = elem;
	} else {
		// pokud list byl prazdny, je nas prvek zaroven poslednim
		list->lastElement = elem;
	}

	// nastavi se jako prvni prvek v listu
	list->firstElement = elem;
}

/**
 * Vloží nový prvek na konec seznamu list (symetrická operace k DLL_InsertFirst).
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení na konec seznamu
 */
void DLL_InsertLast( DLList *list, int data ) {
	DLLElementPtr elem = (DLLElementPtr) malloc(sizeof(struct DLLElement));

	if (!elem) {
		DLL_Error();
		return;
	}

	elem->data = data;
	elem -> nextElement = NULL;
	elem->previousElement = list->lastElement;

	if (list->lastElement) {
		// poslednimu prvku listu se nastavi ukazatel na nasledujici element tedy nas novy posledni element
		list->lastElement->nextElement = elem;
	} else {
		// pokud byl list prazdny, je zaroven prvnim prvkem
		list->firstElement = elem;
	}

	list->lastElement = elem;
}

/**
 * Nastaví první prvek seznamu list jako aktivní.
 * Funkci implementujte jako jediný příkaz, aniž byste testovali,
 * zda je seznam list prázdný.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_First( DLList *list ) {
	list->activeElement = list->firstElement;
}

/**
 * Nastaví poslední prvek seznamu list jako aktivní.
 * Funkci implementujte jako jediný příkaz, aniž byste testovali,
 * zda je seznam list prázdný.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Last( DLList *list ) {
	list->activeElement = list->lastElement;
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu prvního prvku seznamu list.
 * Pokud je seznam list prázdný, volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetFirst( DLList *list, int *dataPtr ) {
	if (list->firstElement) {
		*dataPtr = list->firstElement->data;
	} else {
		DLL_Error();
	}
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu posledního prvku seznamu list.
 * Pokud je seznam list prázdný, volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetLast( DLList *list, int *dataPtr ) {
	if (list->lastElement) {
		*dataPtr = list->lastElement->data;
	} else {
		DLL_Error();
	}
}

/**
 * Zruší první prvek seznamu list.
 * Pokud byl první prvek aktivní, aktivita se ztrácí.
 * Pokud byl seznam list prázdný, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteFirst( DLList *list ) {
	DLLElementPtr elem = list->firstElement;

	if (!elem) {
		return;
	}

	// jestlize je prvni prvek aktivni zrusime aktivitu
	if (elem == list->activeElement) {
		list->activeElement = NULL;
	}

	// jestlize je prvni prvek zaroven posledni prvek, nastavime posledni pvek na NULL
	if (elem == list->lastElement) {
		list->lastElement = NULL;
		list->firstElement = NULL;
	} else {
		// jinak nastavime ukazatel druheho prvku na predchozi prvek tedy prvni na NULL
		list->firstElement->nextElement->previousElement = NULL;
	}

	// jako prvni prvek nastavime druhy prvek a uvolnime pamet pro minuly prvni prvek
	list->firstElement = list->firstElement->nextElement;
	free(elem);
}

/**
 * Zruší poslední prvek seznamu list.
 * Pokud byl poslední prvek aktivní, aktivita seznamu se ztrácí.
 * Pokud byl seznam list prázdný, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteLast( DLList *list ) {
	DLLElementPtr elem = list->lastElement;

	if (!elem) {
		return;
	}

	if (elem == list->activeElement) {
		list->activeElement = NULL;
	}

	if (elem == list->firstElement) {
		list->lastElement = NULL;
		list->firstElement = NULL;
	} else {
		list->lastElement = elem->previousElement;
		list->lastElement->nextElement = NULL;
	}

	free(elem);
}

/**
 * Zruší prvek seznamu list za aktivním prvkem.
 * Pokud je seznam list neaktivní nebo pokud je aktivní prvek
 * posledním prvkem seznamu, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteAfter( DLList *list ) {

	// pokud ma list aktivni prvek a zaroven nebo pokud neni aktivni prvek poslednim prvkem seznamu tak se provede nasledujici
	if ((list->activeElement) && (list->activeElement != list->lastElement)) {
		DLLElementPtr elem = list->activeElement->nextElement;

		// pokud je prvek za aktivnim prvkem posledni prvek
		if (elem == list->lastElement) {
			// nastavi se ukazatel aktivniho prvku na dalsi na NULL a jako posledni prvek se nastavi aktivni prvek
			list->activeElement->nextElement = NULL;
			list->lastElement = list->activeElement;

		} else {
			// jinak pokud neni poslednim prvkem, se nastavi ukazatel na dalsi prvek z aktivniho na dalsi prvek po prvku za aktivnim prvkem
			list->activeElement->nextElement = list->activeElement->nextElement->nextElement;
			// a nastavi se ukazatel prvku za aktivnim prvkem na predchozi prvek na aktivni prvek
			list->activeElement->nextElement->previousElement = list->activeElement;
		}

	// uvolni se pamet
	free(elem);

	}
}

/**
 * Zruší prvek před aktivním prvkem seznamu list .
 * Pokud je seznam list neaktivní nebo pokud je aktivní prvek
 * prvním prvkem seznamu, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteBefore( DLList *list ) {
	
	if ((list->activeElement) && (list->activeElement != list->firstElement)) {
		DLLElementPtr elem = list->activeElement->previousElement;

		if (elem == list->firstElement) {
			list->activeElement->previousElement = NULL;
			list->firstElement = list->activeElement;
		} else {
			list->activeElement->previousElement = list->activeElement->previousElement->previousElement;
			list->activeElement->previousElement->nextElement = list->activeElement;
		}

		free(elem);

	}
}

/**
 * Vloží prvek za aktivní prvek seznamu list.
 * Pokud nebyl seznam list aktivní, nic se neděje.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení do seznamu za právě aktivní prvek
 */


void DLL_InsertAfter( DLList *list, int data ) {

	if (!(list->activeElement)) {
		return;
	}

	DLLElementPtr elem = (DLLElementPtr) malloc(sizeof(struct DLLElement));

	if (!elem) {
		DLL_Error();
		return;
	}

	// nastavi se data a nastavi se predchozi prvek jako aktivni prvek
	elem->data = data;
	elem->previousElement = list->activeElement;

	// jestlize je aktivni prvek zaroven poslednim, nastavi se jako posledni prvek novy prvek
	if (list->activeElement == list->lastElement) {
		list->lastElement = elem;
	} else {
		// jinak se jako ukazatel na dalsi prvek z naseho noveho prvku nastavi prvek hned za aktivnim prvekm
		elem->nextElement = list->activeElement->nextElement;
		// a jako ukazatel nasledujiciho prvku na predchozi se nastavi nas novy prvek
		elem->nextElement->previousElement = elem;
	}

	// jako ukazatel na dalsi prvek z aktivniho prvku se nastavi novy prvek
	list->activeElement->nextElement = elem;
}

/**
 * Vloží prvek před aktivní prvek seznamu list.
 * Pokud nebyl seznam list aktivní, nic se neděje.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení do seznamu před právě aktivní prvek
 */
void DLL_InsertBefore( DLList *list, int data ) {
	if (!(list->activeElement)) {
		return;
	}

	DLLElementPtr elem = (DLLElementPtr) malloc(sizeof(struct DLLElement));

	if (!elem) {
		DLL_Error();
		return;
	}

	elem->data = data;
	elem->nextElement = list->activeElement;

	if (list->activeElement == list->firstElement) {
		list->firstElement = elem;
	} else {
		elem->previousElement = list->activeElement->previousElement;
		elem->previousElement->nextElement = elem;
	}

	list->activeElement->previousElement = elem;
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu aktivního prvku seznamu list.
 * Pokud seznam list není aktivní, volá funkci DLL_Error ().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetValue( DLList *list, int *dataPtr ) {
	if (!(list->activeElement)) {
		DLL_Error();
		return;
	}

	*dataPtr = list->activeElement->data;
}

/**
 * Přepíše obsah aktivního prvku seznamu list.
 * Pokud seznam list není aktivní, nedělá nic.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Nová hodnota právě aktivního prvku
 */
void DLL_SetValue( DLList *list, int data ) {
	if (!(list->activeElement)) {
		return;
	}

	list->activeElement->data = data;
}

/**
 * Posune aktivitu na následující prvek seznamu list.
 * Není-li seznam aktivní, nedělá nic.
 * Všimněte si, že při aktivitě na posledním prvku se seznam stane neaktivním.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Next( DLList *list ) {
	if (!(list->activeElement)) {
		return;
	}

	if (list->activeElement == list->lastElement) {
		list->activeElement = NULL;
	} else {
		list->activeElement = list->activeElement->nextElement;
	}
}


/**
 * Posune aktivitu na předchozí prvek seznamu list.
 * Není-li seznam aktivní, nedělá nic.
 * Všimněte si, že při aktivitě na prvním prvku se seznam stane neaktivním.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Previous( DLList *list ) {
		if (!(list->activeElement)) {
		return;
	}

	if (list->activeElement == list->firstElement) {
		list->activeElement = NULL;
	} else {
		list->activeElement = list->activeElement->previousElement;
	}
}

/**
 * Je-li seznam list aktivní, vrací nenulovou hodnotu, jinak vrací 0.
 * Funkci je vhodné implementovat jedním příkazem return.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 *
 * @returns Nenulovou hodnotu v případě aktivity prvku seznamu, jinak nulu
 */
int DLL_IsActive( DLList *list ) {
	return !(list->activeElement == NULL);
}

/* Konec c206.c */
