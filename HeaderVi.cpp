#include <iostream>
#include <string>
#include "HeaderVi.h"

using namespace std;

namespace HeaderVi {
    ViEditor::ViEditor() {
        /* Initialize variables */
        first = last = actual = NULL;
        line = 1;
        lineas = 0;

        /* Ask for the file name */
        cout << "Ingrese el nombre del archivo:\n";
        getline(cin, name);

        /* Start the main program */
        system("cls");
        editor();
    }

    /* Print the screen output, it will only print 3 lines, the actual, the
     * previous one and the next line. */
    void ViEditor::mostrar() {
        /* Delete last content */
        system("cls");

        /* Check if there are lines */
        if (actual != NULL) {
            /* Prev line */
            if (actual->prev != NULL) {
                cout << "  " << actual->prev->text << endl;
            }

            /* Actual line */
            cout << "- " << actual->text << endl;

            /* Next line */
            if (actual->next != NULL) {
                cout << "  " << actual->next->text << endl;
            }
        }
        cout << "-------------------------------------\n";
    }

    /* Control the main flow of the program, it will wait for the user input
     * and then act acordly to the action. */
    void ViEditor::editor() {
        string comando;

        /* This action will repeat forever until the close action is called */
        while (true) {
            getline(cin, comando);

            if (comando == "l") {
                /* l: Go to first line */
                actual = first;
                line = 1;
            } else if (comando == "a") {
                /* a: append text */
                agregarTexoFinalLinea();
            } else if (comando == "d") {
                /* d: Delete actual line */
                borrarLineaActual();
            } else if (comando.substr(0,3) == "dr ") {
                /* dr: delete lines from num1 - num2 */
                borrarVariasLineas(comando.substr(3));
            } else if (comando[0] == 'f') {
                /* f: rename file */
                name = comando.substr(2);
            } else if (comando[0] == 'g') {
                /* g: go to line */
                goTo(atoi(comando.substr(2).c_str()));
            } else if (comando == "i") {
                /* i: add next line */
                agregarLinea();
            } else if (comando.substr(0,2) == "m ") {
                /* m: move line */
                moverLinea(atoi(comando.substr(2).c_str()));
            } else if (comando.substr(0,3) == "mr ") {
                /* mr: move recursive. move several lines */
                moverLineas(comando.substr(3));
            } else if (comando.substr(0,2) == "t ") {
                /* t: copy from line */
                copiarLinea(atoi(comando.substr(2).c_str()));
            } else if (comando.substr(0,3) == "tr ") {
                /* tr: Copy several lines */
                copiarLineas(comando.substr(3));
            } else if (comando == "-") {
                /* -: move up one line */
                subirLinea();
            } else if (comando == "+") {
                /* +: move down one line */
                bajarLinea();
            } else if (comando == "$") {
                /* $: goto last line */
                actual = last;
                line = lineas;
            }

            /* Refresh the screen */
            mostrar();
        }
    }

    /* Add text to the end of the line */
    void ViEditor::agregarTexoFinalLinea() {
        string temp;
        getline(cin, temp);
        actual->text.append(temp);
    }

    /* Delete the actual line */
    void ViEditor::borrarLineaActual() {
        /* Chechk if there's at least one line */
        if (lineas != 0) {

            nodoPtr borrado = actual;//Creamos un nodoPtr que almacenara el nodo a borrar
            lineas--;

            /* If there are more than one line then we will decrease the line
             * number */
            if (line != 1) {
                line--;
            }

            /* Refresh the new pointer values */
            if (first == borrado) {
                /* Update the actual and first pointers */
                first = borrado->next;
                actual = first;
            } else {
                /* Update the actual and their connections */
                actual = borrado->prev;
                actual->next = borrado->next;
            }

            if (last == borrado) {
                /* Refresh the last pointer */
                last = borrado->prev;
            } else {
                /* Add the link of the next->prev to actual */
                borrado->next->prev = borrado->prev;
            }

            delete borrado;
        }
    }

    /* Move the cursor one line up */
    void ViEditor::subirLinea() {
        if (actual->prev != NULL) {
            line--;
            actual = actual->prev;
        }
    }

    /* Move the cursonr one line down */
    void ViEditor::bajarLinea() {
        if (actual->next != NULL) {
            line++;
            actual = actual->next;
        }
    }

    /* Move the cursor to a specific line */
    void ViEditor::goTo(int num) {
        /* Check if the line is a valid line for the current document */
        if (!valido(num)) {
            system("cls");
            cout << "La linea deseada no existe\n";
            system("PAUSE");
            return;
        }

        /* Move the cursor till we reach the desired line */
        while(num != line) {
            if (line < num) {
                bajarLinea();
            } else {
                subirLinea();
            }
        }
    }

