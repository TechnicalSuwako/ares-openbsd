struct WonderSwan : Emulator {
  WonderSwan();
  auto load(Menu) -> void override;
  auto load() -> bool override;
  auto save() -> bool override;
  auto pak(ares::Node::Object) -> shared_pointer<vfs::directory> override;
  auto input(ares::Node::Input::Input) -> void override;
};

WonderSwan::WonderSwan() {
  medium = mia::medium("WonderSwan");
  manufacturer = "Bandai";
  name = "WonderSwan";
}

auto WonderSwan::load(Menu menu) -> void {
  Menu orientationMenu{&menu};
  orientationMenu.setText("Orientation").setIcon(Icon::Device::Display);
  if(auto orientations = root->find<ares::Node::Setting::String>("PPU/Screen/Orientation")) {
    Group group;
    for(auto& orientation : orientations->readAllowedValues()) {
      MenuRadioItem item{&orientationMenu};
      item.setText(orientation);
      item.onActivate([=] {
        if(auto orientations = root->find<ares::Node::Setting::String>("PPU/Screen/Orientation")) {
          orientations->setValue(orientation);
        }
      });
      group.append(item);
    }
  }
}

auto WonderSwan::load() -> bool {
  system.pak->append("boot.rom", {Resource::WonderSwan::Boot, sizeof Resource::WonderSwan::Boot});
  system.pak->append("save.eeprom", 128);
  Emulator::load(system, "save.eeprom");

  if(!ares::WonderSwan::load(root, "[Bandai] WonderSwan")) return false;

  if(auto port = root->find<ares::Node::Port>("Cartridge Slot")) {
    port->allocate();
    port->connect();
  }

  return true;
}

auto WonderSwan::save() -> bool {
  root->save();
  Emulator::save(system, "save.eeprom");
  medium->save(game.location, game.pak);
  return true;
}

auto WonderSwan::pak(ares::Node::Object node) -> shared_pointer<vfs::directory> {
  if(node->name() == "WonderSwan") return system.pak;
  if(node->name() == "WonderSwan Cartridge") return game.pak;
  return {};
}

auto WonderSwan::input(ares::Node::Input::Input node) -> void {
  auto name = node->name();
  maybe<InputMapping&> mapping;
  if(name == "Y1"   ) mapping = virtualPads[0].l1;
  if(name == "Y2"   ) mapping = virtualPads[0].l2;
  if(name == "Y3"   ) mapping = virtualPads[0].r1;
  if(name == "Y4"   ) mapping = virtualPads[0].r2;
  if(name == "X1"   ) mapping = virtualPads[0].up;
  if(name == "X2"   ) mapping = virtualPads[0].right;
  if(name == "X3"   ) mapping = virtualPads[0].down;
  if(name == "X4"   ) mapping = virtualPads[0].left;
  if(name == "B"    ) mapping = virtualPads[0].a;
  if(name == "A"    ) mapping = virtualPads[0].b;
  if(name == "Start") mapping = virtualPads[0].start;

  if(mapping) {
    auto value = mapping->value();
    if(auto button = node->cast<ares::Node::Input::Button>()) {
      button->setValue(value);
    }
  }
}
