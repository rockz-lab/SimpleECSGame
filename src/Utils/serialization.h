#include <yaml-cpp/yaml.h>

#include "ECS/ECS.h"

#include <iostream>
extern ECSManager manager;

// code for serialization of the library data types, that are used in the components
//namespace YAML {
//    template<>
//    struct convert<glm::vec2> {
//        static Node encode(const glm::vec2& rhs) {
//            Node node;
//            node.push_back(rhs.x);
//            node.push_back(rhs.y);
//            return node;
//        }
//
//        static bool decode(const Node& node, glm::vec2 &rhs) {
//            if (!node.IsSequence() || node.size() != 2) {
//                return false;
//            }
//
//            rhs.x = node[0].as<double>();
//            rhs.y = node[1].as<double>();
//            return true;
//        }
//        
//    };
//}

template<typename T>
void serialize_comp(const T& comp, std::vector<uint8_t>& data);

void serialize_entity(eID entity);


