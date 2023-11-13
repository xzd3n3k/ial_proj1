/* ******************************* c204.c *********************************** */
/*  Předmět: Algoritmy (IAL) - FIT VUT v Brně                                 */
/*  Úkol: c204 - Převod infixového výrazu na postfixový (s využitím c202)     */
/*  Referenční implementace: Petr Přikryl, listopad 1994                      */
/*  Přepis do jazyka C: Lukáš Maršík, prosinec 2012                           */
/*  Upravil: Kamil Jeřábek, září 2019                                         */
/*           Daniel Dolejška, září 2021                                       */
/* ************************************************************************** */
/*
** Implementujte proceduru pro převod infixového zápisu matematického výrazu
** do postfixového tvaru. Pro převod využijte zásobník (Stack), který byl
** implementován v rámci příkladu c202. Bez správného vyřešení příkladu c202
** se o řešení tohoto příkladu nepokoušejte.
**
** Implementujte následující funkci:
**
**    infix2postfix ... konverzní funkce pro převod infixového výrazu
**                      na postfixový
**
** Pro lepší přehlednost kódu implementujte následující pomocné funkce:
**    
**    untilLeftPar ... vyprázdnění zásobníku až po levou závorku
**    doOperation .... zpracování operátoru konvertovaného výrazu
**
** Své řešení účelně komentujte.
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako procedury
** (v jazyce C procedurám odpovídají funkce vracející typ void).
**
**/

#include "c204.h"

bool solved;

/**
 * Pomocná funkce untilLeftPar.
 * Slouží k vyprázdnění zásobníku až po levou závorku, přičemž levá závorka bude
 * také odstraněna.
 * Pokud je zásobník prázdný, provádění funkce se ukončí.
 *
 * Operátory odstraňované ze zásobníku postupně vkládejte do výstupního pole
 * znaků postfixExpression.
 * Délka převedeného výrazu a též ukazatel na první volné místo, na které se má
 * zapisovat, představuje parametr postfixExpressionLength.
 *
 * Aby se minimalizoval počet přístupů ke struktuře zásobníku, můžete zde
 * nadeklarovat a používat pomocnou proměnnou typu char.
 *
 * @param stack Ukazatel na inicializovanou strukturu zásobníku
 * @param postfixExpression Znakový řetězec obsahující výsledný postfixový výraz
 * @param postfixExpressionLength Ukazatel na aktuální délku výsledného postfixového výrazu
 */
void untilLeftPar( Stack *stack, char *postfixExpression, unsigned *postfixExpressionLength ) {
	
	// promenna slouzici k ulozeni prvku z vrcholu zasobniku - pro snizeni poctu pristupu k strukture zasobniku
	char stackTop;

	// dokud neni stack prazdny, ulozim vrchni znak a smazu ho ze zasobniku
	while (!Stack_IsEmpty(stack)) {
		Stack_Top(stack, &stackTop);
		Stack_Pop(stack);

		// pokud se narazilo na levou zavorku, skoncim prochazeni a mazani prvku
		if (stackTop == '(') {
			break;
		}

		// zvetsime delku postfix expression a pridame znak do postfix expression
		postfixExpression[(*postfixExpressionLength)++] = stackTop;
	}

}

/**
 * Pomocná funkce doOperation.
 * Zpracuje operátor, který je předán parametrem c po načtení znaku ze
 * vstupního pole znaků.
 *
 * Dle priority předaného operátoru a případně priority operátoru na vrcholu
 * zásobníku rozhodneme o dalším postupu.
 * Délka převedeného výrazu a taktéž ukazatel na první volné místo, do kterého
 * se má zapisovat, představuje parametr postfixExpressionLength, výstupním
 * polem znaků je opět postfixExpression.
 *
 * @param stack Ukazatel na inicializovanou strukturu zásobníku
 * @param c Znak operátoru ve výrazu
 * @param postfixExpression Znakový řetězec obsahující výsledný postfixový výraz
 * @param postfixExpressionLength Ukazatel na aktuální délku výsledného postfixového výrazu
 */
