// Copyright_

#ifndef SRC_KDTREE_HPP_
#define SRC_KDTREE_HPP_

#include <cmath>
#include <iostream>
#include <set>
#include <stdexcept>
#include <utility>
#include <vector>
#include "Point.hpp"
#include <iostream>       // std::cout
#include <queue>          // std::priority_queue
#include <functional>     // std::greater

template <size_t N, typename ElemType>
class KDTree {
 public:
  typedef std::pair<Point<N>, ElemType> value_type;
  
  struct KDTreeNode
  {
      value_type point_;
      KDTreeNode* left_;
      KDTreeNode* right_;
      KDTreeNode(const value_type& value) {
          this->point_ = value;
          this->left_ = nullptr;
          this->right_ = nullptr;
      }
  };

  KDTreeNode* root;
  KDTree();
  ~KDTree();

  KDTree(const KDTree &rhs);
  KDTree &operator=(const KDTree &rhs);

  size_t dimension() const;

  size_t size() const;
  bool empty() const;

  bool contains(const Point<N> &pt) const;

  void insert(const Point<N> &pt, const ElemType &value);

  ElemType &operator[](const Point<N> &pt);

  ElemType &at(const Point<N> &pt);
  const ElemType &at(const Point<N> &pt) const;

  ElemType knn_value(const Point<N> &key, size_t k) const;

  std::vector<ElemType> knn_query(const Point<N> &key, size_t k) const;
  void PreOrden(KDTreeNode* rhs, std::vector<value_type> &rhsv);
 private:
  size_t dimension_;
  size_t size_;
};

template <size_t N, typename ElemType>
void KDTree<N, ElemType>::PreOrden(KDTreeNode* rhs, std::vector<value_type>& rhsv) {
    if (rhs == NULL)
    {
        return;
    }
    else {
        rhsv.push_back(rhs->point_);
        PreOrden(rhs->left_,rhsv);
        PreOrden(rhs->right_,rhsv);
    }
}

template <size_t N, typename ElemType>
KDTree<N, ElemType>::KDTree() {
    this->dimension_ = N;
    this->size_ = 0;
    this->root = nullptr;
}

template <size_t N, typename ElemType>
KDTree<N, ElemType>::~KDTree() {
  // TODO(me): Fill this in.
}

template <size_t N, typename ElemType>
KDTree<N, ElemType>::KDTree(const KDTree &rhs) {
    this->dimension_ = rhs.dimension();
    this->size_ = rhs.size();

    value_type aux(rhs.root->point_.first, rhs.root->point_.second);
    KDTreeNode* aux1 = new KDTreeNode(aux);

    this->root = aux1;
    //this->size_++;
    std::vector<value_type> rhsv;
    PreOrden(rhs.root, rhsv);
    for (int i1 = 0; i1 < rhsv.size(); i1++)
    {
        Point<N> pt = rhsv[i1].first;
        ElemType value = rhsv[i1].second;
        insert(pt, value);
    }
}

template <size_t N, typename ElemType>
KDTree<N, ElemType> &KDTree<N, ElemType>::operator=(const KDTree &rhs) {
  this->dimension_ = rhs.dimension();
  this->size_ = rhs.size();
  this->root = rhs.root;
  return *this;
}

template <size_t N, typename ElemType>
size_t KDTree<N, ElemType>::dimension() const {
  return this->dimension_;
}

template <size_t N, typename ElemType>
size_t KDTree<N, ElemType>::size() const {
    return this->size_;
}

template <size_t N, typename ElemType>
bool KDTree<N, ElemType>::empty() const {
    bool estado = this->size_ ? false : true;
    return estado;
}

template <size_t N, typename ElemType>
bool KDTree<N, ElemType>::contains(const Point<N> &pt) const {
  //Primero creamos un nodo
  KDTreeNode *root_aux = this->root;
  //Para iterar en el point (x,y,z,..)
  int i = 0;
  //Si esta vacio false
  if (!root_aux)
  {
      return false;
  }
  //Mientras que exista un punto
  while (root_aux)
  {
      if (root_aux->point_.first == pt)
      {
          return true;
      }
      if (root_aux->point_.first[i%this->dimension_] > pt[i% this->dimension_])
      {
          root_aux = root_aux->left_;
      }
      else
      {
          root_aux = root_aux->right_;
      }
      i++;
  }
  return false;
}

template <size_t N, typename ElemType>
void KDTree<N, ElemType>::insert(const Point<N> &pt, const ElemType &value) {
    //Primero creamos un nodo
    KDTreeNode* root_aux = this->root;
    //Para iterar en el point (x,y,z,..)
    int i = 0;
    //Si esta vacio Lo agregamos ahi
    if (!root_aux)
    {
        value_type aux(pt, value);
        KDTreeNode* aux1 = new KDTreeNode(aux);
        this->root = aux1;
        this->size_++;
        return;
    }
    //Mientras que exista un punto
    bool bandera = true;
    while (root_aux)
    {
        if (root_aux->point_.first == pt)
        {
            //Ya que es el mismo punto colo cambiamos el value
            root_aux->point_.second = value;
            bandera = false;
            break;
        }
        if (root_aux->point_.first[i % this->dimension_] > pt[i % this->dimension_])
        {
            if (root_aux->left_)
            {
                root_aux = root_aux->left_;
            }
            else
            {
                break;
            }
            
        }
        else
        {
            if (root_aux->right_)
            {
                root_aux = root_aux->right_;
            }
            else
            {
                break;
            }   
        }
        i++;
    }
    
    if(bandera) {
        //Creamos un nodo con estos valores
        value_type aux(pt, value);
        KDTreeNode* aux1 = new KDTreeNode(aux);

        //Solo vemos si lo ponemos a la derecha o izquierda
        if (root_aux->point_.first[i%this->dimension_] > pt[i%this->dimension_])
        {
            root_aux->left_ = aux1;
        }
        else {
            root_aux->right_ = aux1;
        }
        this->size_++;
    }
}

