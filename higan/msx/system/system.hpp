#include "display.hpp"

struct ROM {
  Memory::Readable<uint8> bios;
  Memory::Readable<uint8> sub;
};

struct System {
  Node::Object node;
  Node::String regionNode;
  enum class Model : uint { MSX, MSX2 };
  enum class Region : uint { NTSC, PAL };

  auto model() const -> Model { return information.model; }
  auto region() const -> Region { return information.region; }
  auto colorburst() const -> double { return information.colorburst; }

  //system.cpp
  auto run() -> void;
  auto runToSave() -> void;

  auto load(Node::Object) -> void;
  auto unload() -> void;
  auto save() -> void;
  auto power() -> void;

  //serialization.cpp
  auto serializeInit() -> void;
  auto serialize() -> serializer;
  auto unserialize(serializer&) -> bool;
  auto serializeAll(serializer&) -> void;
  auto serialize(serializer&) -> void;

private:
  struct Information {
    Model model = Model::MSX;
    Region region = Region::NTSC;
    double colorburst = Constants::Colorburst::NTSC;
    uint serializeSize = 0;
  } information;
};

extern ROM rom;
extern System system;

auto Model::MSX() -> bool { return system.model() == System::Model::MSX; }
auto Model::MSX2() -> bool { return system.model() == System::Model::MSX2; }

auto Region::NTSC() -> bool { return system.region() == System::Region::NTSC; }
auto Region::PAL() -> bool { return system.region() == System::Region::PAL; }