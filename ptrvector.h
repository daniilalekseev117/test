#pragma once

#include <vector>

template <typename T>
class PtrVector {
public:
    PtrVector() = default;

    // Создаёт вектор указателей на копии объектов из other
    PtrVector(const PtrVector& other) {
        // Резервируем место в vector-е для хранения нужного количества элементов
        // Благодаря этому при push_back не будет выбрасываться исключение
        items_.reserve(other.items_.size());

        try {
            for (auto p : other.items_) {
                // Копируем объект, если указатель на него ненулевой
                auto p_copy = p ? new T(*p) : nullptr;  // new может выбросить исключение

                // Не выбросит исключение, т. к. в vector память уже зарезервирована
                items_.push_back(p_copy);
            }
        } catch (...) {
            // удаляем элементы в векторе и перевыбрасываем пойманное исключение
            DeleteItems();
            throw;
        }
    }

    // Деструктор удаляет объекты в куче, на которые ссылаются указатели,
    // в векторе items_
    ~PtrVector() {
        DeleteItems();
    }

    // Возвращает ссылку на вектор указателей
    std::vector<T*>& GetItems() noexcept {
        return items_;
    }

    // Возвращает константную ссылку на вектор указателей
    std::vector<T*> const& GetItems() const noexcept {
        return items_;
    }

    void clear(){
        DeleteItems();
    }

    void push_back(T t){
        T* x = &t;
        items_.push_back(x);
    }

    void push_back(T* x){
        items_.push_back(x);
    }

    T& at(const size_t i) const{
        return *items_.at(i);
    }

    size_t size() const{
        return items_.size();
    }

    T* begin() {
        return *items_.begin();
    }

    T* end() {
        return *items_.end();
    }


private:
    void DeleteItems() noexcept {
        for (auto p : items_) {
            delete p;
        }
    }

    std::vector<T*> items_;
};