template <size_t N, typename ElemType>
ElemType &KDTree<N, ElemType>::operator[](const Point<N> &pt) {
    //Primero creamos un nodo
    KDTreeNode* root_aux = this->root;
    //Para iterar en el point (x,y,z,..)
    int i = 0;
    //Si esta vacio Lo agregamos ahi
    if (!root_aux)
    {
        value_type aux(pt, NULL);
        KDTreeNode* aux1 = new KDTreeNode(aux);
        this->root = aux1;
        this->size_++;
        return aux1->point_.second;
    }
    //Mientras que exista un punto
    bool bandera = true;
    while (root_aux)
    {
        if (root_aux->point_.first == pt)
        {
            //Ya que es el mismo punto colo cambiamos el value
            return root_aux->point_.second;
        }
        if (root_aux->point_.first[i % this->dimension_] > pt[i % this->dimension_])
        {
            if (root_aux->left_)
            {
                root_aux = root_aux->left_;
            }
            else
            {
                break;
            }

        }
        else
        {
            if (root_aux->right_)
            {
                root_aux = root_aux->right_;
            }
            else
            {
                break;
            }
        }
        i++;
    }

    if (bandera) {
        //Creamos un nodo con estos valores
        value_type aux(pt, NULL);
        KDTreeNode* aux1 = new KDTreeNode(aux);

        //Solo vemos si lo ponemos a la derecha o izquierda
        if (root_aux->point_.first[i % this->dimension_] > pt[i % this->dimension_])
        {
            root_aux->left_ = aux1;
        }
        else {
            root_aux->right_ = aux1;
        }
        this->size_++;
        return aux1->point_.second;
    }

}

template <size_t N, typename ElemType>
ElemType &KDTree<N, ElemType>::at(const Point<N> &pt) {
    //Primero creamos un nodo
    KDTreeNode* root_aux = this->root;
    //Para iterar en el point (x,y,z,..)
    int i = 0;
    //Si esta vacio false
    //Mientras que exista un punto
    bool bandera = false;
    while (root_aux)
    {
        if (root_aux->point_.first == pt)
        {
            bandera = true;
            break;
        }
        if (root_aux->point_.first[i % this->dimension_] > pt[i % this->dimension_])
        {
            root_aux = root_aux->left_;
        }
        else
        {
            root_aux = root_aux->right_;
        }
        i++;
    }

    if (bandera)
    {
        return root_aux->point_.second;
    }
    else
    {
        throw std::out_of_range("Fuera de Rango");
    }
}

template <size_t N, typename ElemType>
const ElemType &KDTree<N, ElemType>::at(const Point<N> &pt) const {
    //Primero creamos un nodo
    KDTreeNode* root_aux = this->root;
    //Para iterar en el point (x,y,z,..)
    int i = 0;
    //Si esta vacio false
    //Mientras que exista un punto
    bool bandera = false;
    while (root_aux)
    {
        if (root_aux->point_.first == pt)
        {
            bandera = true;
            break;
        }
        if (root_aux->point_.first[i % this->dimension_] > pt[i % this->dimension_])
        {
            root_aux = root_aux->left_;
        }
        else
        {
            root_aux = root_aux->right_;
        }
        i++;
    }

    if (bandera)
    {
        return root_aux->point_.second;
    }
    else
    {
        throw std::out_of_range("Fuera de Rango");
    }
}

template <size_t N, typename ElemType>
ElemType KDTree<N, ElemType>::knn_value(const Point<N> &key, size_t k) const {
  
  std::vector<ElemType> values = knn_query(key, k);
  std::pair<int, ElemType> p;
  std::priority_queue<std::pair<int,ElemType>,std::vector<std::pair<int, ElemType>>,std::greater<std::pair<int, ElemType>>> pq;
  for (int i = 0; i < values.size(); i++)
  {
      pq.push(std::make_pair(std::count(values.begin(), values.end(), values[i]) - 1, values[i]));
  }
  
  ElemType new_element = pq.top().second;
  return new_element;
}

template <size_t N, typename ElemType>
std::vector<ElemType> KDTree<N, ElemType>::knn_query(const Point<N> &key, size_t k) const {
  std::vector<value_type> rhsv;
  KDTreeNode* root_aux = this->root;
  PreOrden(*root_aux, rhsv);
  double dis = 0;
  std::vector<ElemType> values;
  std::priority_queue<std::pair<double, value_type>, std::vector<std::pair<double, value_type>>> pq;
  for (int i = 0; i < rhsv.size(); i++)
  {
      
      //std::pair<double, value_type> p;
      if (rhsv[i].first!=key)
      {
          dis = rhsv[i].first.distancia(key);
          pq.push(make_pair(dis, rhsv[i]));
      }
  }
  for (int i = 0; i < k; i++)
  {
      values.push_back(pq.top().second);
      pq.pop();
  }
  return values;
}

// TODO(me): finish the implementation of the rest of the KDTree class

#endif  // SRC_KDTREE_HPP_
