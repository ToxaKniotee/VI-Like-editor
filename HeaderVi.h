#include <string>
#include <string.h>

/* If ommited will mark error */
using std::string;

namespace HeaderVi {
    struct nodo {
        nodo* next;
        nodo* prev;
        string text;
    };

    typedef nodo* nodoPtr;

    class ViEditor {
    public:
        ViEditor();

    private:

        /* Pointers */
        nodoPtr first;
        nodoPtr last;
        nodoPtr actual;

        /* line numbers */
        int line;
        int lineas;

        /* filename */
        string  name;

        void mostrar();
        void editor();
        void agregarTexoFinalLinea();
        void borrarLineaActual();
        void bajarLinea();
        void subirLinea();
        void goTo(int);
        void agregarLinea();
        void agregarLinea(string);
        void moverLinea(int);
        void moverLineas(string);
        void moverLineas(int, int, int);
        void copiarLinea(int);
        void borrarVariasLineas(string);
        void borrarVariasLineas(int, int);
        void copiarLineas(string);
        void copiarLineas(int, int, int);
        bool valido(int);
    };
}
