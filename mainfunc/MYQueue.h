//#include <iostream>
#ifndef MYQUEUE
  #define MYQUEUE#include <iostream>
template<typename T> class MYQueue{
public:
    //конструктор
    MYQueue():max_element(1){
        array = new T* [max_element];
        for(int i = 0; i < max_element; i++){
            array[i] = new T[2];
        }
    }
    //змейка просто идет
    void go(T x, T y){
        for(int i = 0; i<max_element-1;i++) {array[i][0]=array[i+1][0];array[i][1]=array[i+1][1];}
        array[max_element-1][0]=x;  array[max_element-1][1]=y;
    }
    //змейка увеличилась после того как съела
    void eat(T x, T y){
        T** backup = new T* [max_element];
        for(unsigned int i = 0; i < max_element; ++i) {
            backup[i] = new T[2];
            backup[i][0] = array[i][0];
            backup[i][1] = array[i][1];
        }

        for(unsigned int i = 0; i < max_element; ++i) {
            delete[] array[i];
        }
        delete[] array;

        max_element++;
        array = new T* [max_element];
        for(unsigned int i = 0; i < max_element - 1; i++){ 
            array[i] = new T[2];
            array[i][0] = backup[i][0];
            array[i][1] = backup[i][1];
        }
        array[max_element - 1] = new T[2]; 
        array[max_element - 1][0] = x;
        array[max_element - 1][1] = y;

        delete[] backup;
    }
    //получить x
    T get_f_x(int ind){if(ind<max_element) return array[ind][0];}
    //получить y
    T get_f_y(int ind){if(ind<max_element) return array[ind][1];}
    //возвращает индекс последнего имеющегося элемента
    int size(){return max_element-1;}
    //посчитать количество одинаковых элементов в массиве, по индексу
    int count(int ind){
        int amount_of_ar_ind = 0;
        for(int i = 0; i<max_element;i++){
        if(array[ind][0]==array[i][0] && array[ind][1]==array[i][1]) amount_of_ar_ind++;
        }
        return amount_of_ar_ind;
    }
    //деструктор
    ~MYQueue(){
        for(unsigned int i = 0; i < max_element; ++i) {
            delete[] array[i];
        }
        delete[] array;
    }
private:
    T** array;
    unsigned int max_element;
};

#endif /*MYQUEUE*/