void doOperation( Stack *stack, char c, char *postfixExpression, unsigned *postfixExpressionLength ) {
	
	// pokud je zasobnik prazdny, vlozime na jeho vrchol operator
	if (Stack_IsEmpty(stack)) {
		Stack_Push(stack, c);
		return;
	}

	// vytvorime promennou do ktere ulozime vrchni prvek ze zasobniku
	char stackTop;
	Stack_Top(stack, &stackTop);

	// pokud je vrchni prvek leva zavorka, vlozime nahoru operator
	if (stackTop == '(') {
		Stack_Push(stack, c);
		return;
	}

	// pokud je na vrcholu operator s nizsi prioritou a zaroven je dany operator operator s vyssi prioritou, tak se operator vlozi na vrchol zasobniku
	if ((stackTop == '-' || stackTop == '+') && (c == '*' || c == '/')) {
		Stack_Push(stack, c);
		return;
	}

	// vlozi znak z vrchu zasobniku na konec postfi expressionu a zvetsi jeho delku o jedno + odstrani prvek ze zasobniku
	postfixExpression[(*postfixExpressionLength)++] = stackTop;
	Stack_Pop(stack);

	// rekurzivne volame dokud se vse nepodari uspesne vlozit
	doOperation(stack, c, postfixExpression, postfixExpressionLength);

}

/**
 * Konverzní funkce infix2postfix.
 * Čte infixový výraz ze vstupního řetězce infixExpression a generuje
 * odpovídající postfixový výraz do výstupního řetězce (postup převodu hledejte
 * v přednáškách nebo ve studijní opoře).
 * Paměť pro výstupní řetězec (o velikosti MAX_LEN) je třeba alokovat. Volající
 * funkce, tedy příjemce konvertovaného řetězce, zajistí korektní uvolnění zde
 * alokované paměti.
 *
 * Tvar výrazu:
 * 1. Výraz obsahuje operátory + - * / ve významu sčítání, odčítání,
 *    násobení a dělení. Sčítání má stejnou prioritu jako odčítání,
 *    násobení má stejnou prioritu jako dělení. Priorita násobení je
 *    větší než priorita sčítání. Všechny operátory jsou binární
 *    (neuvažujte unární mínus).
 *
 * 2. Hodnoty ve výrazu jsou reprezentovány jednoznakovými identifikátory
 *    a číslicemi - 0..9, a..z, A..Z (velikost písmen se rozlišuje).
 *
 * 3. Ve výrazu může být použit předem neurčený počet dvojic kulatých
 *    závorek. Uvažujte, že vstupní výraz je zapsán správně (neošetřujte
 *    chybné zadání výrazu).
 *
 * 4. Každý korektně zapsaný výraz (infixový i postfixový) musí být uzavřen
 *    ukončovacím znakem '='.
 *
 * 5. Při stejné prioritě operátorů se výraz vyhodnocuje zleva doprava.
 *
 * Poznámky k implementaci
 * -----------------------
 * Jako zásobník použijte zásobník znaků Stack implementovaný v příkladu c202.
 * Pro práci se zásobníkem pak používejte výhradně operace z jeho rozhraní.
 *
 * Při implementaci využijte pomocné funkce untilLeftPar a doOperation.
 *
 * Řetězcem (infixového a postfixového výrazu) je zde myšleno pole znaků typu
 * char, jenž je korektně ukončeno nulovým znakem dle zvyklostí jazyka C.
 *
 * Na vstupu očekávejte pouze korektně zapsané a ukončené výrazy. Jejich délka
 * nepřesáhne MAX_LEN-1 (MAX_LEN i s nulovým znakem) a tedy i výsledný výraz
 * by se měl vejít do alokovaného pole. Po alokaci dynamické paměti si vždycky
 * ověřte, že se alokace skutečně zdrařila. V případě chyby alokace vraťte namísto
 * řetězce konstantu NULL.
 *
 * @param infixExpression vstupní znakový řetězec obsahující infixový výraz k převedení
 *
 * @returns znakový řetězec obsahující výsledný postfixový výraz
 */
