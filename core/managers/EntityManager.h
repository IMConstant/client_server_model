//
// Created by Koko on 04.06.2020.
//

#ifndef UNTITLED1_ENTITYMANAGER_H
#define UNTITLED1_ENTITYMANAGER_H

#include "../objects/Entity.h"


namespace core {

    class EntityManager {
        static EntityManager *s_instance;
        std::vector<std::unique_ptr<Entity>> m_entities;

    public:
        EntityManager();

        static EntityManager *getInstance() {
            if (s_instance) {
                return s_instance;
            }
            else {
                return new EntityManager();
            }
        }

        void update(float time);
        void draw() const;

        void add(Entity *entity);
    };

} // namespace core

#endif //UNTITLED1_ENTITYMANAGER_H
