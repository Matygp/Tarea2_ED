#ifndef GENERAL_TREE_H
#define GENERAL_TREE_H

#include <string>
#include <vector>
#include <iostream>

// ESTRUCTURAS DE DATOS

/**
 * @brief Estructura para almacenar la información de los libros similares.
 */
struct SimilarBook {
    std::string title;
    std::string isbn;
    int publicationYear;
};

/**
 * @brief Estructura principal con los datos requeridos de la plataforma GoodReads.
 */
struct BookData {
    std::string id;
    std::string title;
    std::string isbn;
    int publicationYear;
    std::string language;
    std::string description;
    double averageRating;
    int numPages;
    std::vector<SimilarBook> similarBooks; // Almacenados en el struct del nodo
};

// CLASE TREENODE (NODO DEL ÁRBOL)

/**
 * @brief Clase que representa un nodo dentro del árbol general.
 */
class TreeNode {
public:
    BookData data;
    std::vector<TreeNode*> children; // Lista dinámica de punteros a los hijos

    TreeNode(BookData data) : data(data) {}
    
    ~TreeNode() {
        // El destructor elimina recursivamente a todos sus hijos de la memoria
        for (TreeNode* child : children) {
            delete child;
        }
    }
};

// CLASE GENERALTREE (ÁRBOL GENERAL)

/**
 * @brief Clase principal del Árbol General estructurado como Header-Only.
 */
class GeneralTree {
private:
    TreeNode* root; // Puntero a la raíz del árbol

    // MÉTODOS PRIVADOS RECURSIVOS (IMPLEMENTACIÓN)
   
    /**
     * @brief Auxiliar recursivo para listar los IDs en recorrido preorden.
     * @param node Nodo actual del recorrido.
     */
    void preOrderList(TreeNode* node) {
        if (node == nullptr) return;

        // 1. Procesar la raíz del subárbol actual (Imprimir ID)
        std::cout << node->data.id << std::endl;

        // 2. Visitar recursivamente cada uno de sus hijos de izquierda a derecha
        for (TreeNode* child : node->children) {
            preOrderList(child);
        }
    }

    /**
     * @brief Auxiliar recursivo para podar nodos con rating menor o igual a r.
     * @param node Nodo actual bajo evaluación.
     * @param r Criterio de corte para el rating promedio.
     * @return Puntero actualizado del nodo (nullptr si fue eliminado).
     */
    TreeNode* pruneRatings(TreeNode* node, double r) {
        if (node == nullptr) return nullptr;
        
        // 1. Filtrar primero los hijos de abajo hacia arriba (Postorden)
        std::vector<TreeNode*> validChildren;
        for (TreeNode* child : node->children) {
            TreeNode* processedChild = pruneRatings(child, r);
            if (processedChild != nullptr) {
                validChildren.push_back(processedChild);
            }
        }
        node->children = validChildren;

        // 2. Evaluar el nodo actual tras limpiar su descendencia
        if (node->data.averageRating <= r) {
            delete node; // Llama al destructor del nodo (libera memoria)
            return nullptr;
        }
        return node;
    }

    /**
     * @brief Auxiliar recursivo para buscar libros precursores.
     * @param node Nodo actual bajo evaluación.
     */
    void findPrecursors(TreeNode* node) {
        if (node == nullptr) return;

        // Suponemos que es precursor si tiene al menos un libro similar
        bool isPrecursor = !node->data.similarBooks.empty();
        
        // Validar la condición sobre la colección de libros similares
        for (const SimilarBook& similar : node->data.similarBooks) {
            // Si encuentra uno publicado en el mismo año o antes, no es precursor
            if (similar.publicationYear <= node->data.publicationYear) {
                isPrecursor = false;
                break;
            }
        }

        // Si cumple la condición (todos los similares son posteriores), se imprime
        if (isPrecursor) {
            std::cout << node->data.id << std::endl;
        }

        // Continuar el recorrido por todo el árbol general
        for (TreeNode* child : node->children) {
            findPrecursors(child);
        }
    }

public:
    
    // MÉTODOS PÚBLICOS (INTERFAZ)
    
    GeneralTree() : root(nullptr) {}
    
    ~GeneralTree() { 
        delete root; // Al eliminar la raíz, se desencadena la liberación en cadena
    }

    /**
     * @brief Listar los IDs de los libros siguiendo un recorrido preorden.
     */
    void listar() {
        if (this->root != nullptr) {
            preOrderList(this->root);
        }
    }

    /**
     * @brief Eliminar del árbol todos los libros con rating menor o igual al parámetro.
     * @param r Rating límite para la eliminación.
     */
    void borrar_ratings(double r) {
        this->root = pruneRatings(this->root, r);
    }

    /**
     * @brief Listar los IDs de libros cuyos similares fueron publicados en años posteriores.
     */
    void precursores() {
        if (this->root != nullptr) {
            findPrecursors(this->root);
        }
    }

    /**
     * @brief Permite asignar la raíz del árbol tras el procesamiento de los archivos XML.
     * @param newRoot Puntero al nuevo nodo raíz.
     */
    void setRoot(TreeNode* newRoot) { 
        this->root = newRoot; 
    }
};

#endif 