extern Random random;

struct System {
  Node::System node;
  Node::Setting::Boolean tmss;
  VFS::Pak pak;

  struct Controls {
    Node::Object node;
    Node::Input::Button reset;

    //controls.cpp
    auto load(Node::Object) -> void;
    auto poll() -> void;
  } controls;

  enum class Region : u32 { NTSCJ, NTSCU, PAL };

  auto name() const -> string { return information.name; }
  auto region() const -> Region { return information.region; }
  auto megaCD() const -> bool { return information.megaCD; }
  auto frequency() const -> double { return information.frequency; }

  auto game() -> string;
  auto run() -> void;

  auto load(Node::System& node, string name) -> bool;
  auto unload() -> void;
  auto save() -> void;
  auto power(bool reset) -> void;

  //serialization.cpp
  auto serialize(bool synchronize) -> serializer;
  auto unserialize(serializer&) -> bool;

private:
  struct Information {
    string name = "Mega Drive";
    Region region = Region::NTSCJ;
    bool megaCD = false;
    double frequency = Constants::Colorburst::NTSC * 15.0;
  } information;

  //serialization.cpp
  auto serialize(serializer&, bool synchronize) -> void;
};

extern System system;

auto Region::NTSCJ() -> bool { return system.region() == System::Region::NTSCJ; }
auto Region::NTSCU() -> bool { return system.region() == System::Region::NTSCU; }
auto Region::PAL() -> bool { return system.region() == System::Region::PAL; }

auto MegaCD() -> bool { return system.megaCD(); }
