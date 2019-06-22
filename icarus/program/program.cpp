#include "system-selection.cpp"
#include "home.cpp"
#include "game-manager.cpp"
#include "game-importer.cpp"

namespace Instances { Instance<ProgramWindow> programWindow; }
ProgramWindow& programWindow = Instances::programWindow();
SystemSelection& systemSelection = programWindow.systemSelection;
Home& home = programWindow.home;
GameManager& gameManager = programWindow.gameManager;
GameImporter& gameImporter = programWindow.gameImporter;

ProgramWindow::ProgramWindow() {
  actionMenu.setText("Action");
  quitAction.setIcon(Icon::Action::Quit).setText("Quit").onActivate([&] { doClose(); });

  settingsMenu.setText("Settings");
  createManifests.setText("Create Manifests").setChecked(settings.createManifests).onToggle([&] {
    settings.createManifests = createManifests.checked();
  });
  useDatabase.setText("Use Database").setChecked(settings.useDatabase).onToggle([&] {
    settings.useDatabase = useDatabase.checked();
  });
  useHeuristics.setText("Use Heuristics").setChecked(settings.useHeuristics).onToggle([&] {
    settings.useHeuristics = useHeuristics.checked();
  });

  helpMenu.setText("Help");
  aboutAction.setIcon(Icon::Prompt::Question).setText("About ...").onActivate([&] {
    AboutDialog()
    .setName("icarus")
    .setVersion(icarus::Version)
    .setAuthor(icarus::Author)
    .setLicense(icarus::License)
    .setWebsite(icarus::Website)
    .setAlignment(*this)
    .show();
  });

  panels.setPadding(5_sx, 5_sy);
  for(auto& cell : panels.cells()) cell.setSpacing(0);
  resizeGrip.onActivate([&] {
    resizeWidth = panels.cell(systemSelection).size().width();
  });
  resizeGrip.onResize([&](auto offset) {
    float min = 128_sx, max = panels.geometry().width() - 128_sx;
    float width = resizeWidth + offset;
    width = width < min ? min : width > max ? max : width;
    if(panels.cell(systemSelection).size().width() != width) {
      panels.cell(systemSelection).setSize({width, ~0});
      panels.resize();
    }
  });
  systemSelection.setVisible(true);
  show(home);

  onClose(&Application::quit);
  setTitle({"icarus v", icarus::Version});
  setSize({720_sx, 470_sy});
  setAlignment(Alignment::Center);
  setVisible();

  Application::processEvents();
  systemSelection.systemList.resize();
}

auto ProgramWindow::show(Panel& panel) -> void {
  if(activePanel && *activePanel == panel) return;
  if(activePanel) activePanel->setVisible(false);
  activePanel = panel;
  activePanel->setVisible(true);
  panels.resize();
}

auto ProgramWindow::hide(Panel& panel) -> void {
  show(home);
}
