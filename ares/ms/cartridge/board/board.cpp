namespace Board {

#include "sega.cpp"
#include "codemasters.cpp"
#include "korea.cpp"
#include "zemina.cpp"
#include "janggun.cpp"
#include "pak4.cpp"
#include "hicom.cpp"
#include "hap2000.cpp"
#include "k188in1.cpp"

auto Interface::load(Memory::Readable<n8>& memory, string name) -> bool {
  if(auto fp = pak->read(name)) {
    memory.allocate(fp->size());
    memory.load(fp);
    return true;
  }
  return false;
}

auto Interface::load(Memory::Writable<n8>& memory, string name) -> bool {
  if(auto fp = pak->read(name)) {
    memory.allocate(fp->size());
    memory.load(fp);
    return true;
  }
  return false;
}

auto Interface::save(Memory::Writable<n8>& memory, string name) -> bool {
  if(auto fp = pak->write(name)) {
    memory.save(fp);
    return true;
  }
  return false;
}

}
