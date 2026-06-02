#ifndef GENERAL_TREE_H
#define GENERAL_TREE_H

#include <string>
#include <vector>
#include <iostream>


// ESTRUCTURAS DE DATOS DE NEGOCIO (GOODREADS)


/**
 * @brief Estructura para almacenar la información de los libros similares.
 * Requerido específicamente para la función de precursores.
 */
struct SimilarBook {
    std::string title;        // Título del libro similar
    std::string isbn;         // ISBN del libro similar
    int publicationYear;      // Año de publicación del libro similar
};

/**
 * @brief Estructura principal con los datos requeridos del dataset de GoodReads.
 */
struct BookData {
    std::string id;                           // ID único del libro
    std::string title;                        // Título principal
    std::string isbn;                         // Código ISBN
    int publicationYear;                      // Año de publicación original
    std::string language;                     // Idioma del libro
    std::string description;                  // Descripción/Sinopsis
    double averageRating;                     // Rating promedio obtenido
    int numPages;                             // Número total de páginas
    std::vector<SimilarBook> similarBooks;    // Colección de libros similares cargados
};


// CLASE TREENODE (NODO DEL ÁRBOL GENERAL)

/**
 * @brief Clase que representa un nodo individual dentro del árbol general.
 */
class TreeNode {
public:
    BookData data;                    // Información del libro en el nodo actual
    std::vector<TreeNode*> children;  // Lista dinámica de punteros a nodos hijos

    // Constructor estructurado
    TreeNode(BookData data) : data(data) {}
    
    // Destructor recursivo para garantizar la liberación segura de memoria dinámica
    ~TreeNode() {
        for (TreeNode* child : children) {
            delete child;
        }
    }
};


// CLASE GENERALTREE (ÁRBOL GENERAL MÉTODOS INLINE)


/**
 * @brief Clase de control del Árbol General estructurado de forma autocontenida.
 */
class GeneralTree {
private:
    TreeNode* root; // Puntero hacia el nodo raíz

 
    // MÉTODOS PRIVADOS RECURSIVOS (LÓGICA INTERNA)
    
    /**
     * @brief Implementación interna del recorrido Preorden.
     * Imprime primero la raíz del subárbol y luego procesa todos sus hijos.
     */
    void preOrderList(TreeNode* node) {
        if (node == nullptr) return;

        // 1. Procesar la raíz del subárbol actual: Imprimir el ID del libro
        std::cout << node->data.id << std::endl;

        // 2. Visitar recursivamente cada uno de sus hijos de izquierda a derecha
        for (TreeNode* child : node->children) {
            preOrderList(child);
        }
    }

    //Función para eliminar y liberar memoria de los nodos del árbol
    TreeNode* pruneRatings(TreeNode* node, double r) {
        if (node == nullptr) return nullptr;
        
        //Primero, filtrar los hijos de abajo hacia arriba (postorden)
        std::vector<TreeNode*> validChildren;
        for (TreeNode* child : node->children) {
            TreeNode* processedChild = pruneRatings(child, r);
            if (processedChild != nullptr) {
                validChildren.push_back(processedChild);
            }
        }
        node->children = validChildren;

        //Luego, evaluar el nodo actual tras limpiar su descendencia
        if (node->data.averageRating <= r) {
            delete node; //Llama al destructor del nodo (libera memoria)
            return nullptr;
        }
        return node;
    }

    //Función recorre arbol en preorder y busca los libros "precursores" (fecha de publicacion < libros similares)
    void findPrecursors(TreeNode* node) {
        if (node == nullptr) return;

        //Supone que es precursor si tiene al menos un libro similar
        bool isPrecursor = !node->data.similarBooks.empty();
        
        //Valida la condición sobre la colección de libros similares
        for (const SimilarBook& similar : node->data.similarBooks) {
            // Si encuentra uno publicado en el mismo año o antes, no es precursor
            if (similar.publicationYear <= node->data.publicationYear) {
                isPrecursor = false;
                break;
            }
        }

        //Si cumple la condición (todos los similares son posteriores), se imprime
        if (isPrecursor) {
            std::cout << node->data.id << std::endl;
        }

        //Continuar el recorrido por el árbol general
        for (TreeNode* child : node->children) {
            findPrecursors(child);
        }
    }




public:
   
    // MÉTODOS PÚBLICOS (INTERFAZ DE ACCESO EXTERNO)
  

    // Constructor por defecto
    GeneralTree() : root(nullptr) {}
    
    // Destructor principal
    ~GeneralTree() { 
        delete root; // Desencadena de forma segura la destrucción total de los nodos
    }

    /**
     * @brief 1. listar: Listar los IDs de los libros siguiendo un recorrido preorden.
     */
    void listar() {
        if (this->root != nullptr) {
            preOrderList(this->root);
        }
    }

    //Llama a función privada pruneRatings pasándole la raíz del árbol
    void borrar_ratings(double r) {
        this->root = pruneRatings(this->root, r);
    }

    //Inicia el recorrido recursivo con findPrecursors desde la raíz
    void precursores() {
        if (this->root != nullptr) {
            findPrecursors(this->root);
        }
    }

    /**
     * @brief Asigna la dirección del nodo raíz principal. Útil tras procesar los 10.000 XML.
     * @param newRoot Puntero al objeto raíz creado dinámicamente.
     */
    void setRoot(TreeNode* newRoot) { 
        this->root = newRoot; 
    }
};

#endif // GENERAL_TREE_H