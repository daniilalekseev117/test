#pragma once

// Тут можно подключить scopedptr.h и ptrvector.h, 
// если они вам понадобятся.
#include "scopedptr.h"
#include "ptrvector.h"

#include <new> // Для исключения bad_alloc
#include <vector>
#include "ptrvector.h"

// Щупальце
class Tentacle {
public:
    explicit Tentacle(int id) noexcept
        : id_(id) {
    }

    int GetId() const noexcept {
        return id_;
    }

    Tentacle* GetLinkedTentacle() const noexcept {
        return linked_tentacle_;
    }
    void LinkTo(Tentacle& tentacle) noexcept {
        linked_tentacle_ = &tentacle;
    }
    void Unlink() noexcept {
        linked_tentacle_ = nullptr;
    }

private:
    int id_ = 0;
    Tentacle* linked_tentacle_ = nullptr;
};

// Осьминог
class Octopus {
public:
    Octopus()
        : Octopus(8) {
    }

    explicit Octopus(int num_tentacles) { 
        for (int i = 0; i < num_tentacles; ++i) {
            AddTentacle();
        }

        // PtrVector<Tentacle> ptr_tentacles;
        // //tentacles_.reserve(num_tentacles);
        // for (int i = 1; i <= num_tentacles; ++i) {
        //     ptr_tentacles.GetItems().push_back(new Tentacle(i));
        // }
        
        // PtrVector<Tentacle> ptr_tentacles_copy(ptr_tentacles);
        // tentacles_.clear();
        // for(const auto& ptr : ptr_tentacles_copy.GetItems()){
        //     tentacles_.push_back(ptr);
        // }

    }

    ~Octopus() {
        // Осьминог владеет объектами в динамической памяти (щупальца),
        // которые должны быть удалены при его разрушении.
        // Деструктор - лучшее место, чтобы прибраться за собой.
        Cleanup();
    }

    // Добавляет новое щупальце с идентификатором,
    // равным (количество_щупалец + 1):
    // 1, 2, 3, ...
    // Возвращает ссылку на добавленное щупальце

    Tentacle& AddTentacle() {
        int size = GetTentacleCount();

        ScopedPtr<Tentacle> scoped_tentacle(new Tentacle(size));

        tentacles_.push_back(scoped_tentacle.GetRawPtr());

        scoped_tentacle.Release();

        return GetTentacle(size);
    }

    int GetTentacleCount() const noexcept {
        return tentacles_.size();
    }

    const Tentacle& GetTentacle(size_t index) const {
        return *tentacles_.at(index);
    }
    Tentacle& GetTentacle(size_t index) {
        return *tentacles_.at(index);
    }

private:
    void Cleanup() {
        // Удаляем щупальца осьминога из динамической памяти
        for (Tentacle* t : tentacles_) {
            delete t;
        }
        // Очищаем массив указателей на щупальца
        tentacles_.clear();
    }

    // Вектор хранит указатели на щупальца. Сами объекты щупалец находятся в куче
    PtrVector<Tentacle*> tentacles_;
};