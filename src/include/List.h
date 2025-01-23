#pragma once

template <typename TypeElement>
class List {
public:
    List(const int& p_capacite);
    ~List();

    void ajouter(const TypeElement& p_element);
    TypeElement obtenir(const int& p_indice);
    void modifier(const int& p_indice, const TypeElement& p_valeur);

    int nombreElements() const;

    TypeElement* begin() const;
    TypeElement* end() const;
private:
    int m_capacite;
    int m_nbElements;
    TypeElement* m_elements;
};

template <typename TypeElement>
List<TypeElement>::List(const int& p_capacite) :
    m_capacite(p_capacite),
    m_nbElements(0),
    m_elements(new TypeElement[p_capacite])
{
    ;
}

template <typename TypeElement>
List<TypeElement>::~List() {
    delete[] this->m_elements;
    this->m_elements = nullptr;
}

template <typename TypeElement>
void List<TypeElement>::ajouter(const TypeElement& p_element) {
    if (this->m_nbElements == this->m_capacite) {
        this->m_capacite *= 2;
        TypeElement* nouvTab = new TypeElement[this->m_capacite];
        for (int i = 0; i < this->m_nbElements; ++i) {
            nouvTab[i] = this->m_elements[i];
        }
        delete this->m_elements;
        this->m_elements = nouvTab;
    }

    this->m_elements[this->m_nbElements] = p_element;
    ++this->m_nbElements;
}

template <typename TypeElement>
TypeElement List<TypeElement>::obtenir(const int& p_indice) {
    return this->m_elements[p_indice];
}

template <typename TypeElement>
void List<TypeElement>::modifier(const int& p_indice, const TypeElement& p_valeur) {
    this->m_elements[p_indice] = p_valeur;
}

template <typename TypeElement>
int List<TypeElement>::nombreElements() const {
    return this->m_nbElements;
}

template <typename TypeElement>
TypeElement* List<TypeElement>::begin() const {
    return this->m_elements;
}

template <typename TypeElement>
TypeElement* List<TypeElement>::end() const {
    return this->m_elements + this->m_nbElements;
}