    /* Return true if the line number exist in this document */
    bool ViEditor::valido(int num) {
        return (num <= lineas && num >= 0);
    }

    /* Add a new line of text */
    void ViEditor::agregarLinea() {
        string temp;
        getline(cin, temp);
        agregarLinea(temp);
    }

    /* Create a new line */
    void ViEditor::agregarLinea(string temp) {
        /* Create the line */
        nodoPtr nuevo = new nodo;
        nuevo->text = temp;

        /* If this is the first line in the document, so all the pointers link
         * to this one */
        if (first == NULL) {
            first = last = actual = nuevo;
            nuevo->next = nuevo->prev = NULL;
            return;
        }

        /* Link the node the the prev one */
        nuevo->prev = actual;
        nuevo->next = actual->next;

        /* If this is the last line add the correspondig pointer */
        if (last == actual) {
            last = nuevo;
        } else {
            actual->next->prev = nuevo;
        }

        actual->next = nuevo;
        lineas++;
    }

    /* Move the text from the current line to line num */
    void ViEditor::moverLinea(int num) {
        string temp = actual->text;
        borrarLineaActual();

        /* move to the desired line - 1 (because we delete one line) */
        goTo(num-1);
        agregarLinea(temp);
    }

    /* Decode the string to move the lines */
    void ViEditor::moverLineas(string text) {
        string str1, str2, str3;
        int num1, num2, num3;

        /* Separe the string */
        /* from ^ to first space */
        str1 = text.substr(0,text.find(" "));
        /* From first space to second */
        str2 = text.substr(text.find(" ")+1);
        /* Last number */
        str3 = str2.substr(str2.find(" ")+1);
        /* Remo the thir number */
        str2 = str2.substr(0,str2.find(" "));

        /* Convert to int */
        num1 = atoi(str1.c_str());
        num2 = atoi(str2.c_str());
        num3 = atoi(str3.c_str());

        moverLineas(num1, num2, num3);
    }

    /* Move lines num1 - num2 to num3 */
    void ViEditor::moverLineas(int num1, int num2, int num3) {
        /* Check if there are no consistency of the data */
        if (!valido(num1) || !valido(num2) || !valido(num3) || num1 > num2
            || num1 == num2 || num1 == num3)
        {
            system("cls");
            cout << "La linea deseada no existe\n";
            system("PAUSE");
            return;
        }

        /* Execute the operation */
        copiarLineas(num1, num2, num3);
        borrarVariasLineas(num1, num2);

    }

    /* Copy the current line to num line */
    void ViEditor::copiarLinea(int num) {
        string temp = actual->text;
        goTo(num);
        agregarLinea(temp);
    }

    /* Parse the string from copy lines, see moverLineas for description */
    void ViEditor::copiarLineas(string text) {
        string str1, str2, str3;
        int num1, num2, num3;

        str1 = text.substr(0,text.find(" "));
        str2 = text.substr(text.find(" ")+1);
        str3 = str2.substr(str2.find(" ")+1);
        str2 = str2.substr(0,str2.find(" "));

        num1 = atoi(str1.c_str());
        num2 = atoi(str2.c_str());
        num3 = atoi(str3.c_str());
        copiarLineas(num1, num2, num3);
    }

    /* Copy lines from num1-num2 to num3 */
    void ViEditor::copiarLineas(int num1, int num2, int num3) {
        /* Check the data consistency */
        if (!valido(num1) || !valido(num2) || !valido(num3) || num1 > num2
            || num1 == num2 || num1 == num3)
        {
            system("cls");
            cout << "La linea deseada no existe\n";
            system("PAUSE");
            return;
        }

        string temp;

        /* While there are still lines to copy */
        while(num1 <= num2) {
            /* go to the last line to copy */
            goTo(num2);
            /* Copy the text */
            temp = actual->text;
            /* go to the line to paste the text */
            goTo(num3);
            /* Paste the text */
            agregarLinea(temp);
            /* Reduce the line */
            num2--;
        }

    }

    /* parse the string to delete several lines */
    void ViEditor::borrarVariasLineas(string expresion) {
        string str1, str2;
        int num1, num2;

        str1 = expresion.substr(0,expresion.find(" "));
        str2 = expresion.substr(expresion.find(" ")+1);

        num1 = atoi(str1.c_str());
        num2 = atoi(str2.c_str());

        borrarVariasLineas(num1, num2);
    }

    /* Delete several lines */
    void ViEditor::borrarVariasLineas(int num1, int num2) {
        /* Check that the numbers are valid */
        if (!valido(num1) || !valido(num2) || num1 >= num2) {
            system("cls");
            cout << "La linea deseada no existe\n";
            system("PAUSE");
            return;
        }

        /* Delete the lines */
        goTo(num2);
        for(int i=0; i < num2 - num1 + 1; i++) {
            borrarLineaActual();
        }
    }

}