char *infix2postfix( const char *infixExpression ) {
	
	// alokace pameti pro zasobnik, v pripade chyby alokace vrat NULL
	Stack *stack = (Stack *) malloc(sizeof(Stack));

	if (!stack) {
		return NULL;
	}

	Stack_Init(stack);

	// alokace pameti pro postfix expression, v pripade chyby alokace uvolni dosud alokovanou pamet a vrat NULL
	char *postfixExpression = (char *) malloc(MAX_LEN * sizeof(char));

	if (!postfixExpression) {
		free(stack);
		return NULL;
	}

	// promenna pro ulozeni delky retezce
	unsigned int postfixExpressionLength = 0;

	// prochazeni daneho retezce znak po znaku
	for (char ch = *infixExpression; ch != '\0'; ch = *(++infixExpression)) {

		// jestlize je operator, zavolej funkci doOperation
		if (ch == '-' || ch == '+' || ch == '*' || ch == '/') {
			doOperation(stack, ch, postfixExpression, &postfixExpressionLength);
		} else if (ch == '(') {
			// jestlize je to leva zavorka, dej ji na vrch zasobniku
			Stack_Push(stack, ch);

		} else if (ch == ')') {

			untilLeftPar(stack, postfixExpression, &postfixExpressionLength);

		} else if (ch == '=') { // jestlize jsme dosli na konec vyrazu

			while (!Stack_IsEmpty(stack)) {

				// vlozi znak z vrchu zasobniku na konec postfi expressionu a zvetsi jeho delku o jedno + odstrani prvek ze zasobniku
				Stack_Top(stack, &(postfixExpression[postfixExpressionLength++]));
				Stack_Pop(stack);
			}

			postfixExpression[postfixExpressionLength++] = '=';
			break;

		} else if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || (ch >= '0' && ch <= '9')) {
			postfixExpression[postfixExpressionLength++] = ch;
		}
	}

	free(stack);
	postfixExpression[postfixExpressionLength++] = '\0';

	return postfixExpression;
}


/**
 * Pomocná metoda pro vložení celočíselné hodnoty na zásobník.
 *
 * Použitá implementace zásobníku aktuálně umožňuje vkládání pouze
 * hodnot o velikosti jednoho byte (char). Využijte této metody
 * k rozdělení a postupné vložení celočíselné (čtyřbytové) hodnoty
 * na vrchol poskytnutého zásobníku.
 *
 * @param stack ukazatel na inicializovanou strukturu zásobníku
 * @param value hodnota k vložení na zásobník
 */
void expr_value_push( Stack *stack, int value ) {
	
	// postupne rozlozi a ulozi cislo do 4 bytes na vrchol zasobniku
	for (int i = 0; i<4; i++) {
		Stack_Push(stack, ((value >> (3-i)*8) & 0xFF));
	}

}

/**
 * Pomocná metoda pro extrakci celočíselné hodnoty ze zásobníku.
 *
 * Využijte této metody k opětovnému načtení a složení celočíselné
 * hodnoty z aktuálního vrcholu poskytnutého zásobníku. Implementujte
 * tedy algoritmus opačný k algoritmu použitému v metodě
 * `expr_value_push`.
 *
 * @param stack ukazatel na inicializovanou strukturu zásobníku
 * @param value ukazatel na celočíselnou proměnnou pro uložení
 *   výsledné celočíselné hodnoty z vrcholu zásobníku
 */
