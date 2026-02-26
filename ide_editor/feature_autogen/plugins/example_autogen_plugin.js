module.exports = {
  id: "autogen.fullstack.example",
  version: "0.1.0",
  onLoad(api) {
    api.log("autogen fullstack plugin loaded");
    api.registerCommand("autogen.fullstack.ping", () => api.notify("feature stack online"));
  }
};