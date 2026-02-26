module.exports = {
  id: "example.visual.events.tools",
  version: "0.1.0",
  onLoad(api) {
    api.log("Example plugin loaded");
    api.registerCommand("plugin.example.ping", () => api.notify("Plugin ping OK"));
  }
};