void expr_value_pop( Stack *stack, int *value ) {
	char bytes[4];

	// vezme prvni 4 bytes z vrcholku zasobniku a vlozi do arraye bytes
	for (int i = 3; i >= 0; i--) {
		Stack_Top(stack, &bytes[i]);
		Stack_Pop(stack);
	}

	// sestavi zpet ciselnou hodnotu
	*value = (bytes[0] << 24) | (bytes[1] << 16) | (bytes[2] << 8) | bytes[3];

}


/**
 * Tato metoda provede vyhodnocení výrazu zadaném v `infixExpression`,
 * kde hodnoty proměnných použitých v daném výrazu jsou definovány
 * v poli `variableValues`.
 *
 * K vyhodnocení vstupního výrazu využijte implementaci zásobníku
 * ze cvičení c202. Dále také využijte pomocných funkcí `expr_value_push`,
 * respektive `expr_value_pop`. Při řešení si můžete definovat libovolné
 * množství vlastních pomocných funkcí.
 *
 * Předpokládejte, že hodnoty budou vždy definovány
 * pro všechy proměnné použité ve vstupním výrazu.
 *
 * @param infixExpression vstpní infixový výraz s proměnnými
 * @param variableValues hodnoty proměnných ze vstupního výrazu
 * @param variableValueCount počet hodnot (unikátních proměnných
 *   ve vstupním výrazu)
 * @param value ukazatel na celočíselnou proměnnou pro uložení
 *   výsledné hodnoty vyhodnocení vstupního výrazu
 *
 * @return výsledek vyhodnocení daného výrazu na základě poskytnutých hodnot proměnných
 */

// vrati true pokud je dany znak operator, jinak false
bool isOperator(char ch) {
    return (ch == '-' || ch == '+' || ch == '*' || ch == '/');
}

//vrati prioritu operatoru, cim vyssi cislo, tim vyssi priorita
int getOperatorPriority(char ch) {
    if (ch == '-' || ch == '+')
        return 1;

    if (ch == '*' || ch == '/')
        return 2;

    return 0;
}

// vezme dva operandy a operator jako argument a provede operaci
int applyOperator(int operand1, int operand2, char operator) {

    switch (operator) {
        case '+':
            return operand1 + operand2;

        case '-':
            return operand1 - operand2;

        case '*':
            return operand1 * operand2;

        case '/':
            return operand1 / operand2;
			
        default:
            return 0;
    }
}

bool eval( const char *infixExpression, VariableValue variableValues[], int variableValueCount, int *value ) {

	Stack operandStack;
    Stack_Init(&operandStack);

	// udela z infixoveho tvaru postfixovy
	char *postfixExpression = infix2postfix(infixExpression);

	// postupne prochazi znaky z postfix tvaru dokud znak neni rovnitko
    for (char ch = *postfixExpression; ch != '='; ch = *(++postfixExpression)) {

		// pokud znak neni operator
        if (!isOperator(ch)) {
            int operandValue = 0;
			// pokud je znak promenna
            if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')) {
                
				// prochazi se promenne a hleda se jejich hodnota
                bool variableFound = false;
                for (int i = 0; i < variableValueCount; i++) {
                    if (variableValues[i].name == ch) {
                        operandValue = variableValues[i].value;
                        variableFound = true;
                        break;
                    }
                }

                if (!variableFound) {
                    return false;
                }
            } else {
				// pokud je znak cislo, prevede se na int
                operandValue = ch - '0';
            }

			// cislo se ulozi na vrchol zasobniku
            expr_value_push(&operandStack, operandValue);

        } else if (isOperator(ch)) {
			
			// pokud je znak operator, vezmou se ze zasobniku posledni dve cisla a zavola se funkce pro provedeni operace
            int operand2, operand1;
			expr_value_pop(&operandStack, &operand2);
			expr_value_pop(&operandStack, &operand1);

            int result = applyOperator(operand1, operand2, ch);
			expr_value_push(&operandStack, result);

        }
    }

	// odstrani se cislo z vrcholu zasobniku
	expr_value_pop(&operandStack, value);
	return true;

}

/* Konec c204.c */
