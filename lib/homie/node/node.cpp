#include "node.h"

#include <utility>
Node::Node(const char* name, const char* id, Device* device) {
    name_ = name;
    id_ = id;
    device_ = device;
}

bool Node::Init(Homie* homie) {
    homie_ = homie;
    homie->Publish(*this, "name", name_, true);
    homie->Publish(*this, "state", state_, true);

    bool status = true;
    for (auto it = begin(properties_); it != end(properties_); ++it) {
        if (!(*it->second).Init(homie_)) status = false;
    }
    return status;
}

void Node::AddProperty(Property* property) {
    properties_.insert(std::pair<String, Property*>((property->GetId()).c_str(), property));
}

String Node::GetId() const { return id_; }
Device* Node::GetDevice() const { return this->device_; }

Property* Node::GetProperty(String id) { return properties_.find(id)->second; }

void Node::HandleCurrentState() {
    for (auto it = begin(properties_); it != end(properties_); ++it) {
        (*it->second).HandleCurrentState();
    }
}
